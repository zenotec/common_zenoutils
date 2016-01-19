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

#include <time.h>

#include <zutils/zTimer.h>
#include <zutils/zLog.h>

namespace zUtils
{
namespace zTimer
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
// zTimer::TimerEvent Class
//**********************************************************************

TimerEvent::TimerEvent(const TimerEvent::EVENTID id_) :
    zEvent::Event(zEvent::Event::TYPE_TIMER, id_)
{
}

TimerEvent::~TimerEvent()
{
}

//**********************************************************************
// zTimer::Timer Class
//**********************************************************************

Timer::Timer() :
    _timerid(0), _interval(0), _tick_event(zTimer::TimerEvent::EVENTID_TICK)
{

  this->RegisterEvent(&this->_tick_event);

  // Create timer
  struct sigevent sev = { 0 };
  sev.sigev_notify = SIGEV_THREAD;
  sev.sigev_notify_function = this->timer_handler;
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

Timer::~Timer()
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
Timer::Start(uint32_t usec_)
{
  if (this->_lock.Lock())
  {
    this->_interval = usec_;
    this->_start();
    this->_lock.Unlock();
  } // end if
}

void
Timer::Stop(void)
{
  if (this->_lock.Lock())
  {
    this->_stop();
    this->_lock.Unlock();
  } // end if
}

void
Timer::timer_handler(union sigval sv_)
{
  ZLOG_DEBUG("Timer expired");
  Timer *self = (Timer *) sv_.sival_ptr;
  self->_tick_event.Notify(self);
  pthread_detach(pthread_self());
  pthread_exit(0);
}

void
Timer::_start()
{
  // Compute time
  struct itimerspec its = { 0 };
  _add_time(&its.it_value, this->_interval);
  _add_time(&its.it_interval, this->_interval);
  // Start timer
  int stat = timer_settime(this->_timerid, 0, &its, NULL);
  if (stat != 0)
  {
    ZLOG_ERR("Cannot start timer: " + std::string(strerror(errno)));
  } // end if
}

void
Timer::_stop()
{
  // Compute time
  struct itimerspec its = { 0 };
  // Start timer
  int stat = timer_settime(this->_timerid, 0, &its, NULL);
  if (stat != 0)
  {
    ZLOG_ERR("Cannot start timer: " + std::string(strerror(errno)));
  } // end if
}

}
}
