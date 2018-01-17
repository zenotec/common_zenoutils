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
#include <fcntl.h>
#include <sys/timerfd.h>
#include <poll.h>

#include <mutex>
#include <list>
#include <map>

#include <zutils/zEvent.h>
#include <zutils/zSignal.h>
#include <zutils/zTimer.h>

namespace zUtils
{
namespace zTimer
{

static void
_add_time(struct timespec *ts_, uint32_t usec_)
{
  // Compute seconds
  ts_->tv_sec += (usec_ / 1000000);

  // Compute nanoseconds
  ts_->tv_nsec += ((usec_ % 1000000) * 1000);

  // Check for nsec greater than 1 sec
  if (ts_->tv_nsec >= 1000000000)
  {
    ts_->tv_sec++;
    ts_->tv_nsec %= 1000000000;
  }

  return;
}

//**********************************************************************
// Class: TimerThreadFunction
//**********************************************************************

TimerThreadFunction::TimerThreadFunction() :
    _ticks(0)
{

}

TimerThreadFunction::~TimerThreadFunction()
{

}

void
TimerThreadFunction::Run(zThread::ThreadArg *arg_)
{

  uint64_t ticks = 0;
  Timer *timer = (Timer *) arg_;

  if (!arg_)
  {
    return;
  }

  // Setup for poll loop
  struct pollfd fds[1];
  fds[0].fd = timer->_fd;
  fds[0].events = (POLLIN | POLLERR);

  while (!this->Exit())
  {
    int ret = poll(fds, 1, 100);
    if (ret > 0 && (fds[0].revents == POLLIN))
    {
      ret = read(fds[0].fd, &ticks, sizeof(ticks));
      if (ret > 0)
      {
        this->_ticks += ticks;
        timer->Notify(this->_ticks);
      }
    }
  }

}

//**********************************************************************
// Class: Timer
//**********************************************************************

Timer::Timer() :
    zEvent::Event(zEvent::Event::TYPE_TIMER), _fd(0), _thread(&this->_timer_func, this),
        _interval(0)
{

  this->RegisterEvent(this);

  // Create timer
  this->_fd = timerfd_create(CLOCK_REALTIME, O_NONBLOCK);
  if (this->_fd <= 0)
  {
    this->_fd = 0;
    return;
  } // end if

  // Start timer monitor thread
  this->_thread.Start();

  this->_lock.Unlock();
}

Timer::~Timer()
{
  int stat = 0;
  struct itimerspec its = { 0 };

  // Stop timer
  this->Stop();

  // Kill timer monitor thread
  this->_thread.Stop();

  // Delete timer
  if (this->_fd)
  {
    close(this->_fd);
  } // end if

  // Unregister event
  this->UnregisterEvent(this);

  // Wait for lock to be available or timeout
  this->_lock.TimedLock(100);
}

void
Timer::Start(uint32_t usec_)
{
  if (this->_fd && this->_lock.Lock())
  {
    this->_interval = usec_;
    this->_start();
    this->_lock.Unlock();
  } // end if
}

void
Timer::Stop(void)
{
  if (this->_fd && this->_lock.Lock())
  {
    this->_stop();
    this->_lock.Unlock();
  } // end if
}

void
Timer::Notify(uint64_t ticks_)
{
  TimerNotification notification(this);
  notification.tick(ticks_);
  zEvent::Event::Notify(&notification);
  return;
}

void
Timer::_start()
{

  // Compute time
  struct itimerspec its = { 0 };
  _add_time(&its.it_value, this->_interval);
  _add_time(&its.it_interval, this->_interval);

  // Start timer
  if (this->_fd)
  {
    timerfd_settime(this->_fd, 0, &its, NULL);
  }

  return;

}

void
Timer::_stop()
{
  struct itimerspec its = { 0 };

  // Stop timer
  if (this->_fd)
  {
    timerfd_settime(this->_fd, 0, &its, NULL);
  } // end if

}

}
}
