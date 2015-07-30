/*
 * Select.cpp
 *
 *  Created on: Jan 13, 2014
 *      Author: kmahoney
 */
#include <unistd.h>
#include <errno.h>
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

  // Compute nanoseconds
  uint32_t nsec = ((us_ % 1000000) * 1000);
  if ((ts_->tv_nsec + nsec) < ts_->tv_nsec)
  {
    ts_->tv_sec++;
  } // end if
  ts_->tv_nsec += nsec;

  // Compute seconds
  ts_->tv_sec += (us_ / 1000000);
  if (ts_->tv_nsec >= 1000000000)
  {
    ts_->tv_sec++;
    ts_->tv_nsec %= 1000000000;
  }

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
  ZLOG_INFO("Semaphore::Semaphore: Created system semaphore: " + zLog::HexStr(this->_sem) + ":" + zLog::IntStr(value_));
}

Semaphore::~Semaphore()
{
  ZLOG_INFO("Semaphore::Semaphore: Destroying system semaphore: " + zLog::HexStr(this->_sem));
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

  ZLOG_DEBUG("Semaphore::Wait: Waiting on system semaphore " + zLog::HexStr(this->_sem));

  if (sem_wait(&this->_sem) != 0)
  {
    ZLOG_CRIT("Error waiting on system semaphore: " + std::string(strerror(errno)));
    return(false);
  }

  return (true);
}

bool
Semaphore::TryWait()
{
  if (sem_trywait(&this->_sem) != 0)
  {
    ZLOG_CRIT("Error waiting on system semaphore: " + std::string(strerror(errno)));
    return(false);
  }

  return (true);
}

bool
Semaphore::TimedWait(uint32_t us_)
{
  struct timespec ts;

  ZLOG_DEBUG("Semaphore::TimedWait: Waiting on system semaphore " + zLog::HexStr(this->_sem));

  // Get current time
  if (clock_gettime(CLOCK_REALTIME, &ts) != 0)
  {
    return (false);
  } // end if

  // Compute absolute time
  _add_time(&ts, us_);

  if (sem_timedwait(&this->_sem, &ts) != 0)
  {
    ZLOG_CRIT("Error waiting on system semaphore: " + std::string(strerror(errno)));
    return(false);
  }

  return (true);
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
