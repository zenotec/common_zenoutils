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

#include "GetLinkCommand.h"
#include "SetLinkCommand.h"
#include "RouteLinkEvent.h"
using namespace netlink;

#include <zutils/zInterface.h>

// local includes

namespace zUtils
{
namespace zInterface
{

// ****************************************************************************
// Class: Interface
// ****************************************************************************

Interface::Interface(const int index_) :
    zEvent::Event(zEvent::Event::TYPE_INTERFACE), _refreshed(false),
    _getlinkcmd(NULL), _setlinkcmd(NULL), _rtlinkevent(NULL)
{
  this->_getlinkcmd = new GetLinkCommand(index_);
  this->_setlinkcmd = new SetLinkCommand(index_);
  this->_rtlinkevent = new RouteLinkEvent;
  this->_lock.Unlock();
}

Interface::Interface(const std::string& name_) :
    zEvent::Event(zEvent::Event::TYPE_INTERFACE), _refreshed(false),
    _getlinkcmd(NULL), _setlinkcmd(NULL), _rtlinkevent(NULL)

{
  this->_getlinkcmd = new GetLinkCommand(name_);
  this->_setlinkcmd = new SetLinkCommand(name_);
  this->_rtlinkevent = new RouteLinkEvent;
  this->_lock.Unlock();
}

Interface::Interface(const zConfig::ConfigData& config_) :
		Config(config_), zEvent::Event(zEvent::Event::TYPE_INTERFACE),
		_refreshed(false), _getlinkcmd(NULL), _setlinkcmd(NULL), _rtlinkevent(NULL)

{
  ZLOG_DEBUG("Interface::Interface(config_)");
  ZLOG_DEBUG(this->Config.Path());
  ZLOG_DEBUG(this->Config.GetJson());
  this->_getlinkcmd = new GetLinkCommand(Config.Name());
  this->_setlinkcmd = new SetLinkCommand(Config.Name());
  this->_rtlinkevent = new RouteLinkEvent;
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
  if (this->_rtlinkevent)
  {
    delete (this->_rtlinkevent);
    this->_rtlinkevent = NULL;
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

int
Interface::GetIfIndex() const
{
  int index = -1;
  if (this->_lock.Lock())
  {
    if (this->_refreshed && this->_getlinkcmd)
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
    if (this->_refreshed && this->_getlinkcmd)
    {
      name = this->_getlinkcmd->Link.IfName();
    }
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
    if (this->_refreshed && this->_setlinkcmd)
    {
      this->_refreshed = false;
      this->_setlinkcmd->Link.IfName(name_);
      status = this->_setlinkcmd->Exec();
    }
    this->_lock.Unlock();
  }
  return (status);
}

Interface::IFTYPE
Interface::GetIfType() const
{

  Interface::IFTYPE type = Interface::TYPE_DEF;
  std::string str = this->Config.Type();

  if (str == ConfigData::ConfigTypeNone)
  {
    type = Interface::TYPE_NONE;
  }
  else if (str == ConfigData::ConfigTypeLoop)
  {
    type = Interface::TYPE_LOOP;
  }
  else if (str == ConfigData::ConfigTypeWired)
  {
    type = Interface::TYPE_WIRED;
  }
  else if (str == ConfigData::ConfigTypeWireless)
  {
    type = Interface::TYPE_WIRELESS;
  }
  else if (str == ConfigData::ConfigTypeOther)
  {
    type = Interface::TYPE_OTHER;
  }
  else if (str == ConfigData::ConfigTypeBond)
  {
    type = Interface::TYPE_BOND;
  }
  else if (str == ConfigData::ConfigTypeBridge)
  {
    type = Interface::TYPE_BRIDGE;
  }
  else
  {
    type = Interface::TYPE_ERR;
  }

  return (type);
}

bool
Interface::SetIfType(const Interface::IFTYPE type_)
{

  std::string str;

  switch (type_)
  {
  case Interface::TYPE_NONE:
    str = ConfigData::ConfigTypeNone;
    break;
  case Interface::TYPE_LOOP:
    str = ConfigData::ConfigTypeLoop;
    break;
  case Interface::TYPE_WIRED:
    str = ConfigData::ConfigTypeWired;
    break;
  case Interface::TYPE_WIRELESS:
    str = ConfigData::ConfigTypeWireless;
    break;
  case Interface::TYPE_OTHER:
    str = ConfigData::ConfigTypeOther;
    break;
  case Interface::TYPE_BOND:
    str = ConfigData::ConfigTypeBond;
    break;
  case Interface::TYPE_BRIDGE:
    str = ConfigData::ConfigTypeBridge;
    break;
  default:
    return(false);
  }

  return (this->Config.Type(str));
}

std::string
Interface::GetHwAddress() const
{
  std::string addr;
  if (this->_lock.Lock())
  {
    if (this->_refreshed && this->_getlinkcmd)
    {
      addr = this->_getlinkcmd->Link.Mac();
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
    if (this->_refreshed && this->_setlinkcmd)
    {
      this->_refreshed = false;
      this->_setlinkcmd->Link.Mac(addr_);
      status = this->_setlinkcmd->Exec();
      this->_setlinkcmd->Display();
    }
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
    if (this->_refreshed && this->_getlinkcmd)
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
    if (this->_refreshed && this->_setlinkcmd)
    {
      this->_refreshed = false;
      this->_setlinkcmd->Link.Mtu(mtu_);
      status = this->_setlinkcmd->Exec();
      this->_setlinkcmd->Display();
    }
    this->_lock.Unlock();
  }
  return (status);
}

Interface::STATE
Interface::GetAdminState() const
{
  Interface::STATE state = Interface::STATE_ERR;
  if (this->_lock.Lock())
  {
    if (this->_refreshed && this->_getlinkcmd)
    {
      uint32_t flags = this->_getlinkcmd->Link.Flags();
      if ((flags & (IFF_UP|IFF_RUNNING)) == (IFF_UP|IFF_RUNNING))
      {
        state = Interface::STATE_UP;
      }
      else
      {
        state = Interface::STATE_DOWN;
      }
    }
    this->_lock.Unlock();
  }
  return (state);
}

bool
Interface::SetAdminState(const Interface::STATE state_)
{
  bool status = false;
  if (this->_lock.Lock())
  {
    if (this->_refreshed && this->_setlinkcmd)
    {
      this->_refreshed = false;
      if (state_ == Interface::STATE_UP)
      {
        this->_setlinkcmd->Link.SetFlags((IFF_UP|IFF_RUNNING));
      }
      else
      {
        this->_setlinkcmd->Link.ClrFlags((IFF_UP|IFF_RUNNING));
      }
      status = this->_setlinkcmd->Exec();
      this->_setlinkcmd->Display();
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
    if (this->_getlinkcmd && this->_getlinkcmd->Exec())
    {
      status = this->_refreshed = true;
    }
    this->_lock.Unlock();
  }

  return(status);

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
  if (this->_lock.Lock())
  {
    if (this->_refreshed)
    {
      std::cout << prefix_ << "Index:  \t" << this->GetIfIndex() << std::endl;
      std::cout << prefix_ << "Name:   \t" << this->GetIfName() << std::endl;
      std::cout << prefix_ << "Type:   \t" << this->Config.Type() << std::endl;
      std::cout << prefix_ << "MAC:    \t" << this->GetHwAddress() << std::endl;
      std::cout << prefix_ << "MTU:    \t" << this->GetMtu() << std::endl;
      std::cout << prefix_ << "State:  \t" << this->GetAdminState() << std::endl;
    }
    else
    {
      std::cout << prefix_ << "Name:   \t" << this->Config.Name() << std::endl;
      std::cout << prefix_ << "Type:   \t" << this->Config.Type() << std::endl;
      std::cout << prefix_ << "MAC:    \t" << this->Config.HwAddress() << std::endl;
      std::cout << prefix_ << "MTU:    \t" << this->Config.Mtu() << std::endl;
      std::cout << prefix_ << "State:  \t" << this->Config.AdminState() << std::endl;
    }
    this->_lock.Unlock();
  }
}

}
}
