/*
 * Copyright (c) 2014-2018 ZenoTec LLC (http://www.zenotec.net)
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
#include <errno.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/ioctl.h>
#include <sys/un.h>
#include <netinet/in.h>
#include <poll.h>

#include <zutils/zLog.h>
#include <zutils/zSocket.h>
#include <zutils/zUnixSocket.h>

ZLOG_MODULE_INIT(zUtils::zLog::Log::MODULE_SOCKET);

namespace zUtils
{
namespace zSocket
{

//**********************************************************************
// Class: zSocket::UnixSocketRx
//**********************************************************************

void
UnixSocketRx::Run(zThread::ThreadArg *arg_)
{

  bool exit = false;
  class UnixSocket* sock = (class UnixSocket*)arg_;

  // Setup for poll loop
  this->RegisterFd(sock->fd, (POLLIN | POLLERR));

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
      else if ((fd.fd == sock->fd) && (fd.revents == POLLIN))
      {
        sock->rxq.Push(sock->recv());
      }
    }

  }

  this->UnregisterFd(sock->fd);

  return;

}

//**********************************************************************
// Class: zSocket::UnixSocketTx
//**********************************************************************

void
UnixSocketTx::Run(zThread::ThreadArg *arg_)
{

  bool exit = false;
  class UnixSocket* sock = (class UnixSocket*)arg_;

  // Setup for poll loop
  this->RegisterFd(sock->txq.GetFd(), (POLLIN | POLLERR));

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
      else if ((sock->txq.GetFd() == fd.fd) && (fd.revents == POLLIN))
      {
        if (sock->txq.TryWait())
        {
          SHARED_PTR(zSocket::Notification) n(sock->txq.Front());
          sock->txq.Pop();
          sock->rxq.Push(sock->send(n));
        }
      }
    }

  }

  this->UnregisterFd(sock->txq.GetFd());

  return;

}

//**********************************************************************
// Class: zSocket::UnixSocket
//**********************************************************************

UnixSocket::UnixSocket() :
    Socket(SOCKET_TYPE::TYPE_UNIX),
    _rxthread(&_rxfunc, this),
    _txthread(&_txfunc, this),
    fd(0)
{
  this->Open();
}

UnixSocket::~UnixSocket()
{
  this->Close();
}

bool
UnixSocket::Open()
{

  bool status = false;

  if (this->fd)
  {
    ZLOG_WARN(std::string("Socket already open: " + ZLOG_INT(this->GetFd())));
    return (true);
  }

  // Create a AF_UNIX socket
  this->fd = socket( AF_UNIX, (SOCK_DGRAM | SOCK_NONBLOCK), 0);
  if (this->fd > 0)
  {
    ZLOG_INFO("Socket created: " + ZLOG_INT(this->GetFd()));
  }
  else
  {
    this->fd = 0;
    ZLOG_ERR("Cannot create socket: " + std::string(strerror(errno)));
  }

  return (status);

}

bool
UnixSocket::Bind(const Address& addr_)
{

  bool status = false;

  if (!this->fd)
  {
    ZLOG_ERR(std::string("Socket not opened"));
    return (false);
  }

  if (addr_.GetType() != Address::TYPE_UNIX)
  {
    ZLOG_CRIT(std::string("Invalid socket address"));
    return (false);
  }

  UnixAddress addr(addr_);
  struct sockaddr_un sa(addr.GetSA());

  // Bind address to socket
  int ret = bind(this->fd, (struct sockaddr*) &sa, sizeof(sa));
  if (ret < 0)
  {
    ZLOG_CRIT("Cannot bind socket: " + addr_.GetAddress() + ": " + std::string(strerror(errno)));
    return (false);
  } // end if

  ZLOG_INFO("Bind on socket: [" + ZLOG_INT(this->GetFd()) + "] " + addr_.GetAddress());

  if (this->_rxthread.Start() && this->_txthread.Start())
  {
    status = this->SetAddress(addr_);
  }

  return (status);
}

bool
UnixSocket::Close()
{

  bool status = false;

  if (!this->fd)
  {
    ZLOG_ERR(std::string("Socket already closed"));
    return (true);
  }

  UnixAddress addr = UnixAddress(this->GetAddress());
  struct sockaddr_un sa(addr.GetSA());

  if (this->_rxthread.Stop() && this->_txthread.Stop())
  {
    if (close(this->fd) == 0)
    {
      ZLOG_INFO("Closed socket: " + ZLOG_INT(this->GetFd()));
      unlink(addr.GetSA().sun_path);
      this->fd = 0;
      status = true;
    }
    else
    {
      ZLOG_CRIT("Cannot close socket: " + this->GetAddress().GetAddress() + ": " + std::string(strerror(errno)));
    }
  }

  return (status);

}

SHARED_PTR(zSocket::Notification)
UnixSocket::recv()
{

  SHARED_PTR(zSocket::Notification) n(new zSocket::Notification(*this));
  ssize_t nbytes = 0;

  if (this->fd)
  {
    // Query for the number of bytes ready to be read for use creating socket buffer
    ioctl(this->fd, FIONREAD, &nbytes);
    if (nbytes)
    {

      struct sockaddr_un src;
      socklen_t len = sizeof(src);
      SHARED_PTR(Buffer) sb(new Buffer(nbytes));

      nbytes = recvfrom(this->fd, sb->Head(), sb->TotalSize(), 0, (struct sockaddr *) &src, &len);
      if ((nbytes > 0) && sb->Put(nbytes))
      {
        struct timespec ts = { 0 };
        ioctl(this->fd, SIOCGSTAMPNS, &ts);
        sb->Timestamp(ts);
        n->SetSubType(Notification::SUBTYPE_PKT_RCVD);
        n->SetDstAddress(SHARED_PTR(UnixAddress)(new UnixAddress(this->GetAddress())));
        n->SetSrcAddress(SHARED_PTR(UnixAddress)(new UnixAddress(src)));
        n->SetBuffer(sb);
        // NOTE: frame is initialized by optional adapter socket
        ZLOG_DEBUG("(" + ZLOG_INT(this->GetFd()) + ") " + "Received " + ZLOG_INT(nbytes) +
            " bytes from: " + n->GetSrcAddress()->GetAddress());
      }
      else
      {
        n->SetSubType(Notification::SUBTYPE_PKT_ERR);
        ZLOG_ERR(std::string("Cannot receive packet: " + std::string(strerror(errno))));
      }
    }
  }

  return (n);

}

SHARED_PTR(zSocket::Notification)
UnixSocket::send(SHARED_PTR(zSocket::Notification) n_)
{

  // Initialize notification
  UnixAddress addr(*n_->GetDstAddress());
  zSocket::Buffer sb(*n_->GetBuffer());

  // Send
  struct sockaddr_un dst(addr.GetSA());
  ssize_t nbytes = sendto(this->fd, sb.Head(), sb.Size(), 0, (struct sockaddr *) &dst, sizeof(dst));
  if (nbytes > 0)
  {
    ZLOG_DEBUG("(" + ZLOG_INT(this->GetFd()) + ") " + "Sent " + ZLOG_INT(sb.Length()) +
        " bytes to: " + addr.GetAddress());
    n_->SetSubType(Notification::SUBTYPE_PKT_SENT);
  }
  else
  {
    n_->SetSubType(Notification::SUBTYPE_PKT_ERR);
    ZLOG_ERR(std::string("Cannot send packet: " + std::string(strerror(errno))));
    ZLOG_DEBUG("(" + ZLOG_INT(this->GetFd()) + ") Sent 0 bytes to: " + addr.GetAddress());
  }

  return (n_);

}

}
}
