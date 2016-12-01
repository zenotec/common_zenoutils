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

Mutex::Mutex(Mutex::STATE state_) :
    _lock(this->_mutex)
{
  ZLOG_DEBUG("Create mutex: " + zLog::PointerStr(this));
  if (state_ == Mutex::UNLOCKED)
  {
    this->_lock.unlock();
  }
}

Mutex::~Mutex()
{
//  ZLOG_DEBUG("Delete mutex: " + zLog::PointerStr(this));
}

bool
Mutex::Lock()
{
  ZLOG_DEBUG("Lock mutex: " + zLog::PointerStr(this));
  this->_lock.lock();
  return (true);
}

bool
Mutex::TryLock()
{
  ZLOG_DEBUG("Try to lock mutex: " + zLog::PointerStr(this));
  return (!this->_lock.owns_lock() && this->_lock.try_lock());
}

bool
Mutex::TimedLock(uint32_t us_)
{
  return (!this->_lock.owns_lock() && TIMED_LOCK(this->_lock, (us_ * 1000)));
}

bool
Mutex::Unlock()
{
  ZLOG_DEBUG("Unlock mutex: " + zLog::PointerStr(this));
  this->_lock.unlock();
  return (true);
}

Mutex::STATE
Mutex::State()
{
  Mutex::STATE state = Mutex::UNLOCKED;
  if (this->_lock.owns_lock())
  {
    state = Mutex::LOCKED;
  }
  return(state);
}

}
}
