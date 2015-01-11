/*
 * Select.cpp
 *
 *  Created on: Jan 13, 2014
 *      Author: kmahoney
 */
#include <string.h>

#include <zutils/zLog.h>
#include <zutils/zSem.h>

namespace zUtils
{
namespace zSem
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

Mutex::Mutex(Mutex::STATE state_)
{
  memset(&this->_sem, 0, sizeof(this->_sem));
  switch (state_)
  {
    case Mutex::LOCKED:
    case Mutex::UNLOCKED:
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

Mutex::~Mutex()
{
  int ret = sem_destroy(&this->_sem);
  if (ret != 0)
  {
    ZLOG_CRIT("Cannot destroy lock: " + zLog::IntStr(ret));
    throw;
  } // end if
}

bool
Mutex::Lock()
{
  return (sem_wait(&this->_sem) == 0);
}

bool
Mutex::TryLock()
{
  return (sem_trywait(&this->_sem) == 0);
}

bool
Mutex::TimedLock(uint32_t us_)
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
Mutex::Unlock()
{
  int ret = 0;
  if (this->State() == Mutex::LOCKED) // possible race condition here
  {
    ret = sem_post(&this->_sem);
  } // end if
  return (ret == 0);
}

Mutex::STATE
Mutex::State()
{
  int val;
  sem_getvalue(&this->_sem, &val);
  return ((Mutex::STATE) val);
}

}
}
