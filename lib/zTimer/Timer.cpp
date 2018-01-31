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

Timer::Timer(const std::string& name_) :
    zEvent::Event(zEvent::Event::TYPE_TIMER), _fd(0), _interval(0), _name(name_)
{

  // Create timer
  this->_fd = timerfd_create(CLOCK_REALTIME, O_NONBLOCK);
  if (this->_fd <= 0)
  {
    this->_fd = 0;
  } // end if

  fprintf(stderr, "(%d) Creating timer: %s\n", this->_fd, this->_name.c_str());

  this->_lock.Unlock();
}

Timer::~Timer()
{

  fprintf(stderr, "(%d) Destroying timer: %s\n", this->_fd, this->_name.c_str());

  // Make sure the timer is stopped
  this->Stop();

  this->_lock.Lock();

  // Make sure the timer is unregistered from all handlers
  if (!this->_handler_list.empty())
  {
    fprintf(stderr, "BUG: Timer registered with handler, not closing FD\n");
  }
  else
  {
  // Delete timer
  if (this->_fd)
  {
    close(this->_fd);
    this->_fd = 0;
  } // end if
  }
}

bool
Timer::Start(uint32_t usec_)
{

  fprintf(stderr, "(%d) Starting timer: %s\n", this->_fd, this->_name.c_str());

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

  fprintf(stderr, "(%d) Stopping timer: %s\n", this->_fd, this->_name.c_str());

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
  return (uint32_t(this->_fd)); // read only
}

uint64_t
Timer::GetTicks() const
{
  uint64_t ticks = 0;
  if (this->_fd && this->_lock.Lock())
  {
    if (read(this->_fd, &ticks, sizeof(ticks) != sizeof(ticks)))
    {
      ticks = 0;
    }
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
