//*****************************************************************************
//    Copyright (C) 2014 ZenoTec LLC (http://www.zenotec.net)
//
//    File: zThread.cpp
//    Description:
//
//*****************************************************************************

#include <unistd.h>
#include <errno.h>
#include <string.h>

#include "zutils/zThread.h"
#include "zutils/zLog.h"

namespace zUtils
{
namespace zThread
{

//*****************************************************************************
// Class Thread
//*****************************************************************************
Thread::Thread(Function *func_, void *arg_, uint32_t period_) :
    _id(0), _func(func_), _arg(arg_), _exit(true), _period(period_)
{
  // Starting with the mutex in the locked state so that when the thread is
  //   started, it unlocks it
}

Thread::~Thread()
{
  // Terminate listener thread
  this->Join(1000);
}

bool
Thread::Run(uint32_t msecs_)
{

  bool status = false;
  pthread_t id = 0;
  pthread_attr_t attr;

  if (pthread_attr_init(&attr))
  {
    ZLOG_CRIT("Cannot initialize thread attributes: " + std::string(strerror(errno)));
    throw;
  } /* end if */

  if (pthread_attr_setdetachstate(&attr, PTHREAD_CREATE_JOINABLE))
  {
    ZLOG_CRIT("Cannot set detached state: " + std::string(strerror(errno)));
    throw;
  } /* end if */

  if (pthread_create(&id, NULL, Thread::_threadHandler, this))
  {
    ZLOG_CRIT("Cannot create listen thread: " + std::string(strerror(errno)));
    throw;
  } // end if

  if (pthread_attr_destroy(&attr))
  {
    ZLOG_CRIT("Cannot destroy thread attributes: " + std::string(strerror(errno)));
    throw;
  } /* end if */

  if (this->_mutex.TimedLock(msecs_ * 1000))
  {
    this->_id = id;
    status = !this->_exit;
    this->_mutex.Unlock();
    ZLOG_INFO("Created new thread: " + zLog::HexStr((uint32_t )this->_id));
  }

  return (status);
}

bool
Thread::Join(uint32_t msecs_)
{
  bool status = false;
  this->_exit = true;
  if (this->_id && this->_mutex.TimedLock(msecs_ * 1000))
  {
    ZLOG_INFO("Joining thread: " + zLog::HexStr((uint32_t )this->_id));
    pthread_join(this->_id, 0);
    this->_id = 0;
    status = true;
  } // end if
  return (status);
}

unsigned long
Thread::GetId()
{
  return (this->_id);
}

void *
Thread::_threadHandler(void *arg_)
{
  Thread *self = (Thread *) arg_;
  std::string logstr;

  self->_exit = false;
  self->_mutex.Unlock();

  // Pause a moment to allow context switch
  usleep(1000);

  while (self && !self->_exit && self->_func)
  {
    if (self->_mutex.TimedLock(self->_period * 1000))
    {
      if (self->_func->ThreadFunction(self->_arg))
      {
        self->_exit = true;
      }
      self->_mutex.Unlock();
    }
  }

  ZLOG_INFO("Thread exiting");

  pthread_exit(0);
  return (0);
}

}
}
