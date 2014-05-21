//*****************************************************************************
//    Copyright (C) 2014 ZenoTec LLC (http://www.zenotec.net)
//
//    File: zTimer.cpp
//    Description:
//
//*****************************************************************************

#include <signal.h>

#include "zutils/zTimer.h"

#include <iostream>
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

zTimer::zTimer()
{
  // Create timer
  struct sigevent sev = { 0 };
  sev.sigev_notify = SIGEV_THREAD;
  sev.sigev_notify_attributes = NULL;
  sev.sigev_notify_function = zTimer::_handler;
  sev.sigev_value.sival_int = 0;
  sev.sigev_value.sival_ptr = this;
  timer_create(CLOCK_REALTIME, &sev, &this->_timerid);
  // Unlock
  this->_lock.Unlock();
}

zTimer::~zTimer()
{
  this->Stop();
  timer_delete(this->_timerid);
}

void
zTimer::Start(uint32_t usec_)
{

  // Lock
  this->_lock.Lock();

  // Compute time
  struct itimerspec its = { 0 };
  _add_time(&its.it_value, usec_);
  _add_time(&its.it_interval, usec_);

  // Start timer
  timer_settime(this->_timerid, 0, &its, NULL);

  // Unlock
  this->_lock.Unlock();
}

void
zTimer::Stop()
{
  // Lock
  this->_lock.Lock();

  // Stop timer
  struct itimerspec its = { 0 };
  timer_settime(this->_timerid, 0, &its, NULL);

  // Unlock
  this->_lock.Unlock();
}

void
zTimer::Register(zTimerHandler *obs_)
{
  this->_lock.Lock();
  this->_observers.push_front(obs_);
  this->_lock.Unlock();
}

void
zTimer::Unregister(zTimerHandler *obs_)
{
  this->_lock.Lock();
  this->_observers.remove(obs_);
  this->_lock.Unlock();
}

void
zTimer::_handler(union sigval sv_)
{
  zTimer *self = (zTimer *) sv_.sival_ptr;

  self->_lock.Lock();
  std::list<zTimerHandler *>::iterator itr = self->_observers.begin();
  std::list<zTimerHandler *>::iterator end = self->_observers.end();
  for (; itr != end; itr++)
  {
    (*itr)->TimerTick();
  } // end for
  self->_lock.Unlock();
}

}
