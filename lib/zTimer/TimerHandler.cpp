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
Handler::RegisterTimer(Timer* timer_)
{
  bool status = false;
  int ntimer = 0;

  if (timer_ && timer_->_fd && this->_lock.Lock())
  {
    if (this->_timer_list.size() < NTIMER_MAX)
    {
      status = this->RegisterEvent(timer_);
      this->_timer_list[timer_->_fd] = timer_;
      ntimer = this->_timer_list.size();
    }
    this->_lock.Unlock();
  }

  // Conditionally stop/start handler thread so the timer fd gets added
  // to the poll list Note: this needs to be done outside critical section
  this->_thread.Stop();
  if (ntimer > 0)
  {
    this->_thread.Start();
  }

  return (status);
}

bool
Handler::UnregisterTimer(Timer* timer_)
{
  bool status = false;
  int ntimer = 0;

  if (timer_ && this->_lock.Lock())
  {
    if (this->_timer_list.count(timer_->_fd))
    {
      status = this->UnregisterEvent(timer_);
      this->_timer_list.erase(timer_->_fd);
    }
    ntimer = this->_timer_list.size();
    this->_lock.Unlock();
  }

  // Conditionally stop/start handler thread so the timer fd gets removed
  // from the poll list Note: this needs to be done outside critical section
  this->_thread.Stop();
  if (ntimer > 0)
  {
    this->_thread.Start();
  }

  return (status);
}

void
Handler::Run(zThread::ThreadArg *arg_)
{

  int fd_cnt = 0;
  struct pollfd fds[NTIMER_MAX] = { 0 };

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

  while (!this->Exit())
  {

    // Wait on file descriptor set
    // Note: returns immediately if a signal is received
    int ret = poll(fds, fd_cnt, 100);

    switch (ret)
    {
    case 0:
      // Poll timed out
      break;
    case -EINTR:
      // A signal interrupted poll; exit flag should be set
      fprintf(stderr, "Poll interrupted by signal\n"); // TODO: Debug code, remove when no longer needed
      break;
    default:
    {
      if (ret < 0)
      {
        fprintf(stderr, "Timer handler encountered a poll error: %d\n", ret);
      }
      else
      {
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
              t->notifyHandlers(noti);
            }
          }
        }
      }
      break;
    }
    }
  }

  return;
}

}
}
