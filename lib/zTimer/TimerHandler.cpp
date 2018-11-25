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

#include <zutils/zEvent.h>
#include <zutils/zTimer.h>

#define NTIMER_MAX   8

namespace zUtils
{
namespace zTimer
{

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
      SHARED_PTR(Notification) n(h->nq.Front());
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
  this->_notification_thread.Start();
}

Handler::~Handler()
{
  this->_timer_thread.Stop();
  this->_notification_thread.Stop();
}

bool
Handler::RegisterEvent(Timer* timer_)
{
  bool status = false;
  int ntimer = this->getEvents().size();

  // Stop timer handler thread so the timer fd gets added to the poll list
  this->_timer_thread.Stop();

  // Register event with handler
  if (timer_ && timer_->GetId() && !this->_timers.count(timer_->GetId()))
  {
    this->_timers[timer_->GetId()] = timer_;
    status = zEvent::Handler::RegisterEvent(timer_);
    ntimer = this->getEvents().size();
  }

  // Conditionally start handler thread so the timer fd gets added
  // to the poll list Note: this needs to be done outside critical section
  if (ntimer > 0)
  {
    this->_timer_thread.Start();
  }

  return (status);
}

bool
Handler::UnregisterEvent(Timer* timer_)
{
  bool status = false;
  int ntimer = this->getEvents().size();

  // Stop timer handler thread so the timer fd gets added to the poll list
  this->_timer_thread.Stop();

  // Unregister event with handler
  if (timer_ && timer_->GetId() && this->_timers.count(timer_->GetId()))
  {
    status = zEvent::Handler::UnregisterEvent(timer_);
    this->_timers.erase(timer_->GetId());
    ntimer = this->getEvents().size();
  }

  // Conditionally start handler thread so the timer fd gets added
  // to the poll list Note: this needs to be done outside critical section
  if (ntimer > 0)
  {
    this->_timer_thread.Start();
  }

  return (status);
}

std::list<Timer*>
Handler::getTimers()
{
  std::list<Timer*> timers;
  FOREACH(auto& timer, this->_timers)
  {
    timers.push_back(timer.second);
  }
  return (timers);
}

void
Handler::Run(zThread::ThreadArg *arg_)
{

  struct pollfd fds[NTIMER_MAX] = { 0 };
  int fd_cnt = 0;
  std::list<Timer*> timers(this->getTimers());

  // Setup for poll loop
  FOREACH (auto& timer, timers)
  {
    fds[fd_cnt].fd = timer->GetId();
    fds[fd_cnt].events = (POLLIN | POLLERR);
    fd_cnt++;
  }

  while (!this->Exit())
  {

    // Wait on file descriptor set
    int ret = poll(fds, fd_cnt, 100);

    if (ret == 0)
    {
      continue;
    }
    else if (ret > 0)
    {
      for (int i = 0; i < fd_cnt; i++)
      {
        if (fds[i].revents == POLLIN)
        {
          uint64_t ticks = 0;
          ret = read(fds[i].fd, &ticks, sizeof(ticks));
          if (ret > 0 && this->_timers.count(fds[i].fd))
          {
            Timer* t = this->_timers[fds[i].fd];
            SHARED_PTR(Notification) n(new Notification(*t));
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
    }
    else
    {
      fprintf(stderr, "Timer handler encountered a poll error: %d\n", ret);
    }

  }

  return;
}

}
}
