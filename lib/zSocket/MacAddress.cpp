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
#include <string.h>
#include <sys/socket.h>
#include <linux/if_ether.h>
#include <linux/if_packet.h>

// libc++ includes

// libzutils includes

#include <zutils/zUtils.h>
#include <zutils/zLog.h>
#include <zutils/zMacAddress.h>

ZLOG_MODULE_INIT(zUtils::zLog::Log::MODULE_SOCKET);

namespace zUtils
{
namespace zSocket
{

static bool
_str2mac(const std::string& addr_, uint8_t* mac_)
{
  uint8_t tmac[ETH_ALEN+1] = { 0 };
  bool status = (sscanf(addr_.c_str(), "%hhx:%hhx:%hhx:%hhx:%hhx:%hhx",
      &tmac[0], &tmac[1], &tmac[2], &tmac[3], &tmac[4], &tmac[5]) == ETH_ALEN);
  if (status)
  {
    status = (memcpy(mac_, tmac, ETH_ALEN) == mac_);
  }
  return (status);
}

static bool
_mac2str(const uint8_t* mac_, std::string& addr_)
{
  bool status = false;
  char str[18] = { 0 };
  int n = sprintf(str, "%02hhx:%02hhx:%02hhx:%02hhx:%02hhx:%02hhx",
      mac_[0], mac_[1], mac_[2], mac_[3], mac_[4], mac_[5]);
  if (n == 17)
  {
    addr_ = std::string(str);
    status = true;
  }
  return (status);
}

static bool
_str2sa(const std::string &addr_, struct sockaddr_ll &sa_)
{
  bool status = false;

  // Initialize socket address
  memset(&sa_, 0, sizeof(sa_));
  sa_.sll_family = AF_PACKET;
  sa_.sll_pkttype = PACKET_HOST;
  sa_.sll_halen = ETH_ALEN;
  status = _str2mac(addr_, sa_.sll_addr);

  return (status);
}

static bool
_sa2str(const struct sockaddr_ll &sa_, std::string &addr_)
{
  return (_mac2str(sa_.sll_addr, addr_));
}

//**********************************************************************
// Class: zSocket::MacAddress
//**********************************************************************

MacAddress::MacAddress(const uint8_t* mac_) :
    Address(Address::TYPE_MAC), _sa { 0 }
{
  std::string addr;
  if (_mac2str(mac_, addr))
  {
    this->SetAddress(addr);
  }
}

MacAddress::MacAddress(const std::string &addr_) :
    Address(Address::TYPE_MAC), _sa { 0 }
{
  this->SetAddress(addr_);
}

MacAddress::MacAddress(const Address &addr_) :
    Address(Address::TYPE_MAC), _sa { 0 }
{
  if (addr_.GetType() == this->GetType())
  {
    this->SetAddress(addr_.GetAddress());
  }
}

MacAddress::MacAddress(const struct sockaddr_ll& sa_) :
        Address(Address::TYPE_MAC), _sa (sa_)
{
  std::string addr;
  _sa2str(this->_sa, addr);
  this->SetAddress(addr);
}

MacAddress::~MacAddress()
{
}

std::string
MacAddress::GetAddress() const
{
  std::string addr;
  _sa2str(this->_sa, addr);
  return (addr);
}

bool
MacAddress::SetAddress(const std::string& addr_)
{
  bool status = false;
  if (_str2sa(addr_, this->_sa))
  {
    status = Address::SetAddress(this->GetAddress());
  }
  return (status);
}

uint8_t*
MacAddress::GetMac(uint8_t* mac_) const
{
  if (mac_ && _str2mac(this->GetAddress(), mac_))
    return (mac_);
  else
    return (NULL);
}

bool
MacAddress::SetMac(const uint8_t* mac_)
{
  bool status = false;
  std::string addr;
  if (_mac2str(mac_, addr))
  {
    status = _str2sa(addr, this->_sa);
  }
  return (status);
}

struct sockaddr_ll
MacAddress::GetSA() const
{
  return (this->_sa);
}

bool
MacAddress::SetSA(const struct sockaddr_ll& sa_)
{
  this->_sa = sa_;
  return (true);
}

void
MacAddress::Display() const
{
  std::string mac;
  _mac2str(this->_sa.sll_addr, mac);
  Address::Display();
  std::cout << "----------------- Ethernet Address -----------------" << std::endl;
  std::cout << "Family: \t" << this->_sa.sll_family << std::endl;
  std::cout << "IfIndex:\t" << this->_sa.sll_ifindex << std::endl;
  std::cout << "MAC:    \t" << mac << std::endl;
}

}
}
