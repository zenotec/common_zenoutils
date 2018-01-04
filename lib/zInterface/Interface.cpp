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

#define IFFLAGS_UP      (IFF_UP | IFF_RUNNING)

namespace zUtils
{
namespace zInterface
{

static std::string
_type2str(const ConfigData::IFTYPE type_)
{
  std::string str;
  switch (type_)
  {
  case ConfigData::IFTYPE_NONE:
    str = "None";
    break;
  case ConfigData::IFTYPE_IEEE8023:
    str = "IEEE802.3";
    break;
  case ConfigData::IFTYPE_IEEE80211:
    str = "IEEE802.11";
    break;
  default:
    str = "Unknown";
    break;
  }
  return (str);
}

static std::string
_state2str(const ConfigData::STATE state_)
{
  std::string str;
  switch (state_)
  {
  case ConfigData::STATE_NONE:
    str = "None";
    break;
  case ConfigData::STATE_UP:
    str = "Up";
    break;
  case ConfigData::STATE_DOWN:
    str = "Down";
    break;
  case ConfigData::STATE_UNKNOWN:
    // no break
  default:
    str = "Unknown";
    break;
  }
  return (str);
}

// ****************************************************************************
// Class: Interface
// ****************************************************************************

Interface::Interface(const std::string& ifname_) :
    _config(ifname_)
{
  this->lock.Unlock();
}

Interface::~Interface()
{
  this->lock.Lock();
}

zInterface::ConfigData
Interface::GetConfig() const
{
  return (this->_config);
}

bool
Interface::SetConfig(zInterface::ConfigData config_)
{
  this->SetIfIndex(config_.GetIfIndex(this->_config.GetIfIndex()));
  this->SetIfName(config_.GetIfName(this->_config.GetIfName()));
  this->SetIfType(config_.GetIfType(this->_config.GetIfType()));
  this->SetHwAddress(config_.GetHwAddress(this->_config.GetHwAddress()));
  this->SetMtu(config_.GetMtu(this->_config.GetMtu()));
  this->SetIpAddress(config_.GetIpAddress(this->_config.GetIpAddress()));
  this->SetNetmask(config_.GetNetmask(this->_config.GetNetmask()));
  this->SetAdminState(config_.GetAdminState(this->_config.GetAdminState())); // must be last
  return (this->Commit());
}

unsigned int
Interface::GetIfIndex() const
{
  unsigned int ifindex = 0;
  if (this->lock.Lock())
  {
    if (!this->_config.GetIfIndex() && !this->_config.GetIfName().empty())
    {
      GetLinkCommand cmd(this->_config.GetIfName());
      if (cmd.Exec())
      {
        ifindex = cmd.IfIndex();
      }
    }
    else
    {
      ifindex = this->_config.GetIfIndex();
    }
    this->lock.Unlock();
  }
  return (ifindex);
}

bool
Interface::SetIfIndex(const unsigned int ifindex_)
{
  bool status = false;
  if (this->lock.Lock())
  {
    this->_config.SetIfIndex(ifindex_);
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
    if (this->_config.GetIfIndex())
    {
      GetLinkCommand cmd(this->_config.GetIfIndex());
      if (cmd.Exec())
      {
        name = cmd.IfName();
      }
    }
    else
    {
      name = this->_config.GetIfName();
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
    if (name_ != this->_config.GetIfName())
    {
      status = this->_config.SetIfName(name_);
      if (status && this->_config.GetIfIndex())
      {
        SetLinkCommand* cmd = new SetLinkCommand(this->_config.GetIfIndex());
        if (cmd->IfName(name_))
        {
          this->addCommand(cmd);
        }
      }
    }
    else
    {
      status = true;
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
    if (this->_config.GetIfIndex())
    {
      GetLinkCommand cmd(this->_config.GetIfIndex());
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
      type = this->_config.GetIfType();
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
    status = this->_config.SetIfType(type_);
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
    if (this->_config.GetIfIndex())
    {
      GetLinkCommand cmd(this->_config.GetIfIndex());
      if (cmd.Exec())
      {
        addr = cmd.HwAddress();
      }
    }
    else
    {
      addr = this->_config.GetHwAddress();
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
    if (addr_ != this->_config.GetHwAddress())
    {
      status = this->_config.SetHwAddress(addr_);
      if (status && this->_config.GetIfIndex())
      {
        SetLinkCommand* cmd = new SetLinkCommand(this->_config.GetIfIndex());
        if (cmd->HwAddress(addr_))
        {
          this->addCommand(cmd);
        }
      }
    }
    else
    {
      status = true;
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
    if (this->_config.GetIfIndex())
    {
      GetLinkCommand cmd(this->_config.GetIfIndex());
      if (cmd.Exec())
      {
        mtu = cmd.Mtu();
      }
    }
    else
    {
      mtu = this->_config.GetMtu();
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
    if (mtu_ != this->_config.GetMtu())
    {
      status = this->_config.SetMtu(mtu_);
      if (status && this->_config.GetIfIndex())
      {
        SetLinkCommand* cmd = new SetLinkCommand(this->_config.GetIfIndex());
        if (cmd->Mtu(mtu_))
        {
          this->addCommand(cmd);
        }
      }
    }
    else
    {
      status = true;
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
    if (this->_config.GetIfIndex())
    {
      GetLinkCommand cmd(this->_config.GetIfIndex());
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
      state = this->_config.GetAdminState();
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
    if (state_ != this->_config.GetAdminState())
    {
      status = this->_config.SetAdminState(state_);
      if (status && this->_config.GetIfIndex())
      {
        SetLinkCommand* cmd = new SetLinkCommand(this->_config.GetIfIndex());
        if (state_ == ConfigData::STATE_UP)
        {
          cmd->SetFlags(IFFLAGS_UP);
        }
        else
        {
          cmd->ClrFlags(IFFLAGS_UP);
        }
        this->addCommand(cmd);
      }
    }
    else
    {
      status = true;
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
    addr = this->_config.GetIpAddress();
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
    if (addr_ != this->_config.GetIpAddress())
    {
      status = this->_config.SetIpAddress(addr_);
      if (status && this->_config.GetIfIndex())
      {
        status = true;
      }
    }
    else
    {
      status = true;
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
    addr = this->_config.GetNetmask();
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
    if (addr_ != this->_config.GetNetmask())
    {
      if (this->_config.SetNetmask(addr_) && this->_config.GetIfIndex())
      {
        status = true;
      }
    }
    else
    {
      status = true;
    }
    this->lock.Unlock();
  }
  return (status);
}

bool
Interface::Refresh()
{
  bool status = false;
  if (this->GetIfIndex())
  {
    this->_config.SetIfIndex(this->GetIfIndex());
    this->_config.SetIfName(this->GetIfName());
    this->_config.SetIfType(this->GetIfType());
    this->_config.SetHwAddress(this->GetHwAddress());
    this->_config.SetMtu(this->GetMtu());
    this->_config.SetAdminState(this->GetAdminState());
    status = true;
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
    while (status && !this->_cmds.empty())
    {
      Command* cmd = this->_cmds.front();
//      cmd->Display();
      status = cmd->Exec();
      if (status)
      {
        if (cmd->GetIfIndex())
        {
          this->_config.SetIfIndex(cmd->GetIfIndex());
        }
      }
      else
      {
        std::cout << "Error executing command: " << std::endl;
        cmd->Display();
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
  std::cout << prefix_ << "Type:   \t" << _type2str(this->GetIfType()) << std::endl;
  std::cout << prefix_ << "MAC:    \t" << this->GetHwAddress() << std::endl;
  std::cout << prefix_ << "MTU:    \t" << this->GetMtu() << std::endl;
  std::cout << prefix_ << "State:  \t" << _state2str(this->GetAdminState()) << std::endl;
}

size_t
Interface::addCommand(netlink::Command* cmd_, size_t index_)
{

  size_t cnt = 0;

  if (!index_)
  {
    this->_cmds.push_front(cmd_);
  }
  else if (index_ < this->_cmds.size())
  {
    std::list<netlink::Command*>::iterator it = this->_cmds.begin();
    while (cnt++ < index_) ++it;
    this->_cmds.insert(it, cmd_);
  }
  else
  {
    this->_cmds.push_back(cmd_);
    cnt = this->_cmds.size();
  }
  return (cnt);
}

bool
Interface::clrCommands()
{
  while (!this->_cmds.empty())
  {
    delete (this->_cmds.front());
    this->_cmds.pop_front();
  }
  return(this->_cmds.empty());
}

}
}
