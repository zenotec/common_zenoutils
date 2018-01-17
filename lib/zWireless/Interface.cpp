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
#include <string.h>
#include <stdio.h>
#include <unistd.h>
#include <math.h>
#include <sys/types.h>
#include <sys/ioctl.h>
#include <sys/socket.h>
#include <linux/wireless.h>

#include <arpa/inet.h>
#include <netinet/in.h>
#include <ifaddrs.h>

// libc++ includes

#include <string>
#include <iostream>
#include <fstream>
#include <list>
#include <map>

// libzutils includes

#include <zutils/zCompatibility.h>
#include <zutils/zInterface.h>
#include <zutils/zWireless.h>
#include "GetPhyCommand.h"
#include "SetPhyCommand.h"
#include "GetInterfaceCommand.h"
#include "SetInterfaceCommand.h"
#include "NewInterfaceCommand.h"
#include "DelInterfaceCommand.h"
#include "SetChannelCommand.h"

using namespace nl80211;

ZLOG_MODULE_INIT(zUtils::zLog::Log::MODULE_WIRELESS);

namespace zUtils
{
namespace zWireless
{

static std::string
_hwmode2str(ConfigData::HWMODE mode_)
{
  std::string str;
  switch (mode_)
  {
  case ConfigData::HWMODE_NONE:
    str = "None";
    break;
  case ConfigData::HWMODE_ERR:
    str = "Error";
    break;
  case ConfigData::HWMODE_A:
    str = "A";
    break;
  case ConfigData::HWMODE_B:
    str = "B";
    break;
  case ConfigData::HWMODE_G:
    str = "G";
    break;
  case ConfigData::HWMODE_N:
    str = "N";
    break;
  case ConfigData::HWMODE_AC:
    str = "AC";
    break;
  case ConfigData::HWMODE_AD:
    str = "AD";
    break;
  case ConfigData::HWMODE_AX:
    str = "AX";
    break;
  default:
    str = "Unknown";
    break;
  }
  return (str);
}

static std::string
_htmode2str(ConfigData::HTMODE mode_)
{
  std::string str;
  switch (mode_)
  {
  case ConfigData::HTMODE_NONE:
    str = "None";
    break;
  case ConfigData::HTMODE_ERR:
    str = "Error";
    break;
  case ConfigData::HTMODE_NOHT:
    str = "Non-HT";
    break;
  case ConfigData::HTMODE_HT20:
    str = "HT20";
    break;
  case ConfigData::HTMODE_HT40MINUS:
    str = "HT40-";
    break;
  case ConfigData::HTMODE_HT40PLUS:
    str = "HT40+";
    break;
  default:
    str = "Unknown";
    break;
  }
  return (str);
}

static ConfigData::OPMODE
_nl2opmode(const uint32_t iftype_)
{
  ConfigData::OPMODE mode = ConfigData::OPMODE_ERR;
  switch (iftype_)
  {
  case NL80211_IFTYPE_STATION:
    mode = ConfigData::OPMODE_STA;
    break;
  case NL80211_IFTYPE_AP:
    mode = ConfigData::OPMODE_AP;
    break;
  case NL80211_IFTYPE_MONITOR:
    mode = ConfigData::OPMODE_MONITOR;
    break;
  case NL80211_IFTYPE_ADHOC:
    mode = ConfigData::OPMODE_ADHOC;
    break;
  default:
    mode = ConfigData::OPMODE_DEF;
    break;
  }
  return (mode);
}

static uint32_t
_opmode2nl(const ConfigData::OPMODE mode_)
{
  uint32_t iftype = 0;
  // Translate operational mode to NL80211 interface type
  switch (mode_)
  {
  case ConfigData::OPMODE_STA:
    iftype = NL80211_IFTYPE_STATION;
    break;
  case ConfigData::OPMODE_AP:
    iftype = NL80211_IFTYPE_AP;
    break;
  case ConfigData::OPMODE_MONITOR:
    iftype = NL80211_IFTYPE_MONITOR;
    break;
  case ConfigData::OPMODE_ADHOC:
    iftype = NL80211_IFTYPE_ADHOC;
    break;
  default:
    iftype = NL80211_IFTYPE_UNSPECIFIED;
    break;
  }
  return (iftype);
}

static std::string
_opmode2str(const ConfigData::OPMODE mode_)
{
  std::string str;
  switch (mode_)
  {
  case ConfigData::OPMODE_NONE:
    str = "None";
    break;
  case ConfigData::OPMODE_ERR:
    str = "Error";
    break;
  case ConfigData::OPMODE_STA:
    str = "Station";
    break;
  case ConfigData::OPMODE_AP:
    str = "AP";
    break;
  case ConfigData::OPMODE_MONITOR:
    str = "Monitor";
    break;
  case ConfigData::OPMODE_ADHOC:
    str = "AdHoc";
    break;
  default:
    str = "Unknown";
    break;
  }
  return (str);
}

// ****************************************************************************
// Class: zWireless::Interface
// ****************************************************************************

Interface::Interface(const std::string& ifname_) :
    zInterface::Interface(ifname_),
    stagingConfig(zInterface::Interface::stagingConfig),
    workingConfig(zInterface::Interface::workingConfig)
{
  this->SetIfType(zInterface::ConfigData::IFTYPE_IEEE80211);
}

Interface::~Interface()
{
}

zWireless::ConfigData
Interface::GetConfig() const
{
  return (this->workingConfig);
}

bool
Interface::SetConfig(zWireless::ConfigData config_)
{
  bool status = zInterface::Interface::SetConfig(config_);
  status &= this->SetPhyIndex(config_.GetPhyIndex(this->workingConfig.GetPhyIndex()));
  status &= this->SetPhyName(config_.GetPhyName(this->workingConfig.GetPhyName()));
  status &= this->SetHwMode(config_.GetHwMode(this->workingConfig.GetHwMode()));
  status &= this->SetHtMode(config_.GetHtMode(this->workingConfig.GetHtMode()));
  status &= this->SetOpMode(config_.GetOpMode(this->workingConfig.GetOpMode()));
  status &= this->SetChannel(config_.GetChannel(this->workingConfig.GetChannel()));
  status &= this->SetTxPower(config_.GetTxPower(this->workingConfig.GetTxPower()));
  return (status);
}

int
Interface::GetPhyIndex() const
{
  int index = -1;
  if (this->lock.Lock())
  {
    index = this->_getPhyIndex();
    if (index == -1)
    {
      index = this->stagingConfig.GetPhyIndex();
    }
    this->lock.Unlock();
  }
  return (index);
}

bool
Interface::SetPhyIndex(const int index_)
{
  bool status = false;
  if (this->lock.Lock())
  {
    status = this->stagingConfig.SetPhyIndex(index_);
    this->lock.Unlock();
  }
  return (status);
}

std::string
Interface::GetPhyName() const
{
  std::string name;
  if (this->lock.Lock())
  {
    name = this->_getPhyName();
    if (name.empty())
    {
      name = this->stagingConfig.GetPhyName();
    }
    this->lock.Unlock();
  }
  return (name);
}

bool
Interface::SetPhyName(const std::string& name_)
{
  bool status = false;
  if (this->lock.Lock())
  {
    status = this->stagingConfig.SetPhyName(name_);
    this->lock.Unlock();
  }
  return (status);
}

ConfigData::HWMODE
Interface::GetHwMode() const
{
  ConfigData::HWMODE mode = ConfigData::HWMODE_ERR;
  if (this->lock.Lock())
  {
    mode = this->_getHwMode();
    if (mode == ConfigData::HWMODE_ERR)
    {
      mode = this->stagingConfig.GetHwMode();
    }
    this->lock.Unlock();
  }
  return (mode);
}

bool
Interface::SetHwMode(const ConfigData::HWMODE mode_)
{
  bool status = false;
  if (this->lock.Lock())
  {
    status = this->stagingConfig.SetHwMode(mode_);
    this->lock.Unlock();
  }
  return (status);
}

ConfigData::HTMODE
Interface::GetHtMode() const
{
  ConfigData::HTMODE mode = ConfigData::HTMODE_ERR;
  if (this->lock.Lock())
  {
    mode = this->_getHtMode();
    if (mode == ConfigData::HTMODE_ERR)
    {
      mode = this->stagingConfig.GetHtMode();
    }
    this->lock.Unlock();
  }
  return (mode);
}

bool
Interface::SetHtMode(const ConfigData::HTMODE mode_)
{
  bool status = false;
  if (this->lock.Lock())
  {
    status = this->stagingConfig.SetHtMode(mode_);
    this->lock.Unlock();
  }
  return (status);
}

ConfigData::OPMODE
Interface::GetOpMode() const
{
  ConfigData::OPMODE mode = ConfigData::OPMODE_ERR;
  if (this->lock.Lock())
  {
    mode = this->_getOpMode();
    if (mode == ConfigData::OPMODE_ERR)
    {
      mode = this->stagingConfig.GetOpMode();
    }
    this->lock.Unlock();
  }
  return (mode);
}

bool
Interface::SetOpMode(const ConfigData::OPMODE mode_)
{
  bool status = false;
  if (this->lock.Lock())
  {
    status = this->stagingConfig.SetOpMode(mode_);
    this->lock.Unlock();
  }
  return (status);
}

unsigned int
Interface::GetChannel() const
{
  unsigned int channel = 0;
  if (this->lock.Lock())
  {
    channel = this->_getChannel();
    if (channel == 0)
    {
      channel = this->stagingConfig.GetChannel();
    }
    this->lock.Unlock();
  }
  return (channel);
}

bool
Interface::SetChannel(const unsigned int channel_)
{
  bool status = false;
  if (this->lock.Lock())
  {
    status = this->stagingConfig.SetChannel(channel_);
    this->lock.Unlock();
  }
  return (status);
}

unsigned int
Interface::GetTxPower() const
{
  unsigned int power = 0;
  if (this->lock.Lock())
  {
    power = this->_getTxPower();
    if (power == 0)
    {
      power = this->stagingConfig.GetTxPower();
    }
    this->lock.Unlock();
  }
  return (power);
}

bool
Interface::SetTxPower(const unsigned int txpower_)
{
  bool status = false;
  if (this->lock.Lock())
  {
    status = this->stagingConfig.SetTxPower(txpower_);
    this->lock.Unlock();
  }
  return (status);
}

bool
Interface::Refresh()
{
  bool status = zInterface::Interface::Refresh();
  status &= this->workingConfig.SetPhyIndex(this->GetPhyIndex());
  status &= this->workingConfig.SetPhyName(this->GetPhyName());
  status &= this->workingConfig.SetHwMode(this->GetHwMode());
  status &= this->workingConfig.SetHtMode(this->GetHtMode());
  status &= this->workingConfig.SetOpMode(this->GetOpMode());
  status &= this->workingConfig.SetChannel(this->GetChannel());
  status &= this->workingConfig.SetTxPower(this->GetTxPower());
  if (status)
  {
    *this->stagingConfig.GetData() = *this->workingConfig.GetData();
  }
  return (status);
}

bool
Interface::Commit()
{
  bool status = false;
  if (this->lock.Lock())
  {
    status = true; // Innocent until proven guilty

    // Always make this command first to ensure all commands are executed while the interface is down
    if (zWireless::ConfigData::STATE_DOWN != this->workingConfig.GetAdminState())
    {
      this->workingConfig.SetAdminState(zWireless::ConfigData::STATE_DOWN);
      status &= this->setAdminState(zWireless::ConfigData::STATE_DOWN);
    }

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

    if (this->stagingConfig.GetPhyName() != this->workingConfig.GetPhyName())
    {
      status &= this->_setPhyName(this->stagingConfig.GetPhyName());
    }

    if (this->stagingConfig.GetHwMode() != this->workingConfig.GetHwMode())
    {
      status &= this->_setHwMode(this->stagingConfig.GetHwMode());
    }

    if (this->stagingConfig.GetHtMode() != this->workingConfig.GetHtMode())
    {
      status &= this->_setHtMode(this->stagingConfig.GetHtMode());
    }

    if (this->stagingConfig.GetOpMode() != this->workingConfig.GetOpMode())
    {
      status &= this->_setOpMode(this->stagingConfig.GetOpMode());
    }

    // Always make this command last but before setting the channel / TX power to ensure
    //   all commands are executed while the interface is down
    if (this->stagingConfig.GetAdminState() != this->workingConfig.GetAdminState())
    {
      status &= this->setAdminState(this->stagingConfig.GetAdminState());
    }

    if (this->stagingConfig.GetPromiscuousMode() != this->workingConfig.GetPromiscuousMode())
    {
      status &= this->setPromiscuousMode(this->stagingConfig.GetPromiscuousMode());
    }

    if (this->stagingConfig.GetChannel() != this->workingConfig.GetChannel())
    {
      status &= this->_setChannel(this->stagingConfig.GetChannel());
    }

    if (this->stagingConfig.GetTxPower() != this->workingConfig.GetTxPower())
    {
      status &= this->_setTxPower(this->stagingConfig.GetTxPower());
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
    NewInterfaceCommand *cmd =
        new NewInterfaceCommand(this->stagingConfig.GetIfName(), this->stagingConfig.GetPhyIndex());
    cmd->IfType(_opmode2nl(this->stagingConfig.GetOpMode()));
    this->addCommand(cmd);
    status = this->execCommands();
    this->lock.Unlock();
  }

  if (status && this->GetIfIndex())
  {
    this->workingConfig.SetIfIndex(this->GetIfIndex());
    this->workingConfig.SetOpMode(this->GetOpMode());
    status &= this->Commit();
    status &= this->Refresh();
  }

  if (status)
  {
    *this->stagingConfig.GetData() = *this->workingConfig.GetData();
  }

  return (status);

}

bool
Interface::Destroy()
{

  bool status = false;

  if (this->lock.Lock())
  {
    DelInterfaceCommand* cmd = new DelInterfaceCommand(this->workingConfig.GetIfIndex());
    this->addCommand(cmd);
    status = this->execCommands();
    this->lock.Unlock();
  }

  return (status);

}

void
Interface::Display(const std::string &prefix_)
{
  zInterface::Interface::Display(prefix_);
  std::cout << prefix_ << "--------- Wireless Interface -----------" << std::endl;
  std::cout << prefix_ << "PHY:    \t[" << this->GetPhyIndex() << "]: " << this->GetPhyName()
      << std::endl;
  std::cout << prefix_ << "HWMODE: \t" << _hwmode2str(this->GetHwMode()) << std::endl;
  std::cout << prefix_ << "HTMODE: \t" << _htmode2str(this->GetHtMode()) << std::endl;
  std::cout << prefix_ << "OPMODE: \t" << _opmode2str(this->GetOpMode()) << std::endl;
  std::cout << prefix_ << "Channel:\t" << this->GetChannel() << std::endl;
  std::cout << prefix_ << "Power:  \t" << this->GetTxPower() << std::endl;
}

int
Interface::_getPhyIndex() const
{
  int index = -1;
  if (this->workingConfig.GetIfIndex())
  {
    GetInterfaceCommand cmd(this->workingConfig.GetIfIndex());
    if (cmd.Exec())
    {
      index = cmd.PhyIndex();
    }
  }
  return (index);
}

bool
Interface::_setPhyIndex(const int index_)
{
  return (true);
}

std::string
Interface::_getPhyName() const
{
  std::string name;
  if (this->workingConfig.GetPhyIndex() >= 0)
  {
    GetPhyCommand cmd(0); // Interface index is ignored; only PHY index is used
    cmd.PhyIndex(this->workingConfig.GetPhyIndex());
    if (cmd.Exec())
    {
      name = cmd.PhyName();
    }
  }
  return (name);
}

bool
Interface::_setPhyName(const std::string& name_)
{
  bool status = false;
  if (this->workingConfig.GetIfIndex() && (this->workingConfig.GetPhyIndex() >= 0) && !name_.empty())
  {
    SetPhyCommand* cmd = new SetPhyCommand(this->workingConfig.GetIfIndex());
    cmd->PhyIndex(this->workingConfig.GetPhyIndex());
    cmd->PhyName(name_);
    this->addCommand(cmd);
    status = true;
  }
  return (status);
}

ConfigData::HWMODE
Interface::_getHwMode() const
{
  return (ConfigData::HWMODE_ERR);
}

bool
Interface::_setHwMode(const ConfigData::HWMODE mode_)
{
  return (true);
}

ConfigData::HTMODE
Interface::_getHtMode() const
{
  ConfigData::HTMODE mode = ConfigData::HTMODE_ERR;
  if (this->workingConfig.GetIfIndex())
  {
    GetInterfaceCommand cmd(this->workingConfig.GetIfIndex());
    if (cmd.Exec())
    {
      switch (cmd.ChannelType())
      {
      case NL80211_CHAN_NO_HT:
        mode = ConfigData::HTMODE_NOHT;
        break;
      case NL80211_CHAN_HT20:
        mode = ConfigData::HTMODE_HT20;
        break;
      case NL80211_CHAN_HT40MINUS:
        mode = ConfigData::HTMODE_HT40MINUS;
        break;
      case NL80211_CHAN_HT40PLUS:
        mode = ConfigData::HTMODE_HT40PLUS;
        break;
      default:
        break;
      }
      switch (cmd.ChannelWidth())
      {
      case NL80211_CHAN_WIDTH_20_NOHT:
        mode = ConfigData::HTMODE_NOHT;
        break;
      case NL80211_CHAN_WIDTH_20:
        mode = ConfigData::HTMODE_VHT20;
        break;
      case NL80211_CHAN_WIDTH_40:
        mode = ConfigData::HTMODE_VHT40;
        break;
      case NL80211_CHAN_WIDTH_80:
        mode = ConfigData::HTMODE_VHT80;
        break;
      case NL80211_CHAN_WIDTH_80P80:
        mode = ConfigData::HTMODE_VHT80PLUS80;
        break;
      case NL80211_CHAN_WIDTH_160:
        mode = ConfigData::HTMODE_VHT160;
        break;
      default:
        break;
      }
    }
  }
  return (mode);
}

bool
Interface::_setHtMode(const ConfigData::HTMODE mode_)
{
  return (true); // TODO
}

ConfigData::OPMODE
Interface::_getOpMode() const
{
  ConfigData::OPMODE mode = ConfigData::OPMODE_ERR;
  if (this->workingConfig.GetIfIndex())
  {
    GetInterfaceCommand cmd(this->workingConfig.GetIfIndex());
    if (cmd.Exec())
    {
      mode = _nl2opmode(cmd.IfType());
    }
  }
  return (mode);
}

bool
Interface::_setOpMode(const ConfigData::OPMODE mode_)
{
  bool status = false;
  if (this->workingConfig.GetIfIndex())
  {
    SetInterfaceCommand* cmd = new SetInterfaceCommand(this->workingConfig.GetIfIndex());
    cmd->IfType(_opmode2nl(mode_));
    this->addCommand(cmd);
    status = true;
  }
  return (status);
}

unsigned int
Interface::_getChannel() const
{
  unsigned int channel = 0;
  if (this->workingConfig.GetIfIndex())
  {
    GetInterfaceCommand cmd(this->workingConfig.GetIfIndex());
    if (cmd.Exec())
    {
      channel = cmd.Frequency.GetChannel();
    }
  }
  return (channel);
}

bool
Interface::_setChannel(const unsigned int channel_)
{
  bool status = false;
  if (this->workingConfig.GetIfIndex() && (this->workingConfig.GetPhyIndex() >= 0) && channel_)
  {
    SetPhyCommand* cmd = new SetPhyCommand(this->workingConfig.GetIfIndex());
    cmd->PhyIndex(this->workingConfig.GetPhyIndex());
    cmd->Frequency.SetChannel(channel_);
    this->addCommand(cmd);
    status = true;
  }
  return (status);
}

unsigned int
Interface::_getTxPower() const
{
  unsigned int power = 0;
  if (this->workingConfig.GetIfIndex())
  {
    GetInterfaceCommand cmd(this->workingConfig.GetIfIndex());
    if (cmd.Exec())
    {
      power = cmd.TxPowerLevel();
    }
  }
  return (power);
}

bool
Interface::_setTxPower(unsigned int txpower_)
{
  bool status = false;
  if (this->workingConfig.GetIfIndex() && (this->workingConfig.GetPhyIndex() >= 0) && txpower_)
  {
    SetPhyCommand* cmd = new SetPhyCommand(this->workingConfig.GetIfIndex());
    cmd->PhyIndex(this->workingConfig.GetPhyIndex());
    cmd->TxPowerMode.SetMode(nl80211::TxPowerModeAttribute::MODE_FIXED);
    cmd->TxPowerLevel(txpower_);
    this->addCommand(cmd);
    status = true;
  }
  return (status);
}

}
}

