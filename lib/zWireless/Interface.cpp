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
#include <zutils/nl80211/SetChannelCommand.h>

using namespace nl80211;

ZLOG_MODULE_INIT(zUtils::zLog::Log::MODULE_WIRELESS);

namespace zUtils
{
namespace zWireless
{

static uint16_t
_freq2chan(uint16_t freq_)
{

  uint16_t channel = 0;
  // Channels 1 - 13
  if ((freq_ >= 2412) && (freq_ <= 2472))
  {
    channel = (1 + ((freq_ - 2412) / 5));
  }
  // Channels 36 - 64
  else if ((freq_ >= 5170) && (freq_ <= 5320))
  {
    channel = (34 + ((freq_ - 5170) / 5));
  }
  // Channels 100 - 144
  else if ((freq_ >= 5500) && (freq_ <= 5720))
  {
    channel = (100 + ((freq_ - 5500) / 5));
  }
  // Channels 149 - 161
  else if ((freq_ >= 5745) && (freq_ <= 5805))
  {
    channel = (149 + ((freq_ - 5745) / 5));
  }
  // Channel 165
  else if (freq_ == 5825)
  {
    channel = 165;
  }

  return (channel);

}

static uint16_t
_chan2freq(uint16_t chan_)
{
  uint16_t freq = 0;
  if ((chan_ >= 1) && (chan_ <=13))
  {
    freq = (((chan_ - 1) * 5) + 2412);
  }
  else if ((chan_ >= 36) && (chan_ <= 64))
  {
    freq = (((chan_ - 34) * 5) + 5170);
  }
  else if ((chan_ >= 100) && (chan_ <= 144))
  {
    freq = (((chan_ - 100) * 5) + 5500);
  }
  else if ((chan_ >= 149) && (chan_ <= 161))
  {
    freq = (((chan_ - 149) * 5) + 5745);
  }
  else if (chan_ == 165)
  {
    freq = 5825;
  }
  return (freq);
}

static uint32_t
_htmode2nl(ConfigData::HTMODE mode_)
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
    val = NL80211_CHAN_HT20;
    break;
  case ConfigData::HTMODE_HT40MINUS:
    val = NL80211_CHAN_HT40MINUS;
    break;
  case ConfigData::HTMODE_HT40PLUS:
    val = NL80211_CHAN_HT40PLUS;
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
  return (_freq2chan(freq));
}

bool
Interface::SetChannel(const unsigned int channel_)
{
  bool status = false;
  if (this->lock.Lock())
  {
    status = this->stagingConfig.SetFrequency(_chan2freq(channel_));
    this->lock.Unlock();
  }
  return (status);
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
    cmd->IfType.Set(_opmode2nl(this->stagingConfig.GetOpMode()));
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
  std::cout << prefix_ << "OPMODE: \t" << _opmode2str(this->GetOpMode()) << std::endl;
  std::cout << prefix_ << "HtMode: \t" << _htmode2str(this->GetHtMode()) << std::endl;
  std::cout << prefix_ << "Freq:   \t" << this->GetFrequency() << std::endl;
  std::cout << prefix_ << "Center1:\t" << this->GetCenterFrequency1() << std::endl;
  std::cout << prefix_ << "Center2:\t" << this->GetCenterFrequency2() << std::endl;
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
    if (cmd.Exec())
    {
      index = cmd.PhyIndex();
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
    if (cmd.Exec())
    {
      mode = _nl2opmode(cmd.IfType());
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
    cmd->IfType(_opmode2nl(this->stagingConfig.GetOpMode()));
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
    if (cmd.Exec())
    {
      if (cmd.ChannelType.IsValid())
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
      }
      else if (cmd.ChannelWidth.IsValid())
      {
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
      else
      {
        mode = ConfigData::HTMODE_ERR;
      }
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
    if (cmd.Exec())
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
    if (cmd.Exec())
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
    if (cmd.Exec())
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

    //  Set either ChannelType OR ChannelWidth based on HT mode
    switch (this->stagingConfig.GetHtMode())
    {
    case ConfigData::HTMODE_HT40MINUS:
      // no break
    case ConfigData::HTMODE_HT40PLUS:
      // no break
      cmd->CenterFrequency1(this->GetCenterFrequency1());
    case ConfigData::HTMODE_NOHT:
      // no break
    case ConfigData::HTMODE_HT20:
      cmd->ChannelType(_htmode2nl(this->stagingConfig.GetHtMode()));
      break;
    case ConfigData::HTMODE_VHT80PLUS80:
      cmd->CenterFrequency2(this->GetCenterFrequency2());
      // no break
    case ConfigData::HTMODE_VHT20:
      // no break
    case ConfigData::HTMODE_VHT40:
      // no break
    case ConfigData::HTMODE_VHT80:
      // no break
    case ConfigData::HTMODE_VHT160:
      cmd->ChannelWidth(_htmode2nl(this->stagingConfig.GetHtMode()));
      cmd->CenterFrequency1(this->GetCenterFrequency1());
      break;
    default:
      break;
    }
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
  std::map<int, Capabilities> capa;
  if (this->workingConfig.GetPhyIndex() >= 0)
  {
    GetPhyCommand cmd(0); // Interface index is ignored; only PHY index is used
    cmd.PhyIndex(this->workingConfig.GetPhyIndex());
    if (cmd.Exec())
    {
      std::vector<uint8_t> bands = cmd.PhyBands.GetBands();
      FOREACH(auto& band, bands)
      {
        capa[band].SetBitRates(cmd.PhyBands.GetPhyBand(band).GetRates());
      }
    }
  }
  return (capa);
}

}
}

