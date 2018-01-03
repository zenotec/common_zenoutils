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
    zInterface::Interface(ifname_), _config(zInterface::Interface::GetConfig().GetData())
{
  this->Refresh();
  this->SetIfType(zInterface::ConfigData::IFTYPE_IEEE80211);
}

Interface::~Interface()
{
}

zWireless::ConfigData
Interface::GetConfig() const
{
  return (this->_config);
}

bool
Interface::SetConfig(zWireless::ConfigData config_)
{
  this->SetIfIndex(config_.GetIfIndex(this->_config.GetIfIndex()));
  this->SetIfName(config_.GetIfName(this->_config.GetIfName()));
  this->SetIfType(config_.GetIfType(this->_config.GetIfType()));
  this->SetHwAddress(config_.GetHwAddress(this->_config.GetHwAddress()));
  this->SetMtu(config_.GetMtu(this->_config.GetMtu()));
  this->SetIpAddress(config_.GetIpAddress(this->_config.GetIpAddress()));
  this->SetNetmask(config_.GetNetmask(this->_config.GetNetmask()));
  this->SetPhyIndex(config_.GetPhyIndex(this->_config.GetPhyIndex()));
  this->SetPhyName(config_.GetPhyName(this->_config.GetPhyName()));
  this->SetHwMode(config_.GetHwMode(this->_config.GetHwMode()));
  this->SetHtMode(config_.GetHtMode(this->_config.GetHtMode()));
  this->SetOpMode(config_.GetOpMode(this->_config.GetOpMode()));
  this->SetAdminState(config_.GetAdminState(this->_config.GetAdminState())); // must be before setting channel
  this->SetChannel(config_.GetChannel(this->_config.GetChannel()));
  this->SetTxPower(config_.GetTxPower(this->_config.GetTxPower()));
  return (this->Commit());
}

int
Interface::GetPhyIndex() const
{
  int index = -1;
  if (this->lock.Lock())
  {
    if (this->_config.GetIfIndex())
    {
      GetInterfaceCommand cmd(this->_config.GetIfIndex());
      if (cmd.Exec())
      {
        index = cmd.PhyIndex();
      }
    }
    else
    {
      index = this->_config.GetPhyIndex();
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
    this->_config.SetPhyIndex(index_);
    this->lock.Unlock();
    status = true;
  }
  return (status);
}

std::string
Interface::GetPhyName() const
{
  std::string name;
  if (this->lock.Lock())
  {
    if ((this->_config.GetPhyIndex() >= 0))
    {
      GetPhyCommand cmd(this->_config.GetIfIndex()); // Interface index is not used; just the phy index
      cmd.PhyIndex(this->_config.GetPhyIndex());
      if (cmd.Exec())
      {
        name = cmd.PhyName();
      }
    }
    else
    {
      name = this->_config.GetPhyName();
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
    if (name_ != this->_config.GetPhyName())
    {
      if (this->_config.SetPhyName(name_) && this->_config.GetIfIndex() &&
          (this->_config.GetPhyIndex() >= 0))
      {
        SetPhyCommand* cmd = new SetPhyCommand(this->_config.GetIfIndex());
        cmd->PhyIndex(this->_config.GetPhyIndex());
        cmd->PhyName(name_);
        this->addCommand(cmd);
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

ConfigData::HWMODE
Interface::GetHwMode() const
{
  ConfigData::HWMODE mode = ConfigData::HWMODE_ERR;
  if (this->lock.Lock())
  {
    if (this->_config.GetIfIndex())
    {
      GetInterfaceCommand cmd(this->_config.GetIfIndex());
      if (cmd.Exec())
      {
        // TODO: Implement this
        mode = this->_config.GetHwMode();
      }
    }
    else
    {
      mode = this->_config.GetHwMode();
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
    if (mode_ != this->_config.GetHwMode())
    {
      if (this->_config.SetHwMode(mode_) && this->_config.GetIfIndex())
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

ConfigData::HTMODE
Interface::GetHtMode() const
{
  ConfigData::HTMODE mode = ConfigData::HTMODE_ERR;
  if (this->lock.Lock())
  {
    if (this->_config.GetIfIndex())
    {
      GetInterfaceCommand cmd(this->_config.GetIfIndex());
      if (cmd.Exec())
      {
        switch(cmd.ChannelType())
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
        switch(cmd.ChannelWidth())
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
    else
    {
      mode = this->_config.GetHtMode();
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
    if (mode_ != this->_config.GetHtMode())
    {
      if (this->_config.SetHtMode(mode_) && this->_config.GetIfIndex())
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

ConfigData::OPMODE
Interface::GetOpMode() const
{
  ConfigData::OPMODE mode = ConfigData::OPMODE_ERR;
  if (this->lock.Lock())
  {
    if (this->_config.GetIfIndex())
    {
      GetInterfaceCommand cmd(this->_config.GetIfIndex());
      if (cmd.Exec())
      {
        mode = _nl2opmode(cmd.IfType());
      }
    }
    else
    {
      mode = this->_config.GetOpMode();
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
    if (mode_ != this->_config.GetOpMode())
    {
      if (this->_config.SetOpMode(mode_) && this->_config.GetIfIndex())
      {
        SetInterfaceCommand* cmd = new SetInterfaceCommand(this->_config.GetIfIndex());
        cmd->IfType(_opmode2nl(mode_));
        this->addCommand(cmd);
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

unsigned int
Interface::GetChannel() const
{
  unsigned int channel = 0;
  if (this->lock.Lock())
  {
    if (this->_config.GetIfIndex())
    {
      GetInterfaceCommand cmd(this->_config.GetIfIndex());
      if (cmd.Exec())
      {
        channel = cmd.Frequency.GetChannel();
      }
    }
    else
    {
      channel = this->_config.GetChannel();
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
    if (channel_ != this->_config.GetChannel())
    {
      if (this->_config.GetAdminState() != ConfigData::STATE_UP)
      {
        ZLOG_WARN("Interface must be UP before setting channel");
      }
      if (this->_config.SetChannel(channel_) && this->_config.GetIfIndex() &&
          (this->_config.GetPhyIndex() >= 0))
      {
        SetPhyCommand* cmd = new SetPhyCommand(this->_config.GetIfIndex());
        cmd->PhyIndex(this->_config.GetPhyIndex());
        cmd->Frequency.SetChannel(channel_);
        this->addCommand(cmd);
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

unsigned int
Interface::GetTxPower() const
{
  unsigned int power = 0;
  if (this->lock.Lock())
  {
    if (this->_config.GetIfIndex())
    {
      GetInterfaceCommand cmd(this->_config.GetIfIndex());
      if (cmd.Exec())
      {
        power = cmd.TxPowerLevel();
      }
    }
    else
    {
      power = this->_config.GetTxPower();
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
    if (txpower_ != this->_config.GetTxPower())
    {
      if (this->_config.SetTxPower(txpower_) && this->_config.GetIfIndex() &&
          (this->_config.GetPhyIndex() >= 0))
      {
        SetPhyCommand* cmd = new SetPhyCommand(this->_config.GetIfIndex());
        cmd->PhyIndex(this->_config.GetPhyIndex());
        cmd->TxPowerMode.SetMode(nl80211::TxPowerModeAttribute::MODE_FIXED);
        cmd->TxPowerLevel(txpower_);
        this->addCommand(cmd);
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
  if (zInterface::Interface::Refresh())
  {
    this->_config.SetPhyIndex(this->GetPhyIndex());
    this->_config.SetPhyName(this->GetPhyName());
    this->_config.SetHwMode(this->GetHwMode());
    this->_config.SetHtMode(this->GetHtMode());
    this->_config.SetOpMode(this->GetOpMode());
    this->_config.SetChannel(this->GetChannel());
    this->_config.SetTxPower(this->GetTxPower());
    status = true;
  }
  return (status);
}

bool
Interface::Commit()
{
  return (zInterface::Interface::Commit());
}

bool
Interface::Create()
{

  bool status = false;

  if (this->lock.Lock())
  {
    NewInterfaceCommand *cmd =
        new NewInterfaceCommand(this->_config.GetIfName(), this->_config.GetPhyIndex());
    cmd->IfType(_opmode2nl(this->_config.GetOpMode()));
    this->addCommand(cmd);
    status = true;
    this->lock.Unlock();
  }

  if (status)
  {
    status = false;
    if (zInterface::Interface::Create() && this->GetIfIndex())
    {
      status = this->Refresh();
    }
  }

  return (status);

}

bool
Interface::Destroy()
{

  bool status = false;

  if (this->lock.Lock())
  {
    DelInterfaceCommand* cmd = new DelInterfaceCommand(this->_config.GetIfIndex());
    this->addCommand(cmd);
    status = true;
    this->lock.Unlock();
  }

  if (status)
  {
    status = zInterface::Interface::Destroy();
  }

  return (status);

}

void
Interface::Display(const std::string &prefix_)
{
  zInterface::Interface::Display(prefix_);
  std::cout << "--------- Wireless Interface -----------" << std::endl;
  std::cout << prefix_ << "PHY:    \t[" << this->GetPhyIndex() << "]: " << this->GetPhyName() << std::endl;
  std::cout << prefix_ << "HWMODE: \t" << _hwmode2str(this->GetHwMode()) << std::endl;
  std::cout << prefix_ << "HTMODE: \t" << _htmode2str(this->GetHtMode()) << std::endl;
  std::cout << prefix_ << "OPMODE: \t" << _opmode2str(this->GetOpMode()) << std::endl;
  std::cout << prefix_ << "Channel:\t" << this->GetChannel() << std::endl;
  std::cout << prefix_ << "Power:  \t" << this->GetTxPower() << std::endl;
}

}
}

