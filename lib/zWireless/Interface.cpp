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
#include <zutils/zLog.h>
#include <zutils/zInterface.h>
#include <zutils/zWireless.h>
#include <zutils/nl80211/GetPhyCommand.h>
#include <zutils/nl80211/SetPhyCommand.h>
#include <zutils/nl80211/GetInterfaceCommand.h>
#include <zutils/nl80211/SetInterfaceCommand.h>
#include <zutils/nl80211/NewInterfaceCommand.h>
#include <zutils/nl80211/DelInterfaceCommand.h>

using namespace nl80211;

ZLOG_MODULE_INIT(zUtils::zLog::Log::MODULE_WIRELESS);

namespace zUtils
{
namespace zWireless
{

// ****************************************************************************
// Class: zWireless::Interface
// ****************************************************************************

Interface::Interface(const std::string& ifname_) :
    zInterface::Interface(ifname_),
    stagingConfig(zInterface::Interface::stagingConfig),
    workingConfig(zInterface::Interface::workingConfig)
{
  this->SetIfType(ConfigData::IFTYPE_IEEE80211);
}

Interface::~Interface()
{
}

ConfigData
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
  status &= this->SetOpMode(config_.GetOpMode(this->workingConfig.GetOpMode()));
  status &= this->SetHtMode(config_.GetHtMode(this->workingConfig.GetHtMode()));
  status &= this->SetFrequency(config_.GetFrequency(this->workingConfig.GetFrequency()));
  status &= this->SetCenterFrequency1(config_.GetCenterFrequency1(this->workingConfig.GetCenterFrequency1()));
  status &= this->SetCenterFrequency2(config_.GetCenterFrequency2(this->workingConfig.GetCenterFrequency2()));
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

int
Interface::GetPhyDev() const
{
  int index = -1;
  if (this->lock.Lock())
  {
    index = this->_getPhyDev();
    if (index == -1)
    {
      index = this->stagingConfig.GetPhyIndex();
    }
    this->lock.Unlock();
  }
  return (index);
}

bool
Interface::SetPhyDev(const int dev_)
{
  bool status = false;
  if (this->lock.Lock())
  {
    status = this->stagingConfig.SetPhyDev(dev_);
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

unsigned int
Interface::GetChannel() const
{
  return (ConfigData::freq2chan(this->GetFrequency()));
}

bool
Interface::SetChannel(const unsigned int channel_)
{
  return (this->SetFrequency(ConfigData::chan2freq(channel_)));
}

unsigned int
Interface::GetFrequency() const
{
  unsigned int freq = 0;
  if (this->lock.Lock())
  {
    freq = this->_getFrequency();
    if (freq == 0)
    {
      freq = this->stagingConfig.GetFrequency();
    }
    this->lock.Unlock();
  }
  return (freq);
}

bool
Interface::SetFrequency(const unsigned int freq_)
{
  bool status = false;
  if (this->lock.Lock())
  {
    status = this->stagingConfig.SetFrequency(freq_);
    this->lock.Unlock();
  }
  return (status);
}

unsigned int
Interface::GetCenterFrequency1() const
{
  unsigned int freq = 0;
  if (this->lock.Lock())
  {
    freq = this->_getCenterFrequency1();
    if (freq == 0)
    {
      freq = this->stagingConfig.GetCenterFrequency1();
    }
    this->lock.Unlock();
  }
  return (freq);
}

bool
Interface::SetCenterFrequency1(const unsigned int freq_)
{
  bool status = false;
  if (this->lock.Lock())
  {
    status = this->stagingConfig.SetCenterFrequency1(freq_);
    this->lock.Unlock();
  }
  return (status);
}

unsigned int
Interface::GetCenterFrequency2() const
{
  unsigned int freq = 0;
  if (this->lock.Lock())
  {
    freq = this->_getCenterFrequency2();
    if (freq == 0)
    {
      freq = this->stagingConfig.GetCenterFrequency2();
    }
    this->lock.Unlock();
  }
  return (freq);
}

bool
Interface::SetCenterFrequency2(const unsigned int freq_)
{
  bool status = false;
  if (this->lock.Lock())
  {
    status = this->stagingConfig.SetCenterFrequency2(freq_);
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

std::map<int, Capabilities>
Interface::GetCapabilities() const
{
  std::map<int, Capabilities> capa;
  if (this->lock.Lock())
  {
    capa = this->_getCapabilities();
    this->lock.Unlock();
  }
  return (capa);
}

bool
Interface::Refresh()
{
  bool status = zInterface::Interface::Refresh();
  status &= this->workingConfig.SetPhyIndex(this->GetPhyIndex());
  status &= this->workingConfig.SetPhyName(this->GetPhyName());
  status &= this->workingConfig.SetOpMode(this->GetOpMode());
  status &= this->workingConfig.SetHtMode(this->GetHtMode());
  status &= this->workingConfig.SetFrequency(this->GetFrequency());
  status &= this->workingConfig.SetCenterFrequency1(this->GetCenterFrequency1());
  status &= this->workingConfig.SetCenterFrequency2(this->GetCenterFrequency2());
  status &= this->workingConfig.SetTxPower(this->GetTxPower());
  return (status);
}

bool
Interface::Commit()
{
  bool status = false;
  if (this->lock.Lock())
  {
    status = true; // Innocent until proven guilty

    // These commands have to be executed while the interface is not up
    if (ConfigData::STATE_UP != this->workingConfig.GetAdminState())
    {
      if ((this->stagingConfig.GetIfName() != ConfigData::ConfigIfNameDefault) &&
          (this->stagingConfig.GetIfName() != this->workingConfig.GetIfName()))
      {
        status &= this->setIfName(this->stagingConfig.GetIfName());
      }

      if ((this->stagingConfig.GetIfType() != ConfigData::IFTYPE_DEF) &&
          (this->stagingConfig.GetIfType() != this->workingConfig.GetIfType()))
      {
        status &= this->setIfType(this->stagingConfig.GetIfType());
      }

      if ((this->stagingConfig.GetHwAddress() != ConfigData::ConfigHwAddressDefault) &&
          (this->stagingConfig.GetHwAddress() != this->workingConfig.GetHwAddress()))
      {
        status &= this->setHwAddress(this->stagingConfig.GetHwAddress());
      }

      if ((this->stagingConfig.GetOpMode() != ConfigData::OPMODE_DEF) &&
          (this->stagingConfig.GetOpMode() != this->workingConfig.GetOpMode()))
      {
        status &= this->_setOpMode();
      }

    }

    // The following commands can be executed regardless of the interfaces' administrative state
    if ((this->stagingConfig.GetMtu() != ConfigData::ConfigMtuDefault) &&
        (this->stagingConfig.GetMtu() != this->workingConfig.GetMtu()))
    {
      status &= this->setMtu(this->stagingConfig.GetMtu());
    }

    if ((this->stagingConfig.GetIpAddress() != ConfigData::ConfigIpAddressDefault) &&
        (this->stagingConfig.GetIpAddress() != this->workingConfig.GetIpAddress()))
    {
      status &= this->setIpAddress(this->stagingConfig.GetIpAddress());
    }

    if ((this->stagingConfig.GetNetmask() != ConfigData::ConfigNetmaskDefault) &&
        (this->stagingConfig.GetNetmask() != this->workingConfig.GetNetmask()))
    {
      status &= this->setNetmask(this->stagingConfig.GetNetmask());
    }

    if ((this->stagingConfig.GetPromiscuousMode() != ConfigData::PROMODE_DEF) &&
        (this->stagingConfig.GetPromiscuousMode() != this->workingConfig.GetPromiscuousMode()))
    {
      status &= this->setPromiscuousMode(this->stagingConfig.GetPromiscuousMode());
    }

    // Always make this command last to ensure all above commands are executed while the interface is down
    if ((this->stagingConfig.GetAdminState() != ConfigData::STATE_DEF) &&
        (this->stagingConfig.GetAdminState() != this->workingConfig.GetAdminState()))
    {
      status &= this->setAdminState(this->stagingConfig.GetAdminState());
    }

    // These commands have to be executed while the interface is up
    if (ConfigData::STATE_UP == this->stagingConfig.GetAdminState())
    {
      if (((this->stagingConfig.GetHtMode() != ConfigData::HTMODE_DEF) &&
             (this->stagingConfig.GetHtMode() != this->workingConfig.GetHtMode())) ||
          ((this->stagingConfig.GetFrequency() != ConfigData::ConfigFrequencyDefault) &&
             (this->stagingConfig.GetFrequency() != this->workingConfig.GetFrequency())) ||
          ((this->stagingConfig.GetCenterFrequency1() != ConfigData::ConfigCenterFrequency1Default) &&
             (this->stagingConfig.GetCenterFrequency1() != this->workingConfig.GetCenterFrequency1())) ||
          ((this->stagingConfig.GetCenterFrequency2() != ConfigData::ConfigCenterFrequency2Default) &&
             (this->stagingConfig.GetCenterFrequency2() != this->workingConfig.GetCenterFrequency2())))
      {
        status &= this->_setChannel();
      }

      if ((this->stagingConfig.GetTxPower() != ConfigData::ConfigTxPowerDefault) &&
          (this->stagingConfig.GetTxPower() != this->workingConfig.GetTxPower()))
      {
        status &= this->_setTxPower();
      }
    }

    status &= this->execCommands();

    this->lock.Unlock();
  }

  if (status && this->Refresh())
  {
    *this->stagingConfig.GetData() = *this->workingConfig.GetData();
  }

  return (status);
}

bool
Interface::Create()
{

  bool status = false;

  if (this->lock.Lock())
  {
    NewInterfaceCommand *cmd = new NewInterfaceCommand(this->stagingConfig.GetIfName(),
        this->stagingConfig.GetPhyIndex());
    cmd->IfType.Set(this->opmode2nl(this->stagingConfig.GetOpMode()));
    this->addCommand(cmd);
    status = this->execCommands();
    this->lock.Unlock();
  }

  if (status && this->Refresh())
  {
    status = this->Commit();
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
  std::cout << prefix_ << "OPMODE: \t" << this->opmode2str(this->GetOpMode()) << std::endl;
  std::cout << prefix_ << "HtMode: \t" << this->htmode2str(this->GetHtMode()) << std::endl;
  std::cout << prefix_ << "Freq:   \t" << this->GetFrequency() << std::endl;
  std::cout << prefix_ << "Center1:\t" << this->GetCenterFrequency1() << std::endl;
  std::cout << prefix_ << "Center2:\t" << this->GetCenterFrequency2() << std::endl;
  std::cout << prefix_ << "Power:  \t" << this->GetTxPower() << std::endl;
}

ConfigData::HTMODE
Interface::nl2htmode(const uint32_t nl_)
{
  ConfigData::HTMODE mode = ConfigData::HTMODE_ERR;
  switch (nl_)
  {
  case NL80211_CHAN_WIDTH_20_NOHT:
    mode = ConfigData::HTMODE_NOHT;
    break;
  case NL80211_CHAN_WIDTH_20:
    mode = ConfigData::HTMODE_HT20;
    break;
  case NL80211_CHAN_WIDTH_40:
    mode = ConfigData::HTMODE_HT40;
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
  return (mode);
}

uint32_t
Interface::htmode2nl(const ConfigData::HTMODE mode_)
{
  uint32_t val = 0;
  switch (mode_)
  {
  case ConfigData::HTMODE_NONE:
    // no break
  case ConfigData::HTMODE_NOHT:
    val = NL80211_CHAN_NO_HT;
    break;
  case ConfigData::HTMODE_HT20:
    val = NL80211_CHAN_WIDTH_20;
    break;
  case ConfigData::HTMODE_HT40:
    val = NL80211_CHAN_WIDTH_40;
    break;
  case ConfigData::HTMODE_VHT20:
    val = NL80211_CHAN_WIDTH_20;
    break;
  case ConfigData::HTMODE_VHT40:
    val = NL80211_CHAN_WIDTH_40;
    break;
  case ConfigData::HTMODE_VHT80:
    val = NL80211_CHAN_WIDTH_80;
    break;
  case ConfigData::HTMODE_VHT80PLUS80:
    val = NL80211_CHAN_WIDTH_80P80;
    break;
  case ConfigData::HTMODE_VHT160:
    val = NL80211_CHAN_WIDTH_160;
    break;
  case ConfigData::HTMODE_ERR:
    // no break
  default:
    break;
  }
  return (val);
}

std::string
Interface::htmode2str(ConfigData::HTMODE mode_)
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
  case ConfigData::HTMODE_HT40:
    str = "HT40";
    break;
  case ConfigData::HTMODE_VHT20:
    str = "VHT20";
    break;
  case ConfigData::HTMODE_VHT40:
    str = "VHT40";
    break;
  case ConfigData::HTMODE_VHT80:
    str = "VHT80";
    break;
  case ConfigData::HTMODE_VHT80PLUS80:
    str = "VHT80+80";
    break;
  case ConfigData::HTMODE_VHT160:
    str = "VHT160";
    break;
  default:
    str = "Unknown";
    break;
  }
  return (str);
}

ConfigData::OPMODE
Interface::nl2opmode(const uint32_t nl_)
{
  ConfigData::OPMODE mode = ConfigData::OPMODE_ERR;
  switch (nl_)
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

uint32_t
Interface::opmode2nl(const ConfigData::OPMODE mode_)
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

std::string
Interface::opmode2str(const ConfigData::OPMODE mode_)
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

int
Interface::_getPhyIndex() const
{
  int index = -1;
  if (this->workingConfig.GetIfIndex())
  {
    GetInterfaceCommand cmd(this->workingConfig.GetIfIndex());
    if (cmd.Exec() && cmd.PhyIndex.IsValid())
    {
      index = cmd.PhyIndex();
    }
  }
  return (index);
}

bool
Interface::_setPhyIndex()
{
  return (true);
}

int
Interface::_getPhyDev() const
{
  int index = -1;
  if (this->workingConfig.GetIfIndex())
  {
    GetInterfaceCommand cmd(this->workingConfig.GetIfIndex());
    if (cmd.Exec() && cmd.PhyDev.IsValid())
    {
      index = cmd.PhyDev();
    }
  }
  return (index);
}

bool
Interface::_setPhyDev()
{
  return (true);
}

std::string
Interface::_getPhyName() const
{
  std::string name;
  if (this->workingConfig.GetPhyIndex() >= 0)
  {
    GetPhyCommand cmd(this->workingConfig.GetPhyIndex());
    if (cmd.Exec() && cmd.PhyName.IsValid())
    {
      name = cmd.PhyName();
    }
  }
  return (name);
}

bool
Interface::_setPhyName()
{
  bool status = false;
  if (this->workingConfig.GetIfIndex() &&
      (this->workingConfig.GetPhyIndex() >= 0) &&
      !this->GetPhyName().empty())
  {
    SetPhyCommand* cmd = new SetPhyCommand(this->workingConfig.GetIfIndex());
    cmd->PhyIndex(this->workingConfig.GetPhyIndex());
    cmd->PhyName(this->stagingConfig.GetPhyName());
    this->addCommand(cmd);
    status = true;
  }
  return (status);
}

ConfigData::OPMODE
Interface::_getOpMode() const
{
  ConfigData::OPMODE mode = ConfigData::OPMODE_ERR;
  if (this->workingConfig.GetIfIndex())
  {
    GetInterfaceCommand cmd(this->workingConfig.GetIfIndex());
    if (cmd.Exec() && cmd.IfType.IsValid())
    {
      mode = this->nl2opmode(cmd.IfType());
    }
  }
  return (mode);
}

bool
Interface::_setOpMode()
{
  bool status = false;
  if (this->workingConfig.GetIfIndex())
  {
    SetInterfaceCommand* cmd = new SetInterfaceCommand(this->workingConfig.GetIfIndex());
    cmd->IfType(this->opmode2nl(this->stagingConfig.GetOpMode()));
    this->addCommand(cmd);
    status = true;
  }
  return (status);
}

ConfigData::HTMODE
Interface::_getHtMode() const
{
  ConfigData::HTMODE mode = ConfigData::HTMODE_ERR;
  if (this->workingConfig.GetIfIndex())
  {
    GetInterfaceCommand cmd(this->workingConfig.GetIfIndex());
    if (cmd.Exec() && cmd.ChannelWidth.IsValid())
    {
      mode = this->nl2htmode(cmd.ChannelWidth());
    }
  }
  return (mode);
}

unsigned int
Interface::_getFrequency() const
{
  unsigned int freq = 0;
  if (this->workingConfig.GetIfIndex())
  {
    GetInterfaceCommand cmd(this->workingConfig.GetIfIndex());
    if (cmd.Exec() && cmd.Frequency.IsValid())
    {
      freq = cmd.Frequency();
    }
  }
  return (freq);
}

unsigned int
Interface::_getCenterFrequency1() const
{
  unsigned int freq = 0;
  if (this->workingConfig.GetIfIndex())
  {
    GetInterfaceCommand cmd(this->workingConfig.GetIfIndex());
    if (cmd.Exec() && cmd.CenterFrequency1.IsValid())
    {
      freq = cmd.CenterFrequency1();
    }
  }
  return (freq);
}


unsigned int
Interface::_getCenterFrequency2() const
{
  unsigned int freq = 0;
  if (this->workingConfig.GetIfIndex())
  {
    GetInterfaceCommand cmd(this->workingConfig.GetIfIndex());
    if (cmd.Exec() && cmd.CenterFrequency2.IsValid())
    {
      freq = cmd.CenterFrequency2();
    }
  }
  return (freq);
}

bool
Interface::_setChannel()
{
  bool status = false;
  if (this->workingConfig.GetIfIndex() && (this->workingConfig.GetPhyIndex() >= 0))
  {
    SetPhyCommand* cmd = new SetPhyCommand(this->workingConfig.GetIfIndex());
    cmd->PhyIndex(this->workingConfig.GetPhyIndex());
    cmd->Frequency(this->stagingConfig.GetFrequency());
    cmd->CenterFrequency1(this->stagingConfig.GetFrequency());

    //  Set either ChannelType OR ChannelWidth based on HT mode
    switch (this->stagingConfig.GetHtMode())
    {
    case ConfigData::HTMODE_VHT80PLUS80:
      cmd->CenterFrequency2(this->stagingConfig.GetCenterFrequency2());
      // no break
    case ConfigData::HTMODE_HT40:
      // no break
    case ConfigData::HTMODE_VHT40:
      // no break
    case ConfigData::HTMODE_VHT80:
      // no break
    case ConfigData::HTMODE_VHT160:
      cmd->CenterFrequency1(this->stagingConfig.GetCenterFrequency1());
      // no break
    case ConfigData::HTMODE_HT20:
      // no break
    case ConfigData::HTMODE_VHT20:
      cmd->ChannelWidth(this->htmode2nl(this->stagingConfig.GetHtMode()));
      // no break
    case ConfigData::HTMODE_NOHT:
      this->addCommand(cmd);
      status = true;
      break;
    default:
      break;
    }
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
    if (cmd.Exec() && cmd.TxPowerLevel.IsValid())
    {
      power = cmd.TxPowerLevel();
    }
  }
  return (power);
}

bool
Interface::_setTxPower()
{
  bool status = false;
  if (this->workingConfig.GetIfIndex() && (this->workingConfig.GetPhyIndex() >= 0))
  {
    SetPhyCommand* cmd = new SetPhyCommand(this->workingConfig.GetIfIndex());
    cmd->PhyIndex(this->workingConfig.GetPhyIndex());
    cmd->TxPowerMode(nl80211::TxPowerModeAttribute::MODE_FIXED);
    cmd->TxPowerLevel(this->stagingConfig.GetTxPower());
    this->addCommand(cmd);
    status = true;
  }
  return (status);
}

std::map<int, Capabilities>
Interface::_getCapabilities() const
{
  std::map<int, Capabilities> caps;
  if (this->workingConfig.GetPhyIndex() >= 0)
  {
    GetPhyCommand cmd(this->workingConfig.GetPhyIndex());
    if (cmd.Exec() && cmd.PhyBands.IsValid())
    {
      std::vector<uint8_t> bands = cmd.PhyBands.GetBands();
      FOREACH(auto& band, bands)
      {

        // Split bit rates into bit rates (no more than 8 rates) and extended rates
        std::vector<uint8_t> rates = cmd.PhyBands.GetPhyBand(band).GetRates()();
        size_t rate_len = std::min(size_t(8), rates.size());
        std::vector<uint8_t> bitrates(rates.begin(), (rates.begin() + rate_len));
        std::vector<uint8_t> extrates((rates.begin() + rate_len), rates.end());
        caps[band].SetBitRates(bitrates);
        caps[band].SetExtBitRates(extrates);

        // Get HT capabilities
        ieee80211::HtCapsTag::ht_caps htcaps = { 0 };
        htcaps.ht_cap_info = cmd.PhyBands.GetPhyBand(band).GetHtCaps()();
        htcaps.ampdu_parms = cmd.PhyBands.GetPhyBand(band).GetAmpduFactor()();
        htcaps.ampdu_parms |= (cmd.PhyBands.GetPhyBand(band).GetAmpduDensity()() << 2);
        PhyBandsHtMcsAttribute::mcs_set mcs = cmd.PhyBands.GetPhyBand(band).GetMcsSet()();
        htcaps.supported_mcs_set.rx_mcs_bitmask = mcs.rx_mcs_bitmask;
        //// TODO: Temp debug
        htcaps.supported_mcs_set.rx_mcs_bitmask[1] = 0;
        htcaps.supported_mcs_set.rx_mcs_bitmask[2] = 0;
        htcaps.supported_mcs_set.rx_highest_rate = mcs.rx_highest_rate;
        htcaps.supported_mcs_set.tx_mcs_fields.tx_bits = mcs.tx_mcs_fields.tx_bits;
        htcaps.ht_ext_cap = 0x0000;
        htcaps.trans_beam_cap = 0x00000000;
        htcaps.asel_cap = 0x00;
        caps[band].SetHtCaps(htcaps);

      }
    }
  }
  return (caps);
}

}
}

