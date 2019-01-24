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

// libc includes

#include <unistd.h>
#include <errno.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/ioctl.h>
#include <linux/sockios.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <poll.h>

// libc++ includes

// libzutils includes

#include <zutils/zLog.h>
#include <zutils/zSocket.h>
#include <zutils/zUdpSocket.h>

ZLOG_MODULE_INIT(zUtils::zLog::Log::MODULE_SOCKET);

namespace zUtils
{
namespace zSocket
{

//**********************************************************************
// Class: zSocket::UdpSocketRx
//**********************************************************************

void
UdpSocketRx::Run(zThread::ThreadArg *arg_)
{

  bool exit = false;
  class UdpSocket* sock = (class UdpSocket*)arg_;

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
// Class: zSocket::UdpSocketTx
//**********************************************************************

void
UdpSocketTx::Run(zThread::ThreadArg *arg_)
{

  bool exit = false;
  class UdpSocket* sock = (class UdpSocket*)arg_;

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
// zSocket::InetSocket Class
//**********************************************************************

UdpSocket::UdpSocket() :
    Socket(SOCKET_TYPE::TYPE_INET4),
    _rxthread(&_rxfunc, this),
    _txthread(&_txfunc, this),
    fd(0)

{
  // Create a AF_INET socket
  this->fd = socket( AF_INET, (SOCK_DGRAM | SOCK_NONBLOCK), IPPROTO_UDP);
  if (this->fd > 0)
  {
    ZLOG_INFO("Socket created: " + ZLOG_INT(this->fd));
  }
  else
  {
    this->fd = 0;
    ZLOG_ERR("Cannot create socket: " + std::string(strerror(errno)));
  }
}

UdpSocket::~UdpSocket()
{

  this->_rxthread.Stop();
  this->_txthread.Stop();

  // Make sure the socket is unregistered from all handlers
  if (!this->_handler_list.empty())
  {
    fprintf(stderr, "BUG: Socket registered with handler, not closing FD\n");
  }
  else
  {
    // Close socket
    ZLOG_INFO("Closing socket: " + ZLOG_INT(this->fd));
    if (this->fd)
    {
      close(this->fd);
      this->fd = 0;
    } // end if
  }
}

bool
UdpSocket::Getopt(Socket::OPTIONS opt_)
{
  bool status = false;
  switch (opt_)
  {
  case Socket::OPTIONS_ALLOW_REUSE:
  {
    int optval = 0;
    socklen_t optlen = sizeof(optval);
    if (getsockopt(this->fd, SOL_SOCKET, SO_REUSEADDR, &optval, &optlen) < 0)
    {
      ZLOG_ERR("Cannot get socket option: " + std::string(strerror(errno)));
    }
    else
    {
      std::cerr << "OPTIONS_ALLOW_REUSE: " << optval << std::endl;
      status = (optval == 0);
    }
    break;
  }
  case Socket::OPTIONS_ALLOW_BCAST:
  {
    int optval = 0;
    socklen_t optlen = sizeof(optval);
    if (getsockopt(this->fd, SOL_SOCKET, SO_BROADCAST, &optval, &optlen) < 0)
    {
      ZLOG_ERR("Cannot get socket option: " + std::string(strerror(errno)));
    }
    else
    {
      std::cerr << "OPTIONS_ALLOW_BCAST: " << optval << std::endl;
      status = (optval != 0);
    }
    break;
  }
  case Socket::OPTIONS_TOS_UHP:
    // No break
  case Socket::OPTIONS_TOS_HP:
    // No break
  case Socket::OPTIONS_TOS_NP:
    // No break
  case Socket::OPTIONS_TOS_LP:
  {
    int optval = 0;
    socklen_t optlen = sizeof(optval);
    if (getsockopt(this->fd, SOL_IP, IP_TOS, &optval, &optlen) < 0)
    {
      ZLOG_CRIT("Cannot get socket option: " + std::string(strerror(errno)));
    }
    else
    {
      if (opt_ == Socket::OPTIONS_TOS_UHP)
        status = (optval == 48);
      else if (opt_ == Socket::OPTIONS_TOS_HP)
        status = (optval == 32);
      else if (opt_ == Socket::OPTIONS_TOS_NP)
        status = (optval == 16);
      else if (opt_ == Socket::OPTIONS_TOS_LP)
        status = (optval == 0);
    }
    break;
  }
  default:
    break;
  }
  return (status);
}

bool
UdpSocket::Setopt(Socket::OPTIONS opt_)
{
  bool status = false;
  switch (opt_)
  {
  case Socket::OPTIONS_ALLOW_REUSE:
  {
    // Enable reuse of socket
    int optval = 0;
    if (setsockopt(this->fd, SOL_SOCKET, SO_REUSEADDR, &optval, sizeof(optval)) < 0)
    {
      ZLOG_CRIT("Cannot set socket option: " + std::string(strerror(errno)));
    }
    else
    {
      status = true;
    }
    break;
  }
  case Socket::OPTIONS_ALLOW_BCAST:
  {
    // Enable sending to broadcast address
    int optval = 1;
    if (setsockopt(this->fd, SOL_SOCKET, SO_BROADCAST, &optval, sizeof(optval)) < 0)
    {
      ZLOG_CRIT("Cannot set socket option: " + std::string(strerror(errno)));
    }
    else
    {
      status = true;
    }
    break;
  }

  case Socket::OPTIONS_TOS_UHP:
    // No break
  case Socket::OPTIONS_TOS_HP:
    // No break
  case Socket::OPTIONS_TOS_NP:
    // No break
  case Socket::OPTIONS_TOS_LP:
  {
    int optval = 0;
    if (opt_ == Socket::OPTIONS_TOS_UHP)
      optval = 0xb8;
    else if (opt_ == Socket::OPTIONS_TOS_HP)
      optval = 0x78;
    else if (opt_ == Socket::OPTIONS_TOS_NP)
      optval = 0x38;
    else if (opt_ == Socket::OPTIONS_TOS_LP)
      optval = 0x00;

    if (setsockopt(this->fd, SOL_IP, IP_TOS, &optval, sizeof(optval)) < 0)
    {
      ZLOG_CRIT("Cannot set socket option: " + std::string(strerror(errno)));
    }
    else
    {
      status = true;
    }
    break;
  }

  default:
    break;
  }
  return (status);
}

bool
UdpSocket::Bind(const Address& addr_)
{

  if (!this->fd)
  {
    ZLOG_ERR(std::string("Socket not opened"));
    return (false);
  }

  if (addr_.GetType() != Address::TYPE_IPv4)
  {
    ZLOG_CRIT(std::string("Invalid socket address"));
    return (false);
  }

  Ipv4Address addr = Ipv4Address(addr_);
  struct sockaddr_in sa(addr.GetSA());

  // Bind address to socket
  int ret = bind(this->fd, (struct sockaddr*) &sa, sizeof(struct sockaddr_in));
  if (ret < 0)
  {
    ZLOG_CRIT("Cannot bind socket: " + this->GetAddress().GetAddress() + ": " + std::string(strerror(errno)));
    return (false);
  } // end if

  ZLOG_INFO("Bind on socket: " + ZLOG_INT(this->fd));

  this->_rxthread.Start();
  this->_txthread.Start();

  return (this->SetAddress(addr_));

}

SHARED_PTR(zSocket::Notification)
UdpSocket::recv()
{

  SHARED_PTR(zSocket::Notification) n(new zSocket::Notification(*this));
  ssize_t nbytes = 0;

  if (this->fd)
  {
    // Query for the number of bytes ready to be read for use creating socket buffer
    ioctl(this->fd, FIONREAD, &nbytes);
    if (nbytes)
    {
      struct sockaddr_in src;
      socklen_t len = sizeof(src);
      SHARED_PTR(Buffer) sb(new Buffer(nbytes));

      nbytes = recvfrom(this->fd, sb->Head(), sb->TotalSize(), 0, (struct sockaddr *) &src, &len);
      if ((nbytes > 0) && sb->Put(nbytes))
      {
        struct timespec ts = { 0 };
        ioctl(this->fd, SIOCGSTAMPNS, &ts);
        sb->Timestamp(ts);
        n->SetSubType(Notification::SUBTYPE_PKT_RCVD);
        n->SetDstAddress(SHARED_PTR(Ipv4Address)(new Ipv4Address(this->GetAddress())));
        n->SetSrcAddress(SHARED_PTR(Ipv4Address)(new Ipv4Address(src)));
        n->SetBuffer(sb);
        // NOTE: frame is initialized by optional adapter socket
        ZLOG_DEBUG("(" + ZLOG_INT(this->fd) + ") " + "Received " + ZLOG_INT(nbytes) +
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
UdpSocket::send(SHARED_PTR(zSocket::Notification) n_)
{
  // Initialize notification
  SHARED_PTR(Ipv4Address) addr(new Ipv4Address(*n_->GetDstAddress()));
  zSocket::Buffer sb(*n_->GetBuffer());

  // Send
  struct sockaddr_in dst(addr->GetSA());
  ssize_t nbytes = sendto(this->fd, sb.Head(), sb.Size(), 0, (struct sockaddr *) &dst, sizeof(dst));
  if (nbytes > 0)
  {
    ZLOG_DEBUG("(" + ZLOG_INT(this->fd) + ") " + "Sent " + ZLOG_INT(sb.Length()) +
        " bytes to: " + addr->GetAddress());
    n_->SetSubType(Notification::SUBTYPE_PKT_SENT);
  }
  else
  {
    n_->SetSubType(Notification::SUBTYPE_PKT_ERR);
    ZLOG_ERR(std::string("Cannot send packet: " + std::string(strerror(errno))));
  }

  return (n_);

}

}
}
