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
    _lock(Mutex::LOCKED), _empty(Mutex::LOCKED), _cnt(value_)
{

  ZLOG_DEBUG("(" + ZLOG_P(this) + ":" + ZLOG_P(&this->_empty) + "): " +
      ZLOG_UINT(this->_cnt));
  if (this->_cnt != 0)
  {
    this->_empty.Unlock();
  }
  this->_lock.Unlock();
}

Semaphore::~Semaphore()
{
  ZLOG_DEBUG("(" + ZLOG_P(this) + ":" + ZLOG_P(&this->_empty) + "): " +
      ZLOG_UINT(this->_cnt));
}

bool
Semaphore::Post()
{
  bool status = false;

  ZLOG_DEBUG("(" + ZLOG_P(this) + ":" + ZLOG_P(&this->_empty) + "): " +
      ZLOG_UINT(this->_cnt));

  if (this->_lock.Lock())
  {
    // Test for overflow
    if ((this->_cnt + 1) > this->_cnt)
    {
      if (this->_cnt == 0)
      {
        this->_empty.Unlock();
      }
      this->_cnt++;
    }
    status = true;
    this->_lock.Unlock();
  }

  return (status);
}

bool
Semaphore::Wait()
{

  ZLOG_DEBUG("(" + ZLOG_P(this) + ":" + ZLOG_P(&this->_empty) + "): " +
      ZLOG_UINT(this->_cnt));

  bool status = this->TryWait();

  if (!status)
  {
    if (this->_empty.Lock())
    {
      this->_empty.Unlock();
      status = this->TryWait();
    }
  }

  return (status);
}

bool
Semaphore::TryWait()
{
  bool status = false;

  ZLOG_DEBUG("(" + ZLOG_P(this) + ":" + ZLOG_P(&this->_empty) + "): " +
      ZLOG_UINT(this->_cnt));

  if (this->_lock.Lock())
  {
    ZLOG_DEBUG("Got lock");
    if (this->_cnt != 0)
    {
      ZLOG_DEBUG("Not zero");
      this->_cnt--;
      if (this->_cnt == 0)
      {
        ZLOG_DEBUG("Now zero");
        this->_empty.Lock();
      }
      status = true;
    }
    this->_lock.Unlock();
    ZLOG_DEBUG("Returned lock");
  }

  ZLOG_DEBUG("STATUS: " + ZLOG_BOOL(status));
  return (status);
}

bool
Semaphore::TimedWait(uint32_t msec_)
{

  ZLOG_DEBUG("(" + ZLOG_P(this) + ":" + ZLOG_P(&this->_empty) + "): Count" +
      ZLOG_UINT(this->_cnt) + "; Time: " + ZLOG_UINT(msec_));

  bool status = this->TryWait();

  if (!status)
  {
    if (this->_empty.TimedLock(msec_))
    {
      this->_empty.Unlock();
      status = this->TryWait();
    }
  }

  return (status);
}

}
}
