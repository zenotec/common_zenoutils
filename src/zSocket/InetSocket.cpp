//*****************************************************************************
//    Copyright (C) 2014 ZenoTec LLC (http://www.zenotec.net)
//
//    File: Inet.cpp
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
_sock2ip(struct sockaddr_in* sockaddr_)
{
  char ip[INET_ADDRSTRLEN] = { 0 };
  std::string str;

  if ((char*) &ip == inet_ntop( AF_INET, &sockaddr_->sin_addr, (char*) ip, INET_ADDRSTRLEN))
  {
    str = std::string(ip);
  }

  return (str);
}

static in_port_t
_sock2port(struct sockaddr_in* sockaddr_)
{
  return (ntohs(sockaddr_->sin_port));
}

static std::string
_sock2portstr(struct sockaddr_in* sockaddr_)
{
  char port[10] = { 0 };
  snprintf(port, 10, "%u", _sock2port(sockaddr_));
  std::string str(port);
  return (port);
}

static std::string
_addr2ip(const SocketAddress *addr_)
{
  int pos = 0;
  int npos = 0;
  std::string addr;
  std::string str;

  addr = addr_->Address();
  if (!addr.empty())
  {
    npos = addr.find_first_of(":");
    if (npos != addr.npos)
    {
      str = addr.substr(pos, npos);
    }
  }

  return (str);
}

static std::string
_addr2port(const SocketAddress *addr_)
{
  int pos = 0;
  std::string addr;
  std::string str;

  addr = addr_->Address();
  if (!addr.empty())
  {
    pos = addr.find_first_of(":");
    if (pos != addr.npos)
    {
      str = addr.substr(++pos, addr.npos);
    }
  }

  return (str);
}

static bool
_addr2sock(const SocketAddress *addr_, struct sockaddr_in *sockaddr_)
{

  in_port_t port = 0;
  memset((void *) sockaddr_, 0, sizeof(struct sockaddr_in));

  sockaddr_->sin_family = AF_INET;
  inet_pton( AF_INET, _addr2ip(addr_).c_str(), &sockaddr_->sin_addr);
  sscanf(_addr2port(addr_).c_str(), "%hu", &port);
  sockaddr_->sin_port = htons(port);

  return (true);
}

static bool
_sock2addr(struct sockaddr_in *sockaddr_, SocketAddress *addr_)
{
  std::string addr = _sock2ip(sockaddr_) + ":" + _sock2portstr(sockaddr_);
  return (addr_->Address(addr));
}

//**********************************************************************
// InetAddress Class
//**********************************************************************

InetAddress::InetAddress(const std::string &ifname_) :
    zSocket::SocketAddress(SocketType::TYPE_INET)
{

  struct ifaddrs *iflist, *ifaddr = 0;

  if (!ifname_.empty())
  {
    if (getifaddrs(&iflist) == 0)
    {
      for (ifaddr = iflist; ifaddr; ifaddr = ifaddr->ifa_next)
      {
        ZLOG_INFO("zSocket::InetAddress: Found interface: " + std::string(ifaddr->ifa_name));
        if ((strcmp(ifaddr->ifa_name, ifname_.c_str()) == 0)
            && (ifaddr->ifa_addr->sa_family == AF_INET))
        {
          _sock2addr((struct sockaddr_in *) ifaddr->ifa_addr, this);
          break;
        }
      }
    }
    if (!ifaddr)
    {
      std::string errMsg = std::string("zSocket::InetAddress: Interface '");
      errMsg += ifname_ + std::string("' does not exist");
      ZLOG_CRIT(errMsg);
      throw(errMsg);
    }
    freeifaddrs(iflist);
    return;
  }
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
InetAddress::verify(SocketType type_, const std::string &addr_)
{
  struct sockaddr_in sockaddr;
  return ((type_ == SocketType::TYPE_INET) && (_addr2sock(this, &sockaddr)));
}

//**********************************************************************
// zSocket::InetSocketRecv Class
//**********************************************************************

void *
InetSocketRecv::ThreadFunction(void *arg_)
{
  InetSocket *sock = (InetSocket *) arg_;
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

  return (0);

}

//**********************************************************************
// zSocket::InetSocketSend Class
//**********************************************************************

void *
InetSocketSend::ThreadFunction(void *arg_)
{
  InetSocket *sock = (InetSocket *) arg_;

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

  if (!this->_sock)
  {

    // Create a AF_INET socket
    this->_sock = socket( AF_INET, (SOCK_DGRAM | SOCK_NONBLOCK), IPPROTO_UDP);
    if (this->_sock < 0)
    {
      ZLOG_CRIT("Cannot create socket: " + std::string(strerror(errno)));
      return (false);
    } // end if

    ZLOG_DEBUG("Opening socket: " + zLog::IntStr(this->_sock));

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
  ZLOG_DEBUG("Closing socket: " + zLog::IntStr(this->_sock));
  // Close socket
  if (this->_sock && this->_rx_thread.Join() && this->_tx_thread.Join())
  {
    ZLOG_DEBUG("Socket Closed: " + zLog::IntStr(this->_sock));
    close(this->_sock);
    this->_sock = 0;
  } // end if
}

bool
InetSocket::Bind()
{

  if (!this->_sock)
  {
    ZLOG_CRIT(std::string("Socket not opened"));
    return (false);
  }

  ZLOG_DEBUG("Bind on socket: " + zLog::IntStr(this->_sock));

  // Convert string notation address to sockaddr_in
  struct sockaddr_in addr = { 0 };
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
InetSocket::Connect()
{

  if (!this->_sock)
  {
    ZLOG_CRIT(std::string("Socket not opened"));
    return (false);
  }

  ZLOG_DEBUG("Connect on socket: " + zLog::IntStr(this->_sock));

  // Convert string notation address to sockaddr_in
  struct sockaddr_in addr = { 0 };
  if (!_addr2sock(this->Address(), &addr))
  {
    ZLOG_CRIT("Cannot convert socket address: " + std::string(strerror(errno)));
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
InetSocket::_recv(zSocket::InetAddress &addr_, zSocket::SocketBuffer &sb_)
{
  if (!this->_sock)
  {
    ZLOG_CRIT(std::string("Socket not opened"));
    return (false);
  }

  if (addr_.Type() != SocketType::TYPE_INET)
  {
    ZLOG_CRIT(std::string("Invalid address type"));
    return (false);
  }

  struct sockaddr_in src = { 0 };
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
InetSocket::_send(const zSocket::InetAddress &addr_, zSocket::SocketBuffer &sb_)
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

  struct sockaddr_in src = { 0 };
  if (!_addr2sock(&addr_, &src))
  {
    ZLOG_CRIT("Cannot convert socket address: " + std::string(strerror(errno)));
    return (-1);
  }
  socklen_t slen = sizeof(struct sockaddr_in);

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
