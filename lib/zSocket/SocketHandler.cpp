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
Handler::RegisterEvent(Socket* sock_)
{
  bool status = false;
  int nsock = 0;

  // Always stop thread before registering socket
  this->_thread.Stop();

  if (sock_ && this->_lock.Lock())
  {
    ZLOG_INFO("Registering socket: " + ZLOG_UINT(sock_->GetFd()));
    if (zEvent::Handler::RegisterEvent(sock_))
    {
      this->_smap[sock_->GetFd()] = sock_;
      nsock = this->_smap.size();
      status = true;
    }
    this->_lock.Unlock();
  }

  // Conditionally stop/start handler thread so the socket fd gets added
  // to the poll list Note: this needs to be done outside critical section
  if (nsock > 0)
  {
    this->_thread.Start();
  }

  return (status);
}

bool
Handler::UnregisterEvent(Socket* sock_)
{
  bool status = false;
  int nsock = 0;

  // Always stop thread before unregistering socket
  this->_thread.Stop();

  if (sock_ && this->_lock.Lock())
  {
    ZLOG_INFO("Unregistering socket: " + ZLOG_UINT(sock_->GetFd()));
    if (zEvent::Handler::UnregisterEvent(sock_))
    {
      this->_smap.erase(sock_->GetFd());
      nsock = this->_smap.size();
      status = true;
    }
    this->_lock.Unlock();
  }

  // Conditionally start handler thread so the socket fd gets removed
  // from the poll list Note: this needs to be done outside critical section
  if (nsock > 0)
  {
    this->_thread.Start();
  }

  return (status);
}

void
Handler::Run(zThread::ThreadArg *arg_)
{

  int fd_cnt = 0;
  struct pollfd fds[NSOCK_MAX] = { 0 };

  // Setup for polling sockets receive queue
  if (this->_lock.Lock())
  {
    FOREACH (auto& t, this->_smap)
    {
      fds[fd_cnt].fd = t.second->GetFd();
      fds[fd_cnt].events = (POLLIN | POLLERR);
      fd_cnt++;
    }
    this->_lock.Unlock();
  }

  while (!this->Exit())
  {

    // Wait on file descriptor set
    // Note: returns immediately when a signal is received
    int ret = poll(fds, fd_cnt, 100);

    if (ret > 0)
    {
      for (int i = 0; i < fd_cnt; i++)
      {
        int fd = fds[i].fd;
        if (fds[i].revents == POLLIN)
        {
          if (this->_smap.count(fd) && this->_smap[fd])
          {
            Socket* sock = this->_smap[fd];
            SHPTR(zEvent::Notification) n(sock->Recv());
            sock->notifyHandlers(n);
          }
          else
          {
            fprintf(stderr, "[%d] BUG: Unexpected socket received data: %d\n", __LINE__, fd);
          }
        }
      }
    }
    else if (ret == 0)
    {
      continue;
    }
    else
    {
      if (errno == -EINTR)
      {
        // A signal interrupted poll; exit flag should be set
        fprintf(stderr, "Poll interrupted by signal\n"); // TODO: Debug code, remove when no longer needed
        ZLOG_INFO("Socket poll interrupted by signal");
      }
      else
      {
        fprintf(stderr, "Socket handler encountered a poll error: %d\n", ret);
        ZLOG_INFO("Socket handler encountered a poll error: " + ZLOG_INT(ret));
      }
    }
  }

  return;

}


}
}
