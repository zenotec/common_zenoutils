/*
 * Copyright (c) 2014-2016 ZenoTec LLC (http://www.zenotec.net)
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at:
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#include <errno.h>
#include <string.h>
#include <signal.h>
#include <unistd.h>
#include <time.h>

#include <mutex>
#include <list>
#include <map>

#include <zutils/zLog.h>
#include <zutils/zEvent.h>
#include <zutils/zSignal.h>
#include <zutils/zTimer.h>

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
// zTimer::Timer Class
//**********************************************************************

Timer::Timer() :
    _interval(0), _sigev( { 0 }), _timerid(0), _tick(0), zEvent::Event(zEvent::Event::TYPE_TIMER)
{

  this->RegisterEvent(this);
  zSignal::SignalManager::Instance().RegisterObserver(zSignal::Signal::ID_SIGUSR1, this);

  // Setup signal event
  memset(&this->_sigev, 0, sizeof(this->_sigev));
  this->_sigev.sigev_notify = SIGEV_SIGNAL;
  this->_sigev.sigev_signo = SIGUSR1;
  this->_sigev.sigev_notify_function = NULL;
  this->_sigev.sigev_notify_attributes = NULL;
  this->_sigev.sigev_value.sival_int = 0;
  this->_sigev.sigev_value.sival_ptr = this;

  // Create timer
  int stat = timer_create(CLOCK_REALTIME, &this->_sigev, &this->_timerid);
  if (stat != 0)
  {
    ZLOG_CRIT("Cannot create timer: " + std::string(strerror(errno)));
    return;
  } // end if

  this->_lock.Unlock();
}

Timer::~Timer()
{
  int stat = 0;
  struct itimerspec its = { 0 };

  // Stop timer
  this->Stop();

  // Delete timer
  if (this->_timerid)
  {

    // Delete timer
    stat = timer_delete(this->_timerid);
    if (stat != 0)
    {
      ZLOG_ERR("Cannot delete timer: " + std::string(strerror(errno)));
    } // end if
  } // end if

  // Unregister for signal
  zSignal::SignalManager::Instance().UnregisterObserver(zSignal::Signal::ID_SIGUSR1, this);

  // Unregister event
  this->UnregisterEvent(this);

  // Wait for lock to be available or timeout
  this->_lock.TimedLock(100);
}

void
Timer::Start(uint32_t usec_)
{
  ZLOG_DEBUG("Starting interval timer");
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
  ZLOG_DEBUG("Stopping interval timer");
  if (this->_lock.Lock())
  {
    this->_stop();
    this->_lock.Unlock();
  } // end if
}

bool
Timer::Notify()
{
  ZLOG_DEBUG("Notifying timer observer");
  if (this->_lock.TryLock())
  {
    this->_lock.Unlock();
  }
}

bool
Timer::EventHandler(const zEvent::EventNotification* notification_)
{
  bool status = false;
  const zSignal::SignalNotification *n = NULL;

  if (notification_ && (notification_->Type() == zEvent::Event::TYPE_SIGNAL))
  {
    n = static_cast<const zSignal::SignalNotification*>(notification_);
    if ((n->Id() == zSignal::Signal::ID_SIGUSR1) && n->SigInfo()
        && (n->SigInfo()->si_code == SI_TIMER) && (n->SigInfo()->si_ptr == this))
    {
      this->_tick++;
      TimerNotification notification(this);
      notification.tick(this->_tick);
      zEvent::Event::Notify(&notification);
      status = true;
    }
  }
  return status;
}

void
Timer::_start()
{
  int stat = 0;

  // Compute time
  struct itimerspec its = { 0 };
  _add_time(&its.it_value, this->_interval);
  _add_time(&its.it_interval, this->_interval);

  // Start timer
  stat = timer_settime(this->_timerid, 0, &its, NULL);
  if (stat != 0)
  {
    ZLOG_ERR("Cannot start timer: " + std::string(strerror(errno)));
  } // end if

  return;

}

void
Timer::_stop()
{
  int stat = 0;
  struct itimerspec its = { 0 };

  if (this->_timerid)
  {

    // Stop timer
    stat = timer_settime(this->_timerid, 0, &its, NULL);
    if (stat != 0)
    {
      ZLOG_ERR("Cannot stop timer: " + std::string(strerror(errno)));
    } // end if

  } // end if

}

}
}
