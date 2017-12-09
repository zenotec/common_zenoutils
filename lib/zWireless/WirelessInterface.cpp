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
  this->config.SetIfType(ConfigData::IFTYPE_IEEE80211);
}

WirelessInterface::WirelessInterface(const zInterface::ConfigData& config_) :
    Interface(config_), wconfig(this->config), _modified(false)
{
  this->Refresh();
  WirelessInterfaceConfigData wconfig_((zInterface::ConfigData&)config_);
  this->config.SetIfType(ConfigData::IFTYPE_IEEE80211);
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

unsigned int
WirelessInterface::GetPhyIndex() const
{
  unsigned int index = 0;
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
WirelessInterface::SetPhyIndex(const unsigned int index_)
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
    if (this->ifindex)
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
    if ((name_ != this->wconfig.GetPhyName()) && this->wconfig.SetPhyName(name_))
    {
      status = this->_modified = true;
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
    status = this->wconfig.SetHwMode(mode_);
    this->_modified = true;
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
    status = this->wconfig.SetHtMode(mode_);
    this->_modified = true;
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
    status = this->wconfig.SetOpMode(mode_);
    this->_modified = true;
    this->lock.Unlock();
  }
  return (status);
}

unsigned int
WirelessInterface::GetChannel() const
{
}

bool
WirelessInterface::SetChannel(const unsigned int channel_)
{
  bool status = false;
  if (this->lock.Lock())
  {
    status = this->wconfig.SetChannel(channel_);
    this->_modified = true;
    this->lock.Unlock();
  }
  return (status);
}

unsigned int
WirelessInterface::GetTxPower() const
{
}

bool
WirelessInterface::SetTxPower(const unsigned int txpower_)
{
  bool status = false;
  if (this->lock.Lock())
  {
    status = this->wconfig.SetTxPower(txpower_);
    this->_modified = true;
    this->lock.Unlock();
  }
  return (status);
}

bool
WirelessInterface::Refresh()
{
  bool status = Interface::Refresh();
  if (status)
  {

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
      cmd.IfName(this->GetIfName());
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
      status = cmd.Exec();
      this->_modified = !status;
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

}
}

