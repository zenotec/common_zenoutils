//*****************************************************************************
//    Copyright (C) 2014 ZenoTec LLC (http://www.zenotec.net)
//
//    File: Unix.cpp
//    Description:
//
//*****************************************************************************

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <unistd.h>
#include <errno.h>
#include <sys/time.h>
#include <sys/types.h>
#include <sys/poll.h>
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
_addr2sock(const SocketAddress *addr_, struct sockaddr_un *sockaddr_)
{
  memset((void *) sockaddr_, 0, sizeof(struct sockaddr_un));
  sockaddr_->sun_family = AF_UNIX;
  strncpy(sockaddr_->sun_path, addr_->Address().c_str(), 108);
  return (true);
}

static bool
_sock2addr(struct sockaddr_un *sockaddr_, SocketAddress *addr_)
{
  addr_->Address(std::string(sockaddr_->sun_path));
  return (true);
}

//**********************************************************************
// UnixAddress Class
//**********************************************************************

UnixAddress::UnixAddress() :
    SocketAddress(SocketType::TYPE_UNIX)
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
UnixAddress::verify(SocketType type_, const std::string &addr_)
{
  struct sockaddr_un sockaddr;
  return ((type_ == SocketType::TYPE_UNIX) && (_addr2sock(this, &sockaddr)));
}

//**********************************************************************
// zSocket::UnixSocketRecv Class
//**********************************************************************

void *
UnixSocketRecv::ThreadFunction(void *arg_)
{
  UnixSocket *sock = (UnixSocket *) arg_;
  ssize_t bytes = -1;

  if (!sock->_sock)
  {
    ZLOG_CRIT(std::string("Socket not opened"));
    return ((void*) -1);
  }

  // Setup for poll loop
  struct pollfd fds[1];
  fds[0].fd = sock->_sock;
  fds[0].events = (POLLIN | POLLERR);
  int ret = poll(fds, 1, 100);
  if (ret > 0 && (fds[0].revents == POLLIN))
  {
    ZLOG_INFO("Received packet on socket: " + zLog::IntStr(sock->_sock));
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

  return (0);

}

//**********************************************************************
// zSocket::UnixSocketSend Class
//**********************************************************************

void *
UnixSocketSend::ThreadFunction(void *arg_)
{
  UnixSocket *sock = (UnixSocket *) arg_;

  ZLOG_DEBUG("TX: Polling socket for data");

  // Setup for poll loop
  struct pollfd fds[1];
  fds[0].fd = sock->_sock;
  fds[0].events = (POLLOUT | POLLERR);
  int ret = poll(fds, 1, 100);
  if (ret > 0 && (fds[0].revents == POLLOUT))
  {
    SocketAddressBufferPair p;
    if (sock->txbuf(p, 100000))
    {
      ZLOG_DEBUG(
          "Sending packet: " + p.first->Address() + "(" + zLog::IntStr(p.second->Size()) + ")");
      if (sock->_send(*p.first, *p.second) != p.second->Size())
      {
        ZLOG_ERR("Error sending packet");
      }
    }
  }

  return (0);

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

  if (this->Address() == NULL)
  {
    ZLOG_ERR("Failed to open Unix Socket, address not set.");
    return (false);
  }

  if (!this->_sock)
  {

    // Create a AF_UNIX socket
    this->_sock = socket( AF_UNIX, (SOCK_DGRAM | SOCK_NONBLOCK), IPPROTO_UDP);
    if (this->_sock < 0)
    {
      ZLOG_CRIT("Cannot create socket: " + std::string(strerror(errno)));
      return (false);
    } // end if

    ZLOG_DEBUG("Opening socket: " + zLog::IntStr(this->_sock));

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
  ZLOG_DEBUG("Closing socket: " + zLog::IntStr(this->_sock));
  // Close socket
  if (this->_sock)
  {
    if (this->_rx_thread.Join() && this->_tx_thread.Join())
    {
      ZLOG_DEBUG("Socket Closed: " + zLog::IntStr(this->_sock));
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
UnixSocket::Bind()
{

  if (!this->_sock)
  {
    ZLOG_CRIT(std::string("Socket not opened"));
    return (false);
  }

  ZLOG_DEBUG("Bind on socket: " + zLog::IntStr(this->_sock));

  // Convert string notation address to sockaddr_un
  struct sockaddr_un addr = { 0 };
  if (!_addr2sock(this->Address(), &addr))
  {
    ZLOG_CRIT("Cannot convert socket address: " + std::string(strerror(errno)));
    return (false);
  }

  // Override address to always listen on all interfaces
//  addr.sin_addr.s_addr = INADDR_ANY;

  // Bind address to socket
  int ret = bind(this->_sock, (struct sockaddr*) &addr, sizeof(addr));
  if (ret < 0)
  {
    ZLOG_CRIT("Cannot bind socket: " + this->Address()->Address() +
        ": " + std::string(strerror(errno)));
    return (false);
  } // end if

  if (!this->_rx_thread.Run() || !this->_tx_thread.Run())
  {
    ZLOG_ERR("Error starting listening threads");
    return (false);
  }

  return (true);

}

bool
UnixSocket::Connect()
{

  if (!this->_sock)
  {
    ZLOG_CRIT(std::string("Socket not opened"));
    return (false);
  }

  ZLOG_DEBUG("Connect on socket: " + zLog::IntStr(this->_sock));

  struct sockaddr_un addr = { 0 };
  if (!_addr2sock(this->Address(), &addr))
  {
    ZLOG_CRIT("Cannot convert socket address: "
        + std::string(strerror(errno)));
    return (false);
  }

  int ret = connect(this->_sock, (struct sockaddr*) &addr, sizeof(addr));
  if (ret < 0)
  {
    ZLOG_CRIT("Cannot connect socket: " + std::string(strerror(errno)));
    return (false);
  } // end if

  if (!this->_rx_thread.Run() || !this->_tx_thread.Run())
  {
    ZLOG_ERR("Error starting listening threads");
    return (false);
  }

  return (true);

}

ssize_t
UnixSocket::_recv(zSocket::UnixAddress &addr_, zSocket::SocketBuffer &sb_)
{
  if (!this->_sock)
  {
    ZLOG_CRIT(std::string("Socket not opened"));
    return (false);
  }

  if (this->Address()->Type() != SocketType::TYPE_UNIX)
  {
    ZLOG_CRIT(std::string("Invalid address type"));
    return (false);
  }

  struct sockaddr_un src = { 0 };
  socklen_t len = sizeof(src);

  int n = recvfrom(this->_sock, sb_.Head(), sb_.TotalSize(), 0, (struct sockaddr *) &src, &len);
  if (n > 0)
  {
    sb_.Put(n);
    _sock2addr(&src, &addr_);

    std::string logstr;
    logstr += "Receiving on socket:\t";
    logstr += "To: " + this->Address()->Address() + ";\t";
    logstr += "From: " + addr_.Address() + ";\t";
    logstr += "Size: " + zLog::IntStr(n) + ";";
    ZLOG_INFO(logstr);

  } // end if

  return (n);
}

ssize_t
UnixSocket::_send(const zSocket::UnixAddress &addr_, zSocket::SocketBuffer &sb_)
{

  if (!this->_sock)
  {
    ZLOG_CRIT(std::string("Socket not opened"));
    return (-1);
  }

  // Log info message about message being sent
  std::string logstr;
  logstr += "Sending on socket:\t";
  logstr += "To: " + addr_.Address() + ";\t";
  logstr += "From: " + this->Address()->Address() + ";\t";
  logstr += "Size: " + zLog::IntStr(sb_.Size()) + ";";
  ZLOG_INFO(logstr);

  struct sockaddr_un src = { 0 };
  if (!_addr2sock(&addr_, &src))
  {
    ZLOG_CRIT("Cannot convert socket address: " + std::string(strerror(errno)));
    return (-1);
  }
  socklen_t slen = sizeof(struct sockaddr_un);

  ssize_t bytes_sent = sendto(this->_sock, sb_.Head(), sb_.Size(), 0, (struct sockaddr *) &src,
      slen);
  if (bytes_sent < 0)
  {
    ZLOG_ERR(std::string("Cannot send packet: " + std::string(strerror(errno))));
  }

  return (bytes_sent);

}

}
}
