//*****************************************************************************
//    Copyright (C) 2014 ZenoTec LLC (http://www.zenotec.net)
//
//    File: Inet.cpp
//    Description:
//
//*****************************************************************************

#include <stdio.h>
#include <stdlib.h>
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

#include <zutils/zLog.h>
#include <zutils/zInet.h>

namespace zUtils
{
namespace zSocket
{

static std::string
_get_ipaddr_string(struct sockaddr_in *sockaddr_)
{
  char ip[INET_ADDRSTRLEN] = { 0 };
  inet_ntop( AF_INET, &sockaddr_->sin_addr, ip, INET_ADDRSTRLEN);
  std::string str(ip);
  return (str);
}

static std::string
_get_port_string(struct sockaddr_in *sockaddr_)
{
  char port[10] = { 0 };
  snprintf(port, 10, "%d", ntohs(sockaddr_->sin_port));
  std::string str(port);
  return (port);
}

static bool
_get_sockaddr(const InetAddress *addr_, struct sockaddr_in *sockaddr_)
{
  memset((void *) sockaddr_, 0, sizeof(struct sockaddr_in));
  sockaddr_->sin_family = AF_INET;
  inet_pton( AF_INET, addr_->GetIpAddr().c_str(), &sockaddr_->sin_addr);
  int port = 0;
  sscanf(addr_->GetPort().c_str(), "%d", &port);
  sockaddr_->sin_port = htons(port);
  return (true);
}

static bool
_get_inetaddress(struct sockaddr_in *sockaddr_, InetAddress *addr_)
{
  addr_->SetIpAddr(_get_ipaddr_string(sockaddr_));
  addr_->SetPort(_get_port_string(sockaddr_));
  return (true);
}

//**********************************************************************
// InetAddress Class
//**********************************************************************

InetAddress::InetAddress() :
    zSocket::SocketAddress(zSocket::SocketAddress::TYPE_INET)
{
  this->SetIpAddr("0.0.0.0");
  this->SetPort("0");
  this->SetNetmask("0.0.0.0");
  this->SetBroadcast("0.0.0.0");
}

InetAddress::InetAddress(const std::string &ifname_) :
    zSocket::SocketAddress(zSocket::SocketAddress::TYPE_INET)
{
  struct ifaddrs *iflist, *ifaddr = 0;
  if (getifaddrs(&iflist) == 0)
  {
    for (ifaddr = iflist; ifaddr; ifaddr = ifaddr->ifa_next)
    {
      ZLOG_INFO("zSocket::InetAddress: Found interface: " + std::string(ifaddr->ifa_name));
      if ((strcmp(ifaddr->ifa_name, ifname_.c_str()) == 0)
          && (ifaddr->ifa_addr->sa_family == AF_INET))
      {
        this->SetIpAddr(_get_ipaddr_string((struct sockaddr_in *) ifaddr->ifa_addr));
        this->SetPort(_get_port_string((struct sockaddr_in *) ifaddr->ifa_addr));
        this->SetNetmask(
            _get_ipaddr_string((struct sockaddr_in *) ifaddr->ifa_netmask));
        this->SetBroadcast(
            _get_ipaddr_string((struct sockaddr_in *) ifaddr->ifa_broadaddr));
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
}

InetAddress::InetAddress(InetAddress &other_)
{
  this->_type = other_._type;
  this->SetAddress(other_.GetAddress());
}

InetAddress::InetAddress(const InetAddress &other_)
{
  this->_type = other_._type;
  this->SetAddress(other_.GetAddress());
}

InetAddress &
InetAddress::operator=(InetAddress &other_)
{
  this->_type = other_._type;
  this->SetAddress(other_.GetAddress());
  return (*this);
}

InetAddress &
InetAddress::operator=(const InetAddress &other_)
{
  this->_type = other_._type;
  this->SetAddress(other_.GetAddress());
  return (*this);
}

InetAddress::~InetAddress()
{
}

bool
InetAddress::operator ==(const InetAddress &other_) const
    {
  bool status = true;
  status = (status && (this->_ipaddr == other_._ipaddr));
  status = (status && (this->_port == other_._port));
  status = (status && (this->_netmask == other_._netmask));
  status = (status && (this->_bcast == other_._bcast));
  return (status);
}

bool
InetAddress::operator !=(const InetAddress &other_) const
    {
  bool status = true;
  status = (status && (this->_ipaddr == other_._ipaddr));
  status = (status && (this->_port == other_._port));
  status = (status && (this->_netmask == other_._netmask));
  status = (status && (this->_bcast == other_._bcast));
  return (!status);
}

bool
InetAddress::operator <(const InetAddress &other_) const
    {
  bool status = false;
  return (status);
}

bool
InetAddress::operator >(const InetAddress &other_) const
    {
  bool status = false;
  return (status);
}

std::string
InetAddress::GetAddress() const
{
  std::string addr = this->_ipaddr + std::string(":") + this->_port;
  return (addr);
}

bool
InetAddress::SetAddress(const std::string &addr_)
{
  bool status = false;
  char *str = 0;
  char *c = 0;

  str = strdup(addr_.c_str());
  if (str != NULL)
  {
    c = strtok(str, ":");
    if ((c != NULL) && this->SetIpAddr(std::string(c)))
    {
      c = strtok( NULL, ":");
      if ((c != NULL) && this->SetPort(std::string(c)))
      {
        status = true;
      }
    }
    free(str);
  }

  return (status);
}

std::string
InetAddress::GetIpAddr() const
{
  return (this->_ipaddr);
}

std::string
InetAddress::GetPort() const
{
  return (this->_port);
}

std::string
InetAddress::GetNetmask() const
{
  return (this->_netmask);
}

std::string
InetAddress::GetBroadcast() const
{
  return (this->_bcast);
}

bool
InetAddress::SetIpAddr(const std::string &ipaddr_)
{
  bool status = true;
  // TODO: Add error checking
  this->_ipaddr = ipaddr_;
  return (status);
}

bool
InetAddress::SetPort(const std::string &port_)
{
  bool status = false;
  int port = 0;
  std::istringstream(port_) >> port;
  if ((port >= 0) && (port < 65536))
  {
    this->_port = port_;
    status = true;
  }
  return (status);
}

bool
InetAddress::SetNetmask(const std::string &netmask_)
{
  bool status = true;
  // TODO: Add error checking
  this->_netmask = netmask_;
  return (status);
}

bool
InetAddress::SetBroadcast(const std::string &bcast_)
{
  bool status = true;
  // TODO: Add error checking
  this->_bcast = bcast_;
  return (status);
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
    SocketAddress addr;
    SocketBuffer sb;
    bytes = sock->_recv(&addr, &sb);
    if (bytes > 0)
    {
      sock->rxq.Push(std::make_pair(addr, sb));
      sock->rx_event.Notify(sock);
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
  ssize_t bytes = -1;

  ZLOG_DEBUG("TX: Polling socket for data");

  // Setup for poll loop
  struct pollfd fds[1];
  fds[0].fd = sock->_sock;
  fds[0].events = (POLLOUT | POLLERR);
  int ret = poll(fds, 1, 100);
  if (ret > 0 && (fds[0].revents == POLLOUT) && sock->txq.TimedWait(100000))
  {
    std::pair<const zSocket::SocketAddress, zSocket::SocketBuffer> p = sock->txq.Front();
    ZLOG_DEBUG(
        "Sending packet: " + p.first.GetAddress() + "(" + zLog::IntStr(p.second.Size()) + ")");
    if ((bytes = sock->_send(&p.first, &p.second)) == p.second.Size())
    {
      sock->txq.Pop();
      sock->tx_event.Notify(sock);
    }
    else
    {
      ZLOG_ERR("Error sending packet: " + zLog::IntStr(bytes));
    }
  }

  return (0);

}

//**********************************************************************
// zSocket::InetSocket Class
//**********************************************************************

InetSocket::InetSocket(const InetAddress &addr_) :
    _rx_thread(&this->_rx_func, this), _tx_thread(&this->_tx_func, this),
        _sock(0), _inetaddr(addr_)
{
  ZLOG_DEBUG("New socket: " + zLog::IntStr(this->_sock));
}

InetSocket::~InetSocket()
{
  ZLOG_DEBUG("Destroying socket: " + zLog::IntStr(this->_sock));
  this->Close();
}

const zSocket::SocketAddress *
InetSocket::GetAddress()
{
  return (&this->_inetaddr);
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
  if (!_get_sockaddr(&this->_inetaddr, &addr))
  {
    ZLOG_CRIT("Cannot convert socket address: "
        + std::string(strerror(errno)));
    return (false);
  }

  // Override address to always listen on all interfaces
//  addr.sin_addr.s_addr = INADDR_ANY;

  // Bind address to socket
  int ret = bind(this->_sock, (struct sockaddr*) &addr, sizeof(addr));
  if (ret < 0)
  {
    ZLOG_CRIT("Cannot bind socket: " + this->_inetaddr.GetAddress() +
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

  struct sockaddr_in addr = { 0 };
  if (!_get_sockaddr(&this->_inetaddr, &addr))
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
InetSocket::_send(const zSocket::SocketAddress *addr_, zSocket::SocketBuffer *sb_)
{

  if (!this->_sock)
  {
    ZLOG_CRIT(std::string("Socket not opened"));
    return (-1);
  }

  if (addr_->GetType() != SocketAddress::TYPE_INET)
  {
    ZLOG_CRIT(std::string("Invalid address type: " +
        zLog::IntStr(addr_->GetType())));
    return (-1);
  }

  // Log info message about message being sent
  std::string logstr;
  logstr += "Sending on socket:\t";
  logstr += "To: " + addr_->GetAddress() + ";\t";
  logstr += "From: " + this->_inetaddr.GetAddress() + ";\t";
  logstr += "Size: " + zLog::IntStr(sb_->Size()) + ";";
  ZLOG_INFO(logstr);

  struct sockaddr_in src = { 0 };
  if (!_get_sockaddr((InetAddress *) addr_, &src))
  {
    ZLOG_CRIT("Cannot convert socket address: " + std::string(strerror(errno)));
    return (-1);
  }
  socklen_t slen = sizeof(struct sockaddr_in);

  ssize_t bytes_sent = sendto(this->_sock, sb_->Head(), sb_->Size(), 0,
      (struct sockaddr *) &src, slen);
  if (bytes_sent < 0)
  {
    ZLOG_ERR(std::string("Cannot send packet: " + std::string(strerror(errno))));
  }

  return (bytes_sent);

}

ssize_t
InetSocket::_broadcast(zSocket::SocketBuffer *sb_)
{
  zSocket::InetAddress *addr = new zSocket::InetAddress;
  addr->SetAddress(this->_inetaddr.GetBroadcast() + ":" + this->_inetaddr.GetPort());
  ssize_t size = this->_send(addr, sb_);
  delete (addr);
  return (size);
}

ssize_t
InetSocket::_recv(zSocket::SocketAddress *addr_, zSocket::SocketBuffer *sb_)
{
  if (!this->_sock)
  {
    ZLOG_CRIT(std::string("Socket not opened"));
    return (false);
  }

  if (addr_->GetType() != SocketAddress::TYPE_INET)
  {
    ZLOG_CRIT(std::string("Invalid address type"));
    return (false);
  }

  struct sockaddr_in src = { 0 };
  socklen_t len = sizeof(src);

  int n = recvfrom(this->_sock, sb_->Head(), sb_->TotalSize(), 0, (struct sockaddr *) &src,
      &len);
  if (n > 0)
  {
    sb_->Put(n);
    _get_inetaddress(&src, (InetAddress *) addr_);

    std::string logstr;
    logstr += "Receiving on socket:\t";
    logstr += "To: " + this->_inetaddr.GetAddress() + ";\t";
    logstr += "From: " + addr_->GetAddress() + ";\t";
    logstr += "Size: " + zLog::IntStr(n) + ";";
    ZLOG_INFO(logstr);

  } // end if

  return (n);
}

}
}
