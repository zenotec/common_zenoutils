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

#include <unistd.h>
#include <poll.h>

#include <list>
#include <vector>

#include <zutils/zCompatibility.h>
#include <zutils/zEvent.h>
#include <zutils/zTimer.h>

namespace zUtils
{
namespace zTimer
{

static std::vector<struct pollfd>
_getfds(std::list<SHPTR(Timer)> timers_)
{

}

//**********************************************************************
// Class: zTimer::NotificationThread
//**********************************************************************

void
NotificationThread::Run(zThread::ThreadArg *arg_)
{

  Handler* h = (Handler*)arg_;

  while (!this->Exit())
  {
    if (h->nq.TimedWait(100))
    {
      SHPTR(zEvent::Notification) n(h->nq.Front());
      h->nq.Pop();
      h->notifyObservers(n);
    }
  }

}

//**********************************************************************
// Class: zTimer::Handler
//**********************************************************************

Handler::Handler() :
    _timer_thread(this, this), _notification_thread(&_notification_func, this)
{
  this->_lock.Unlock();
  this->_notification_thread.Start();
  this->_timer_thread.Start();
}

Handler::~Handler()
{
  this->_timer_thread.Stop();
  this->_notification_thread.Stop();
  this->_lock.Lock();
  this->_timers.clear();
}

bool
Handler::RegisterEvent(SHPTR(Timer) timer_)
{
  bool status = false;

  // Register event with handler
  if (this->_lock.Lock())
  {
    if (timer_ && timer_->GetFd() && !this->_timers.count(timer_->GetFd()))
    {
      this->_timers[timer_->GetFd()] = timer_;
      status = zEvent::Handler::RegisterEvent(timer_.get());
      this->_reload.Post();
    }
    this->_lock.Unlock();
  }

  return (status);
}

bool
Handler::UnregisterEvent(SHPTR(Timer) timer_)
{
  bool status = false;

  // Unregister event with handler
  if (this->_lock.Lock())
  {
    if (timer_ && timer_->GetFd() && this->_timers.count(timer_->GetFd()))
    {
      status = zEvent::Handler::UnregisterEvent(timer_.get());
      this->_timers.erase(timer_->GetFd());
      this->_reload.Post();
    }
    this->_lock.Unlock();
  }

  return (status);
}

void
Handler::Run(zThread::ThreadArg *arg_)
{

  while (!this->Exit())
  {

    std::vector<struct pollfd> fds;

    // Begin critical section
    if (!this->_lock.Lock())
    {
      this->Exit(true);
      break;
    }

    // Add the reload semaphore to the list of file descriptors to poll
    struct pollfd reload = { 0 };
    reload.fd = this->_reload.GetFd();
    reload.events = (POLLIN || POLLERR);
    fds.push_back(reload);

    // Add all known timers to the list of file descriptors to poll
    FOREACH (auto& timer, this->_timers)
    {
      struct pollfd fd = { 0 };
      fd.fd = timer.second->GetFd();
      fd.events = (POLLIN | POLLERR);
      fds.push_back(fd);
    }

    // End critical section
    this->_lock.Unlock();

    // Wait on file descriptor set
    int ret = poll(fds.data(), fds.size(), 100);

    if (ret == 0)
    {
      continue;
    }
    else if (ret > 0)
    {
      FOREACH (auto& fd, fds)
      {
        if (fd.fd == this->_reload.GetFd() && (fd.revents == POLLIN))
        {
          this->_reload.GetCount(); // need to read the counter to clear
        }
        else if (this->_timers.count(fd.fd) && (fd.revents == POLLIN))
        {
          uint64_t ticks = 0;
          ret = read(fd.fd, &ticks, sizeof(ticks));
          if (ret > 0)
          {
            SHPTR(Timer) t(this->_timers[fd.fd]);
            SHPTR(Notification) n(new Notification(*t));
            this->nq.Push(n);
          }
        }
      }
    }
    // TODO: Debug code, remove when no longer needed
    else if (ret == -EINTR)
    {
      // A signal interrupted poll; exit flag should be set
      fprintf(stderr, "Poll interrupted by signal\n");
      continue;
    }
    else
    {
      fprintf(stderr, "BUG: Timer handler encountered a poll error: %d\n", ret);
      continue;
    }

  }

  return;
}

}
}
