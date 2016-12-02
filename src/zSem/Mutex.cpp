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
  ZLOG_DEBUG("Create mutex: " + ZLOG_P(this));
  if (state_ == Mutex::UNLOCKED)
  {
    this->_lock.unlock();
  }
}

Mutex::~Mutex()
{
//  ZLOG_DEBUG("Delete mutex: " + ZLOG_P(this));
}

bool
Mutex::Lock()
{
  bool status = false;
  ZLOG_DEBUG("Lock mutex: " + ZLOG_P(this));
//  if (!this->_lock.owns_lock())
//  {
    this->_lock.lock();
    status = true;
//  }
  return (status);
}

bool
Mutex::TryLock()
{
  bool status = false;
  ZLOG_DEBUG("Try to lock mutex: " + ZLOG_P(this));
//  if (!this->_lock.owns_lock())
//  {
    status = this->_lock.try_lock();
//  }
  return (status);
}

bool
Mutex::TimedLock(uint32_t msec_)
{
  bool status = false;
  ZLOG_DEBUG("Waiting for mutex(" + ZLOG_P(this) + "): " + ZLOG_UINT(msec_) + " ms(s)");
//  if (!this->_lock.owns_lock())
//  {
    status = TIMED_LOCK(this->_lock, msec_);
//  }
  return (status);
}

bool
Mutex::Unlock()
{
  ZLOG_DEBUG("Unlock mutex: " + ZLOG_P(this));
  bool status = false;
//  if (this->_lock.owns_lock())
//  {
    this->_lock.unlock();
    status = true;
//  }
  return (status);
}

}
}
