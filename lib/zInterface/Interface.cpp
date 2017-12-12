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

Interface::Interface(const std::string& ifname_) :
    _modified(false), ifindex(0), config(ifname_)
{
  this->lock.Unlock();
  this->_init();
}

Interface::Interface(const zInterface::ConfigData& config_) :
    _modified(false), ifindex(0), config(config_)
{
  this->lock.Unlock();
  this->_init();
  this->SetIfName(config_.GetIfName(this->config.GetIfName()));
  this->SetIfType(config_.GetIfType(this->config.GetIfType()));
  this->SetHwAddress(config_.GetHwAddress(this->config.GetHwAddress()));
  this->SetMtu(config_.GetMtu(this->config.GetMtu()));
  this->SetAdminState(config_.GetAdminState(this->config.GetAdminState()));
  this->SetIpAddress(config_.GetIpAddress(this->config.GetIpAddress()));
  this->SetNetmask(config_.GetNetmask(this->config.GetNetmask()));
}

Interface::~Interface()
{
  this->lock.Lock();
}

unsigned int
Interface::GetIfIndex()
{
  unsigned int index = 0;
  if (this->lock.Lock())
  {
    if (!this->ifindex)
    {
      GetLinkCommand cmd(this->config.GetIfName());
      if (cmd.Exec())
      {
        this->ifindex = cmd.IfIndex();
      }
    }
    index = this->ifindex;
    this->lock.Unlock();
  }
  return (index);
}

bool
Interface::SetIfIndex(const unsigned int ifindex_)
{
  bool status = false;
  if (this->lock.Lock())
  {
    this->ifindex = ifindex_;
    this->lock.Unlock();
    status = true;
  }
  return (status);
}

std::string
Interface::GetIfName() const
{
  std::string name;
  if (this->lock.Lock())
  {
    if (this->ifindex)
    {
      GetLinkCommand cmd(this->ifindex);
      if (cmd.Exec())
      {
        name = cmd.IfName();
      }
    }
    else
    {
      name = this->config.GetIfName();
    }
    this->lock.Unlock();
  }
  return (name);
}

bool
Interface::SetIfName(const std::string& name_)
{
  bool status = false;
  if (this->lock.Lock())
  {
    if ((name_ != this->config.GetIfName()) && this->config.SetIfName(name_) && this->ifindex)
    {
      SetLinkCommand* cmd = new SetLinkCommand(this->ifindex);
      if (cmd->IfName(name_))
      {
        this->_cmds.push_back(cmd);
        this->set_modified();
        status = true;
      }
    }
    this->lock.Unlock();
  }
  return (status);
}

ConfigData::IFTYPE
Interface::GetIfType() const
{
  ConfigData::IFTYPE type = ConfigData::IFTYPE_ERR;
  if (this->lock.Lock())
  {
    if (this->ifindex)
    {
      GetLinkCommand cmd(this->ifindex);
      if (cmd.Exec())
      {
        switch (cmd.ArpType())
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
    }
    else
    {
      type = this->config.GetIfType();
    }
    this->lock.Unlock();
  }
  return (type);
}

bool
Interface::SetIfType(const ConfigData::IFTYPE type_)
{
  bool status = false;
  if (this->lock.Lock())
  {
    status = this->config.SetIfType(type_);
    this->lock.Unlock();
  }
  return (status);
}

std::string
Interface::GetHwAddress() const
{
  std::string addr;
  if (this->lock.Lock())
  {
    if (this->ifindex)
    {
      GetLinkCommand cmd(this->ifindex);
      if (cmd.Exec())
      {
        addr = cmd.HwAddress();
      }
    }
    else
    {
      addr = this->config.GetHwAddress();
    }
    this->lock.Unlock();
  }
  return (addr);
}

bool
Interface::SetHwAddress(const std::string& addr_)
{
  bool status = false;
  if (this->lock.Lock())
  {
    if ((addr_ != this->config.GetHwAddress()) && this->config.SetHwAddress(addr_) && this->ifindex)
    {
      SetLinkCommand* cmd = new SetLinkCommand(this->ifindex);
      if (cmd->HwAddress(addr_))
      {
        this->_cmds.push_back(cmd);
        this->set_modified();
        status = true;
      }
    }
    this->lock.Unlock();
  }
  return (status);
}

unsigned int
Interface::GetMtu() const
{
  unsigned int mtu = 0;
  if (this->lock.Lock())
  {
    if (this->ifindex)
    {
      GetLinkCommand cmd(this->ifindex);
      if (cmd.Exec())
      {
        mtu = cmd.Mtu();
      }
    }
    else
    {
      mtu = this->config.GetMtu();
    }
    this->lock.Unlock();
  }
  return (mtu);
}

bool
Interface::SetMtu(const unsigned int mtu_)
{
  bool status = false;
  if (this->lock.Lock())
  {
    if ((mtu_ != this->config.GetMtu()) && this->config.SetMtu(mtu_) && this->ifindex)
    {
      SetLinkCommand* cmd = new SetLinkCommand(this->ifindex);
      if (cmd->Mtu(mtu_))
      {
        this->_cmds.push_back(cmd);
        this->set_modified();
        status = true;
      }
    }
    this->lock.Unlock();
  }
  return (status);
}

ConfigData::STATE
Interface::GetAdminState() const
{
  ConfigData::STATE state = ConfigData::STATE_ERR;
  if (this->lock.Lock())
  {
    if (this->ifindex)
    {
      GetLinkCommand cmd(this->ifindex);
      if (cmd.Exec())
      {
        if ((cmd.Flags() & IFF_UP))
        {
          state = ConfigData::STATE_UP;
        }
        else
        {
          state = ConfigData::STATE_DOWN;
        }
      }
    }
    else
    {
      state = this->config.GetAdminState();
    }
    this->lock.Unlock();
  }
  return (state);
}

bool
Interface::SetAdminState(const ConfigData::STATE state_)
{
  bool status = false;
  if (this->lock.Lock())
  {
    if ((state_ != this->config.GetAdminState()) && this->config.SetAdminState(state_))
    {
      SetLinkCommand* cmd = new SetLinkCommand(this->ifindex);
      if (state_ == ConfigData::STATE_UP)
      {
        status = cmd->SetFlags(IFF_UP);
      }
      else
      {
        status = cmd->ClrFlags(IFF_UP);
      }
      if (status)
      {
        this->_cmds.push_back(cmd);
        this->set_modified();
      }
    }
    this->lock.Unlock();
  }
  return (status);
}

std::string
Interface::GetIpAddress() const
{
  std::string addr;
  if (this->lock.Lock())
  {
    addr = this->config.GetIpAddress();
    this->lock.Unlock();
  }
  return (addr);
}

bool
Interface::SetIpAddress(const std::string& addr_)
{
  bool status = false;
  if (this->lock.Lock())
  {
    if ((addr_ != this->config.GetIpAddress()) && this->config.SetIpAddress(addr_))
    {
      status = this->_modified = true;
    }
    this->lock.Unlock();
  }
  return (status);
}

std::string
Interface::GetNetmask() const
{
  std::string addr;
  if (this->lock.Lock())
  {
    addr = this->config.GetNetmask();
    this->lock.Unlock();
  }
  return (addr);
}

bool
Interface::SetNetmask(const std::string& addr_)
{
  bool status = false;
  if (this->lock.Lock())
  {
    if ((addr_ != this->config.GetNetmask()) && this->config.SetNetmask(addr_))
    {
      status = this->_modified = true;
    }
    this->lock.Unlock();
  }
  return (status);
}

bool
Interface::Refresh()
{
  bool status = false;
  GetLinkCommand* cmd = NULL;

  // Use the interface index if it is known otherwise lookup by name
  if (this->ifindex)
  {
    cmd = new GetLinkCommand(this->ifindex);
  }
  else
  {
    cmd = new GetLinkCommand(this->config.GetIfName());
  }

  // Execute command
  if (cmd)
  {
    if (cmd->Exec())
    {
      this->ifindex = cmd->IfIndex();
      this->SetIfName(this->GetIfName());
      this->SetIfType(this->GetIfType());
      this->SetHwAddress(this->GetHwAddress());
      this->SetMtu(this->GetMtu());
      this->SetAdminState(this->GetAdminState());
      status = true;
    }
    delete (cmd);
  }
  return(status);
}

bool
Interface::Commit()
{
  bool status = false;
  if (this->lock.Lock())
  {
    status = true;
    while (!this->_cmds.empty())
    {
      Command* cmd = this->_cmds.front();
      cmd->Display();
      if (!(status = cmd->Exec()))
      {
        break;
      }
      this->_cmds.pop_front();
      delete (cmd);
    }
    this->lock.Unlock();
  }
  return (status);
}

bool
Interface::Create()
{
  return(this->Commit());
}

bool
Interface::Destroy()
{
  return(this->Commit());
}

void
Interface::Display(const std::string &prefix_)
{
  std::cout << std::endl << "--------------- Interface ---------------" << std::endl;
  std::cout << prefix_ << "Index:  \t" << this->GetIfIndex() << std::endl;
  std::cout << prefix_ << "Name:   \t" << this->GetIfName() << std::endl;
  std::cout << prefix_ << "Type:   \t" << this->GetIfType() << std::endl;
  std::cout << prefix_ << "MAC:    \t" << this->GetHwAddress() << std::endl;
  std::cout << prefix_ << "MTU:    \t" << this->GetMtu() << std::endl;
  std::cout << prefix_ << "State:  \t" << this->GetAdminState() << std::endl;
}

bool
Interface::is_modified() const
{
  return (this->_modified);
}

void
Interface::set_modified()
{
  this->_modified = true;
}

void
Interface::clr_modified()
{
  while (!this->_cmds.empty())
  {
    delete (this->_cmds.front());
    this->_cmds.pop_front();
  }
  this->_modified = false;
}

void
Interface::_init()
{
  GetLinkCommand cmd(this->config.GetIfName());
  if (cmd.Exec())
  {
    this->ifindex = cmd.IfIndex();
    this->SetIfName(this->GetIfName());
    this->SetIfType(this->GetIfType());
    this->SetHwAddress(this->GetHwAddress());
    this->SetMtu(this->GetMtu());
    this->SetAdminState(this->GetAdminState());
    this->clr_modified();
  }
}

}
}
