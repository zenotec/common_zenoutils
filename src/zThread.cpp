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
zThread::zThread()
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

//  if (pthread_create(&this->_listenThreadId, NULL, PacketListener::_threadFunc, this))
//  {
//    ZLOG_CRIT("Cannot create listen thread: " + std::string(strerror(errno)));
//    throw;
//  } // end if

  if (pthread_attr_destroy(&attr))
  {
    ZLOG_CRIT("Cannot destroy thread attributes: " + std::string(strerror(errno)));
    throw;
  } /* end if */

}

zThread::~zThread()
{

}

}
