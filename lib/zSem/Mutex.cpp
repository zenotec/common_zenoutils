//*****************************************************************************
//    Copyright (C) 2016 ZenoTec LLC (http://www.zenotec.net)
//
//    File:
//    Description:
//
//*****************************************************************************

#include <zutils/zLog.h>
#include <zutils/zSem.h>

namespace zUtils
{
namespace zSem
{

//*****************************************************************************
// zMutex Class
//*****************************************************************************

Mutex::Mutex(Mutex::STATE state_)
{
  ZLOG_DEBUG("(" + ZLOG_P(this) + ")");
  if (state_ == Mutex::LOCKED)
  {
    this->_lock.lock();
  }
}

Mutex::~Mutex()
{
//  ZLOG_DEBUG("(" + ZLOG_P(this) + ")"); // TODO: Causes segfault
}

bool
Mutex::Lock()
{
  bool status = false;
  ZLOG_DEBUG("(" + ZLOG_P(this) + ")");
  this->_lock.lock();
  status = true;
  return (status);
}

bool
Mutex::TryLock()
{
  bool status = false;
  ZLOG_DEBUG("(" + ZLOG_P(this) + ")");
  status = this->_lock.try_lock();
  ZLOG_DEBUG("STATUS: " + ZLOG_BOOL(status));
  return (status);
}

bool
Mutex::TimedLock(uint32_t msec_)
{
  bool status = false;
  ZLOG_DEBUG("(" + ZLOG_P(this) + "): " + ZLOG_UINT(msec_) + " ms(s)");
  status = TIMED_LOCK(this->_lock, msec_);
  ZLOG_DEBUG("STATUS: " + ZLOG_BOOL(status));
  return (status);
}

bool
Mutex::Unlock()
{
  ZLOG_DEBUG("(" + ZLOG_P(this) + ")");
  bool status = false;
  this->_lock.unlock();
  status = true;
  return (status);
}

}
}
