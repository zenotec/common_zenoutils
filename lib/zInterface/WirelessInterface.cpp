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

#include "GetLinkCommand.h"
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

WirelessInterface::WirelessInterface(const std::string& name_) :
    Interface(name_), WiConfig(this->Config), _getphycmd(NULL), _setphycmd(NULL),
    _getifacecmd(NULL), _setifacecmd(NULL), _newifacecmd(NULL), _delifacecmd(NULL)
{
  if (name_.empty())
  {
    ZLOG_WARN("WirelessInterface(name_): Name is empty!");
  }
  this->_init();
}

WirelessInterface::WirelessInterface(const zInterface::ConfigData& config_) :
    Interface(config_), WiConfig(this->Config), _getphycmd(NULL), _setphycmd(NULL),
    _getifacecmd(NULL), _setifacecmd(NULL), _newifacecmd(NULL), _delifacecmd(NULL)
{
  if (this->Config.GetIfName().empty())
  {
    ZLOG_WARN("WirelessInterface(config_): Name is empty!");
  }
  this->_init();
}

WirelessInterface::~WirelessInterface()
{
  if (this->_getphycmd)
  {
    delete (this->_getphycmd);
    this->_getphycmd = NULL;
  }
  if (this->_setphycmd)
  {
    delete (this->_setphycmd);
    this->_setphycmd = NULL;
  }
  if (this->_getifacecmd)
  {
    delete (this->_getifacecmd);
    this->_getifacecmd = NULL;
  }
  if (this->_setifacecmd)
  {
    delete (this->_setifacecmd);
    this->_setifacecmd = NULL;
  }
  if (this->_newifacecmd)
  {
    delete (this->_newifacecmd);
    this->_newifacecmd = NULL;
  }
  if (this->_delifacecmd)
  {
    delete (this->_delifacecmd);
    this->_delifacecmd = NULL;
  }
}

bool
WirelessInterface::SetIfName(const std::string& name_)
{
  bool status = Interface::SetIfName(name_);
  if (status && this->_lock.Lock())
  {
    this->_getifacecmd->IfName(name_);
    this->_setifacecmd->IfName(name_);
    this->_newifacecmd->IfName(name_);
    this->_delifacecmd->IfName(name_);
    this->_lock.Unlock();
  }
  return (status);
}

unsigned int
WirelessInterface::GetPhyIndex() const
{
  int index = 0;
  if (this->_lock.Lock())
  {
    if (this->_refreshed && this->_getphycmd)
    {
      index = this->_getphycmd->PhyIndex();
    }
    this->_lock.Unlock();
  }
  return (index);
}

bool
WirelessInterface::SetPhyIndex(const int index_)
{
  bool status = false;
  if (this->_lock.Lock())
  {
    this->_refreshed = false;
    this->_getphycmd->PhyIndex(index_);
    this->_setphycmd->PhyIndex(index_);
    this->_newifacecmd->PhyIndex(index_);
    this->_lock.Unlock();
  }
  return (status);
}

std::string
WirelessInterface::GetPhyName() const
{
  std::string name;
  if (this->_lock.Lock())
  {
    if (this->_refreshed && this->_getphycmd)
    {
      name = this->_getphycmd->PhyName();
    }
    this->_lock.Unlock();
  }
  return (name);
}

bool
WirelessInterface::SetPhyName(const std::string& name_)
{
  bool status = false;
  if (this->_lock.Lock())
  {
    this->_refreshed = false;
    this->_setphycmd->PhyName(name_);
    this->_lock.Unlock();
  }
  return (status);
}

WirelessInterfaceConfigData::HWMODE
WirelessInterface::GetHwMode() const
{
  WirelessInterfaceConfigData::HWMODE mode = WirelessInterfaceConfigData::HWMODE_ERR;
  if (this->_lock.Lock())
  {
    if (this->_refreshed && this->_getifacecmd)
    {
    }
    this->_lock.Unlock();
  }
  return (mode);
}

bool
WirelessInterface::SetHwMode(const WirelessInterfaceConfigData::HWMODE mode_)
{
  bool status = false;
  if (this->_lock.Lock())
  {
    if (this->_refreshed)
    {
      if (this->_refreshed && this->_setlinkcmd)
      {
      }
    }
    this->_lock.Unlock();
  }
  return (status);
}

WirelessInterfaceConfigData::HTMODE
WirelessInterface::GetHtMode() const
{
  WirelessInterfaceConfigData::HTMODE mode = WirelessInterfaceConfigData::HTMODE_ERR;
  if (this->_lock.Lock())
  {
    if (this->_refreshed && this->_getifacecmd)
    {
      switch(this->_getifacecmd->ChannelType())
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
      switch(this->_getifacecmd->ChannelWidth())
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
    this->_lock.Unlock();
  }
  return (mode);
}

bool
WirelessInterface::SetHtMode(const WirelessInterfaceConfigData::HTMODE mode_)
{
  bool status = false;
  if (this->_lock.Lock())
  {
    if (this->_refreshed)
    {
      if (this->_refreshed && this->_setifacecmd)
      {
      }
    }
    this->_lock.Unlock();
  }
  return (status);
}

WirelessInterfaceConfigData::OPMODE
WirelessInterface::GetOpMode() const
{
  WirelessInterfaceConfigData::OPMODE mode = WirelessInterfaceConfigData::OPMODE_ERR;
  if (this->_lock.Lock())
  {
    if (this->_refreshed && this->_getifacecmd)
    {
      switch (this->_getifacecmd->ChannelType())
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
    this->_lock.Unlock();
  }
  return (mode);
}

bool
WirelessInterface::SetOpMode(const WirelessInterfaceConfigData::OPMODE mode_)
{
  bool status = false;
  int iftype = -1;
  if (this->_lock.Lock())
  {
    if (this->_refreshed)
    {
      if (this->_refreshed && this->_setifacecmd)
      {
        switch(mode_)
        {
        case WirelessInterfaceConfigData::OPMODE_STA:
          iftype = NL80211_IFTYPE_STATION;
          break;
        case WirelessInterfaceConfigData::OPMODE_AP:
          iftype = NL80211_IFTYPE_AP;
          break;
        case WirelessInterfaceConfigData::OPMODE_MONITOR:
          iftype = NL80211_IFTYPE_MONITOR;
          break;
        case WirelessInterfaceConfigData::OPMODE_ADHOC:
          iftype = NL80211_IFTYPE_ADHOC;
          break;
        default:
          break;
        }
        if (iftype != -1)
        {
          this->_setifacecmd->IfType(iftype);
          status = this->_setifacecmd->Exec();
        }
      }
    }
    this->_lock.Unlock();
  }
  return (status);
}

std::string
WirelessInterface::GetSsid() const
{
}

bool
WirelessInterface::SetSsid(const std::string& ssid_)
{
}

std::string
WirelessInterface::GetBssid() const
{
}

bool
WirelessInterface::SetBssid(const std::string& bssid_)
{
}

unsigned int
WirelessInterface::GetChannel() const
{
}

bool
WirelessInterface::SetChannel(const unsigned int channel_)
{
}

unsigned int
WirelessInterface::GetTxPower() const
{
}

bool
WirelessInterface::SetTxPower(const unsigned int txpower_)
{
}

bool
WirelessInterface::Refresh()
{

  bool status = Interface::Refresh();

  if (status && this->_lock.Lock())
  {
    status = false;
    this->_refreshed = false;
    if (this->_getphycmd && this->_getifacecmd)
    {
      this->_getifacecmd->IfIndex(this->_getlinkcmd->Link.IfIndex());
      if (this->_getifacecmd->Exec())
      {
        this->_getphycmd->PhyIndex(this->_getifacecmd->PhyIndex());
        status = this->_refreshed = this->_getphycmd->Exec();
      }
    }
    this->_lock.Unlock();
  }
  else
  {
    ZLOG_ERR("Error refreshing interface: " + this->GetIfName());
  }

  return (status);

}

bool
WirelessInterface::Commit()
{
  return (Interface::Commit());
}

bool
WirelessInterface::Create()
{

  bool status = false;

  ZLOG_DEBUG("WirelessInterface::Create(): Enter");
  if (this->_lock.Lock())
  {
    if (this->_newifacecmd)
    {
      this->_newifacecmd->PhyIndex(this->_getphycmd->PhyIndex());
      if (this->WiConfig.GetOpMode() == WirelessInterfaceConfigData::OPMODE_STA)
      {
        this->_newifacecmd->IfType(NL80211_IFTYPE_STATION);
        this->_setifacecmd->IfType(NL80211_IFTYPE_STATION);
      }
      else if (this->WiConfig.GetOpMode() == WirelessInterfaceConfigData::OPMODE_AP)
      {
        this->_newifacecmd->IfType(NL80211_IFTYPE_AP);
        this->_setifacecmd->IfType(NL80211_IFTYPE_AP);
      }
      else if (this->WiConfig.GetOpMode() == WirelessInterfaceConfigData::OPMODE_ADHOC)
      {
        this->_newifacecmd->IfType(NL80211_IFTYPE_ADHOC);
        this->_setifacecmd->IfType(NL80211_IFTYPE_ADHOC);
      }
      else if (this->WiConfig.GetOpMode() == WirelessInterfaceConfigData::OPMODE_MONITOR)
      {
        this->_newifacecmd->IfType(NL80211_IFTYPE_MONITOR);
        this->_setifacecmd->IfType(NL80211_IFTYPE_MONITOR);
      }
      else
      {
        this->_newifacecmd->IfType(NL80211_IFTYPE_UNSPECIFIED);
        this->_setifacecmd->IfType(NL80211_IFTYPE_UNSPECIFIED);
      }
      status = this->_newifacecmd->Exec();
    }
    if (status && this->_setphycmd)
    {
      std::cout << "New interface created: [" << this->_newifacecmd->IfIndex() << "]: " << this->_newifacecmd->IfName() << std::endl;
      this->_setphycmd->PhyIndex(this->_getphycmd->PhyIndex());
      this->_getifacecmd->IfIndex(this->_newifacecmd->IfIndex());
      this->_setifacecmd->IfIndex(this->_newifacecmd->IfIndex());
      this->_delifacecmd->IfIndex(this->_newifacecmd->IfIndex());
    }
    this->_lock.Unlock();
  }

  ZLOG_DEBUG("WirelessInterface::Create(): Exit");

  return(status);

}

bool
WirelessInterface::Destroy()
{

  bool status = false;

  ZLOG_DEBUG("WirelessInterface::Destroy(): Enter");

  if (this->_lock.Lock())
  {
    if (this->_refreshed && this->_delifacecmd)
    {
      this->_delifacecmd->IfIndex(this->_getifacecmd->IfIndex());
      status = this->_delifacecmd->Exec();
    }
    this->_lock.Unlock();
  }

  ZLOG_DEBUG("WirelessInterface::Destroy(): Exit");

  return(status);

}

void
WirelessInterface::Display(const std::string &prefix_)
{
  Interface::Display(prefix_);
}

void
WirelessInterface::_init()
{
  this->Config.SetIfType(ConfigData::IFTYPE_IEEE80211);
  this->_getphycmd = new GetPhyCommand();
  this->_setphycmd = new SetPhyCommand();
  this->_getifacecmd = new GetInterfaceCommand(this->Config.GetIfName());
  this->_setifacecmd = new SetInterfaceCommand(this->Config.GetIfName());
  this->_newifacecmd = new NewInterfaceCommand(this->Config.GetIfName());
  this->_delifacecmd = new DelInterfaceCommand(this->Config.GetIfName());
}

}
}

