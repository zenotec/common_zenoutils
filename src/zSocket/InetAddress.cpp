//*****************************************************************************
//    Copyright (C) 2014 ZenoTec LLC (http://www.zenotec.net)
//
//    File: Inet.cpp
//    Description:
//
//*****************************************************************************

#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <errno.h>
#include <arpa/inet.h>
#include <ifaddrs.h>

#include <zutils/zLog.h>
#include <zutils/zInetSocket.h>

namespace zUtils
{
namespace zSocket
{


//**********************************************************************
// InetAddress Class
//**********************************************************************

InetAddress::InetAddress(const Address &addr_)
{
  memset(&this->_ipaddr, 0, sizeof(this->_ipaddr));
  if ((addr_.GetType() != Address::TYPE_INET) || !this->SetAddr(addr_.GetAddr()))
  {
    ZLOG_CRIT("zSocket::InetAddress: Cannot set address");
  } // end if
}

InetAddress::InetAddress(const std::string& addr_)
{
  memset(&this->_ipaddr, 0, sizeof(this->_ipaddr));
  if (!this->SetAddr(addr_))
  {
    ZLOG_CRIT("zSocket::InetAddress: Cannot set address");
  } // end if
}

InetAddress::InetAddress(const struct sockaddr_in& addr_)
{
  memset(&this->_ipaddr, 0, sizeof(this->_ipaddr));
  if (!this->SetAddr(addr_))
  {
    ZLOG_CRIT("zSocket::InetAddress: Cannot set address");
  } // end if
}

InetAddress::~InetAddress()
{
}

bool
InetAddress::operator ==(const InetAddress &other_) const
{
  return ((this->GetAddrString() == other_.GetAddrString()));
}

bool
InetAddress::operator !=(const InetAddress &other_) const
{
  return ((this->GetAddrString() != other_.GetAddrString()));
}

std::string
InetAddress::GetAddrString() const
{
  std::string addr = this->_getIpAddrString() + ":" + this->_getPortString();
  return (addr);
}

struct sockaddr_in
InetAddress::GetAddrSockAddr() const
{
  return (this->_ipaddr);
}

bool
InetAddress::SetAddr(const std::string& addr_)
{
  sockaddr_in ipaddr = { 0 };
  char ip[INET_ADDRSTRLEN + 1] = { 0 };
  int port = 0;
  int ret = 0;

  // Initialize address
  ipaddr.sin_family = AF_INET;
  ipaddr.sin_addr.s_addr = 0;
  ipaddr.sin_port = 0;

  // Parse address string
  ret = sscanf(addr_.c_str(), "%[^:]:%d", &ip[0], &port);
  if (ret < 1)
  {
    ZLOG_ERR("SocketAddr::SetAddr: Cannot parse address! " + ret);
    return (false);
  } // end if

  // Perform conversion
  if (ret > 0)
  {
    inet_pton(AF_INET, ip, &ipaddr.sin_addr);
    if (ret > 1)
    {
      ipaddr.sin_port = htons(port);
    }
    this->_ipaddr = ipaddr;
  } // end if

  // Return success
  return (ret > 0);
}

bool
InetAddress::SetAddr(const struct sockaddr_in &addr_)
{
  bool status = false;
  if (addr_.sin_family == AF_INET)
  {
    this->_ipaddr = addr_;
    status = true;
  }
  return (status);
}

in_addr_t
InetAddress::_getIpAddr() const
{
  return (ntohl(this->_ipaddr.sin_addr.s_addr));
}

std::string
InetAddress::_getIpAddrString() const
{
  char ip[INET_ADDRSTRLEN] = { 0 };
  inet_ntop(AF_INET, &this->_ipaddr.sin_addr, ip, INET_ADDRSTRLEN);
  return (std::string(ip));
}

in_port_t
InetAddress::_getPort() const
{
  return (ntohs(this->_ipaddr.sin_port));
}

std::string
InetAddress::_getPortString() const
{
  char port[10] = { 0 };
  snprintf(port, 10, "%d", this->_getPort());
  return (std::string(port));
}

}
}
