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
#include <net/if.h>
#include <netinet/in.h>

// libc++ includes

// libzutils includes

#include <zutils/zUtils.h>
#include <zutils/zLog.h>
#include <zutils/zRawAddress.h>

ZLOG_MODULE_INIT(zUtils::zLog::Log::MODULE_SOCKET);

namespace zUtils
{
namespace zSocket
{

//**********************************************************************
// Class: zSocket::RawAddress
//**********************************************************************

RawAddress::RawAddress(const std::string &addr_) :
    Address(Address::TYPE_RAW), _sa { 0 }
{
  this->SetAddress(addr_);
}

RawAddress::RawAddress(const Address &addr_) :
    Address(Address::TYPE_RAW), _sa { 0 }
{
  if (addr_.GetType() == this->GetType())
  {
    this->SetAddress(addr_.GetAddress());
  }
}

RawAddress::RawAddress(const struct sockaddr_ll& sa_) :
        Address(Address::TYPE_RAW), _sa (sa_)
{
  Address::SetAddress(this->GetAddress());
}

RawAddress::~RawAddress()
{
}

std::string
RawAddress::GetAddress() const
{
  std::string addr(Address::GetAddress());
  char ifname[64] = { 0 };
  if (if_indextoname(this->_sa.sll_ifindex, ifname) == ifname)
  {
    addr = std::string(ifname);
  }
  return (addr);
}

bool
RawAddress::SetAddress(const std::string& addr_)
{
  bool status = false;
  int ifindex = if_nametoindex(addr_.c_str());
  if (ifindex)
  {
    this->_sa.sll_ifindex = ifindex;
    status = Address::SetAddress(this->GetAddress());
  }

  return (status);
}

struct sockaddr_ll
RawAddress::GetSA() const
{
  return (this->_sa);
}

bool
RawAddress::SetSA(const struct sockaddr_ll& sa_)
{
  this->_sa = sa_;
  return (true);
}

void
RawAddress::Display() const
{
  std::string mac;
  Address::Display();
  std::cout << "----------------- Raw Address -----------------" << std::endl;
  std::cout << "IfIndex: \t" << int(this->_sa.sll_ifindex) << std::endl;
  std::cout << "IfName:  \t" << this->GetAddress() << std::endl;
  std::cout << "Protocol:\t" << std::hex << int(ntohs(this->_sa.sll_protocol)) << std::dec << std::endl;
  std::cout << "PType:   \t" << int(this->_sa.sll_pkttype) << std::endl;
}

}
}
