//*****************************************************************************
//
//
//
//*****************************************************************************

#include <stdio.h>
#include <string.h>
#include <arpa/inet.h>

#include "zutils/zLog.h"
#include "zutils/zSocket.h"

namespace zUtils
{
namespace zSocket
{
#if 0

const uint8_t *
InetAddress::GetHwAddr() const
{
  return (this->_hwaddr);
}

std::string
InetAddress::GetHwAddrStr() const
{
  char addr[18] =
  { 0};
  sprintf(addr, "%02x:%02x:%02x:%02x:%02x:%02x", this->_hwaddr[0], this->_hwaddr[1],
      this->_hwaddr[2], this->_hwaddr[3], this->_hwaddr[4], this->_hwaddr[5]);
  return (std::string(addr));
}

InetAddress::InetAddress(const std::string& addr_)
{
  memset(this->_hwaddr, 0, sizeof(this->_hwaddr));
  if (!this->SetAddr(addr_))
  {
    ZLOG_CRIT("Cannot set zSocket address");
    throw;
  } // end if
}

InetAddress::InetAddress(const struct sockaddr_in& addr_)
{
  if (!this->SetAddr(addr_))
  {
    ZLOG_CRIT("Cannot set zSocket address");
    throw;
  } // end if
}

InetAddress::~InetAddress()
{
}

bool
InetAddress::operator ==(const InetAddress &other_) const
{
  bool same = (this->_ipaddr.sin_family == other_._ipaddr.sin_family)
      && (this->_ipaddr.sin_addr.s_addr == other_._ipaddr.sin_addr.s_addr)
      && (this->_ipaddr.sin_port == other_._ipaddr.sin_port);
  return (same);
}

bool
InetAddress::operator !=(const InetAddress &other_) const
{
  bool same = (this->_ipaddr.sin_family == other_._ipaddr.sin_family)
      && (this->_ipaddr.sin_addr.s_addr == other_._ipaddr.sin_addr.s_addr)
      && (this->_ipaddr.sin_port == other_._ipaddr.sin_port);
  return (!same);
}

bool
InetAddress::operator <(const InetAddress &other_) const
{
  if (this->_ipaddr.sin_family == other_._ipaddr.sin_family)
  {
    if (this->_ipaddr.sin_addr.s_addr == other_._ipaddr.sin_addr.s_addr)
    {
      return (this->_ipaddr.sin_port < other_._ipaddr.sin_port);
    } // end if
    else
    {
      return (this->_ipaddr.sin_addr.s_addr < other_._ipaddr.sin_addr.s_addr);
    } // end else
  } // end if
  else
  {
    return (this->_ipaddr.sin_family < other_._ipaddr.sin_family);
  } // end else
}

bool
InetAddress::operator >(const InetAddress &other_) const
{
  if (this->_ipaddr.sin_family == other_._ipaddr.sin_family)
  {
    if (this->_ipaddr.sin_addr.s_addr == other_._ipaddr.sin_addr.s_addr)
    {
      return (this->_ipaddr.sin_port > other_._ipaddr.sin_port);
    } // end if
    else
    {
      return (this->_ipaddr.sin_addr.s_addr > other_._ipaddr.sin_addr.s_addr);
    } // end else
  } // end if
  else
  {
    return (this->_ipaddr.sin_family > other_._ipaddr.sin_family);
  } // end else
}

bool
InetAddress::SetAddr(const std::string& addr_)
{
  char ip[INET_ADDRSTRLEN] = { 0 };
  int port = 0;
  int ret = 0;

  // Initialize address
  this->_ipaddr.sin_family = AF_INET;
  this->_ipaddr.sin_addr.s_addr = 0;
  this->_ipaddr.sin_port = 0;

  // Parse address string
  ret = sscanf(addr_.c_str(), "%[^:]:%d", &ip[0], &port);
  if (ret < 1)
  {
    ZLOG_ERR("SocketAddr::SetAddr: Cannot parse address! " + ret);
    return (false);
  } // end if

  // Perform conversion
  inet_pton(AF_INET, ip, &this->_ipaddr.sin_addr);
  if (ret > 1)
  {
    this->_ipaddr.sin_port = htons(port);
  } // end if

  // Return success
  return (true);
}

bool
InetAddress::SetAddr(const struct sockaddr_in& addr_)
{
  this->_ipaddr = addr_;
  return (true);
}

struct sockaddr_in
InetAddress::GetAddr()
{
  return (this->_ipaddr);
}

bool
InetAddress::SetHwAddr(const std::string &addr_)
{

}

const uint8_t *
InetAddress::GetHwAddr() const
{
  return (this->_hwaddr);
}

std::string
InetAddress::GetHwAddrStr() const
{
  char addr[18] = { 0 };
  sprintf(addr, "%02x:%02x:%02x:%02x:%02x:%02x", this->_hwaddr[0], this->_hwaddr[1],
      this->_hwaddr[2], this->_hwaddr[3], this->_hwaddr[4], this->_hwaddr[5]);
  return (std::string(addr));
}

in_addr_t
InetAddress::GetIpAddr() const
{
  return (ntohl(this->_ipaddr.sin_addr.s_addr));
}

std::string
InetAddress::GetIpAddrStr() const
{
  char ip[INET_ADDRSTRLEN] = { 0 };
  inet_ntop(AF_INET, &this->_ipaddr.sin_addr, ip, INET_ADDRSTRLEN);
  return (std::string(ip));
}

in_port_t
InetAddress::GetPort()
{
  return (ntohs(this->_ipaddr.sin_port));
}

std::string
InetAddress::GetPortStr()
{
  char port[6] = { 0 };
  sprintf(port, "%d", ntohs(this->_ipaddr.sin_port));
  return (std::string(port));
}
#endif
}
}
