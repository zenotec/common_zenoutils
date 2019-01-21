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
  this->_thread.Start();
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

  // Register event with handler
  if (this->_lock.Lock())
  {
    if (sock_ && sock_->GetFd() && !this->_smap.count(sock_->GetFd()))
    {
      this->_smap[sock_->GetFd()] = sock_;
      status = zEvent::Handler::RegisterEvent(sock_);
      this->RegisterFd(sock_->GetFd(), (POLLIN | POLLERR));
    }
    this->_lock.Unlock();
  }

  return (status);
}

bool
Handler::UnregisterEvent(Socket* sock_)
{
  bool status = false;

  // Unregister event with handler
  if (this->_lock.Lock())
  {
    if (sock_ && sock_->GetFd() && this->_smap.count(sock_->GetFd()))
    {
      status = zEvent::Handler::UnregisterEvent(sock_);
      this->_smap.erase(sock_->GetFd());
      this->UnregisterFd(sock_->GetFd());
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
      else if (this->_smap.count(fd.fd) && (fd.revents == POLLIN))
      {
        Socket* sock = this->_smap[fd.fd];
        SHPTR(zEvent::Notification) n(sock->Recv());
        sock->notifyHandlers(n);
      }
    }

  }

  return;

}


}
}
