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
#include <sys/ioctl.h>
#include <sys/socket.h>
#include <net/if.h>
#include <net/if_arp.h>

// libc++ includes

#include <string>

// libzutils includes

#include <zutils/zCompatibility.h>
#include <zutils/zLog.h>
#include <zutils/zInterface.h>
#include <zutils/netlink/GetLinkCommand.h>
#include <zutils/netlink/SetLinkCommand.h>
#include <zutils/netlink/RouteLinkEvent.h>
using namespace netlink;

ZLOG_MODULE_INIT(zUtils::zLog::Log::MODULE_INTERFACE);

// local includes


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
    stagingConfig(ifname_), workingConfig(ifname_)
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
  return (this->stagingConfig);
}

bool
Interface::SetConfig(zInterface::ConfigData config_)
{
  bool status = this->SetIfIndex(config_.GetIfIndex(this->workingConfig.GetIfIndex()));
  status &= this->SetIfName(config_.GetIfName(this->workingConfig.GetIfName()));
  status &= this->SetIfType(config_.GetIfType(this->workingConfig.GetIfType()));
  status &= this->SetHwAddress(config_.GetHwAddress(this->workingConfig.GetHwAddress()));
  status &= this->SetMtu(config_.GetMtu(this->workingConfig.GetMtu()));
  status &= this->SetIpAddress(config_.GetIpAddress(this->workingConfig.GetIpAddress()));
  status &= this->SetNetmask(config_.GetNetmask(this->workingConfig.GetNetmask()));
  status &= this->SetPromiscuousMode(config_.GetPromiscuousMode(this->workingConfig.GetPromiscuousMode()));
  status &= this->SetAdminState(config_.GetAdminState(this->workingConfig.GetAdminState()));
  return (status);
}

unsigned int
Interface::GetIfIndex() const
{
  unsigned int ifindex = 0;
  if (this->lock.Lock())
  {
    ifindex = this->getIfIndex();
    if (!ifindex)
    {
      ifindex = this->stagingConfig.GetIfIndex();
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
    status = this->stagingConfig.SetIfIndex(ifindex_);
    this->lock.Unlock();
  }
  return (status);
}

std::string
Interface::GetIfName() const
{
  std::string name;
  if (this->lock.Lock())
  {
    name = this->getIfName();
    if (name.empty())
    {
      name = this->stagingConfig.GetIfName();
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
    status = this->stagingConfig.SetIfName(name_);
    this->lock.Unlock();
  }
  return (status);
}

unsigned int
Interface::GetMasterIfIndex() const
{
  unsigned int ifindex = 0;
  if (this->lock.Lock())
  {
    ifindex = this->getMasterIfIndex();
    if (!ifindex)
    {
      ifindex = this->stagingConfig.GetMasterIfIndex();
    }
    this->lock.Unlock();
  }
  return (ifindex);
}

bool
Interface::SetMasterIfIndex(const unsigned int ifindex_)
{
  bool status = false;
  if (this->lock.Lock())
  {
    status = this->stagingConfig.SetMasterIfIndex(ifindex_);
    this->lock.Unlock();
  }
  return (status);
}

std::string
Interface::GetMasterIfName() const
{
  std::string name;
  if (this->lock.Lock())
  {
    name = this->getMasterIfName();
    if (name.empty())
    {
      name = this->stagingConfig.GetMasterIfName();
    }
    this->lock.Unlock();
  }
  return (name);
}

bool
Interface::SetMasterIfName(const std::string& name_)
{
  bool status = false;
  if (this->lock.Lock())
  {
    status = this->stagingConfig.SetMasterIfName(name_);
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
    type = this->getIfType();
    if (type == ConfigData::IFTYPE_ERR)
    {
      type = this->stagingConfig.GetIfType();
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
    status = this->stagingConfig.SetIfType(type_);
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
    addr = this->getHwAddress();
    if (addr.empty())
    {
      addr = this->stagingConfig.GetHwAddress();
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
    status = this->stagingConfig.SetHwAddress(addr_);
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
    mtu = this->getMtu();
    if (!mtu)
    {
      mtu = this->stagingConfig.GetMtu();
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
    status = this->stagingConfig.SetMtu(mtu_);
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
    state = this->getAdminState();
    if (state == ConfigData::STATE_ERR)
    {
      state = this->stagingConfig.GetAdminState();
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
    status = this->stagingConfig.SetAdminState(state_);
    this->lock.Unlock();
  }
  return (status);
}

ConfigData::PROMODE
Interface::GetPromiscuousMode() const
{
  ConfigData::PROMODE mode = ConfigData::PROMODE_ERR;
  if (this->lock.Lock())
  {
    mode = this->getPromiscuousMode();
    if (mode == ConfigData::PROMODE_ERR)
    {
      mode = this->stagingConfig.GetPromiscuousMode();
    }
    this->lock.Unlock();
  }
  return (mode);
}

bool
Interface::SetPromiscuousMode(const ConfigData::PROMODE mode_)
{
  bool status = false;
  if (this->lock.Lock())
  {
    status = this->stagingConfig.SetPromiscuousMode(mode_);
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
    addr = this->getIpAddress();
    if (addr.empty())
    {
      addr = this->stagingConfig.GetIpAddress();
    }
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
    status = this->stagingConfig.SetIpAddress(addr_);
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
    addr = this->getNetmask();
    if (addr.empty())
    {
      addr = this->stagingConfig.GetNetmask();
    }
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
    status = this->stagingConfig.SetNetmask(addr_);
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
    status =  this->workingConfig.SetIfIndex(this->GetIfIndex());
    status &= this->workingConfig.SetIfName(this->GetIfName());
    status &= this->workingConfig.SetIfType(this->GetIfType());
    status &= this->workingConfig.SetHwAddress(this->GetHwAddress());
    status &= this->workingConfig.SetMtu(this->GetMtu());
    status &= this->workingConfig.SetAdminState(this->GetAdminState());
    status &= this->workingConfig.SetIpAddress(this->GetIpAddress());
    status &= this->workingConfig.SetNetmask(this->GetNetmask());
  }
  if (status)
  {
    *this->stagingConfig.GetData() = *this->workingConfig.GetData();
  }
  return(status);
}

bool
Interface::Commit()
{
  bool status = false;
  if (this->lock.Lock())
  {
    status = true; // Innocent until proven guilty

    // Always make this command first to ensure all commands are executed while the interface is down
    if (zInterface::ConfigData::STATE_DOWN == this->workingConfig.GetAdminState())
    {
      if (this->stagingConfig.GetIfName() != this->workingConfig.GetIfName())
      {
        status &= this->setIfName(this->stagingConfig.GetIfName());
      }

      if (this->stagingConfig.GetIfType() != this->workingConfig.GetIfType())
      {
        status &= this->setIfType(this->stagingConfig.GetIfType());
      }

      if (this->stagingConfig.GetHwAddress() != this->workingConfig.GetHwAddress())
      {
        status &= this->setHwAddress(this->stagingConfig.GetHwAddress());
      }
    }

    // The following commands can be executed regardless of the interfaces' administrative state
    if (this->stagingConfig.GetMtu() != this->workingConfig.GetMtu())
    {
      status &= this->setMtu(this->stagingConfig.GetMtu());
    }

    if (this->stagingConfig.GetIpAddress() != this->workingConfig.GetIpAddress())
    {
      status &= this->setIpAddress(this->stagingConfig.GetIpAddress());
    }

    if (this->stagingConfig.GetNetmask() != this->workingConfig.GetNetmask())
    {
      status &= this->setNetmask(this->stagingConfig.GetNetmask());
    }

    // Always make this command last to ensure all above commands are executed while the interface is down
    if (this->stagingConfig.GetAdminState() != this->workingConfig.GetAdminState())
    {
      status &= this->setAdminState(this->stagingConfig.GetAdminState());
    }

    if (this->stagingConfig.GetPromiscuousMode() != this->workingConfig.GetPromiscuousMode())
    {
      status &= this->setPromiscuousMode(this->stagingConfig.GetPromiscuousMode());
    }

    status &= this->execCommands();

    this->lock.Unlock();
  }
  return (status);
}

bool
Interface::Create()
{
  bool status = false;
  if (this->lock.Lock())
  {
    status = this->execCommands();
    this->lock.Unlock();
  }
  return (status);
}

bool
Interface::Destroy()
{
  bool status = false;
  if (this->lock.Lock())
  {
    status = this->execCommands();
    this->lock.Unlock();
  }
  return (status);
}

void
Interface::Display(const std::string &prefix_)
{
  std::cout << std::endl << "--------------- Interface ---------------" << std::endl;
  std::cout << prefix_ << "Master: \t" << this->GetMasterIfIndex() << std::endl;
  std::cout << prefix_ << "Index:  \t" << this->GetIfIndex() << std::endl;
  std::cout << prefix_ << "Name:   \t" << this->GetIfName() << std::endl;
  std::cout << prefix_ << "Type:   \t" << _type2str(this->GetIfType()) << std::endl;
  std::cout << prefix_ << "MAC:    \t" << this->GetHwAddress() << std::endl;
  std::cout << prefix_ << "MTU:    \t" << this->GetMtu() << std::endl;
  std::cout << prefix_ << "State:  \t" << _state2str(this->GetAdminState()) << std::endl;
  std::cout << prefix_ << "ProMode:\t" << int(this->GetPromiscuousMode()) << std::endl;
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
Interface::execCommands()
{
  bool status = true;
  // Loop through all commands and execute each independently; don't bail on any one failure
  while (!this->_cmds.empty())
  {
    Command* cmd = this->_cmds.front();
//    cmd->Display();
    if (!cmd->Exec())
    {
      status = false;
      std::cout << "Error executing command: " << std::endl;
      cmd->Display();
    }
    this->_cmds.pop_front();
    delete (cmd);
  }

  return (status);
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

unsigned int
Interface::getIfIndex() const
{
  unsigned int ifindex = 0;
  if (!this->workingConfig.GetIfName().empty())
  {
    GetLinkCommand cmd(this->workingConfig.GetIfName());
    if (cmd.Exec())
    {
      ifindex = cmd.IfIndex();
    }
  }
  return (ifindex);
}

bool
Interface::setIfIndex(const unsigned int ifindex_)
{
  return (true);
}

unsigned int
Interface::getMasterIfIndex() const
{
  unsigned int ifindex = 0;
  if (this->workingConfig.GetIfIndex())
  {
    GetLinkCommand cmd(this->workingConfig.GetIfIndex());
    if (cmd.Exec())
    {
      ifindex = cmd.MasterIfIndex();
    }
  }
  return (ifindex);
}

bool
Interface::setMasterIfIndex(const unsigned int ifindex_)
{
  bool status = false;
  if (this->workingConfig.GetIfIndex())
  {
    SetLinkCommand* cmd = new SetLinkCommand(this->workingConfig.GetIfIndex());
    if (cmd->MasterIfIndex(ifindex_))
    {
      this->addCommand(cmd);
      status = true;
    }
  }
  return (status);
}

std::string
Interface::getIfName() const
{
  std::string name;
  if (this->workingConfig.GetIfIndex())
  {
    GetLinkCommand cmd(this->workingConfig.GetIfIndex());
    if (cmd.Exec())
    {
      name = cmd.IfName();
    }
  }
  return (name);
}

bool
Interface::setIfName(const std::string& name_)
{
  bool status = false;
  if (this->workingConfig.GetIfIndex())
  {
    SetLinkCommand* cmd = new SetLinkCommand(this->workingConfig.GetIfIndex());
    if (cmd->IfName(name_))
    {
      this->addCommand(cmd);
      status = true;
    }
  }
  return (status);
}

std::string
Interface::getMasterIfName() const
{
  std::string name;
  if (this->workingConfig.GetIfIndex())
  {
    GetLinkCommand cmd(this->workingConfig.GetIfIndex());
    if (cmd.Exec())
    {
      name = cmd.MasterIfName();
    }
  }
  return (name);
}

bool
Interface::setMasterIfName(const std::string& name_)
{
  bool status = false;
  if (this->workingConfig.GetIfIndex())
  {
    SetLinkCommand* cmd = new SetLinkCommand(this->workingConfig.GetIfIndex());
    if (cmd->MasterIfName(name_))
    {
      this->addCommand(cmd);
      status = true;
    }
  }
  return (status);
}

ConfigData::IFTYPE
Interface::getIfType() const
{
  ConfigData::IFTYPE type = ConfigData::IFTYPE_ERR;
  if (this->workingConfig.GetIfIndex())
  {
    GetLinkCommand cmd(this->workingConfig.GetIfIndex());
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
  return (type);
}

bool
Interface::setIfType(const ConfigData::IFTYPE type_)
{
  bool status = true; // TODO
  return (status);
}

std::string
Interface::getHwAddress() const
{
  std::string addr;
  if (this->workingConfig.GetIfIndex())
  {
    GetLinkCommand cmd(this->workingConfig.GetIfIndex());
    if (cmd.Exec())
    {
      addr = cmd.HwAddress();
    }
  }
  return (addr);
}

bool
Interface::setHwAddress(const std::string& addr_)
{
  bool status = false;
  if (this->workingConfig.GetIfIndex())
  {
    SetLinkCommand* cmd = new SetLinkCommand(this->workingConfig.GetIfIndex());
    if (cmd->HwAddress(addr_))
    {
      this->addCommand(cmd);
      status = true;
    }
  }
  return (status);
}

unsigned int
Interface::getMtu() const
{
  unsigned int mtu = 0;
  if (this->workingConfig.GetIfIndex())
  {
    GetLinkCommand cmd(this->workingConfig.GetIfIndex());
    if (cmd.Exec())
    {
      mtu = cmd.Mtu();
    }
  }
  return (mtu);
}

bool
Interface::setMtu(const unsigned int mtu_)
{
  bool status = false;
  if (this->workingConfig.GetIfIndex())
  {
    SetLinkCommand* cmd = new SetLinkCommand(this->workingConfig.GetIfIndex());
    if (cmd->Mtu(mtu_))
    {
      this->addCommand(cmd);
      status = true;
    }
  }
  return (status);
}

ConfigData::STATE
Interface::getAdminState() const
{
  ConfigData::STATE state = ConfigData::STATE_ERR;
  if (this->workingConfig.GetIfIndex())
  {
    GetLinkCommand cmd(this->workingConfig.GetIfIndex());
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
  return (state);
}

bool
Interface::setAdminState(const ConfigData::STATE state_)
{
  bool status = false;
  if (this->workingConfig.GetIfIndex())
  {
    SetLinkCommand* cmd = new SetLinkCommand(this->workingConfig.GetIfIndex());
    if (state_ == ConfigData::STATE_UP)
    {
      cmd->SetFlags(IFFLAGS_UP);
    }
    else
    {
      cmd->ClrFlags(IFFLAGS_UP);
    }
    this->addCommand(cmd);
    status = true;
  }
  return (status);
}

std::string
Interface::getIpAddress() const
{
  std::string addr;
  if (this->workingConfig.GetIfIndex())
  {
  }
  return (addr);
}

bool
Interface::setIpAddress(const std::string& addr_)
{
  bool status = true; // TODO
  return (status);
}

std::string
Interface::getNetmask() const
{
  std::string addr;
  if (this->workingConfig.GetIfIndex())
  {
  }
  return (addr);
}

bool
Interface::setNetmask(const std::string& addr_)
{
  bool status = true; // TODO
  return (status);
}

ConfigData::PROMODE
Interface::getPromiscuousMode() const
{
  ConfigData::PROMODE mode = ConfigData::PROMODE_ERR;
  if (this->workingConfig.GetIfIndex())
  {
    GetLinkCommand cmd(this->workingConfig.GetIfIndex());
    if (cmd.Exec())
    {
      if ((cmd.Flags() & IFF_PROMISC))
      {
        mode = ConfigData::PROMODE_ENABLED;
      }
      else
      {
        mode = ConfigData::PROMODE_DISABLED;
      }
    }
  }
  return (mode);
}

bool
Interface::setPromiscuousMode(const ConfigData::PROMODE mode_)
{
  bool status = false;
  if (this->workingConfig.GetIfIndex())
  {
    SetLinkCommand* cmd = new SetLinkCommand(this->workingConfig.GetIfIndex());
    if (mode_ == ConfigData::PROMODE_ENABLED)
    {
      status = cmd->SetFlags(IFF_PROMISC);
    }
    else if (mode_ == ConfigData::PROMODE_DISABLED)
    {
      status = cmd->ClrFlags(IFF_PROMISC);
    }

    if (status)
    {
      this->addCommand(cmd);
    }
  }
  return (status);
}

}
}
