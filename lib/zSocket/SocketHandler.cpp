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

#include <poll.h>
#include <errno.h>

#include <zutils/zLog.h>
#include <zutils/zSocket.h>
ZLOG_MODULE_INIT(zUtils::zLog::Log::MODULE_SOCKET);

#define NSOCK_MAX   8

namespace zUtils
{
namespace zSocket
{

//*****************************************************************************
// Class: zSocket::Handler
//*****************************************************************************

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
Handler::RegisterSocket(Socket* sock_)
{
  bool status = false;
  int nsock = 0;

  if (sock_ && sock_->_get_fd() && this->_lock.Lock())
  {
    if (this->_sock_list.size() < NSOCK_MAX)
    {
      ZLOG_INFO("Registering socket: " + ZLOG_P(sock_));
      status = this->RegisterEvent(sock_);
      this->_sock_list[sock_->_get_fd()] = sock_;
      nsock = this->_sock_list.size();
    }
    this->_lock.Unlock();
  }

  // Conditionally stop/start handler thread so the socket fd gets added
  // to the poll list Note: this needs to be done outside critical section
  switch (nsock)
  {
  case 2:
    this->_thread.Stop();
    // no break
  case 1:
    this->_thread.Start();
    break;
  default:
    break;
  }

  return (status);
}

bool
Handler::UnregisterSocket(Socket* sock_)
{
  bool status = false;
  int nsock = 0;

  if (sock_ && this->_lock.Lock())
  {
    if (this->_sock_list.count(sock_->_get_fd()))
    {
      ZLOG_INFO("Unregistering socket: " + ZLOG_P(sock_));
      status = this->UnregisterEvent(sock_);
      this->_sock_list.erase(sock_->_get_fd());
    }
    nsock = this->_sock_list.size();
    this->_lock.Unlock();
  }

  // Conditionally stop/start handler thread so the socket fd gets removed
  // from the poll list Note: this needs to be done outside critical section
  switch (nsock)
  {
  case 2:
    this->_thread.Stop();
    // no break
  case 1:
    this->_thread.Start();
    break;
  default:
    break;
  }

  return (status);
}

void
Handler::Run(zThread::ThreadArg *arg_)
{

  int fd_cnt = 0;
  struct pollfd fds[NSOCK_MAX] = { 0 };

  // Setup for poll loop
  if (this->_lock.Lock())
  {
    FOREACH (auto& t, this->_sock_list)
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
      ZLOG_INFO("Socket poll interrupted by signal");
      break;
    default:
    {
      if (ret < 0)
      {
        fprintf(stderr, "Socket handler encountered a poll error: %d\n", ret);
      }
      else
      {
        for (int i = 0; i < fd_cnt; i++)
        {
          int fd = fds[i].fd;
          if (fds[i].revents == POLLIN)
          {
            if (this->_sock_list.count(fd))
            {
              this->_sock_list[fd]->_recv();
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
