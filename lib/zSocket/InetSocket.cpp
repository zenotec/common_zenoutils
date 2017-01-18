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

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <unistd.h>
#include <errno.h>
#include <sys/time.h>
#include <sys/types.h>
#include <sys/poll.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <ifaddrs.h>

#include <sstream>

#include <string>
#include <list>
#include <mutex>
#include <memory>

#include <zutils/zLog.h>
#include <zutils/zSem.h>
#include <zutils/zThread.h>
#include <zutils/zQueue.h>
#include <zutils/zEvent.h>
#include <zutils/zSocket.h>

#include <zutils/zInetSocket.h>

namespace zUtils
{
namespace zSocket
{

static std::string
_sock2ip(struct sockaddr_in& sockaddr_)
{
  char ip[INET_ADDRSTRLEN] = { 0 };
  std::string str;

  if ((char*) &ip == inet_ntop( AF_INET, &sockaddr_.sin_addr, (char*) ip, INET_ADDRSTRLEN))
  {
    str = std::string(ip);
  }

  return (str);
}

static in_port_t
_sock2port(struct sockaddr_in& sockaddr_)
{
  return (ntohs(sockaddr_.sin_port));
}

static std::string
_sock2portstr(struct sockaddr_in& sockaddr_)
{
  char port[10] = { 0 };
  snprintf(port, 10, "%u", _sock2port(sockaddr_));
  std::string str(port);
  return (port);
}

static std::string
_addr2ip(const std::string& addr_)
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
_addr2port(const std::string& addr_)
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
_addr2sock(const std::string& addr_, struct sockaddr_in& sockaddr_)
{

  bool status = false;
  uint32_t port = 0;
  memset((void *) &sockaddr_, 0, sizeof(struct sockaddr_in));

  sockaddr_.sin_family = AF_INET;
  int ret = inet_pton( AF_INET, _addr2ip(addr_).c_str(), &sockaddr_.sin_addr);
  if (ret == 1)
  {
    if ((sscanf(_addr2port(addr_).c_str(), "%u", &port) == 1) && (port < 0xffff))
    {
      sockaddr_.sin_port = htons(port);
      status = true;
    }
    else
    {
      ZLOG_ERR("Error converting address to Port: '" + addr_ + "'");
    }
  }
  else
  {
    ZLOG_ERR("Error converting address to IP: '" + addr_ + "'");
  }

  return (status);
}

static bool
_sock2addr(struct sockaddr_in& sockaddr_, std::string& addr_)
{
  bool status = false;
  struct sockaddr_in sockaddr;
  std::string addr = _sock2ip(sockaddr_) + ":" + _sock2portstr(sockaddr_);
  if (_addr2sock(addr, sockaddr))
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
    zSocket::SocketAddress(SocketType::TYPE_INET, addr_)
{
}

InetAddress::InetAddress(SocketAddress &addr_) :
    SocketAddress(addr_)
{
}

InetAddress::InetAddress(const SocketAddress &addr_) :
    SocketAddress(addr_)
{
}

InetAddress::~InetAddress()
{
}

bool
InetAddress::verify(const SocketType type_, const std::string &addr_)
{
  struct sockaddr_in sockaddr;
  return ((type_ == SocketType::TYPE_INET) && (_addr2sock(addr_, sockaddr)));
}

//**********************************************************************
// zSocket::InetSocketRecv Class
//**********************************************************************

void
InetSocketRecv::Run(zThread::ThreadArg *arg_)
{
  InetSocket *sock = (InetSocket *) arg_;
  ssize_t bytes = -1;

  if (!sock || !sock->_sock)
  {
    ZLOG_CRIT(std::string("Socket not opened"));
    return;
  }

  ZLOG_DEBUG("Waiting for RX data on INET socket");

  // Setup for poll loop
  struct pollfd fds[1];
  fds[0].fd = sock->_sock;
  fds[0].events = (POLLIN | POLLERR);

  while (!this->Exit())
  {

    // Poll for received data
    int ret = poll(fds, 1, 100);
    if (ret > 0 && (fds[0].revents == POLLIN))
    {
      ZLOG_INFO("Received packet on socket: " + ZLOG_INT(sock->_sock));
      std::shared_ptr<InetAddress> addr(new InetAddress);
      std::shared_ptr<SocketBuffer> sb(new SocketBuffer);
      bytes = sock->_recv(*addr, *sb);
      if (bytes > 0)
      {
        SocketAddressBufferPair p(addr, sb);
        sock->rxbuf(p);
      }
    } // end if

    if (ret < 0)
    {
      ZLOG_ERR("Error selecting on socket: " + std::string(strerror(errno)));
    } // end if

  }

  return;

}

//**********************************************************************
// zSocket::InetSocketSend Class
//**********************************************************************

void
InetSocketSend::Run(zThread::ThreadArg *arg_)
{
  InetSocket *sock = (InetSocket *) arg_;
  SocketAddressBufferPair p;

  if (!sock || !sock->_sock)
  {
    ZLOG_CRIT(std::string("Socket not opened"));
    return;
  }

  ZLOG_DEBUG("Waiting for TX data on INET socket");

  // Setup for poll loop
  struct pollfd fds[1];
  fds[0].fd = sock->_sock;
  fds[0].events = (POLLOUT | POLLERR);

  while (!this->Exit())
  {

    // Wait for data to send
    if (sock->txbuf(p, 100))
    {
      int ret = poll(fds, 1, 100);
      if (ret > 0 && (fds[0].revents == POLLOUT))
      {
        ZLOG_DEBUG("Sending packet: " + p.first->Address() +
            "(" + ZLOG_INT(p.second->Size()) + ")");
        if (sock->_send(*p.first, *p.second) != p.second->Size())
        {
          ZLOG_ERR("Error sending packet");
        }
      }
    }

  }

  return;

}

//**********************************************************************
// zSocket::InetSocket Class
//**********************************************************************

InetSocket::InetSocket() :
    Socket(SocketType::TYPE_INET), _rx_thread(&this->_rx_func, this),
        _tx_thread(&this->_tx_func, this), _sock(0)
{
}

InetSocket::~InetSocket()
{
  this->Close();
}

bool
InetSocket::Open()
{

  if (this->Address().Type() != SocketType::TYPE_INET)
  {
    ZLOG_CRIT(std::string("Invalid socket address"));
    return (false);
  }

  if (!this->_sock)
  {

    // Create a AF_INET socket
    this->_sock = socket( AF_INET, (SOCK_DGRAM | SOCK_NONBLOCK), IPPROTO_UDP);
    if (this->_sock < 0)
    {
      ZLOG_CRIT("Cannot create socket: " + std::string(strerror(errno)));
      return (false);
    } // end if

    ZLOG_DEBUG("Opening socket: " + ZLOG_INT(this->_sock));

    // Enable sending to broadcast address
    int bcastEnable = 1;
    if (setsockopt(this->_sock, SOL_SOCKET, SO_BROADCAST, &bcastEnable, sizeof(bcastEnable))
        < 0)
    {
      ZLOG_CRIT("Cannot configure socket: " + std::string(strerror(errno)));
      return (false);
    }

  } // end if
  else
  {
    ZLOG_WARN("Socket already open");
  }
  return (true);
}

void
InetSocket::Close()
{
  ZLOG_DEBUG("Closing socket: " + ZLOG_INT(this->_sock));
  // Close socket
  if (this->_sock)
  {
    if (this->_rx_thread.Stop() && this->_tx_thread.Stop())
    {
      ZLOG_DEBUG("Socket Closed: " + ZLOG_INT(this->_sock));
      close(this->_sock);
      this->_sock = 0;
    } // end if
  }
  else
  {
    ZLOG_WARN("Socket not open");
  }
}

bool
InetSocket::Bind()
{

  ZLOG_DEBUG("Bind on socket: " + ZLOG_INT(this->_sock));

  if (!this->_sock)
  {
    ZLOG_CRIT(std::string("Socket not opened"));
    return (false);
  }

  if (this->Address().Type() != SocketType::TYPE_INET)
  {
    ZLOG_CRIT(std::string("Invalid socket address"));
    return (false);
  }

  // Convert string notation address to sockaddr_un
  struct sockaddr_in addr =
      { 0 };
  if (!_addr2sock(this->Address().Address(), addr))
  {
    ZLOG_CRIT("Cannot convert socket address: " + std::string(strerror(errno)));
    return (false);
  }

  // Bind address to socket
  int ret = bind(this->_sock, (struct sockaddr*) &addr, sizeof(addr));
  if (ret < 0)
  {
    ZLOG_CRIT("Cannot bind socket: " + this->Address().Address() +
        ": " + std::string(strerror(errno)));
    return (false);
  } // end if

  // Start listener threads
  if (!this->_rx_thread.Start() || !this->_tx_thread.Start())
  {
    ZLOG_ERR("Error starting listening threads");
    return (false);
  }

  return (true);

}

bool
InetSocket::Connect(const SocketAddress& addr_)
{

  ZLOG_DEBUG("Connect on socket: " + ZLOG_INT(this->_sock));

  if (!this->_sock)
  {
    ZLOG_CRIT(std::string("Socket not opened"));
    return (false);
  }

  if (this->Address().Type() != SocketType::TYPE_INET)
  {
    ZLOG_CRIT(std::string("Invalid socket address"));
    return (false);
  }

  if (addr_.Type() != SocketType::TYPE_INET)
  {
    ZLOG_CRIT(std::string("Invalid socket address type"));
    return (false);
  }

  // Convert string notation address to sockaddr_un
  struct sockaddr_in addr =
      { 0 };
  if (!_addr2sock(addr_.Address(), addr))
  {
    ZLOG_CRIT("Cannot convert socket address: " + std::string(strerror(errno)));
    return (false);
  }

  // Connect to target address
  int ret = connect(this->_sock, (struct sockaddr*) &addr, sizeof(addr));
  if (ret < 0)
  {
    ZLOG_CRIT("Cannot connect socket: " + std::string(strerror(errno)));
    return (false);
  } // end if

  // Start listener threads
  if (!this->_rx_thread.Start() || !this->_tx_thread.Start())
  {
    ZLOG_ERR("Error starting listening threads");
    return (false);
  }

  return (true);

}

ssize_t
InetSocket::_recv(zSocket::InetAddress & addr_, zSocket::SocketBuffer & sb_)
{

  ssize_t n = -1;

  if (!this->_sock)
  {
    ZLOG_CRIT(std::string("Socket not opened"));
    return (-1);
  }

  if (this->Address().Type() != SocketType::TYPE_INET)
  {
    ZLOG_CRIT(std::string("Invalid socket address"));
    return (-1);
  }

  struct sockaddr_in src = { 0 };
  socklen_t len = sizeof(src);

  n = recvfrom(this->_sock, sb_.Head(), sb_.TotalSize(), 0, (struct sockaddr *) &src, &len);
  if (n > 0)
  {
    sb_.Put(n);
    std::string addr;
    if (_sock2addr(src, addr))
    {
      if (!addr_.Type(SocketType::TYPE_INET) || !addr_.Address(addr))
      {
        ZLOG_WARN("Error occurred setting source address");
      }
    }

    std::string logstr;
    logstr += "Receiving on socket:\t";
    logstr += "To: " + this->Address().Address() + ";\t";
    logstr += "From: " + addr_.Address() + ";\t";
    logstr += "Size: " + ZLOG_INT(n) + ";";
    ZLOG_INFO(logstr);

  } // end if

  return (n);
}

ssize_t
InetSocket::_send(const zSocket::InetAddress &addr_, zSocket::SocketBuffer &sb_)
{

  ssize_t n = -1;

  if (!this->_sock)
  {
    ZLOG_CRIT(std::string("Socket not opened"));
    return (-1);
  }

  if (this->Address().Type() != SocketType::TYPE_INET)
  {
    ZLOG_CRIT(std::string("Invalid socket address"));
    return (-1);
  }

  if (addr_.Type() != SocketType::TYPE_INET)
  {
    ZLOG_CRIT(std::string("Invalid destination address type"));
    return (-1);
  }

  // Log info message about message being sent
  std::string logstr;
  logstr += "Sending on socket:\t";
  logstr += "To: " + addr_.Address() + ";\t";
  logstr += "From: " + this->Address().Address() + ";\t";
  logstr += "Size: " + ZLOG_INT(sb_.Size()) + ";";
  ZLOG_INFO(logstr);

  struct sockaddr_in dst =
      { 0 };
  if (!_addr2sock(addr_.Address(), dst))
  {
    ZLOG_CRIT("Cannot convert socket address: " + std::string(strerror(errno)));
    return (-1);
  }

  n = sendto(this->_sock, sb_.Head(), sb_.Size(), 0, (struct sockaddr *) &dst, sizeof(dst));
  if (n < 0)
  {
    ZLOG_ERR(std::string("Cannot send packet: " + std::string(strerror(errno))));
  }

  return (n);

}

}
}
