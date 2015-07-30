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
#include <sys/select.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <ifaddrs.h>

#include <sstream>

#include <zutils/zLog.h>
#include <zutils/zInetSocket.h>

namespace zUtils
{
namespace zSocket
{

static std::string
_get_ipaddr_string(struct sockaddr_in *sockaddr_)
{
  char ip[INET_ADDRSTRLEN] = { 0 };
  inet_ntop(AF_INET, &sockaddr_->sin_addr, ip, INET_ADDRSTRLEN);
  return (std::string(ip));
}

static std::string
_get_port_string(struct sockaddr_in *sockaddr_)
{
  char port[10] = { 0 };
  snprintf(port, 10, "%d", ntohs(sockaddr_->sin_port));
  return (std::string(port));
}

static bool
_get_sockaddr(const InetAddress *addr_, struct sockaddr_in *sockaddr_)
{
  memset((void *) sockaddr_, 0, sizeof(struct sockaddr_in));
  sockaddr_->sin_family = AF_INET;
  inet_pton(AF_INET, addr_->GetIpAddr().c_str(), &sockaddr_->sin_addr);
  int port = 0;
  sscanf(addr_->GetPort().c_str(), "%d", &port);
  sockaddr_->sin_port = htons(port);
  return (true);
}

static bool
_get_inetaddress(struct sockaddr_in *sockaddr_, InetAddress *addr_)
{
  addr_->SetType(zSocket::Address::TYPE_INET);
  addr_->SetIpAddr(_get_ipaddr_string(sockaddr_));
  addr_->SetPort(_get_port_string(sockaddr_));
  return (true);
}

//**********************************************************************
// InetAddress Class
//**********************************************************************

InetAddress::InetAddress()
{
  this->SetType(zSocket::Address::TYPE_INET);
  this->SetIpAddr(std::string("0.0.0.0"));
  this->SetPort(std::string("0"));
  this->SetNetmask(std::string("0.0.0.0"));
  this->SetBroadcast(std::string("0.0.0.0"));
}

InetAddress::InetAddress(const std::string &ifname_)
{
  struct ifaddrs *iflist, *ifaddr = 0;
  if (getifaddrs(&iflist) == 0)
  {
    for (ifaddr = iflist; ifaddr; ifaddr = ifaddr->ifa_next)
    {
      ZLOG_INFO("zSocket::InetAddress: Found interface: " + std::string(ifaddr->ifa_name));
      if ((strcmp(ifaddr->ifa_name, ifname_.c_str()) == 0) &&
          (ifaddr->ifa_addr->sa_family == AF_INET))
      {
        this->SetType(zSocket::Address::TYPE_INET);
        this->SetIpAddr(_get_ipaddr_string((struct sockaddr_in *) ifaddr->ifa_addr));
        this->SetPort(_get_port_string((struct sockaddr_in *) ifaddr->ifa_addr));
        this->SetNetmask(_get_ipaddr_string((struct sockaddr_in *) ifaddr->ifa_netmask));
        this->SetBroadcast(_get_ipaddr_string((struct sockaddr_in *) ifaddr->ifa_broadaddr));
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
      c = strtok(NULL, ":");
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
// InetSocket Class
//**********************************************************************

InetSocket::InetSocket(const InetAddress &addr_) :
    _thread(this, this), _sock(0), _inetaddr(addr_)
{
  ZLOG_DEBUG("InetSocket::InetSocket: New socket: " + zLog::IntStr(this->_sock));
}

InetSocket::~InetSocket()
{
  ZLOG_DEBUG("InetSocket::~InetSocket: Destroying socket: " + zLog::IntStr(this->_sock));
  this->_close();
}

void *
InetSocket::ThreadFunction(void *arg_)
{
  InetSocket *self = (InetSocket *) arg_;
  ssize_t bytes = -1;

  if (!self->_sock)
  {
    ZLOG_CRIT(std::string("InetSocket::ThreadFunction: Socket not opened"));
    return ((void*) -1);
  }

  fd_set rxFds;
  struct timeval to;

  // Setup for select loop
  FD_ZERO(&rxFds);
  FD_SET(self->_sock, &rxFds);
  to.tv_sec = 0;
  to.tv_usec = 10000;

  // Select on socket
  int ret = select(self->_sock + 1, &rxFds, NULL, NULL, &to);
  if (ret > 0 && FD_ISSET(this->_sock, &rxFds))
  {
    ZLOG_INFO("InetSocket::ThreadFunction: Received packet on socket: " + zLog::IntStr(this->_sock));
    Address *addr = new InetAddress;
    Buffer *sb = new Buffer;
    bytes = this->_recv(addr, sb);
    if (bytes > 0)
    {
      sb->Put(bytes);
      this->Push(std::make_pair(addr, sb));
    }
  } // end if

  if (ret < 0)
  {
    ZLOG_ERR(
        "InetSocket::ThreadFunction: Error selecting on socket: " + std::string(strerror(errno)));
  } // end if

  return (0);

}

bool
InetSocket::_open()
{
  ZLOG_DEBUG("InetSocket::_open: Opening socket: " + zLog::IntStr(this->_sock));

  if (!this->_sock)
  {

    // Create a AF_INET socket
    this->_sock = socket(AF_INET, (SOCK_DGRAM | SOCK_NONBLOCK), IPPROTO_UDP);
    if (this->_sock < 0)
    {
      ZLOG_CRIT("InetSocket::_open: Cannot create socket: " + std::string(strerror(errno)));
      return (false);
    } // end if

    // Enable sending to broadcast address
    int bcastEnable = 1;
    if (setsockopt(this->_sock, SOL_SOCKET, SO_BROADCAST, &bcastEnable, sizeof(bcastEnable)) < 0)
    {
      ZLOG_CRIT("InetSocket::_open: Cannot configure socket: " + std::string(strerror(errno)));
      return (false);
    }

  } // end if
  else
  {
    ZLOG_WARN("InetSocket::_open: Socket already open");
  }
  return (true);
}

void
InetSocket::_close()
{
  ZLOG_DEBUG("InetSocket::_close: Closing socket: " + zLog::IntStr(this->_sock));
  // Close socket
  if (this->_sock && this->_thread.Join(1000))
  {
    ZLOG_DEBUG("InetSocket::_close: Socket Closed: " + zLog::IntStr(this->_sock));
    close(this->_sock);
    this->_sock = 0;
  } // end if
}

bool
InetSocket::_bind()
{

  if (!this->_sock)
  {
    ZLOG_CRIT(std::string("InetSocket::_bind: Socket not opened"));
    return (false);
  }

  ZLOG_DEBUG("InetSocket::_bind: Bind on socket: " + zLog::IntStr(this->_sock));

  struct sockaddr_in addr = { 0 };
  if (!_get_sockaddr(&this->_inetaddr, &addr))
  {
    ZLOG_CRIT("InetSocket::_bind: Cannot convert socket address: " + std::string(strerror(errno)));
  }

  int ret = bind(this->_sock, (struct sockaddr*) &addr, sizeof(addr));
  if (ret < 0)
  {
    ZLOG_CRIT("InetSocket::_bind: Cannot bind socket: " + std::string(strerror(errno)));
    return (false);
  } // end if

  this->_thread.Run(1000);

  return (true);

}

bool
InetSocket::_connect()
{

  if (!this->_sock)
  {
    ZLOG_CRIT(std::string("InetSocket::_connect: Socket not opened"));
    return (false);
  }

  ZLOG_DEBUG("InetSocket::_connect: Connect on socket: " + zLog::IntStr(this->_sock));

  struct sockaddr_in addr = { 0 };
  if (!_get_sockaddr(&this->_inetaddr, &addr))
  {
    ZLOG_CRIT("InetSocket::_bind: Cannot convert socket address: " + std::string(strerror(errno)));
  }

  int ret = connect(this->_sock, (struct sockaddr*) &addr, sizeof(addr));
  if (ret < 0)
  {
    ZLOG_CRIT("InetSocket::_connect: Cannot connect socket: " + std::string(strerror(errno)));
    return (false);
  } // end if

  this->_thread.Run(1000);

  return (true);

}

ssize_t
InetSocket::_send(const zSocket::Address *addr_, zSocket::Buffer *sb_)
{
  ssize_t bytes_sent = 0;

  if (!this->_sock)
  {
    ZLOG_CRIT(std::string("InetSocket::_send: Socket not opened"));
    return (-1);
  }

  if (addr_->GetType() != Address::TYPE_INET)
  {
    ZLOG_CRIT(std::string("InetSocket::_send: Invalid address type: " + zLog::IntStr(addr_->GetType())));
    return (false);
  }

  // Log info message about message being sent
  std::string logstr;
  logstr += "Sending on socket:\t";
  logstr += "To: " + addr_->GetAddress() + ";\t";
  logstr += "From: " + this->_inetaddr.GetAddress() + ";\t";
  logstr += "Size: " + zLog::IntStr(sb_->Size()) + ";";
  ZLOG_INFO(logstr);

  if (addr_->GetAddress() == this->_inetaddr.GetAddress())
  {
    ZLOG_INFO("InetSocket::_send: Address match");
    InetAddress *addr = new InetAddress(*(const InetAddress *) addr_);
    zSocket::Buffer *sb = new zSocket::Buffer(*sb_);
    this->Push(std::make_pair(addr, sb));
    return (sb->Size());
  }
  else
  {

    struct sockaddr_in src = { 0 };
    if (!_get_sockaddr((InetAddress *) addr_, &src))
    {
      ZLOG_CRIT("InetSocket::_bind: Cannot convert socket address: " + std::string(strerror(errno)));
    }
    socklen_t slen = sizeof(struct sockaddr_in);

    ssize_t bytes_sent = sendto(this->_sock, sb_->Head(), sb_->Size(), 0, (struct sockaddr *) &src,
        slen);
    if (bytes_sent < 0)
    {
      ZLOG_ERR(
          std::string("InetSocket::_send: Cannot send packet: " + std::string(strerror(errno))));
    }

    return (bytes_sent);
  }

}

ssize_t
InetSocket::_broadcast(zSocket::Buffer *sb_)
{
  zSocket::InetAddress addr;
  addr.SetAddress(this->_inetaddr.GetBroadcast() + ":" + this->_inetaddr.GetPort());
  return (this->_send(&addr, sb_));
}

ssize_t
InetSocket::_recv(zSocket::Address *addr_, zSocket::Buffer *sb_)
{
  if (!this->_sock)
  {
    ZLOG_CRIT(std::string("InetSocket::_recv: Socket not opened"));
    return (false);
  }

  if (addr_->GetType() != Address::TYPE_INET)
  {
    ZLOG_CRIT(std::string("InetSocket::_recv: Invalid address type"));
    return (false);
  }

  struct sockaddr_in src = { 0 };
  socklen_t len = sizeof(src);

  int n = recvfrom(this->_sock, sb_->Head(), sb_->TotalSize(), 0, (struct sockaddr *) &src, &len);
  if (n > 0)
  {
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
