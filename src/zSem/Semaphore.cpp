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

#include <zutils/zLog.h>
#include <zutils/zSem.h>

namespace zUtils
{
namespace zSem
{

//*****************************************************************************
// Semaphore Class
//*****************************************************************************

Semaphore::Semaphore(const uint32_t value_) :
    _lock(Mutex::LOCKED), _mutex(Mutex::LOCKED), _value(value_)
{
  ZLOG_DEBUG("Create semaphore: " + zLog::PointerStr(this));
  if (this->_value)
  {
    this->_mutex.Unlock();
  }
  this->_lock.Unlock();
}

Semaphore::~Semaphore()
{
//  ZLOG_DEBUG("Delete semaphore: " + zLog::PointerStr(this));
}

bool
Semaphore::Post(uint32_t value_)
{
  bool status = false;
  ZLOG_DEBUG("Post semaphore(" + zLog::PointerStr(this) + "): " + zLog::UintStr(value_));
  if (this->_lock.Lock())
  {
    this->_value += value_;
    if ((this->_value > 0) && (this->_mutex.State() == Mutex::LOCKED))
    {
      this->_mutex.Unlock();
    }
    status = true;
    this->_lock.Unlock();
  }
  return (status);
}

bool
Semaphore::Wait()
{
  bool status = false;

  ZLOG_DEBUG("Wait for semaphore: " + zLog::PointerStr(this));

  if (this->_lock.Lock())
  {
    if (this->_mutex.Lock())
    {
      this->_value--;
      if (this->_value > 0)
      {
        this->_mutex.Unlock();
      }
      status = true;
    }
    this->_lock.Unlock();
  }
  return (status);
}

bool
Semaphore::TryWait()
{
  bool status = false;
  ZLOG_DEBUG("Try to wait for semaphore: " + zLog::PointerStr(this));
  if (this->_lock.Lock())
  {
    if (this->_mutex.TryLock())
    {
      this->_value--;
      if (this->_value > 0)
      {
        this->_mutex.Unlock();
      }
      status = true;
    }
    this->_lock.Unlock();
  }
  return (status);
}

bool
Semaphore::TimedWait(uint32_t us_)
{
  bool status = false;
  if (this->_lock.Lock())
  {
    if (this->_mutex.TimedLock(us_))
    {
      this->_value--;
      if (this->_value > 0)
      {
        this->_mutex.Unlock();
      }
      status = true;
    }
    this->_lock.Unlock();
  }
  return (status);
}

uint32_t
Semaphore::Value()
{
  uint32_t value = 0;
  if (this->_lock.Lock())
  {
    value = this->_value;
    this->_lock.Unlock();
  }
  return (value);
}

}
}
