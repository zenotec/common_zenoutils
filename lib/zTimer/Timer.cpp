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
// Class: Timer
//**********************************************************************

Timer::Timer() :
    zEvent::Event(zEvent::Event::TYPE_TIMER), _fd(0), _interval(0), _ticks(0)
{

  // Create timer
  this->_fd = timerfd_create(CLOCK_REALTIME, O_NONBLOCK);
  if (this->_fd <= 0)
  {
    this->_fd = 0;
    return;
  } // end if

  this->_lock.Unlock();
}

Timer::~Timer()
{
  // Make sure the timer is stopped
  this->Stop();

  this->_lock.Lock();

  // Delete timer
  if (this->_fd)
  {
    close(this->_fd);
  } // end if
}

bool
Timer::Start(uint32_t usec_)
{
  bool status = false;
  if (this->_lock.Lock())
  {
    this->_interval = usec_;
    status = this->_start();
    this->_lock.Unlock();
  } // end if
  return (status);
}

bool
Timer::Stop(void)
{
  bool status  = false;
  if (this->_lock.Lock())
  {
    status = this->_stop();
    this->_lock.Unlock();
  } // end if
  return (status);
}

uint32_t
Timer::GetId() const
{
  uint32_t id = 0;
  if (this->_lock.Lock())
  {
    id = uint32_t(this->_fd);
    this->_lock.Unlock();
  } // end if
  return (id);
}

uint64_t
Timer::GetTicks() const
{
  uint64_t ticks = 0;
  if (this->_lock.Lock())
  {
    ticks = this->_ticks;
    this->_lock.Unlock();
  } // end if
  return (ticks);
}

bool
Timer::_start()
{
  bool status  = false;
  if (this->_fd)
  {
    // Compute time and set interval time
    struct itimerspec its = { 0 };
    _add_time(&its.it_value, this->_interval);
    _add_time(&its.it_interval, this->_interval);
    status = (timerfd_settime(this->_fd, 0, &its, NULL) == 0);
  }
  return (status);
}

bool
Timer::_stop()
{
  bool status  = false;
  if (this->_fd)
  {
    struct itimerspec its = { 0 };
    status = (timerfd_settime(this->_fd, 0, &its, NULL) == 0);
  }
  return (status);
}

}
}
