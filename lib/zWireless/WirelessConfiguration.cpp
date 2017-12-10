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

// libc++ includes

#include <string>

// libzutils includes

#include <zutils/zCompatibility.h>
#include <zutils/zInterface.h>
#include <zutils/zWirelessInterface.h>

// local includes

namespace zUtils
{
namespace zInterface
{

static std::string
_hwmode2str(const WirelessInterfaceConfigData::HWMODE mode_)
{
  std::string str;
  switch (mode_)
  {
  case WirelessInterfaceConfigData::HWMODE_NONE:
    str = WirelessInterfaceConfigData::ConfigHwModeNone;
    break;
  case WirelessInterfaceConfigData::HWMODE_A:
    str = WirelessInterfaceConfigData::ConfigHwModeA;
    break;
  case WirelessInterfaceConfigData::HWMODE_B:
    str = WirelessInterfaceConfigData::ConfigHwModeB;
    break;
  case WirelessInterfaceConfigData::HWMODE_G:
    str = WirelessInterfaceConfigData::ConfigHwModeG;
    break;
  case WirelessInterfaceConfigData::HWMODE_N:
    str = WirelessInterfaceConfigData::ConfigHwModeN;
    break;
  case WirelessInterfaceConfigData::HWMODE_AC:
    str = WirelessInterfaceConfigData::ConfigHwModeAC;
    break;
  case WirelessInterfaceConfigData::HWMODE_AD:
    str = WirelessInterfaceConfigData::ConfigHwModeAD;
    break;
  case WirelessInterfaceConfigData::HWMODE_AX:
    str = WirelessInterfaceConfigData::ConfigHwModeAX;
    break;
  default:
    str = WirelessInterfaceConfigData::ConfigHwModeDefault;
    break;
  }
  return (str);
}

static WirelessInterfaceConfigData::HWMODE
_str2hwmode(const std::string& str)
{
  WirelessInterfaceConfigData::HWMODE mode = WirelessInterfaceConfigData::HWMODE_ERR;

  if (str == WirelessInterfaceConfigData::ConfigHwModeNone)
  {
    mode = WirelessInterfaceConfigData::HWMODE_NONE;
  }
  else if (str == WirelessInterfaceConfigData::ConfigHwModeA)
  {
    mode = WirelessInterfaceConfigData::HWMODE_A;
  }
  else if (str == WirelessInterfaceConfigData::ConfigHwModeB)
  {
    mode = WirelessInterfaceConfigData::HWMODE_B;
  }
  else if (str == WirelessInterfaceConfigData::ConfigHwModeG)
  {
    mode = WirelessInterfaceConfigData::HWMODE_G;
  }
  else if (str == WirelessInterfaceConfigData::ConfigHwModeN)
  {
    mode = WirelessInterfaceConfigData::HWMODE_N;
  }
  else if (str == WirelessInterfaceConfigData::ConfigHwModeAC)
  {
    mode = WirelessInterfaceConfigData::HWMODE_AC;
  }
  else if (str == WirelessInterfaceConfigData::ConfigHwModeAD)
  {
    mode = WirelessInterfaceConfigData::HWMODE_AD;
  }
  else if (str == WirelessInterfaceConfigData::ConfigHwModeAX)
  {
    mode = WirelessInterfaceConfigData::HWMODE_AX;
  }
  else
  {
    mode = WirelessInterfaceConfigData::HWMODE_DEF;
  }
  return (mode);
}

static std::string
_htmode2str(const WirelessInterfaceConfigData::HTMODE mode_)
{
  std::string str;
  return (str);
}

static WirelessInterfaceConfigData::HTMODE
_str2htmode(const std::string& str)
{
  WirelessInterfaceConfigData::HTMODE mode = WirelessInterfaceConfigData::HTMODE_ERR;
  return (mode);
}

static std::string
_opmode2str(const WirelessInterfaceConfigData::OPMODE mode_)
{
  std::string str;
  switch (mode_)
  {
  case WirelessInterfaceConfigData::OPMODE_NONE:
    str = WirelessInterfaceConfigData::ConfigOpModeNone;
    break;
  case WirelessInterfaceConfigData::OPMODE_STA:
    str = WirelessInterfaceConfigData::ConfigOpModeSTA;
    break;
  case WirelessInterfaceConfigData::OPMODE_AP:
    str = WirelessInterfaceConfigData::ConfigOpModeAP;
    break;
  case WirelessInterfaceConfigData::OPMODE_ADHOC:
    str = WirelessInterfaceConfigData::ConfigOpModeAdhoc;
    break;
  case WirelessInterfaceConfigData::OPMODE_MONITOR:
    str = WirelessInterfaceConfigData::ConfigOpModeMonitor;
    break;
  case WirelessInterfaceConfigData::OPMODE_MESH:
    str = WirelessInterfaceConfigData::ConfigOpModeMesh;
    break;
  default:
    str = WirelessInterfaceConfigData::ConfigOpModeDefault;
    break;
  }
  return (str);
}

static WirelessInterfaceConfigData::OPMODE
_str2opmode(const std::string& str)
{
  WirelessInterfaceConfigData::OPMODE mode = WirelessInterfaceConfigData::OPMODE_ERR;
  if (str == WirelessInterfaceConfigData::ConfigOpModeSTA)
  {
    mode = WirelessInterfaceConfigData::OPMODE_STA;
  }
  else if (str == WirelessInterfaceConfigData::ConfigOpModeAP)
  {
    mode = WirelessInterfaceConfigData::OPMODE_AP;
  }
  else if (str == WirelessInterfaceConfigData::ConfigOpModeAdhoc)
  {
    mode = WirelessInterfaceConfigData::OPMODE_ADHOC;
  }
  else if (str == WirelessInterfaceConfigData::ConfigOpModeMonitor)
  {
    mode = WirelessInterfaceConfigData::OPMODE_MONITOR;
  }
  else if (str == WirelessInterfaceConfigData::ConfigOpModeMesh)
  {
    mode = WirelessInterfaceConfigData::OPMODE_MESH;
  }
  else
  {
    mode = WirelessInterfaceConfigData::OPMODE_DEF;
  }
  return (mode);
}

// ****************************************************************************
// Class: WirelessInterfaceConfigPath
// ****************************************************************************

const std::string WirelessInterfaceConfigPath::ConfigPhyIndexPath("PhyIndex");
const std::string WirelessInterfaceConfigPath::ConfigPhyNamePath("PhyName");
const std::string WirelessInterfaceConfigPath::ConfigHwModePath("HwMode");
const std::string WirelessInterfaceConfigPath::ConfigHtModePath("HtMode");
const std::string WirelessInterfaceConfigPath::ConfigOpModePath("OpMode");
const std::string WirelessInterfaceConfigPath::ConfigSsidPath("Ssid");
const std::string WirelessInterfaceConfigPath::ConfigBssidPath("Bssid");
const std::string WirelessInterfaceConfigPath::ConfigChannelPath("Channel");
const std::string WirelessInterfaceConfigPath::ConfigTxPowerPath("TxPower");

WirelessInterfaceConfigPath::WirelessInterfaceConfigPath(const std::string& root_)
{
  this->Append(root_);
}

WirelessInterfaceConfigPath::~WirelessInterfaceConfigPath()
{
}

// ****************************************************************************
// Class: WirelessInterfaceConfigData
// ****************************************************************************

const int WirelessInterfaceConfigData::ConfigPhyIndexDefault(-1);

const std::string WirelessInterfaceConfigData::ConfigPhyNameDefault("");

const std::string WirelessInterfaceConfigData::ConfigHwModeNone("");
const std::string WirelessInterfaceConfigData::ConfigHwModeA("A");
const std::string WirelessInterfaceConfigData::ConfigHwModeB("B");
const std::string WirelessInterfaceConfigData::ConfigHwModeG("G");
const std::string WirelessInterfaceConfigData::ConfigHwModeN("N");
const std::string WirelessInterfaceConfigData::ConfigHwModeAC("AC");
const std::string WirelessInterfaceConfigData::ConfigHwModeAD("AD");
const std::string WirelessInterfaceConfigData::ConfigHwModeAX("AX");
const std::string WirelessInterfaceConfigData::ConfigHwModeDefault(ConfigHwModeNone);

const std::string WirelessInterfaceConfigData::ConfigHtModeNone("");
const std::string WirelessInterfaceConfigData::ConfigHtModeDefault(ConfigHtModeNone);

const std::string WirelessInterfaceConfigData::ConfigOpModeNone("");
const std::string WirelessInterfaceConfigData::ConfigOpModeSTA("STATION");
const std::string WirelessInterfaceConfigData::ConfigOpModeAP("AP");
const std::string WirelessInterfaceConfigData::ConfigOpModeAdhoc("ADHOC");
const std::string WirelessInterfaceConfigData::ConfigOpModeMonitor("MONITOR");
const std::string WirelessInterfaceConfigData::ConfigOpModeMesh("MESH");
const std::string WirelessInterfaceConfigData::ConfigOpModeDefault(ConfigOpModeNone);

const std::string WirelessInterfaceConfigData::ConfigSsidDefault("");

const std::string WirelessInterfaceConfigData::ConfigBssidDefault("");

const unsigned int WirelessInterfaceConfigData::ConfigChannelDefault(1);

const unsigned int WirelessInterfaceConfigData::ConfigTxPowerDefault(17);

WirelessInterfaceConfigData::WirelessInterfaceConfigData(zInterface::ConfigData& config_) :
    _config(config_)
{
  this->_config.SetIfType(ConfigData::IFTYPE_IEEE80211);
}

WirelessInterfaceConfigData::~WirelessInterfaceConfigData()
{
}

int
WirelessInterfaceConfigData::GetPhyIndex(const int index_) const
{
  int val = 0;
  WirelessInterfaceConfigPath path(WirelessInterfaceConfigPath::ConfigPhyIndexPath);
  if (!this->_config.GetValue(path, val))
  {
    val = index_;
  }
  return (val);
}

bool
WirelessInterfaceConfigData::SetPhyIndex(const int index_)
{
  WirelessInterfaceConfigPath path(WirelessInterfaceConfigPath::ConfigPhyIndexPath);
  return (this->_config.PutValue(path, index_));
}

std::string
WirelessInterfaceConfigData::GetPhyName(const std::string& name_) const
{
  std::string str;
  WirelessInterfaceConfigPath path(WirelessInterfaceConfigPath::ConfigPhyNamePath);
  if (!this->_config.GetValue(path, str))
  {
    str = name_;
  }
  return (str);
}

bool
WirelessInterfaceConfigData::SetPhyName(const std::string& name_)
{
  WirelessInterfaceConfigPath path(WirelessInterfaceConfigPath::ConfigPhyNamePath);
  return (this->_config.PutValue(path, name_));
}

WirelessInterfaceConfigData::HWMODE
WirelessInterfaceConfigData::GetHwMode(const WirelessInterfaceConfigData::HWMODE mode_) const
{
  WirelessInterfaceConfigData::HWMODE mode = mode_;
  WirelessInterfaceConfigPath path(WirelessInterfaceConfigPath::ConfigHwModePath);
  std::string str;
  if (this->_config.GetValue(path, str))
  {
    mode = _str2hwmode(str);
  }
  return (mode);
}

bool
WirelessInterfaceConfigData::SetHwMode(const WirelessInterfaceConfigData::HWMODE mode_)
{
  WirelessInterfaceConfigPath path(WirelessInterfaceConfigPath::ConfigHwModePath);
  std::string str = _hwmode2str(mode_);
  return (this->_config.PutValue(path, str));
}

WirelessInterfaceConfigData::HTMODE
WirelessInterfaceConfigData::GetHtMode(const WirelessInterfaceConfigData::HTMODE mode_) const
{
  WirelessInterfaceConfigData::HTMODE mode = mode_;
  WirelessInterfaceConfigPath path(WirelessInterfaceConfigPath::ConfigHtModePath);
  std::string str;
  if (this->_config.GetValue(path, str))
  {
    mode = _str2htmode(str);
  }
  return (mode);
}

bool
WirelessInterfaceConfigData::SetHtMode(const WirelessInterfaceConfigData::HTMODE mode_)
{
  WirelessInterfaceConfigPath path(WirelessInterfaceConfigPath::ConfigHtModePath);
  std::string str = _htmode2str(mode_);
  return (this->_config.PutValue(path, str));
}

WirelessInterfaceConfigData::OPMODE
WirelessInterfaceConfigData::GetOpMode(const WirelessInterfaceConfigData::OPMODE mode_) const
{
  WirelessInterfaceConfigData::OPMODE mode = mode_;
  WirelessInterfaceConfigPath path(WirelessInterfaceConfigPath::ConfigOpModePath);
  std::string str;
  if (this->_config.GetValue(path, str))
  {
    mode = _str2opmode(str);
  }
  return (mode);
}

bool
WirelessInterfaceConfigData::SetOpMode(const WirelessInterfaceConfigData::OPMODE mode_)
{
  WirelessInterfaceConfigPath path(WirelessInterfaceConfigPath::ConfigOpModePath);
  std::string str = _opmode2str(mode_);
  return (this->_config.PutValue(path, str));
}

std::string
WirelessInterfaceConfigData::GetSsid(const std::string& ssid_) const
{
  std::string str;
  WirelessInterfaceConfigPath path(WirelessInterfaceConfigPath::ConfigSsidPath);
  if (!this->_config.GetValue(path, str))
  {
    str = ssid_;
  }
  return (str);
}

bool
WirelessInterfaceConfigData::SetSsid(const std::string& ssid_)
{
  WirelessInterfaceConfigPath path(WirelessInterfaceConfigPath::ConfigSsidPath);
  return (this->_config.PutValue(path, ssid_));
}

std::string
WirelessInterfaceConfigData::GetBssid(const std::string& bssid_) const
{
  std::string str;
  WirelessInterfaceConfigPath path(WirelessInterfaceConfigPath::ConfigBssidPath);
  if (!this->_config.GetValue(path, str))
  {
    str = bssid_;
  }
  return (str);
}

bool
WirelessInterfaceConfigData::SetBssid(const std::string& bssid_)
{
  WirelessInterfaceConfigPath path(WirelessInterfaceConfigPath::ConfigBssidPath);
  return (this->_config.PutValue(path, bssid_));
}

unsigned int
WirelessInterfaceConfigData::GetChannel(const unsigned int channel_) const
{
  unsigned int val = 0;
  WirelessInterfaceConfigPath path(WirelessInterfaceConfigPath::ConfigChannelPath);
  if (!this->_config.GetValue(path, val))
  {
    val = channel_;
  }
  return (val);
}

bool
WirelessInterfaceConfigData::SetChannel(const unsigned int channel_)
{
  WirelessInterfaceConfigPath path(WirelessInterfaceConfigPath::ConfigChannelPath);
  return (this->_config.PutValue(path, channel_));
}

unsigned int
WirelessInterfaceConfigData::GetTxPower(const unsigned int power_) const
{
  unsigned int val = 0;
  WirelessInterfaceConfigPath path(WirelessInterfaceConfigPath::ConfigTxPowerPath);
  if (!this->_config.GetValue(path, val))
  {
    val = power_;
  }
  return (val);
}

bool
WirelessInterfaceConfigData::SetTxPower(const unsigned int power_)
{
  WirelessInterfaceConfigPath path(WirelessInterfaceConfigPath::ConfigTxPowerPath);
  return (this->_config.PutValue(path, power_));
}

}
}

