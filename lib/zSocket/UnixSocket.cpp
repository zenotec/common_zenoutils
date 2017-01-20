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
#include <poll.h>
#include <sys/socket.h>
#include <sys/un.h>
#include <netinet/in.h>

#include <string>
#include <sstream>
#include <list>
#include <mutex>
#include <memory>

#include <zutils/zLog.h>
#include <zutils/zSem.h>
#include <zutils/zThread.h>
#include <zutils/zQueue.h>
#include <zutils/zEvent.h>

#include <zutils/zSocket.h>
#include <zutils/zUnixSocket.h>

namespace zUtils
{
namespace zSocket
{

static bool
_addr2sock(const std::string &addr_, struct sockaddr_un &sockaddr_)
{
  memset((void *) &sockaddr_, 0, sizeof(struct sockaddr_un));
  sockaddr_.sun_family = AF_UNIX;
  strncpy(sockaddr_.sun_path, addr_.c_str(), (sizeof(sockaddr_.sun_path) - 1));
  return (true);
}

static bool
_sock2addr(struct sockaddr_un &sockaddr_, std::string &addr_)
{
  addr_ = std::string(sockaddr_.sun_path);
  return (true);
}

//**********************************************************************
// UnixAddress Class
//**********************************************************************

UnixAddress::UnixAddress(const std::string &addr_) :
    SocketAddress(SocketType::TYPE_UNIX, addr_)
{
}

UnixAddress::UnixAddress(SocketAddress &addr_) :
    SocketAddress(addr_)
{
}

UnixAddress::UnixAddress(const SocketAddress &addr_) :
    SocketAddress(addr_)
{
}

UnixAddress::~UnixAddress()
{
}

bool
UnixAddress::verify(const SocketType type_, const std::string &addr_)
{
  struct sockaddr_un sockaddr = { 0 };
  return ((type_ == SocketType::TYPE_UNIX) && (_addr2sock(addr_, sockaddr)));
}

//**********************************************************************
// zSocket::UnixSocketRecv Class
//**********************************************************************

void
UnixSocketRecv::Run(zThread::ThreadArg *arg_)
{
  UnixSocket *sock = (UnixSocket *) arg_;
  ssize_t bytes = -1;

  if (!sock || !sock->_sock)
  {
    ZLOG_CRIT(std::string("Socket not opened"));
    return;
  }

  ZLOG_DEBUG("Waiting for RX data on Unix socket");

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
      std::shared_ptr<UnixAddress> addr(new UnixAddress);
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
// zSocket::UnixSocketSend Class
//**********************************************************************

void
UnixSocketSend::Run(zThread::ThreadArg *arg_)
{

  UnixSocket *sock = (UnixSocket *) arg_;
  SocketAddressBufferPair p;

  if (!sock || !sock->_sock)
  {
    ZLOG_CRIT(std::string("Socket not opened"));
    return;
  }

  ZLOG_DEBUG("Waiting for TX data on Unix socket");

  while (!this->Exit())
  {

    // Setup for poll loop
    struct pollfd fds[1];
    fds[0].fd = sock->_sock;
    fds[0].events = (POLLOUT | POLLERR);

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
// zSocket::UnixSocket Class
//**********************************************************************

UnixSocket::UnixSocket() :
    Socket(SocketType::TYPE_UNIX), _rx_thread(&this->_rx_func, this),
        _tx_thread(&this->_tx_func, this), _sock(0)
{
}

UnixSocket::~UnixSocket()
{
  this->Close();
}

bool
UnixSocket::Open()
{

  if (this->Address().Type() != SocketType::TYPE_UNIX)
  {
    ZLOG_CRIT(std::string("Invalid socket address"));
    return (false);
  }

  if (!this->_sock)
  {

    // Create a AF_UNIX socket
    this->_sock = socket( AF_UNIX, (SOCK_DGRAM | SOCK_NONBLOCK), 0);
    if (this->_sock < 0)
    {
      ZLOG_CRIT("Cannot create socket: " + std::string(strerror(errno)));
      return (false);
    } // end if

    ZLOG_DEBUG("Opening socket: " + ZLOG_INT(this->_sock));

  } // end if
  else
  {
    ZLOG_WARN("Socket already open");
  }
  return (true);
}

void
UnixSocket::Close()
{
  ZLOG_DEBUG("Closing socket: " + ZLOG_INT(this->_sock));

  if (!this->_sock)
  {
    return;
  }

  if (this->Address().Type() != SocketType::TYPE_UNIX)
  {
    return;
  }

  // Convert string notation address to sockaddr_un
  struct sockaddr_un addr = { 0 };
  if (!_addr2sock(this->Address().Address(), addr))
  {
    ZLOG_CRIT("Cannot convert socket address: " + std::string(strerror(errno)));
    return;
  }

  // Close socket
  if (this->_rx_thread.Stop() && this->_tx_thread.Stop())
  {
    close(this->_sock);
    this->_sock = 0;
    unlink(addr.sun_path);
    ZLOG_DEBUG("Socket Closed: " + ZLOG_INT(this->_sock));
  } // end if

}

bool
UnixSocket::Bind()
{

  ZLOG_DEBUG("Bind on socket: " + ZLOG_INT(this->_sock));

  if (!this->_sock)
  {
    ZLOG_CRIT(std::string("Socket not opened"));
    return (false);
  }

  if (this->Address().Type() != SocketType::TYPE_UNIX)
  {
    ZLOG_CRIT(std::string("Invalid socket address"));
    return (false);
  }

  // Convert string notation address to sockaddr_un
  struct sockaddr_un addr = { 0 };
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
UnixSocket::Connect(const SocketAddress& addr_)
{

  ZLOG_DEBUG("Connect on socket: " + ZLOG_INT(this->_sock));

  if (!this->_sock)
  {
    ZLOG_CRIT(std::string("Socket not opened"));
    return (false);
  }

  if (this->Address().Type() != SocketType::TYPE_UNIX)
  {
    ZLOG_CRIT(std::string("Invalid socket address"));
    return (false);
  }

  if (addr_.Type() != SocketType::TYPE_UNIX)
  {
    ZLOG_CRIT(std::string("Invalid socket address type"));
    return (false);
  }

  // Convert string notation address to sockaddr_un
  struct sockaddr_un addr = { 0 };
  if (!_addr2sock(addr_.Address(), addr))
  {
    ZLOG_CRIT("Cannot convert socket address: " + addr_.Address());
    ZLOG_CRIT("Error: " + std::string(strerror(errno)));
    return (false);
  }

  // Connect to target address
  int ret = connect(this->_sock, (struct sockaddr*) &addr, sizeof(addr));
  if (ret < 0)
  {
    ZLOG_CRIT("Cannot connect socket: " + addr_.Address());
    ZLOG_CRIT("Error: " + std::string(strerror(errno)));
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
UnixSocket::_recv(zSocket::UnixAddress &addr_, zSocket::SocketBuffer &sb_)
{

  ssize_t n = -1;

  if (!this->_sock)
  {
    ZLOG_CRIT(std::string("Socket not opened"));
    return (-1);
  }

  if (this->Address().Type() != SocketType::TYPE_UNIX)
  {
    ZLOG_CRIT(std::string("Invalid socket address"));
    return (-1);
  }

  struct sockaddr_un src = { 0 };
  socklen_t len = sizeof(src);

  n = recvfrom(this->_sock, sb_.Head(), sb_.TotalSize(), 0, (struct sockaddr *) &src, &len);
  if (n > 0)
  {
    sb_.Put(n);
    std::string addr;
    if (_sock2addr(src, addr))
    {
      if (!addr_.Type(SocketType::TYPE_UNIX) || !addr_.Address(addr))
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
UnixSocket::_send(const zSocket::UnixAddress &addr_, zSocket::SocketBuffer &sb_)
{

  ssize_t n = -1;

  if (!this->_sock)
  {
    ZLOG_CRIT(std::string("Socket not opened"));
    return (-1);
  }

  if (this->Address().Type() != SocketType::TYPE_UNIX)
  {
    ZLOG_CRIT(std::string("Invalid socket address"));
    return (-1);
  }

  if (addr_.Type() != SocketType::TYPE_UNIX)
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

  struct sockaddr_un dst = { 0 };
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
