//*****************************************************************************
//    Copyright (C) 2016 ZenoTec LLC (http://www.zenotec.net)
//
//    File:
//    Description:
//
//*****************************************************************************

#include <unistd.h>
#include <errno.h>
#include <string.h>

#include <list>
#include <mutex>

#include <zutils/zSem.h>
#include <zutils/zThread.h>
#include <zutils/zLog.h>

namespace zUtils
{
namespace zThread
{

//*****************************************************************************
// Class Thread
//*****************************************************************************
Thread::Thread(Function *func_, void *arg_) :
    _id(0), _func(func_), _arg(arg_), _exit(true)
{
  // Starting with the mutex in the locked state so that when the thread is
  //   started, it unlocks it
}

Thread::~Thread()
{
  // Terminate listener thread
  this->Join();
}

bool
Thread::Run()
{

  bool status = false;
  pthread_t id = 0;
  pthread_attr_t attr;

  if (pthread_attr_init(&attr))
  {
    std::string errmsg = "Cannot initialize thread attributes: " + std::string(strerror(errno));
    ZLOG_CRIT(errmsg);
    throw errmsg;
  } /* end if */

  if (pthread_attr_setdetachstate(&attr, PTHREAD_CREATE_JOINABLE))
  {
    std::string errmsg = "Cannot set detached state: " + std::string(strerror(errno));
    ZLOG_CRIT(errmsg);
    throw errmsg;
  } /* end if */

  if (pthread_create(&id, &attr, Thread::_threadHandler, this))
  {
    std::string errmsg = "Cannot create listen thread: " + std::string(strerror(errno));
    ZLOG_CRIT(errmsg);
    throw errmsg;
  } // end if

  if (pthread_attr_destroy(&attr))
  {
    std::string errmsg = "Cannot destroy thread attributes: " + std::string(strerror(errno));
    ZLOG_CRIT(errmsg);
    throw errmsg;
  } /* end if */

  if (this->_mutex.TimedLock(1000000))
  {
    this->_id = id;
    this->_mutex.Unlock();
    ZLOG_INFO("Created new thread: " + zLog::HexStr((uint32_t )this->_id));
    status = true;
  }

  return (status);
}

bool
Thread::Join()
{
  bool status = false;
  this->_exit = true;
  if (this->_id && this->_mutex.TimedLock(1000000))
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

  ZLOG_INFO("Thread::_threadHandler: Thread starting");

  // Pause a moment to allow context switch
  usleep(1000);

  while (self && !self->_exit && self->_func)
  {
    if (self->_mutex.TimedLock(1000000))
    {
      if (self->_func->ThreadFunction(self->_arg))
      {
        self->_exit = true;
      }
      self->_mutex.Unlock();
    }
  }

  ZLOG_INFO("Thread::_threadHandler: Thread exiting");

  pthread_exit(0);
  return (0);
}

}
}
