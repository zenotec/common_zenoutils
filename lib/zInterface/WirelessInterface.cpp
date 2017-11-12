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
// Class: WirelessInterfaceConfigPath
// ****************************************************************************

const std::string WirelessInterfaceConfigPath::ConfigPhyIndexPath("PhyIndex");
const std::string WirelessInterfaceConfigPath::ConfigPhyNamePath("PhyName");
const std::string WirelessInterfaceConfigPath::ConfigHwModePath("HwMode");
const std::string WirelessInterfaceConfigPath::ConfigHtModePath("HtMode");
const std::string WirelessInterfaceConfigPath::ConfigOpModePath("OpMode");

WirelessInterfaceConfigPath::WirelessInterfaceConfigPath(const std::string& root_)
{
  if (!root_.empty())
  {
    this->Append(root_);
  }
}

WirelessInterfaceConfigPath::WirelessInterfaceConfigPath(const zData::DataPath& path_)
{
}

WirelessInterfaceConfigPath::WirelessInterfaceConfigPath(const WirelessInterfaceConfigPath& other_) :
    zInterface::ConfigPath(other_)
{
}

WirelessInterfaceConfigPath::~WirelessInterfaceConfigPath()
{
}

// ****************************************************************************
// Class: WirelessInterface
// ****************************************************************************

const unsigned int WirelessInterface::ConfigPhyIndexDefault(0);

const std::string WirelessInterface::ConfigPhyNameDefault("");

const std::string WirelessInterface::ConfigHwModeNone("");
const std::string WirelessInterface::ConfigHwModeA("A");
const std::string WirelessInterface::ConfigHwModeB("B");
const std::string WirelessInterface::ConfigHwModeG("G");
const std::string WirelessInterface::ConfigHwModeN("N");
const std::string WirelessInterface::ConfigHwModeAC("AC");
const std::string WirelessInterface::ConfigHwModeAD("AD");
const std::string WirelessInterface::ConfigHwModeAX("AX");
const std::string WirelessInterface::ConfigHwModeDefault(WirelessInterface::ConfigHwModeNone);

const std::string WirelessInterface::ConfigOpModeNone("");
const std::string WirelessInterface::ConfigOpModeSTA("STATION");
const std::string WirelessInterface::ConfigOpModeAP("AP");
const std::string WirelessInterface::ConfigOpModeAdhoc("ADHOC");
const std::string WirelessInterface::ConfigOpModeMonitor("MONITOR");
const std::string WirelessInterface::ConfigOpModeMesh("MESH");
const std::string WirelessInterface::ConfigOpModeDefault(WirelessInterface::ConfigOpModeNone);

WirelessInterface::WirelessInterface(const int index_) :
    Interface(index_), _getphycmd(NULL), _getifacecmd(NULL), _setifacecmd(NULL),
    _newifacecmd(NULL), _delifacecmd(NULL)
{
  this->Config.Type(zInterface::ConfigData::ConfigTypeWireless);
  this->_getphycmd = new GetPhyCommand;
  this->_getifacecmd = new GetInterfaceCommand(index_);
  this->_setifacecmd = new SetInterfaceCommand(index_);
  this->_delifacecmd = new DelInterfaceCommand(index_);
}

WirelessInterface::WirelessInterface(const std::string& name_) :
    Interface(name_), _getphycmd(NULL), _getifacecmd(NULL), _setifacecmd(NULL),
    _newifacecmd(NULL), _delifacecmd(NULL)
{
  this->Config.Type(zInterface::ConfigData::ConfigTypeWireless);
  this->_getphycmd = new GetPhyCommand;
  this->_getifacecmd = new GetInterfaceCommand(name_);
  this->_setifacecmd = new SetInterfaceCommand(name_);
  this->_newifacecmd = new NewInterfaceCommand(name_);
  this->_delifacecmd = new DelInterfaceCommand(name_);
}

WirelessInterface::WirelessInterface(const zInterface::ConfigData& config_) :
    Interface(config_), _getphycmd(NULL), _getifacecmd(NULL),
    _setifacecmd(NULL), _newifacecmd(NULL), _delifacecmd(NULL)
{
  this->_getphycmd = new GetPhyCommand(this->ConfigPhyIndex());
  this->_getifacecmd = new GetInterfaceCommand(this->Config.Name());
  this->_setifacecmd = new SetInterfaceCommand(this->Config.Name());
  this->_newifacecmd = new NewInterfaceCommand(this->Config.Name());
  this->_delifacecmd = new DelInterfaceCommand(this->Config.Name());
}

WirelessInterface::~WirelessInterface()
{
  if (this->_getphycmd)
  {
    delete (this->_getphycmd);
    this->_getphycmd = NULL;
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
unsigned int
WirelessInterface::ConfigPhyIndex() const
{
  unsigned int val = 0;
  WirelessInterfaceConfigPath path(WirelessInterfaceConfigPath::ConfigPhyIndexPath);
  if (!this->Config.GetValue<unsigned int>(path, val))
  {
    val = ConfigPhyIndexDefault;
  }
  return (val);
}

bool
WirelessInterface::ConfigPhyIndex(const unsigned int index_)
{
  WirelessInterfaceConfigPath path(WirelessInterfaceConfigPath::ConfigPhyIndexPath);
  return (this->Config.PutValue(path, index_));
}

std::string
WirelessInterface::ConfigPhyName() const
{
  std::string str;
  WirelessInterfaceConfigPath path(WirelessInterfaceConfigPath::ConfigPhyNamePath);
  if (!this->Config.GetValue(path, str))
  {
    str = ConfigPhyNameDefault;
  }
  return (str);
}

bool
WirelessInterface::ConfigPhyName(const std::string& name_)
{
  WirelessInterfaceConfigPath path(WirelessInterfaceConfigPath::ConfigPhyNamePath);
  return (this->Config.PutValue(path, name_));
}

std::string
WirelessInterface::ConfigHwMode() const
{
  std::string str;
  WirelessInterfaceConfigPath path(WirelessInterfaceConfigPath::ConfigHwModePath);
  if (!this->Config.GetValue(path, str))
  {
    str = ConfigHwModeDefault;
  }
  return (str);

}

bool
WirelessInterface::ConfigHwMode(const std::string& mode_)
{
  WirelessInterfaceConfigPath path(WirelessInterfaceConfigPath::ConfigHwModePath);
  return (this->Config.PutValue(path, mode_));
}

std::string
WirelessInterface::ConfigOpMode() const
{
  std::string str;
  WirelessInterfaceConfigPath path(WirelessInterfaceConfigPath::ConfigOpModePath);
  if (!this->Config.GetValue(path, str))
  {
    str = ConfigOpModeDefault;
  }
  return (str);

}

bool
WirelessInterface::ConfigOpMode(const std::string& mode_)
{
  WirelessInterfaceConfigPath path(WirelessInterfaceConfigPath::ConfigOpModePath);
  return (this->Config.PutValue(path, mode_));
}

unsigned int
WirelessInterface::GetPhyIndex() const
{
  int index = -1;
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

WirelessInterface::HWMODE
WirelessInterface::GetHwMode() const
{
  WirelessInterface::HWMODE mode = WirelessInterface::HWMODE_ERR;
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
WirelessInterface::SetHwMode(const WirelessInterface::HWMODE mode_)
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

WirelessInterface::HTMODE
WirelessInterface::GetHtMode() const
{
  WirelessInterface::HTMODE mode = WirelessInterface::HTMODE_ERR;
  if (this->_lock.Lock())
  {
    if (this->_refreshed && this->_getifacecmd)
    {
      switch(this->_getifacecmd->ChannelType())
      {
      case NL80211_CHAN_NO_HT:
        mode = WirelessInterface::HTMODE_NOHT;
        break;
      case NL80211_CHAN_HT20:
        mode = WirelessInterface::HTMODE_HT20;
        break;
      case NL80211_CHAN_HT40MINUS:
        mode = WirelessInterface::HTMODE_HT40MINUS;
        break;
      case NL80211_CHAN_HT40PLUS:
        mode = WirelessInterface::HTMODE_HT40PLUS;
        break;
      default:
        break;
      }
      switch(this->_getifacecmd->ChannelWidth())
      {
      case NL80211_CHAN_WIDTH_20_NOHT:
        mode = WirelessInterface::HTMODE_NOHT;
        break;
      case NL80211_CHAN_WIDTH_20:
        mode = WirelessInterface::HTMODE_VHT20;
        break;
      case NL80211_CHAN_WIDTH_40:
        mode = WirelessInterface::HTMODE_VHT40;
        break;
      case NL80211_CHAN_WIDTH_80:
        mode = WirelessInterface::HTMODE_VHT80;
        break;
      case NL80211_CHAN_WIDTH_80P80:
        mode = WirelessInterface::HTMODE_VHT80PLUS80;
        break;
      case NL80211_CHAN_WIDTH_160:
        mode = WirelessInterface::HTMODE_VHT160;
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
WirelessInterface::SetHtMode(const WirelessInterface::HTMODE mode_)
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

WirelessInterface::OPMODE
WirelessInterface::GetOpMode() const
{
  WirelessInterface::OPMODE mode = WirelessInterface::OPMODE_ERR;
  if (this->_lock.Lock())
  {
    if (this->_refreshed && this->_getifacecmd)
    {
      switch (this->_getifacecmd->ChannelType())
      {
      case NL80211_IFTYPE_STATION:
        mode = WirelessInterface::OPMODE_STA;
        break;
      case NL80211_IFTYPE_AP:
        mode = WirelessInterface::OPMODE_AP;
        break;
      case NL80211_IFTYPE_MONITOR:
        mode = WirelessInterface::OPMODE_MONITOR;
        break;
      case NL80211_IFTYPE_ADHOC:
        mode = WirelessInterface::OPMODE_ADHOC;
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
WirelessInterface::SetOpMode(const WirelessInterface::OPMODE mode_)
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
        case WirelessInterface::OPMODE_STA:
          iftype = NL80211_IFTYPE_STATION;
          break;
        case WirelessInterface::OPMODE_AP:
          iftype = NL80211_IFTYPE_AP;
          break;
        case WirelessInterface::OPMODE_MONITOR:
          iftype = NL80211_IFTYPE_MONITOR;
          break;
        case WirelessInterface::OPMODE_ADHOC:
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
    ZLOG_ERR("Error refreshing interface: " + this->Config.Name());
  }

  return (status);

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
      this->_newifacecmd->PhyIndex(this->ConfigPhyIndex());
      if (this->ConfigOpMode() == WirelessInterface::ConfigOpModeSTA)
      {
        this->_newifacecmd->IfType(NL80211_IFTYPE_STATION);
      }
      else if (this->ConfigOpMode() == WirelessInterface::ConfigOpModeAP)
      {
        this->_newifacecmd->IfType(NL80211_IFTYPE_AP);
      }
      else if (this->ConfigOpMode() == WirelessInterface::ConfigOpModeAdhoc)
      {
        this->_newifacecmd->IfType(NL80211_IFTYPE_ADHOC);
      }
      else if (this->ConfigOpMode() == WirelessInterface::ConfigOpModeMonitor)
      {
        this->_newifacecmd->IfType(NL80211_IFTYPE_MONITOR);
      }
      else
      {
        this->_newifacecmd->IfType(NL80211_IFTYPE_UNSPECIFIED);
      }
      status = this->_newifacecmd->Exec();
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

}
}
