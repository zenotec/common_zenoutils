//*****************************************************************************
//    Copyright (C) 2014 ZenoTec LLC (http://www.zenotec.net)
//
//    File: zThread.cpp
//    Description:
//
//*****************************************************************************

#include <errno.h>
#include <string.h>

#include "zutils/zThread.h"
#include "zutils/zLog.h"

namespace zUtils
{

//*****************************************************************************
// Class PacketListener
//*****************************************************************************
zThread::zThread(zThreadFunction *func_, void *arg_) :
    _id(0), _func(func_), _arg(arg_), _exit(false)
{

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

  if (pthread_create(&this->_id, NULL, zThread::_threadHandler, this))
  {
    ZLOG_CRIT("Cannot create listen thread: " + std::string(strerror(errno)));
    throw;
  } // end if

  if (pthread_attr_destroy(&attr))
  {
    ZLOG_CRIT("Cannot destroy thread attributes: " + std::string(strerror(errno)));
    throw;
  } /* end if */

  ZLOG_INFO("Created new thread: " + zLog::HexStr((uint32_t)this->_id));

}

zThread::~zThread()
{
  // Terminate listener thread
  if (this->_id)
  {
    this->_exit = true;
    pthread_join(this->_id, 0);
    this->_id = 0;
  } // end if
}

uint32_t
zThread::Id()
{
  return ((uint32_t) this->_id);
}

void *
zThread::_threadHandler(void *arg_)
{
  zThread *self = (zThread *) arg_;

  while (self->_exit != true)
  {
    if (self->_func != NULL)
    {
      self->_func->Function(self->_arg);
    } // end if
  } // end while

  pthread_exit(0);
  return (0);
}

}
