//*****************************************************************************
//    Copyright (C) 2014 ZenoTec LLC (http://www.zenotec.net)
//
//    File: zTimer.cpp
//    Description:
//
//*****************************************************************************

#include <errno.h>
#include <string.h>
#include <signal.h>

#include "zutils/zTimer.h"
#include "zutils/zLog.h"

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

//**********************************************************************
// zTimer Class
//**********************************************************************

zTimer::zTimer() :
    _timerid(0), _interval(0)
{
  // Create timer
  struct sigevent sev = { 0 };
  sev.sigev_notify = SIGEV_THREAD;
  sev.sigev_notify_function = zTimer::_handler;
  sev.sigev_notify_attributes = NULL;
  sev.sigev_value.sival_int = 0;
  sev.sigev_value.sival_ptr = this;
  int stat = timer_create(CLOCK_REALTIME, &sev, &this->_timerid);
  if (stat != 0)
  {
    ZLOG_CRIT("Cannot create timer: " + std::string(strerror(errno)));
    throw;
  } // end if
  this->_lock.Unlock();
}

zTimer::~zTimer()
{
  this->_lock.Lock();
  if (this->_timerid)
  {
    int stat = timer_delete(this->_timerid);
    if (stat != 0)
    {
      ZLOG_ERR("Cannot delete timer: " + std::string(strerror(errno)));
    } // end if
  } // end if
}

void
zTimer::Start(uint32_t usec_)
{
  if (this->_lock.Lock())
  {
    this->_interval = usec_;
    this->_start();
    this->_lock.Unlock();
  } // end if
}

void
zTimer::Register(zTimerHandler *obs_)
{
  if (this->_lock.Lock())
  {
    this->_observers.push_front(obs_);
    this->_lock.Unlock();
  } // end if
}

void
zTimer::Unregister(zTimerHandler *obs_)
{
  if (this->_lock.Lock())
  {
    this->_observers.remove(obs_);
    this->_lock.Unlock();
  } // end if
}

void
zTimer::_start()
{
  // Compute time
  struct itimerspec its = { 0 };
  _add_time(&its.it_value, this->_interval);
  // Start timer
  int stat = timer_settime(this->_timerid, 0, &its, NULL);
  if (stat != 0)
  {
    ZLOG_ERR("Cannot start timer: " + std::string(strerror(errno)));
  } // end if
}

void
zTimer::_handler(union sigval sv_)
{
  zTimer *self = (zTimer *) sv_.sival_ptr;

  if (self->_lock.Lock())
  {
    std::list<zTimerHandler *>::iterator itr = self->_observers.begin();
    std::list<zTimerHandler *>::iterator end = self->_observers.end();
    for (; itr != end; itr++)
    {
      (*itr)->TimerTick();
    } // end for
    self->_start();
    self->_lock.Unlock();
  } // end if
}

}
