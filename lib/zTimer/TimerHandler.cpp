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

//**********************************************************************
// Class: zTimer::NotificationThread
//**********************************************************************

void
NotificationThread::Run(zThread::ThreadArg *arg_)
{

  bool exit = false;
  Handler* h = (Handler*)arg_;

  this->RegisterFd(h->nq.GetFd(), (POLLIN | POLLERR));

  while (!exit)
  {

    std::vector<struct pollfd> fds;

    // Wait on file descriptor set
    int ret = this->Poll(fds);

    FOREACH (auto& fd, fds)
    {
      if (this->IsExitFd(fd))
      {
        exit = true;
        continue;
      }
      else if (this->IsReloadFd(fd))
      {
        continue;
      }
      else if ((h->nq.GetFd() == fd.fd) && (fd.revents == POLLIN))
      {
        if (h->nq.TryWait())
        {
          SHPTR(zEvent::Notification) n(h->nq.Front());
          h->nq.Pop();
          h->notifyObservers(n);
        }
      }
    }

  }

  this->UnregisterFd(h->nq.GetFd());

  return;

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
      this->RegisterFd(timer_->GetFd(), (POLLIN | POLLERR));
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
      this->UnregisterFd(timer_->GetFd());
    }
    this->_lock.Unlock();
  }

  return (status);
}

void
Handler::Run(zThread::ThreadArg *arg_)
{

  bool exit = false;

  while (!exit)
  {

    std::vector<struct pollfd> fds;

    // Wait on file descriptor set
    int ret = this->Poll(fds);

    FOREACH (auto& fd, fds)
    {
      if (this->IsExitFd(fd))
      {
        exit = true;
        continue;
      }
      else if (this->IsReloadFd(fd))
      {
        continue;
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

  return;

}

}
}
