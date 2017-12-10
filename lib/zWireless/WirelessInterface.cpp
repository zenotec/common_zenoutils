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
#include <zutils/zWirelessInterface.h>

// local includes

#include "GetPhyCommand.h"
#include "SetPhyCommand.h"
#include "GetInterfaceCommand.h"
#include "SetInterfaceCommand.h"
#include "NewInterfaceCommand.h"
#include "DelInterfaceCommand.h"
using namespace nl80211;

namespace zUtils
{
namespace zInterface
{

// ****************************************************************************
// Class: WirelessInterface
// ****************************************************************************

WirelessInterface::WirelessInterface(const std::string& ifname_) :
    Interface(ifname_), wconfig(this->config), _modified(false)
{
  this->_init();
}

WirelessInterface::WirelessInterface(const zInterface::ConfigData& config_) :
    Interface(config_), wconfig(this->config), _modified(false)
{
  WirelessInterfaceConfigData wconfig_((zInterface::ConfigData&)config_);
  this->_init();
  this->SetPhyIndex(wconfig_.GetPhyIndex(this->wconfig.GetPhyIndex()));
  this->SetPhyName(wconfig_.GetPhyName(this->wconfig.GetPhyName()));
  this->SetHwMode(wconfig_.GetHwMode(this->wconfig.GetHwMode()));
  this->SetHtMode(wconfig_.GetHtMode(this->wconfig.GetHtMode()));
  this->SetOpMode(wconfig_.GetOpMode(this->wconfig.GetOpMode()));
  this->SetChannel(wconfig_.GetChannel(this->wconfig.GetChannel()));
  this->SetTxPower(wconfig_.GetTxPower(this->wconfig.GetTxPower()));
}

WirelessInterface::~WirelessInterface()
{
}

int
WirelessInterface::GetPhyIndex() const
{
  int index = -1;
  if (this->lock.Lock())
  {
    if (this->ifindex)
    {
      GetInterfaceCommand cmd(this->ifindex);
      if (cmd.Exec())
      {
        index = cmd.PhyIndex();
      }
    }
    else
    {
      index = this->wconfig.GetPhyIndex();
    }
    this->lock.Unlock();
  }
  return (index);
}

bool
WirelessInterface::SetPhyIndex(const int index_)
{
  bool status = false;
  if (this->lock.Lock())
  {
    if (index_ != this->wconfig.GetPhyIndex())
    {
      if (this->wconfig.SetPhyIndex(index_))
      {
        status = this->_modified = true;
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
WirelessInterface::GetPhyName() const
{
  std::string name;
  int index = this->GetPhyIndex();
  if (this->lock.Lock())
  {
    if (index >= 0)
    {
      GetPhyCommand cmd(index);
      if (cmd.Exec())
      {
        name = cmd.PhyName();
      }
    }
    else
    {
      name = this->wconfig.GetPhyName();
    }
    this->lock.Unlock();
  }
  return (name);
}

bool
WirelessInterface::SetPhyName(const std::string& name_)
{
  bool status = false;
  if (this->lock.Lock())
  {
    if (name_ != this->wconfig.GetPhyName())
    {
      if (this->wconfig.SetPhyName(name_))
      {
      status = this->_modified = true;
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

WirelessInterfaceConfigData::HWMODE
WirelessInterface::GetHwMode() const
{
  WirelessInterfaceConfigData::HWMODE mode = WirelessInterfaceConfigData::HWMODE_ERR;
  if (this->lock.Lock())
  {
    if (this->ifindex)
    {
      GetInterfaceCommand cmd(this->ifindex);
      if (cmd.Exec())
      {
      }
    }
    else
    {
      mode = this->wconfig.GetHwMode();
    }
    this->lock.Unlock();
  }
  return (mode);
}

bool
WirelessInterface::SetHwMode(const WirelessInterfaceConfigData::HWMODE mode_)
{
  bool status = false;
  if (this->lock.Lock())
  {
    if (mode_ != this->wconfig.GetHwMode())
    {
      if (this->wconfig.SetHwMode(mode_))
      {
        status = this->_modified = true;
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

WirelessInterfaceConfigData::HTMODE
WirelessInterface::GetHtMode() const
{
  WirelessInterfaceConfigData::HTMODE mode = WirelessInterfaceConfigData::HTMODE_ERR;
  if (this->lock.Lock())
  {
    if (this->ifindex)
    {
      GetInterfaceCommand cmd(this->ifindex);
      if (cmd.Exec())
      {
        switch(cmd.ChannelType())
        {
        case NL80211_CHAN_NO_HT:
          mode = WirelessInterfaceConfigData::HTMODE_NOHT;
          break;
        case NL80211_CHAN_HT20:
          mode = WirelessInterfaceConfigData::HTMODE_HT20;
          break;
        case NL80211_CHAN_HT40MINUS:
          mode = WirelessInterfaceConfigData::HTMODE_HT40MINUS;
          break;
        case NL80211_CHAN_HT40PLUS:
          mode = WirelessInterfaceConfigData::HTMODE_HT40PLUS;
          break;
        default:
          break;
        }
        switch(cmd.ChannelWidth())
        {
        case NL80211_CHAN_WIDTH_20_NOHT:
          mode = WirelessInterfaceConfigData::HTMODE_NOHT;
          break;
        case NL80211_CHAN_WIDTH_20:
          mode = WirelessInterfaceConfigData::HTMODE_VHT20;
          break;
        case NL80211_CHAN_WIDTH_40:
          mode = WirelessInterfaceConfigData::HTMODE_VHT40;
          break;
        case NL80211_CHAN_WIDTH_80:
          mode = WirelessInterfaceConfigData::HTMODE_VHT80;
          break;
        case NL80211_CHAN_WIDTH_80P80:
          mode = WirelessInterfaceConfigData::HTMODE_VHT80PLUS80;
          break;
        case NL80211_CHAN_WIDTH_160:
          mode = WirelessInterfaceConfigData::HTMODE_VHT160;
          break;
        default:
          break;
        }
      }
    }
    else
    {
      mode = this->wconfig.GetHtMode();
    }
    this->lock.Unlock();
  }
  return (mode);
}

bool
WirelessInterface::SetHtMode(const WirelessInterfaceConfigData::HTMODE mode_)
{
  bool status = false;
  if (this->lock.Lock())
  {
    if (mode_ != this->wconfig.GetHtMode())
    {
      if (this->wconfig.SetHtMode(mode_))
      {
        status = this->_modified = true;
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

WirelessInterfaceConfigData::OPMODE
WirelessInterface::GetOpMode() const
{
  WirelessInterfaceConfigData::OPMODE mode = WirelessInterfaceConfigData::OPMODE_ERR;
  if (this->lock.Lock())
  {
    if (this->ifindex)
    {
      GetInterfaceCommand cmd(this->ifindex);
      if (cmd.Exec())
      {
        switch (cmd.ChannelType())
        {
        case NL80211_IFTYPE_STATION:
          mode = WirelessInterfaceConfigData::OPMODE_STA;
          break;
        case NL80211_IFTYPE_AP:
          mode = WirelessInterfaceConfigData::OPMODE_AP;
          break;
        case NL80211_IFTYPE_MONITOR:
          mode = WirelessInterfaceConfigData::OPMODE_MONITOR;
          break;
        case NL80211_IFTYPE_ADHOC:
          mode = WirelessInterfaceConfigData::OPMODE_ADHOC;
          break;
        default:
          break;
        }
      }
    }
    else
    {
      mode = this->wconfig.GetOpMode();
    }
    this->lock.Unlock();
  }
  return (mode);
}

bool
WirelessInterface::SetOpMode(const WirelessInterfaceConfigData::OPMODE mode_)
{
  bool status = false;
  if (this->lock.Lock())
  {
    if (mode_ != this->wconfig.GetOpMode())
    {
      if (this->wconfig.SetOpMode(mode_))
      {
        status = this->_modified = true;
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
WirelessInterface::GetChannel() const
{
  unsigned int channel = 0;
  if (this->lock.Lock())
  {
    if (this->ifindex)
    {
      GetInterfaceCommand cmd(this->ifindex);
      if (cmd.Exec())
      {
        channel = cmd.Frequency.GetChannel();
      }
    }
    else
    {
      channel = this->wconfig.GetChannel();
    }
    this->lock.Unlock();
  }
  return (channel);
}

bool
WirelessInterface::SetChannel(const unsigned int channel_)
{
  bool status = false;
  if (this->lock.Lock())
  {
    if (channel_ != this->wconfig.GetChannel())
    {
      if (this->wconfig.SetChannel(channel_))
      {
        status = this->_modified = true;
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
WirelessInterface::GetTxPower() const
{
  unsigned int power = 0;
  if (this->lock.Lock())
  {
    if (this->ifindex)
    {
      GetInterfaceCommand cmd(this->ifindex);
      if (cmd.Exec())
      {
        power = 0;
      }
    }
    else
    {
      power = this->wconfig.GetTxPower();
    }
    this->lock.Unlock();
  }
  return (power);
}

bool
WirelessInterface::SetTxPower(const unsigned int txpower_)
{
  bool status = false;
  if (this->lock.Lock())
  {
    if (txpower_ != this->wconfig.GetTxPower())
    {
      if (this->wconfig.SetTxPower(txpower_))
      {
        status = this->_modified = true;
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
WirelessInterface::Refresh()
{
  bool status = false;
  if (Interface::Refresh())
  {
    this->SetPhyIndex(this->GetPhyIndex());
    this->SetPhyName(this->GetPhyName());
    this->SetHwMode(this->GetHwMode());
    this->SetHtMode(this->GetHtMode());
    this->SetOpMode(this->GetOpMode());
    this->SetChannel(this->GetChannel());
    this->SetTxPower(this->GetTxPower());
    status = true;
  }
  return (status);
}

bool
WirelessInterface::Commit()
{

  int iftype = 0;
  bool status = Interface::Commit();

  if (this->lock.Lock())
  {
    if (this->ifindex && this->_modified)
    {
      SetInterfaceCommand cmd(this->ifindex);
      cmd.IfName(this->config.GetIfName());
      // Translate hardware mode
      switch (this->wconfig.GetHwMode())
      {
      default:
        break;
      }
      // Translate high throughput mode
      switch (this->wconfig.GetHtMode())
      {
      default:
        break;
      }
      // Translate operational mode
      switch (this->wconfig.GetOpMode())
      {
      case WirelessInterfaceConfigData::OPMODE_STA:
        cmd.IfType(NL80211_IFTYPE_STATION);
        break;
      case WirelessInterfaceConfigData::OPMODE_AP:
        cmd.IfType(NL80211_IFTYPE_AP);
        break;
      case WirelessInterfaceConfigData::OPMODE_MONITOR:
        cmd.IfType(NL80211_IFTYPE_MONITOR);
        break;
      case WirelessInterfaceConfigData::OPMODE_ADHOC:
        cmd.IfType(NL80211_IFTYPE_ADHOC);
        break;
      default:
        break;
      }
      if (cmd.Exec())
      {
        this->clr_modified();
        status = true;
      }
    }
    this->lock.Unlock();
  }

  return (status);
}

bool
WirelessInterface::Create()
{

  bool status = false;

  ZLOG_DEBUG("WirelessInterface::Create(): Enter");
  if (this->lock.Lock())
  {
    NewInterfaceCommand cmd(this->config.GetIfName());
    // Translate operational mode
    switch (this->wconfig.GetOpMode())
    {
    case WirelessInterfaceConfigData::OPMODE_STA:
      cmd.IfType(NL80211_IFTYPE_STATION);
      break;
    case WirelessInterfaceConfigData::OPMODE_AP:
      cmd.IfType(NL80211_IFTYPE_AP);
      break;
    case WirelessInterfaceConfigData::OPMODE_MONITOR:
      cmd.IfType(NL80211_IFTYPE_MONITOR);
      break;
    case WirelessInterfaceConfigData::OPMODE_ADHOC:
      cmd.IfType(NL80211_IFTYPE_ADHOC);
      break;
    default:
      cmd.IfType(NL80211_IFTYPE_UNSPECIFIED);
      break;
    }
    if (cmd.Exec())
    {
      std::cout << "New interface created: [" << cmd.IfIndex() << "]: " << cmd.IfName() << std::endl;
      this->_modified = false;
      status = true;
      this->ifindex = cmd.IfIndex();
    }
    this->lock.Unlock();
  }

  ZLOG_DEBUG("WirelessInterface::Create(): Exit");

  return(status);

}

bool
WirelessInterface::Destroy()
{

  bool status = false;

  ZLOG_DEBUG("WirelessInterface::Destroy(): Enter");

  if (this->lock.Lock())
  {
    DelInterfaceCommand cmd(this->ifindex);
    status = cmd.Exec();
    this->lock.Unlock();
  }

  ZLOG_DEBUG("WirelessInterface::Destroy(): Exit");

  return(status);

}

void
WirelessInterface::Display(const std::string &prefix_)
{
  Interface::Display(prefix_);
}

bool
WirelessInterface::is_modified() const
{
  return (this->_modified);
}

void
WirelessInterface::set_modified()
{
  this->_modified = true;
}

void
WirelessInterface::clr_modified()
{
  this->_modified = false;
}

void
WirelessInterface::_init()
{
  this->config.SetIfType(ConfigData::IFTYPE_IEEE80211);
  this->SetPhyIndex(this->GetPhyIndex());
  this->SetPhyName(this->GetPhyName());
  this->SetHwMode(this->GetHwMode());
  this->SetHtMode(this->GetHtMode());
  this->SetOpMode(this->GetOpMode());
  this->SetChannel(this->GetChannel());
  this->SetTxPower(this->GetTxPower());
  this->clr_modified();
}

}
}

