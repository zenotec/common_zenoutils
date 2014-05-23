//*****************************************************************************
//
//
//
//*****************************************************************************

#include <stdio.h>
#include <arpa/inet.h>

#include "zutils/zLog.h"
#include "zutils/zSocket.h"

namespace zUtils
{
namespace zSocket
{

SocketAddr::SocketAddr(const std::string& addr_)
{
  if (!this->SetAddr(addr_))
  {
    ZLOG_CRIT("Cannot set zSocket address");
    throw;
  } // end if
}

SocketAddr::SocketAddr(const struct sockaddr_in& addr_)
{
  if (!this->SetAddr(addr_))
  {
    ZLOG_CRIT("Cannot set zSocket address");
    throw;
  } // end if
}

SocketAddr::~SocketAddr()
{
}

bool
SocketAddr::operator ==(const SocketAddr &other_) const
{
  bool same = (this->_ipaddr.sin_family == other_._ipaddr.sin_family)
      && (this->_ipaddr.sin_addr.s_addr == other_._ipaddr.sin_addr.s_addr)
      && (this->_ipaddr.sin_port == other_._ipaddr.sin_port);
  return (same);
}

bool
SocketAddr::operator !=(const SocketAddr &other_) const
{
  bool same = (this->_ipaddr.sin_family == other_._ipaddr.sin_family)
      && (this->_ipaddr.sin_addr.s_addr == other_._ipaddr.sin_addr.s_addr)
      && (this->_ipaddr.sin_port == other_._ipaddr.sin_port);
  return (!same);
}

bool
SocketAddr::operator <(const SocketAddr &other_) const
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
SocketAddr::operator >(const SocketAddr &other_) const
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
SocketAddr::SetAddr(const std::string& addr_)
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
SocketAddr::SetAddr(const struct sockaddr_in& addr_)
{
  this->_ipaddr = addr_;
  return (true);
}

struct sockaddr_in
SocketAddr::GetAddr()
{
  return (this->_ipaddr);
}

uint8_t *
SocketAddr::GetHwAddr() const
{

}

std::string
SocketAddr::GetHwAddrStr() const
{

}

in_addr_t
SocketAddr::GetIpAddr() const
{
  return (ntohl(this->_ipaddr.sin_addr.s_addr));
}

std::string
SocketAddr::GetIpAddrStr() const
{
  char ip[INET_ADDRSTRLEN] = { 0 };
  inet_ntop(AF_INET, &this->_ipaddr.sin_addr, ip, INET_ADDRSTRLEN);
  return (std::string(ip));
}

in_port_t
SocketAddr::GetPort()
{
  return (ntohs(this->_ipaddr.sin_port));
}

std::string
SocketAddr::GetPortStr()
{
  char port[6] = { 0 };
  sprintf(port, "%d", ntohs(this->_ipaddr.sin_port));
  return (std::string(port));
}

}
}
