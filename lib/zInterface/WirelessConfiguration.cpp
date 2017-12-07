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

const unsigned int WirelessInterfaceConfigData::ConfigPhyIndexDefault(0);

const std::string WirelessInterfaceConfigData::ConfigPhyNameDefault("");

const std::string WirelessInterfaceConfigData::ConfigHwModeNone("");
const std::string WirelessInterfaceConfigData::ConfigHwModeA("A");
const std::string WirelessInterfaceConfigData::ConfigHwModeB("B");
const std::string WirelessInterfaceConfigData::ConfigHwModeG("G");
const std::string WirelessInterfaceConfigData::ConfigHwModeN("N");
const std::string WirelessInterfaceConfigData::ConfigHwModeAC("AC");
const std::string WirelessInterfaceConfigData::ConfigHwModeAD("AD");
const std::string WirelessInterfaceConfigData::ConfigHwModeAX("AX");
const std::string WirelessInterfaceConfigData::ConfigHwModeDefault(WirelessInterfaceConfigData::ConfigHwModeNone);

const std::string WirelessInterfaceConfigData::ConfigHtModeNone("");
const std::string WirelessInterfaceConfigData::ConfigHtModeDefault(WirelessInterfaceConfigData::ConfigHtModeNone);

const std::string WirelessInterfaceConfigData::ConfigOpModeNone("");
const std::string WirelessInterfaceConfigData::ConfigOpModeSTA("STATION");
const std::string WirelessInterfaceConfigData::ConfigOpModeAP("AP");
const std::string WirelessInterfaceConfigData::ConfigOpModeAdhoc("ADHOC");
const std::string WirelessInterfaceConfigData::ConfigOpModeMonitor("MONITOR");
const std::string WirelessInterfaceConfigData::ConfigOpModeMesh("MESH");
const std::string WirelessInterfaceConfigData::ConfigOpModeDefault(WirelessInterfaceConfigData::ConfigOpModeNone);

const std::string WirelessInterfaceConfigData::ConfigSsidDefault("");

const std::string WirelessInterfaceConfigData::ConfigBssidDefault("00:00:00:00:00:00");

const unsigned int WirelessInterfaceConfigData::ConfigChannelDefault(1);

const unsigned int WirelessInterfaceConfigData::ConfigTxPowerDefault(23);

WirelessInterfaceConfigData::WirelessInterfaceConfigData(zInterface::ConfigData& config_) :
    _config(config_)
{
  this->_config.SetIfType(ConfigData::IFTYPE_IEEE80211);
  this->SetPhyIndex(this->GetPhyIndex());
  this->SetPhyName(this->GetPhyName());
  this->SetHwMode(this->GetHwMode());
  this->SetHtMode(this->GetHtMode());
  this->SetOpMode(this->GetOpMode());
  this->SetSsid(this->GetSsid());
  this->SetBssid(this->GetBssid());
}

WirelessInterfaceConfigData::~WirelessInterfaceConfigData()
{
}

unsigned int
WirelessInterfaceConfigData::GetPhyIndex() const
{
  unsigned int val = 0;
  WirelessInterfaceConfigPath path(WirelessInterfaceConfigPath::ConfigPhyIndexPath);
  if (!this->_config.GetValue(path, val))
  {
    val = ConfigPhyIndexDefault;
  }
  return (val);
}

bool
WirelessInterfaceConfigData::SetPhyIndex(const unsigned int index_)
{
  WirelessInterfaceConfigPath path(WirelessInterfaceConfigPath::ConfigPhyIndexPath);
  return (this->_config.PutValue(path, index_));
}

std::string
WirelessInterfaceConfigData::GetPhyName() const
{
  std::string str;
  WirelessInterfaceConfigPath path(WirelessInterfaceConfigPath::ConfigPhyNamePath);
  if (!this->_config.GetValue(path, str))
  {
    str = ConfigPhyNameDefault;
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
WirelessInterfaceConfigData::GetHwMode() const
{
  std::string str;
  WirelessInterfaceConfigData::HWMODE mode = WirelessInterfaceConfigData::HWMODE_ERR;

  WirelessInterfaceConfigPath path(WirelessInterfaceConfigPath::ConfigHwModePath);
  if (!this->_config.GetValue(path, str))
  {
    str = ConfigHwModeDefault;
  }

  if (str == ConfigHwModeA)
  {
    mode = HWMODE_A;
  }
  else if (str == ConfigHwModeB)
  {
    mode = HWMODE_B;
  }
  else if (str == ConfigHwModeG)
  {
    mode = HWMODE_G;
  }
  else if (str == ConfigHwModeN)
  {
    mode = HWMODE_N;
  }
  else if (str == ConfigHwModeAC)
  {
    mode = HWMODE_AC;
  }
  else if (str == ConfigHwModeAD)
  {
    mode = HWMODE_AD;
  }
  else if (str == ConfigHwModeAX)
  {
    mode = HWMODE_AX;
  }

  return (mode);
}

bool
WirelessInterfaceConfigData::SetHwMode(const WirelessInterfaceConfigData::HWMODE mode_)
{
  std::string str;
  WirelessInterfaceConfigPath path(WirelessInterfaceConfigPath::ConfigHwModePath);

  switch (mode_)
  {
  case HWMODE_A:
    str = ConfigHwModeA;
    break;
  case HWMODE_B:
    str = ConfigHwModeB;
    break;
  case HWMODE_G:
    str = ConfigHwModeG;
    break;
  case HWMODE_N:
    str = ConfigHwModeN;
    break;
  case HWMODE_AC:
    str = ConfigHwModeAC;
    break;
  case HWMODE_AD:
    str = ConfigHwModeAD;
    break;
  case HWMODE_AX:
    str = ConfigHwModeAX;
    break;
  default:
    str = ConfigHwModeNone;
    break;
  }

  return (this->_config.PutValue(path, str));
}

WirelessInterfaceConfigData::HTMODE
WirelessInterfaceConfigData::GetHtMode() const
{
  std::string str;
  WirelessInterfaceConfigData::HTMODE mode = WirelessInterfaceConfigData::HTMODE_ERR;

  WirelessInterfaceConfigPath path(WirelessInterfaceConfigPath::ConfigHtModePath);
  if (!this->_config.GetValue(path, str))
  {
    str = ConfigHtModeDefault;
  }
  return (mode);
}

bool
WirelessInterfaceConfigData::SetHtMode(const WirelessInterfaceConfigData::HTMODE mode_)
{
  std::string str;
  WirelessInterfaceConfigPath path(WirelessInterfaceConfigPath::ConfigHtModePath);

  return (this->_config.PutValue(path, str));
}

WirelessInterfaceConfigData::OPMODE
WirelessInterfaceConfigData::GetOpMode() const
{
  std::string str;
  WirelessInterfaceConfigData::OPMODE mode = WirelessInterfaceConfigData::OPMODE_ERR;

  WirelessInterfaceConfigPath path(WirelessInterfaceConfigPath::ConfigOpModePath);
  if (!this->_config.GetValue(path, str))
  {
    str = ConfigOpModeDefault;
  }

  if (str == ConfigOpModeSTA)
  {
    mode = OPMODE_STA;
  }
  else if (str == ConfigOpModeAP)
  {
    mode = OPMODE_AP;
  }
  else if (str == ConfigOpModeAdhoc)
  {
    mode = OPMODE_ADHOC;
  }
  else if (str == ConfigOpModeMonitor)
  {
    mode = OPMODE_MONITOR;
  }
  else if (str == ConfigOpModeMesh)
  {
    mode = OPMODE_MESH;
  }

  return (mode);

}

bool
WirelessInterfaceConfigData::SetOpMode(const WirelessInterfaceConfigData::OPMODE mode_)
{
  std::string str;
  WirelessInterfaceConfigPath path(WirelessInterfaceConfigPath::ConfigOpModePath);

  switch (mode_)
  {
  case OPMODE_STA:
    str = ConfigOpModeSTA;
    break;
  case OPMODE_AP:
    str = ConfigOpModeAP;
    break;
  case OPMODE_ADHOC:
    str = ConfigOpModeAdhoc;
    break;
  case OPMODE_MONITOR:
    str = ConfigOpModeMonitor;
    break;
  case OPMODE_MESH:
    str = ConfigOpModeMesh;
    break;
  default:
    str = ConfigOpModeNone;
    break;
  }

  return (this->_config.PutValue(path, str));
}

std::string
WirelessInterfaceConfigData::GetSsid() const
{
  std::string str;
  WirelessInterfaceConfigPath path(WirelessInterfaceConfigPath::ConfigSsidPath);
  if (!this->_config.GetValue(path, str))
  {
    str = ConfigSsidDefault;
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
WirelessInterfaceConfigData::GetBssid() const
{
  std::string str;
  WirelessInterfaceConfigPath path(WirelessInterfaceConfigPath::ConfigBssidPath);
  if (!this->_config.GetValue(path, str))
  {
    str = ConfigBssidDefault;
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
WirelessInterfaceConfigData::GetChannel() const
{
  unsigned int val = 0;
  WirelessInterfaceConfigPath path(WirelessInterfaceConfigPath::ConfigChannelPath);
  if (!this->_config.GetValue(path, val))
  {
    val = ConfigPhyIndexDefault;
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
WirelessInterfaceConfigData::GetTxPower() const
{
  unsigned int val = 0;
  WirelessInterfaceConfigPath path(WirelessInterfaceConfigPath::ConfigTxPowerPath);
  if (!this->_config.GetValue(path, val))
  {
    val = ConfigPhyIndexDefault;
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

