/*
 * Copyright (c) 2017 Cable Television Laboratories, Inc. ("CableLabs")
 *                    and others.  All rights reserved.
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
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/socket.h>
#include <net/if.h>
#include <netinet/in.h>
#include <netlink/netlink.h>
#include <linux/if_arp.h>

// libc++ includes
#include <iostream>

// libzutils includes
#include <zutils/zLog.h>
using namespace zUtils;

// local includes
#include "RouteLink.h"

ZLOG_MODULE_INIT(zUtils::zLog::Log::MODULE_INTERFACE);

namespace netlink
{

static std::string
_arptype2str(int type_)
{
  std::string str;
  switch(type_)
  {
  case ARPHRD_ETHER:
    str = "ethernet";
    break;
  case ARPHRD_LOOPBACK:
    str = "loopback";
    break;
  case ARPHRD_IEEE80211:
    str = "ieee80211";
    break;
  case ARPHRD_IEEE80211_RADIOTAP:
    str = "radiotap";
    break;
  case ARPHRD_VOID:
    // no break
  default:
    str = "unknown";
    break;
  }
  return (str);
}

static std::string
_state2str(RouteLink::STATE state_)
{
  std::string str;
  switch(state_)
  {
  case RouteLink::STATE_UNKNOWN:
    str = "unknown";
    break;
  case RouteLink::STATE_UP:
    str = "up";
    break;
  case RouteLink::STATE_DOWN:
    str = "down";
    break;
  default:
    str = "error";
    break;
  }
  return(str);
}

static RouteLink::STATE
_operstate2state(uint8_t state_)
{
  switch(state_)
  {
  case 1:
    return(RouteLink::STATE_UP);
  case 2:
    return(RouteLink::STATE_DOWN);
  case 4:
    return(RouteLink::STATE_UNKNOWN);
  case 3: // "testing"
    // no break
  case 5: // "dormant"
    // no break
  case 6: // "not present"
    // no break
  case 7: // "lower layer down"
    // no break;
  default:
    return(RouteLink::STATE_ERR);
  }
}

static RouteLink::STATE
_carrier2state(uint8_t state_)
{
  switch(state_)
  {
  case 0:
    return(RouteLink::STATE_DOWN);
  case 1:
    return(RouteLink::STATE_UP);
  default:
    return(RouteLink::STATE_ERR);
  }
}

//*****************************************************************************
// Class: RouteLink
//*****************************************************************************

RouteLink::RouteLink(struct rtnl_link *link_) :
    _link(link_)
{
  if (this->_link == NULL)
  {
    this->_link = rtnl_link_alloc();
  }
}

RouteLink::RouteLink(const RouteLink& other_) :
    _link(rtnl_link_alloc())
{
  this->_copy(other_);
}

RouteLink::~RouteLink()
{
  if (this->_link)
  {
    rtnl_link_put(this->_link);
    this->_link = NULL;
  }
}

struct rtnl_link*
RouteLink::operator ()()
{
  return (this->_link);
}

struct rtnl_link*
RouteLink::operator ()(struct rtnl_link *link_)
{
  if (this->_link)
  {
    rtnl_link_put(this->_link);
  }
  this->_link = link_;
  return (this->_link);
}

RouteLink&
RouteLink::operator =(const RouteLink& other_)
{
  this->_copy(other_);
  return(*this);
}

int
RouteLink::IfIndex() const
{
  int index = 0;
  if (this->_link)
  {
    index = rtnl_link_get_ifindex(this->_link);
  }
  return (index);
}

bool
RouteLink::IfIndex(const int index_)
{
  bool status = false;
  if (this->_link)
  {
    rtnl_link_set_ifindex(this->_link, index_);
    status = true;
  }
  return (status);
}

int
RouteLink::MasterIfIndex() const
{
  int index = 0;
  if (this->_link)
  {
    index = rtnl_link_get_link(this->_link);
  }
  return (index);
}

bool
RouteLink::MasterIfIndex(const int index_)
{
  bool status = false;
  if (this->_link)
  {
    rtnl_link_set_link(this->_link, index_);
    status = true;
  }
  return (status);
}

std::string
RouteLink::IfName() const
{
  const char* name = NULL;
  if (this->_link)
  {
    name = rtnl_link_get_name(this->_link);
  }
  return (name ? std::string(name) : std::string(""));
}

bool
RouteLink::IfName(const std::string& name_)
{
  bool status = false;
  if (this->_link)
  {
    rtnl_link_set_name(this->_link, name_.c_str());
    status = true;
  }
  return (status);
}

std::string
RouteLink::MasterIfName() const
{
  char str[256] = { 0 };
  if_indextoname(this->MasterIfIndex(), str);
  return (std::string(str));
}

bool
RouteLink::MasterIfName(const std::string& ifname_)
{
  bool status = false;
  int index = if_nametoindex(ifname_.c_str());
  if (index)
  {
    status = this->MasterIfIndex(index);
  }
  return (status);
}

unsigned int
RouteLink::ArpType() const
{
  int type = -1;
  if (this->_link)
  {
    type = rtnl_link_get_arptype(this->_link);
  }
  return(type);
}

bool
RouteLink::ArpType(const unsigned int type_)
{
  bool status = false;
  if (this->_link)
  {
    rtnl_link_set_arptype(this->_link, type_);
    status = true;
  }
  return (status);
}

std::string
RouteLink::TypeString() const
{
  const char* type = NULL;
  if (this->_link)
  {
    type = rtnl_link_get_type(this->_link);
  }
  return(type ? std::string(type) : std::string(""));
}

bool
RouteLink::TypeString(const std::string& type_)
{
  bool status = false;
  if (this->_link)
  {
    int ret = rtnl_link_set_type(this->_link, type_.c_str());
    status = (ret == 0);
  }
  return (status);
}

uint32_t
RouteLink::Flags() const
{
  uint32_t flags = 0;
  if (this->_link)
  {
    flags = rtnl_link_get_flags(this->_link);
  }
  return(flags);
}

bool
RouteLink::SetFlags(const uint32_t flags_)
{
  bool status = false;
  if (this->_link)
  {
    rtnl_link_set_flags(this->_link, flags_);
    status = true;
  }
  return (status);
}

bool
RouteLink::ClrFlags(const uint32_t flags_)
{
  bool status = false;
  if (this->_link)
  {
    rtnl_link_unset_flags(this->_link, flags_);
    status = true;
  }
  return (status);
}

uint32_t
RouteLink::Mtu() const
{
  uint32_t mtu = 0;
  if (this->_link)
  {
    mtu = rtnl_link_get_mtu(this->_link);
  }
  return(mtu);
}

bool
RouteLink::Mtu(const uint32_t mtu_)
{
  bool status = false;
  if (this->_link)
  {
    rtnl_link_set_mtu(this->_link, mtu_);
    status = true;
  }
  return (status);
}

RouteLink::STATE
RouteLink::OperationalState() const
{
  uint8_t state = -1;
  if (this->_link)
  {
    state = rtnl_link_get_operstate(this->_link);
  }
  return (_operstate2state(state));
}

bool
RouteLink::OperationalState(const RouteLink::STATE state_)
{
  bool status = false;
  if (this->_link)
  {
    if (state_ == RouteLink::STATE_UP)
    {
      rtnl_link_set_operstate(this->_link, 1);
    }
    else if (state_ == RouteLink::STATE_DOWN)
    {
      rtnl_link_set_operstate(this->_link, 2);
    }
    else
    {
      rtnl_link_set_operstate(this->_link, 0);
    }
    status = true;
  }
  return (status);
}

RouteLink::STATE
RouteLink::CarrierState() const
{
  uint8_t carrier = -1;
  if (this->_link)
  {
    carrier = rtnl_link_get_carrier(this->_link);
  }
  return (_carrier2state(carrier));
}

bool
RouteLink::CarrierState(const RouteLink::STATE state_)
{
  bool status = false;
  if (this->_link)
  {
    if (state_ == RouteLink::STATE_UP)
    {
      rtnl_link_set_carrier(this->_link, 1);
    }
    else
    {
      rtnl_link_set_carrier(this->_link, 0);
    }
    status = true;
  }
  return (status);
}

std::string
RouteLink::HwAddress() const
{
  struct nl_addr* addr = NULL;
  char mac[256] = { 0 };
  if (this->_link)
  {
    addr = rtnl_link_get_addr(this->_link);
    nl_addr2str(addr, mac, 256);
  }
  return(std::string(mac));
}

bool
RouteLink::HwAddress(const std::string& addr_)
{
  bool status = false;
  int ret = 0;
  struct nl_addr* addr = NULL;
  ret = nl_addr_parse(addr_.c_str(), AF_LLC, &addr);
  if (this->_link && (ret == 0))
  {
    rtnl_link_set_addr(this->_link, addr);
    nl_addr_put(addr);
    status = true;
  }
  return(status);
}

void
RouteLink::Display() const
{
  std::cout << "Link: " << std::endl;
  std::cout << "\tMaster:\t" << this->MasterIfIndex() << std::endl;
  std::cout << "\tIndex: \t" << this->IfIndex() << std::endl;
  std::cout << "\tName:  \t" << this->IfName() << std::endl;
  std::cout << "\tType:  \t" << _arptype2str(this->ArpType()) << std::endl;
  std::cout << "\tType:  \t" << this->TypeString() << std::endl;
  std::cout << "\tFlags: \t" << this->Flags() << std::endl;
  std::cout << "\tMTU:   \t" << this->Mtu() << std::endl;
  std::cout << "\tLink:  \t" << _state2str(this->CarrierState()) << std::endl;
  std::cout << "\tState: \t" << _state2str(this->OperationalState()) << std::endl;
  std::cout << "\tMAC:   \t" << this->HwAddress() << std::endl;
}

bool
RouteLink::_copy(const RouteLink& other_)
{
  bool status = true;
  status &= this->IfIndex(other_.IfIndex());
  status &= this->IfName(other_.IfName());
  status &= this->ArpType(other_.ArpType());
  status &= this->TypeString(other_.TypeString());
  status &= this->ClrFlags(~other_.Flags());
  status &= this->SetFlags(other_.Flags());
  status &= this->Mtu(other_.Mtu());
  status &= this->OperationalState(other_.OperationalState());
  status &= this->CarrierState(other_.CarrierState());
  status &= this->HwAddress(other_.HwAddress());
  return (status);
}

}
