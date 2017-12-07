/*
 * Copyright (c) 2016 Cable Television Laboratories, Inc. ("CableLabs")
 *                    and others.  All rights reserved.
 *
 * Copyright (c) 2014-2016 ZenoTec LLC (http://www.zenotec.net)
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
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/ioctl.h>
#include <sys/socket.h>
#include <linux/if.h>
#include <linux/if_arp.h>
#include <linux/wireless.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <ifaddrs.h>

// libc++ includes

#include <string>
#include <fstream>
#include <iostream>

// libzutils includes

#include <zutils/zCompatibility.h>

#include <zutils/zInterface.h>

// local includes

#include "GetLinkCommand.h"
#include "SetLinkCommand.h"
#include "RouteLinkEvent.h"
using namespace netlink;

namespace zUtils
{
namespace zInterface
{

// ****************************************************************************
// Class: Interface
// ****************************************************************************

Interface::Interface(const std::string& name_) :
    zEvent::Event(zEvent::Event::TYPE_INTERFACE), Config(name_), _refreshed(false),
    _modified(false), _getlinkcmd(NULL), _setlinkcmd(NULL), _rtlinkevent(NULL)

{
  if (name_.empty())
  {
    ZLOG_WARN("Interface(name_): Name is empty!");
  }
  this->_init();
  this->_lock.Unlock();
}

Interface::Interface(const zInterface::ConfigData& config_) :
    zEvent::Event(zEvent::Event::TYPE_INTERFACE), Config(config_), _refreshed(false),
    _modified(false), _getlinkcmd(NULL), _setlinkcmd(NULL), _rtlinkevent(NULL)

{
  if (this->Config.GetIfName().empty())
  {
    ZLOG_WARN("Interface(config_): Name is empty!");
  }
  this->_init();
  this->_lock.Unlock();
}

Interface::~Interface()
{
  this->_lock.Lock();
  if (this->_getlinkcmd)
  {
    delete (this->_getlinkcmd);
    this->_getlinkcmd = NULL;
  }
  if (this->_setlinkcmd)
  {
    delete (this->_setlinkcmd);
    this->_setlinkcmd = NULL;
  }
}

bool
Interface::IsRefreshed() const
{
  bool flag = false;
  if (this->_lock.Lock())
  {
    flag = this->_refreshed;
    this->_lock.Unlock();
  }
  return (flag);
}

bool
Interface::IsModified() const
{
  bool flag = false;
  if (this->_lock.Lock())
  {
    flag = this->_modified;
    this->_lock.Unlock();
  }
  return (flag);
}

unsigned int
Interface::GetIfIndex() const
{
  unsigned int index = 0;
  if (this->_lock.Lock())
  {
    if (this->_refreshed)
    {
      index = this->_getlinkcmd->Link.IfIndex();
    }
    this->_lock.Unlock();
  }
  return (index);
}

std::string
Interface::GetIfName() const
{
  std::string name;
  if (this->_lock.Lock())
  {
    name = this->_getlinkcmd->GetIfName();
    this->_lock.Unlock();
  }
  return (name);
}

bool
Interface::SetIfName(const std::string& name_)
{
  bool status = false;
  if (this->_lock.Lock())
  {
    this->_refreshed = false;
    this->_getlinkcmd->SetIfName(name_);
    this->_setlinkcmd->SetIfName(name_);
    this->_lock.Unlock();
  }
  return (status);
}

ConfigData::IFTYPE
Interface::GetIfType() const
{
  ConfigData::IFTYPE type = ConfigData::IFTYPE_ERR;
  if (this->_lock.Lock())
  {
    if (this->_refreshed)
    {
      unsigned int arptype = this->_getlinkcmd->Link.ArpType();
      switch (arptype)
      {
      case ARPHRD_ETHER:
        // no break
      case ARPHRD_IEEE802:
        type = ConfigData::IFTYPE_IEEE8023;
        break;
      case ARPHRD_LOOPBACK:
        type = ConfigData::IFTYPE_LOOP;
        break;
      case ARPHRD_IEEE80211:
        // no break
      case ARPHRD_IEEE80211_RADIOTAP:
        type = ConfigData::IFTYPE_IEEE80211;
        break;
      default:
        type = ConfigData::IFTYPE_UNKNOWN;
        break;
      }
    }
    this->_lock.Unlock();
  }
  return (type);
}

std::string
Interface::GetHwAddress() const
{
  std::string addr;
  if (this->_lock.Lock())
  {
    if (this->_refreshed)
    {
      addr = this->_getlinkcmd->Link.HwAddress();
    }
    this->_lock.Unlock();
  }
  return (addr);
}

bool
Interface::SetHwAddress(const std::string& addr_)
{
  bool status = false;
  if (this->_lock.Lock())
  {
    status = this->_modified = this->_setlinkcmd->Link.HwAddress(addr_);
    this->_lock.Unlock();
  }
  return (status);
}

unsigned int
Interface::GetMtu() const
{
  unsigned int mtu = 0;
  if (this->_lock.Lock())
  {
    if (this->_refreshed)
    {
      mtu = this->_getlinkcmd->Link.Mtu();
    }
    this->_lock.Unlock();
  }
  return (mtu);
}

bool
Interface::SetMtu(const unsigned int mtu_)
{
  bool status = false;
  if (this->_lock.Lock())
  {
    status = this->_modified = this->_setlinkcmd->Link.Mtu(mtu_);
    this->_lock.Unlock();
  }
  return (status);
}

ConfigData::STATE
Interface::GetAdminState() const
{
  ConfigData::STATE state = ConfigData::STATE_ERR;
  if (this->_lock.Lock())
  {
    if (this->_refreshed)
    {
      uint32_t flags = this->_getlinkcmd->Link.Flags();
      if ((flags & (IFF_UP | IFF_RUNNING)) == (IFF_UP | IFF_RUNNING))
      {
        state = ConfigData::STATE_UP;
      }
      else
      {
        state = ConfigData::STATE_DOWN;
      }
    }
    this->_lock.Unlock();
  }
  return (state);
}

bool
Interface::SetAdminState(const ConfigData::STATE state_)
{
  bool status = false;

  if (this->_lock.Lock())
  {
    if (state_ == ConfigData::STATE_UP)
    {
      ZLOG_INFO("Administrative UP");
      status = this->_modified = this->_setlinkcmd->Link.SetFlags((IFF_UP | IFF_RUNNING));
    }
    else
    {
      ZLOG_INFO("Administrative DOWN");
      status = this->_modified = this->_setlinkcmd->Link.ClrFlags((IFF_UP | IFF_RUNNING));
    }
    this->_lock.Unlock();
  }
  return (status);
}

bool
Interface::Refresh()
{

  bool status = false;
  if (this->_lock.Lock())
  {
    // Validate and execute get link command
    status = this->_refreshed = this->_getlinkcmd->Exec();
    this->_lock.Unlock();
  }
  return(status);

}

bool
Interface::Commit()
{
  bool status = false;
  if (this->_lock.Lock())
  {
    if (this->_modified)
    {
      // Execute set link command and refresh
      if (this->_setlinkcmd->Exec())
      {
        this->_modified = false;
        status = this->_getlinkcmd->Exec();
      }
    }
    this->_lock.Unlock();
  }
  return (status);

}

bool
Interface::Create()
{
  return(false);
}

bool
Interface::Destroy()
{
  return(false);
}

void
Interface::Display(const std::string &prefix_)
{
  std::cout << prefix_ << "Refreshed: " << (this->IsRefreshed() ? "true" : "false");
  if (this->IsRefreshed())
  {
    std::cout << prefix_ << "Index:  \t" << this->GetIfIndex() << std::endl;
    std::cout << prefix_ << "Name:   \t" << this->GetIfName() << std::endl;
    std::cout << prefix_ << "Type:   \t" << this->GetIfType() << std::endl;
    std::cout << prefix_ << "MAC:    \t" << this->GetHwAddress() << std::endl;
    std::cout << prefix_ << "MTU:    \t" << this->GetMtu() << std::endl;
    std::cout << prefix_ << "State:  \t" << this->GetAdminState() << std::endl;
  }
}

void
Interface::_init()
{
  this->_getlinkcmd = new GetLinkCommand(this->Config.GetIfName());
  this->_setlinkcmd = new SetLinkCommand(this->Config.GetIfName());
}

}
}
