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

namespace zUtils
{
namespace zTimer
{

//**********************************************************************
// Class: Handler
//**********************************************************************

Handler::Handler() :
    _thread(this, NULL)
{
  this->_lock.Unlock();
}

Handler::~Handler()
{
  this->_thread.Stop();
  this->_lock.Lock();
}

bool
Handler::RegisterTimer(Timer* event_)
{
  bool status = false;
  bool empty = true;
  if (event_ && this->_lock.Lock())
  {
    status = this->RegisterEvent(event_);
    this->_timer_list[event_->_fd] = event_;
    empty = this->_timer_list.empty();
    this->_lock.Unlock();
  }

  // Conditionally start handler thread
  if (!empty)
  {
    this->_thread.Start();
  }
  return (status);
}

bool
Handler::UnregisterTimer(Timer* event_)
{
  bool status = false;
  bool empty = false;
  if (event_ && this->_lock.Lock())
  {
    if (this->_timer_list.count(event_->_fd))
    {
      status = this->UnregisterEvent(event_);
      this->_timer_list.erase(event_->_fd);
    }
    empty = this->_timer_list.empty();
    this->_lock.Unlock();
  }

  // Conditionally stop handler thread
  if (empty)
  {
    this->_thread.Stop();
  }
  return (status);
}

void
Handler::Run(zThread::ThreadArg *arg_)
{

  struct pollfd fds[32] = { 0 };

  while (!this->Exit())
  {
    int fd_cnt = 0;
    if (this->_lock.Lock())
    {
      // Setup for poll loop
      FOREACH (auto& t, this->_timer_list)
      {
        fds[fd_cnt].fd = t.first;
        fds[fd_cnt].events = (POLLIN | POLLERR);
        fd_cnt++;
      }
      this->_lock.Unlock();
    }

    int ret = poll(fds, fd_cnt, 100);
    if (ret <= 0)
    {
      continue;
    }

    for (int i = 0; i < fd_cnt; i++)
    {
      if (fds[i].revents == POLLIN)
      {
        uint64_t ticks = 0;
        ret = read(fds[i].fd, &ticks, sizeof(ticks));
        if (ret > 0 && this->_timer_list.count(fds[i].fd))
        {
          Timer* t = this->_timer_list[fds[i].fd];
          SHARED_PTR(Notification) noti(new Notification(*t));
          t->NotifyHandlers(noti);
        }
      }
    }

  }


}

}
}
