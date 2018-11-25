/*
 * Copyright (c) 2014-2018 ZenoTec LLC (http://www.zenotec.net)
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

// libc includes

#include <unistd.h>
#include <errno.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/ioctl.h>
#include <linux/sockios.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <poll.h>

// libc++ includes

// libzutils includes

#include <zutils/zLog.h>
#include <zutils/zSocket.h>
#include <zutils/zUdpSocket.h>

ZLOG_MODULE_INIT(zUtils::zLog::Log::MODULE_SOCKET);

namespace zUtils
{
namespace zSocket
{

static in_addr_t
_sa2ip(const struct sockaddr_in& sa_)
{
  return (ntohl(sa_.sin_addr.s_addr));
}

static std::string
_sa2ipstr(const struct sockaddr_in& sa_)
{
  char ip[INET_ADDRSTRLEN] = { 0 };
  std::string str;

  if ((char*) &ip == inet_ntop( AF_INET, &sa_.sin_addr, (char*) ip, INET_ADDRSTRLEN))
  {
    str = std::string(ip);
  }

  return (str);
}

static in_port_t
_sa2port(const struct sockaddr_in& sa_)
{
  return (ntohs(sa_.sin_port));
}

static std::string
_sa2portstr(const struct sockaddr_in& sa_)
{
  char port[10] = { 0 };
  snprintf(port, 10, "%u", _sa2port(sa_));
  std::string str(port);
  return (port);
}

static std::string
_str2ip(const std::string& addr_)
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
_str2port(const std::string& addr_)
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
_str2sa(const std::string& addr_, struct sockaddr_in& sa_)
{

  bool status = false;
  struct sockaddr_in sa = { 0 };
  uint32_t port = 0;

  sa.sin_family = AF_INET;
  int ret = inet_pton( AF_INET, _str2ip(addr_).c_str(), &sa.sin_addr);
  if (ret == 1)
  {
    if ((sscanf(_str2port(addr_).c_str(), "%u", &port) == 1) && (port < 0xffff))
    {
      sa.sin_port = htons(port);
      sa_ = sa;
      status = true;
    }
  }

  return (status);
}

static bool
_sa2str(const struct sockaddr_in& sa_, std::string& addr_)
{
  bool status = false;
  struct sockaddr_in sockaddr;
  std::string addr = _sa2ipstr(sa_) + ":" + _sa2portstr(sa_);
  if (_str2sa(addr, sockaddr))
  {
    addr_ = addr;
    status = true;
  }
  return (status);
}

//**********************************************************************
// Class: zSocket::Ipv4Address
//**********************************************************************

Ipv4Address::Ipv4Address(const in_addr_t ip_, const in_port_t port_) :
    Address(Address::TYPE_IPv4), _sa { 0 }
{
  this->_sa.sin_addr.s_addr = ip_;
  this->_sa.sin_port = port_;
}

Ipv4Address::Ipv4Address(const std::string &addr_) :
    Address(Address::TYPE_IPv4), _sa { 0 }
{
  this->SetAddress(addr_);
}

Ipv4Address::Ipv4Address(const Address &addr_) :
    Address(Address::TYPE_IPv4), _sa { 0 }
{
  this->SetAddress(addr_.GetAddress());
}

Ipv4Address::Ipv4Address(const struct sockaddr_in& sa_) :
        Address(Address::TYPE_IPv4), _sa (sa_)
{
  Address::SetAddress(this->GetAddress());
}

Ipv4Address::~Ipv4Address()
{
}

std::string
Ipv4Address::GetAddress() const
{
  std::string addr;
  _sa2str(this->_sa, addr);
  return (addr);
}

bool
Ipv4Address::SetAddress(const std::string& addr_)
{
  bool status = false;
  if (_str2sa(addr_, this->_sa))
  {
    status = Address::SetAddress(addr_);
  }
  return (status);
}

uint32_t
Ipv4Address::GetIp() const
{
  return (_sa2ip(this->_sa));
}

std::string
Ipv4Address::GetIpString() const
{
  return (_sa2ipstr(this->_sa));
}

bool
Ipv4Address::SetIp(const uint32_t ip_)
{
  this->_sa.sin_addr.s_addr = htonl(ip_);
  return (Address::SetAddress(this->GetAddress()));
}

bool
Ipv4Address::SetIp(const std::string& ip_)
{
  return (this->SetAddress(ip_ + ":" + _sa2portstr(this->_sa)));
}

uint16_t
Ipv4Address::GetPort() const
{
  return (_sa2port(this->_sa));
}

std::string
Ipv4Address::GetPortString() const
{
  return (_sa2portstr(this->_sa));
}

bool
Ipv4Address::SetPort(const uint16_t port_)
{
  this->_sa.sin_port = htons(port_);
  return (Address::SetAddress(this->GetAddress()));
}

bool
Ipv4Address::SetPort(const std::string& port_)
{
  return(this->SetAddress(this->GetIp() + ":" + port_));
}

struct sockaddr_in
Ipv4Address::GetSA() const
{
  return (this->_sa);
}

bool
Ipv4Address::SetSA(const struct sockaddr_in& sa_)
{
  bool status = false;
  if (sa_.sin_family == AF_INET)
  {
    this->_sa = sa_;
    status = true;
  }
  return (status);
}

void
Ipv4Address::Display() const
{
  Address::Display();
  std::cout << "----------------- IPv4 Address -----------------" << std::endl;
  std::cout << "Family: \t" << this->_sa.sin_family << std::endl;
  std::cout << "IP:     \t" << _sa2ipstr(this->_sa) << std::endl;
  std::cout << "Port:   \t" << _sa2portstr(this->_sa) << std::endl;
}

}
}
