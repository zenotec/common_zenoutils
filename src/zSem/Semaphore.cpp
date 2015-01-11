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
// Semaphore Class
//*****************************************************************************

Semaphore::Semaphore(const uint32_t value_)
{
  int ret = sem_init(&this->_sem, 0, value_);
  if (ret != 0)
  {
    ZLOG_CRIT("Cannot initialize lock: " + zLog::IntStr(ret));
    throw;
  } // end if
}

Semaphore::~Semaphore()
{
  int ret = sem_destroy(&this->_sem);
  if (ret != 0)
  {
    ZLOG_CRIT("Cannot destroy lock: " + zLog::IntStr(ret));
    throw;
  } // end if
}

bool
Semaphore::Post(uint32_t value_)
{
  int stat = 0;
  while (!stat && value_--)
  {
    stat = sem_post(&this->_sem);
  } // end while
  return (stat == 0);
}

bool
Semaphore::Wait()
{
  return (sem_wait(&this->_sem) == 0);
}

bool
Semaphore::TryWait()
{
  return (sem_trywait(&this->_sem) == 0);
}

bool
Semaphore::TimedWait(uint32_t us_)
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
Semaphore::Value()
{
  int val;
  sem_getvalue(&this->_sem, &val);
  return ((uint32_t) val);
}

}
}
