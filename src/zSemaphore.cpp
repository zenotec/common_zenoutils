/*
 * Select.cpp
 *
 *  Created on: Jan 13, 2014
 *      Author: kmahoney
 */

#include "zutils/zLog.h"
#include "zutils/zSemaphore.h"

namespace zUtils
{

static void
_add_time(struct timespec *ts_, uint32_t us_)
{
  // Compute seconds
  ts_->tv_sec += (us_ / 1000000);

  // Compute nanoseconds
  uint32_t nsec = ((us_ % 1000000) * 1000);
  if ((ts_->tv_nsec + nsec) < ts_->tv_nsec)
  {
    ts_->tv_sec++;
  } // end if
  ts_->tv_nsec += nsec;

}

//*****************************************************************************
// zMutex Class
//*****************************************************************************

zMutex::zMutex(zMutex::state state_)
{
  switch (state_)
  {
    case zMutex::LOCKED:
    case zMutex::UNLOCKED:
    {
      int ret = sem_init(&this->_sem, 0, state_);
      if (ret != 0)
      {
        ZLOG_CRIT("Cannot initialize lock: " + zLog::IntStr(ret));
        throw;
      } // end if
      break;
    }
    default:
    {
      ZLOG_CRIT("Invalid mutex state: " + zLog::IntStr(state_));
      throw;
    }
  }
}

zMutex::~zMutex()
{
  int ret = sem_destroy(&this->_sem);
  if (ret != 0)
  {
    ZLOG_CRIT("Cannot destroy lock: " + zLog::IntStr(ret));
    throw;
  } // end if
}

bool
zMutex::Lock()
{
  return (sem_wait(&this->_sem) == 0);
}

bool
zMutex::TryLock()
{
  return (sem_trywait(&this->_sem) == 0);
}

bool
zMutex::TimedLock(uint32_t us_)
{

  struct timespec ts;

  // Get current time
  if (clock_gettime(CLOCK_REALTIME, &ts) != 0)
  {
    return (false);
  } // end if

  // Compute absolute time
  _add_time(&ts, us_);

  return (sem_timedwait(&this->_sem, &ts) == 0);
}

bool
zMutex::Unlock()
{
  int ret = 0;
  if (this->State() == zMutex::LOCKED) // possible race condition here
  {
    ret = sem_post(&this->_sem);
  } // end if
  return (ret == 0);
}

zMutex::state
zMutex::State()
{
  int val;
  sem_getvalue(&this->_sem, &val);
  return ((zMutex::state) val);
}

//*****************************************************************************
// Semaphore Class
//*****************************************************************************

zSemaphore::zSemaphore(const uint32_t value_)
{
  int ret = sem_init(&this->_sem, 0, value_);
  if (ret != 0)
  {
    ZLOG_CRIT("Cannot initialize lock: " + zLog::IntStr(ret));
    throw;
  } // end if
}

zSemaphore::~zSemaphore()
{
  int ret = sem_destroy(&this->_sem);
  if (ret != 0)
  {
    ZLOG_CRIT("Cannot destroy lock: " + zLog::IntStr(ret));
    throw;
  } // end if
}

bool
zSemaphore::Post(uint32_t value_)
{
  int stat = 0;
  while (!stat && value_--)
  {
    stat = sem_post(&this->_sem);
  } // end while
  return (stat == 0);
}

bool
zSemaphore::Wait()
{
  return (sem_wait(&this->_sem) == 0);
}

bool
zSemaphore::TryWait()
{
  return (sem_trywait(&this->_sem) == 0);
}

bool
zSemaphore::TimedWait(uint32_t us_)
{
  struct timespec ts;

  // Get current time
  if (clock_gettime(CLOCK_REALTIME, &ts) != 0)
  {
    return (false);
  } // end if

  // Compute absolute time
  _add_time(&ts, us_);

  return (sem_timedwait(&this->_sem, &ts) == 0);
}

uint32_t
zSemaphore::Value()
{
  int val;
  sem_getvalue(&this->_sem, &val);
  return ((uint32_t) val);
}

}
