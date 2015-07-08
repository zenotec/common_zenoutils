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

const uint8_t *
EthAddress::GetHwAddr() const
{
  return (this->_hwaddr);
}

std::string
EthAddress::GetHwAddrStr() const
{
  char addr[18] =
  { 0};
  sprintf(addr, "%02x:%02x:%02x:%02x:%02x:%02x", this->_hwaddr[0], this->_hwaddr[1],
      this->_hwaddr[2], this->_hwaddr[3], this->_hwaddr[4], this->_hwaddr[5]);
  return (std::string(addr));
}

EthAddress::EthAddress(const std::string& addr_)
{
  memset(this->_hwaddr, 0, sizeof(this->_hwaddr));
  if (!this->SetAddr(addr_))
  {
    ZLOG_CRIT("Cannot set zSocket address");
    throw;
  } // end if
}

EthAddress::EthAddress(const struct sockaddr_in& addr_)
{
  if (!this->SetAddr(addr_))
  {
    ZLOG_CRIT("Cannot set zSocket address");
    throw;
  } // end if
}

EthAddress::~EthAddress()
{
}

bool
EthAddress::operator ==(const EthAddress &other_) const
{
  bool same = (this->_ipaddr.sin_family == other_._ipaddr.sin_family)
      && (this->_ipaddr.sin_addr.s_addr == other_._ipaddr.sin_addr.s_addr)
      && (this->_ipaddr.sin_port == other_._ipaddr.sin_port);
  return (same);
}

bool
EthAddress::operator !=(const EthAddress &other_) const
{
  bool same = (this->_ipaddr.sin_family == other_._ipaddr.sin_family)
      && (this->_ipaddr.sin_addr.s_addr == other_._ipaddr.sin_addr.s_addr)
      && (this->_ipaddr.sin_port == other_._ipaddr.sin_port);
  return (!same);
}

bool
EthAddress::operator <(const EthAddress &other_) const
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
EthAddress::operator >(const EthAddress &other_) const
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
EthAddress::SetAddr(const std::string& addr_)
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
EthAddress::SetAddr(const struct sockaddr_in& addr_)
{
  this->_ipaddr = addr_;
  return (true);
}

struct sockaddr_in
EthAddress::GetAddr()
{
  return (this->_ipaddr);
}

bool
EthAddress::SetHwAddr(const std::string &addr_)
{

}

const uint8_t *
EthAddress::GetHwAddr() const
{
  return (this->_hwaddr);
}

std::string
EthAddress::GetHwAddrStr() const
{
  char addr[18] = { 0 };
  sprintf(addr, "%02x:%02x:%02x:%02x:%02x:%02x", this->_hwaddr[0], this->_hwaddr[1],
      this->_hwaddr[2], this->_hwaddr[3], this->_hwaddr[4], this->_hwaddr[5]);
  return (std::string(addr));
}

in_addr_t
EthAddress::GetIpAddr() const
{
  return (ntohl(this->_ipaddr.sin_addr.s_addr));
}

std::string
EthAddress::GetIpAddrStr() const
{
  char ip[INET_ADDRSTRLEN] = { 0 };
  inet_ntop(AF_INET, &this->_ipaddr.sin_addr, ip, INET_ADDRSTRLEN);
  return (std::string(ip));
}

in_port_t
EthAddress::GetPort()
{
  return (ntohs(this->_ipaddr.sin_port));
}

std::string
EthAddress::GetPortStr()
{
  char port[6] = { 0 };
  sprintf(port, "%d", ntohs(this->_ipaddr.sin_port));
  return (std::string(port));
}
#endif
}
}
