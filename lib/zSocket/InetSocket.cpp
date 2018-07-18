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
#include <zutils/zInetSocket.h>

ZLOG_MODULE_INIT(zUtils::zLog::Log::MODULE_SOCKET);

namespace zUtils
{
namespace zSocket
{

static std::string
_sa2ip(const struct sockaddr_in& sa_)
{
  char ip[INET_ADDRSTRLEN] = { 0 };
  std::string str;

  if ((char*) &ip == inet_ntop( AF_INET, &sa_.sin_addr, (char*) ip, INET_ADDRSTRLEN))
  {
    str = std::string(ip);
  }

  return (str);
}

static in_port_t
_sa2port(const struct sockaddr_in& sa_)
{
  return (ntohs(sa_.sin_port));
}

static std::string
_sa2portstr(const struct sockaddr_in& sa_)
{
  char port[10] = { 0 };
  snprintf(port, 10, "%u", _sa2port(sa_));
  std::string str(port);
  return (port);
}

static std::string
_str2ip(const std::string& addr_)
{
  int pos = 0;
  int npos = 0;
  std::string str;

  if (!addr_.empty())
  {
    npos = addr_.find_first_of(":");
    if (npos != addr_.npos)
    {
      str = addr_.substr(pos, npos);
    }
  }

  return (str);
}

static std::string
_str2port(const std::string& addr_)
{
  int pos = 0;
  std::string str;

  if (!addr_.empty())
  {
    pos = addr_.find_first_of(":");
    if (pos != addr_.npos)
    {
      str = addr_.substr(++pos, addr_.npos);
    }
  }

  return (str);
}

static bool
_addr2sa(const std::string& addr_, struct sockaddr_in& sa_)
{

  bool status = false;
  struct sockaddr_in sa = { 0 };
  uint32_t port = 0;

  sa.sin_family = AF_INET;
  int ret = inet_pton( AF_INET, _str2ip(addr_).c_str(), &sa.sin_addr);
  if (ret == 1)
  {
    if ((sscanf(_str2port(addr_).c_str(), "%u", &port) == 1) && (port < 0xffff))
    {
      sa.sin_port = htons(port);
      sa_ = sa;
      status = true;
    }
  }

  return (status);
}

static bool
_sa2addr(const struct sockaddr_in& sa_, std::string& addr_)
{
  bool status = false;
  struct sockaddr_in sockaddr;
  std::string addr = _sa2ip(sa_) + ":" + _sa2portstr(sa_);
  if (_addr2sa(addr, sockaddr))
  {
    addr_ = addr;
    status = true;
  }
  return (status);
}

//**********************************************************************
// Class: zSocket::InetAddress
//**********************************************************************

InetAddress::InetAddress(const std::string &addr_) :
    Address(SOCKET_TYPE::TYPE_INET4, addr_), sa { 0 }
{
  _addr2sa(addr_, this->sa);
}

InetAddress::InetAddress(const Address &addr_) :
    Address(SOCKET_TYPE::TYPE_INET4), sa { 0 }
{
  if (addr_.GetType() == this->GetType())
  {
    this->SetAddress(addr_.GetAddress());
  }
}

InetAddress::InetAddress(const struct sockaddr_in& sa_) :
        Address(SOCKET_TYPE::TYPE_INET4), sa (sa_)
{
  this->SetAddress(this->GetAddress());
}

InetAddress::~InetAddress()
{
}

std::string
InetAddress::GetAddress() const
{
  std::string addr;
  _sa2addr(this->sa, addr);
  return (addr);
}

bool
InetAddress::SetAddress(const std::string& addr_)
{
  bool status = false;
  if (_addr2sa(addr_, this->sa))
  {
    status = Address::SetAddress(addr_);
  }
  return (status);
}

void
InetAddress::Display() const
{
  Address::Display();
  std::cout << "----------------- INET4 Address -----------------" << std::endl;
  std::cout << "Family: \t" << this->sa.sin_family << std::endl;
  std::cout << "IP:     \t" << _sa2ip(this->sa) << std::endl;
  std::cout << "Port:   \t" << _sa2portstr(this->sa) << std::endl;
}

//**********************************************************************
// zSocket::InetSocket Class
//**********************************************************************

InetSocket::InetSocket() :
    Socket(SOCKET_TYPE::TYPE_INET4)
{
  // Create a AF_INET socket
  this->_fd = socket( AF_INET, (SOCK_DGRAM | SOCK_NONBLOCK), IPPROTO_UDP);
  if (this->_fd > 0)
  {
    ZLOG_INFO("Socket created: " + ZLOG_INT(this->_fd));
  }
  else
  {
    this->_fd = 0;
    ZLOG_ERR("Cannot create socket: " + std::string(strerror(errno)));
  }
}

InetSocket::~InetSocket()
{
  // Make sure the socket is unregistered from all handlers
  if (!this->_handler_list.empty())
  {
    fprintf(stderr, "BUG: Socket registered with handler, not closing FD\n");
  }
  else
  {
    // Close socket
    ZLOG_INFO("Closing socket: " + ZLOG_INT(this->_fd));
    if (this->_fd)
    {
      close(this->_fd);
      this->_fd = 0;
    } // end if
  }
}

int
InetSocket::GetId() const
{
  return (this->_fd);
}

const Address&
InetSocket::GetAddress() const
{
  return (this->_addr);
}

bool
InetSocket::Getopt(Socket::OPTIONS opt_)
{
  bool status = false;
  switch (opt_)
  {
  case Socket::OPTIONS_ALLOW_REUSE:
  {
    int optval = 0;
    socklen_t optlen = sizeof(optval);
    if (getsockopt(this->_fd, SOL_SOCKET, SO_REUSEADDR, &optval, &optlen) < 0)
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
    if (getsockopt(this->_fd, SOL_SOCKET, SO_BROADCAST, &optval, &optlen) < 0)
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
    if (getsockopt(this->_fd, SOL_IP, IP_TOS, &optval, &optlen) < 0)
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
InetSocket::Setopt(Socket::OPTIONS opt_)
{
  bool status = false;
  switch (opt_)
  {
  case Socket::OPTIONS_ALLOW_REUSE:
  {
    // Enable reuse of socket
    int optval = 0;
    if (setsockopt(this->_fd, SOL_SOCKET, SO_REUSEADDR, &optval, sizeof(optval)) == -1)
    {
      ZLOG_CRIT("Cannot set socket option: " + std::string(strerror(errno)));
    }
    break;
  }
  case Socket::OPTIONS_ALLOW_BCAST:
  {
    // Enable sending to broadcast address
    int optval = 1;
    if (setsockopt(this->_fd, SOL_SOCKET, SO_BROADCAST, &optval, sizeof(optval)) < 0)
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

    if (setsockopt(this->_fd, SOL_IP, IP_TOS, &optval, sizeof(optval)) < 0)
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
InetSocket::Bind(const Address& addr_)
{

  if (!this->_fd)
  {
    ZLOG_ERR(std::string("Socket not opened"));
    return (false);
  }

  if (addr_.GetType() != SOCKET_TYPE::TYPE_INET4)
  {
    ZLOG_CRIT(std::string("Invalid socket address"));
    return (false);
  }

  this->_addr = InetAddress(addr_);

  // Bind address to socket
  int ret = bind(this->_fd, (struct sockaddr*) &this->_addr.sa, sizeof(this->_addr.sa));
  if (ret < 0)
  {
    ZLOG_CRIT("Cannot bind socket: " + this->_addr.GetAddress() + ": " + std::string(strerror(errno)));
    return (false);
  } // end if

  ZLOG_INFO("Bind on socket: " + ZLOG_INT(this->_fd));

  return (true);
}

SHARED_PTR(zSocket::Notification)
InetSocket::Recv()
{

  SHARED_PTR(zSocket::Notification) n(new zSocket::Notification(*this));
  int nbytes = 0;

  // Initialize notification
  n->SetSubType(Notification::SUBTYPE_PKT_ERR);
  n->SetDstAddress(this->GetAddress());

  if (this->_fd)
  {
    // Query for the number of bytes ready to be read for use creating socket buffer
    ioctl(this->_fd, FIONREAD, &nbytes);
    if (nbytes)
    {
      struct sockaddr_in src;
      socklen_t len = sizeof(src);
      Buffer sb(nbytes);
      nbytes = recvfrom(this->_fd, sb.Head(), sb.TotalSize(), 0, (struct sockaddr *) &src, &len);
      if ((nbytes > 0) && sb.Put(nbytes))
      {
        struct timespec ts = { 0 };
        ioctl(this->_fd, SIOCGSTAMPNS, &ts);
        sb.Timestamp(ts);
        InetAddress addr(src);
        n->SetSubType(Notification::SUBTYPE_PKT_RCVD);
        n->SetSrcAddress(addr);
        n->SetBuffer(sb);
        ZLOG_DEBUG("(" + ZLOG_INT(this->_fd) + ") " + "Received " + ZLOG_INT(nbytes) +
            " bytes from: " + addr.GetAddress());
      }
      else
      {
        ZLOG_ERR(std::string("Cannot receive packet: " + std::string(strerror(errno))));
      }
    }
  }

  return (n);
}

SHARED_PTR(zSocket::Notification)
InetSocket::Send(const Address& to_, const Buffer& sb_)
{

  SHARED_PTR(zSocket::Notification) n(new zSocket::Notification(*this));
  ssize_t nbytes = -1;

  // Setup for poll loop
  struct pollfd fds[1];
  fds[0].fd = this->_fd;
  fds[0].events = (POLLOUT | POLLERR);

  // Initialize notification
  n->SetSubType(Notification::SUBTYPE_PKT_ERR);
  n->SetSrcAddress(this->GetAddress());
  n->SetDstAddress(to_);
  n->SetBuffer(sb_);

  int ret = poll(fds, 1, 100);
  if (ret > 0 && (fds[0].revents == POLLOUT))
  {
    InetAddress addr(to_);
    nbytes = sendto(this->_fd, sb_.Head(), sb_.Size(), 0, (struct sockaddr *) &addr.sa, sizeof(addr.sa));
    if (nbytes > 0)
    {
      ZLOG_DEBUG("(" + ZLOG_INT(this->_fd) + ") " + "Sent " + ZLOG_INT(sb_.Length()) +
          " bytes to: " + addr.GetAddress());
      n->SetSubType(Notification::SUBTYPE_PKT_SENT);
    }
    else
    {
      ZLOG_ERR(std::string("Cannot send packet: " + std::string(strerror(errno))));
    }
  }
  else
  {
    fprintf(stderr, "BUG: Timed out polling on TX\n");
    ZLOG_ERR(std::string("Cannot send packet: " + std::string(strerror(errno))));
  }

  return (n);
}

}
}
