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
#include <zutils/zWireless.h>

// local includes

namespace zUtils
{
namespace zWireless
{

static std::string
_hwmode2str(const ConfigData::HWMODE mode_)
{
  std::string str;
  switch (mode_)
  {
  case ConfigData::HWMODE_NONE:
    str = ConfigData::ConfigHwModeNone;
    break;
  case ConfigData::HWMODE_A:
    str = ConfigData::ConfigHwModeA;
    break;
  case ConfigData::HWMODE_B:
    str = ConfigData::ConfigHwModeB;
    break;
  case ConfigData::HWMODE_G:
    str = ConfigData::ConfigHwModeG;
    break;
  case ConfigData::HWMODE_N:
    str = ConfigData::ConfigHwModeN;
    break;
  case ConfigData::HWMODE_AC:
    str = ConfigData::ConfigHwModeAC;
    break;
  case ConfigData::HWMODE_AD:
    str = ConfigData::ConfigHwModeAD;
    break;
  case ConfigData::HWMODE_AX:
    str = ConfigData::ConfigHwModeAX;
    break;
  default:
    str = ConfigData::ConfigHwModeDefault;
    break;
  }
  return (str);
}

static ConfigData::HWMODE
_str2hwmode(const std::string& str)
{
  ConfigData::HWMODE mode = ConfigData::HWMODE_ERR;

  if (str == ConfigData::ConfigHwModeNone)
  {
    mode = ConfigData::HWMODE_NONE;
  }
  else if (str == ConfigData::ConfigHwModeA)
  {
    mode = ConfigData::HWMODE_A;
  }
  else if (str == ConfigData::ConfigHwModeB)
  {
    mode = ConfigData::HWMODE_B;
  }
  else if (str == ConfigData::ConfigHwModeG)
  {
    mode = ConfigData::HWMODE_G;
  }
  else if (str == ConfigData::ConfigHwModeN)
  {
    mode = ConfigData::HWMODE_N;
  }
  else if (str == ConfigData::ConfigHwModeAC)
  {
    mode = ConfigData::HWMODE_AC;
  }
  else if (str == ConfigData::ConfigHwModeAD)
  {
    mode = ConfigData::HWMODE_AD;
  }
  else if (str == ConfigData::ConfigHwModeAX)
  {
    mode = ConfigData::HWMODE_AX;
  }
  else
  {
    mode = ConfigData::HWMODE_DEF;
  }
  return (mode);
}

static std::string
_htmode2str(const ConfigData::HTMODE mode_)
{
  std::string str;
  switch (mode_)
  {
  case ConfigData::HTMODE_NONE:
    str = ConfigData::ConfigHtModeNone;
    break;
  case ConfigData::HTMODE_HT20:
    str = ConfigData::ConfigHtModeHT20;
    break;
  case ConfigData::HTMODE_HT40MINUS:
    str = ConfigData::ConfigHtModeHT40Minus;
    break;
  case ConfigData::HTMODE_HT40PLUS:
    str = ConfigData::ConfigHtModeHT40Plus;
    break;
  case ConfigData::HTMODE_VHT20:
    str = ConfigData::ConfigHtModeVHT20;
    break;
  case ConfigData::HTMODE_VHT40:
    str = ConfigData::ConfigHtModeVHT40;
    break;
  case ConfigData::HTMODE_VHT80:
    str = ConfigData::ConfigHtModeVHT80;
    break;
  case ConfigData::HTMODE_VHT80PLUS80:
    str = ConfigData::ConfigHtModeVHT80Plus80;
    break;
  case ConfigData::HTMODE_VHT160:
    str = ConfigData::ConfigHtModeVHT160;
    break;
  default:
    str = ConfigData::ConfigHtModeDefault;
    break;
  }
  return (str);
}

static ConfigData::HTMODE
_str2htmode(const std::string& str)
{
  ConfigData::HTMODE mode = ConfigData::HTMODE_ERR;
  if (str == ConfigData::ConfigHtModeNone)
  {
    mode = ConfigData::HTMODE_NONE;
  }
  else if (str == ConfigData::ConfigHtModeHT20)
  {
    mode = ConfigData::HTMODE_HT20;
  }
  else if (str == ConfigData::ConfigHtModeHT40Minus)
  {
    mode = ConfigData::HTMODE_HT40MINUS;
  }
  else if (str == ConfigData::ConfigHtModeHT40Plus)
  {
    mode = ConfigData::HTMODE_HT40PLUS;
  }
  else if (str == ConfigData::ConfigHtModeVHT20)
  {
    mode = ConfigData::HTMODE_VHT20;
  }
  else if (str == ConfigData::ConfigHtModeVHT40)
  {
    mode = ConfigData::HTMODE_VHT40;
  }
  else if (str == ConfigData::ConfigHtModeVHT80)
  {
    mode = ConfigData::HTMODE_VHT80;
  }
  else if (str == ConfigData::ConfigHtModeVHT80Plus80)
  {
    mode = ConfigData::HTMODE_VHT80PLUS80;
  }
  else if (str == ConfigData::ConfigHtModeVHT160)
  {
    mode = ConfigData::HTMODE_VHT160;
  }
  else
  {
    mode = ConfigData::HTMODE_DEF;
  }
  return (mode);
}

static std::string
_opmode2str(const ConfigData::OPMODE mode_)
{
  std::string str;
  switch (mode_)
  {
  case ConfigData::OPMODE_NONE:
    str = ConfigData::ConfigOpModeNone;
    break;
  case ConfigData::OPMODE_STA:
    str = ConfigData::ConfigOpModeSTA;
    break;
  case ConfigData::OPMODE_AP:
    str = ConfigData::ConfigOpModeAP;
    break;
  case ConfigData::OPMODE_ADHOC:
    str = ConfigData::ConfigOpModeAdhoc;
    break;
  case ConfigData::OPMODE_MONITOR:
    str = ConfigData::ConfigOpModeMonitor;
    break;
  case ConfigData::OPMODE_MESH:
    str = ConfigData::ConfigOpModeMesh;
    break;
  default:
    str = ConfigData::ConfigOpModeDefault;
    break;
  }
  return (str);
}

static ConfigData::OPMODE
_str2opmode(const std::string& str)
{
  ConfigData::OPMODE mode = ConfigData::OPMODE_ERR;
  if (str == ConfigData::ConfigOpModeSTA)
  {
    mode = ConfigData::OPMODE_STA;
  }
  else if (str == ConfigData::ConfigOpModeAP)
  {
    mode = ConfigData::OPMODE_AP;
  }
  else if (str == ConfigData::ConfigOpModeAdhoc)
  {
    mode = ConfigData::OPMODE_ADHOC;
  }
  else if (str == ConfigData::ConfigOpModeMonitor)
  {
    mode = ConfigData::OPMODE_MONITOR;
  }
  else if (str == ConfigData::ConfigOpModeMesh)
  {
    mode = ConfigData::OPMODE_MESH;
  }
  else
  {
    mode = ConfigData::OPMODE_DEF;
  }
  return (mode);
}

// ****************************************************************************
// Class: WirelessInterfaceConfigPath
// ****************************************************************************

const std::string ConfigPath::ConfigPhyIndexPath("PhyIndex");
const std::string ConfigPath::ConfigPhyNamePath("PhyName");
const std::string ConfigPath::ConfigHwModePath("HwMode");
const std::string ConfigPath::ConfigHtModePath("HtMode");
const std::string ConfigPath::ConfigOpModePath("OpMode");
const std::string ConfigPath::ConfigSsidPath("Ssid");
const std::string ConfigPath::ConfigBssidPath("Bssid");
const std::string ConfigPath::ConfigChannelPath("Channel");
const std::string ConfigPath::ConfigTxPowerPath("TxPower");

ConfigPath::ConfigPath(const std::string& root_)
{
  this->Append(root_);
}

ConfigPath::~ConfigPath()
{
}

// ****************************************************************************
// Class: zWireless::ConfigData
// ****************************************************************************

const int ConfigData::ConfigPhyIndexDefault(-1);

const std::string ConfigData::ConfigPhyNameDefault("");

const std::string ConfigData::ConfigHwModeNone("");
const std::string ConfigData::ConfigHwModeA("A");
const std::string ConfigData::ConfigHwModeB("B");
const std::string ConfigData::ConfigHwModeG("G");
const std::string ConfigData::ConfigHwModeN("N");
const std::string ConfigData::ConfigHwModeAC("AC");
const std::string ConfigData::ConfigHwModeAD("AD");
const std::string ConfigData::ConfigHwModeAX("AX");
const std::string ConfigData::ConfigHwModeDefault(ConfigHwModeNone);

const std::string ConfigData::ConfigHtModeNone("");
const std::string ConfigData::ConfigHtModeHT20("HT20");
const std::string ConfigData::ConfigHtModeHT40Minus("HT40-");
const std::string ConfigData::ConfigHtModeHT40Plus("HT40+");
const std::string ConfigData::ConfigHtModeVHT20("VHT20");
const std::string ConfigData::ConfigHtModeVHT40("VHT40");
const std::string ConfigData::ConfigHtModeVHT80("VHT80");
const std::string ConfigData::ConfigHtModeVHT80Plus80("VHT80+80");
const std::string ConfigData::ConfigHtModeVHT160("VHT160");
const std::string ConfigData::ConfigHtModeDefault(ConfigHtModeNone);

const std::string ConfigData::ConfigOpModeNone("");
const std::string ConfigData::ConfigOpModeSTA("STATION");
const std::string ConfigData::ConfigOpModeAP("AP");
const std::string ConfigData::ConfigOpModeAdhoc("ADHOC");
const std::string ConfigData::ConfigOpModeMonitor("MONITOR");
const std::string ConfigData::ConfigOpModeMesh("MESH");
const std::string ConfigData::ConfigOpModeDefault(ConfigOpModeNone);

const std::string ConfigData::ConfigSsidDefault("");

const std::string ConfigData::ConfigBssidDefault("");

const unsigned int ConfigData::ConfigChannelDefault(1);

const unsigned int ConfigData::ConfigTxPowerDefault(17);


ConfigData::ConfigData(const std::string& name_) :
    zInterface::ConfigData(name_)
{
  ZLOG_DEBUG("zWireless::ConfigData::ConfigData(name_)");
  this->SetIfType(zInterface::ConfigData::IFTYPE_IEEE80211);
}

ConfigData::ConfigData(SHARED_PTR(zConfig::ConfigData) data_) :
    zInterface::ConfigData(data_)
{
  ZLOG_DEBUG("zWireless::ConfigData::ConfigData(name_)");
  this->SetIfType(zInterface::ConfigData::IFTYPE_IEEE80211);
}

ConfigData::~ConfigData()
{
}

int
ConfigData::GetPhyIndex(const int index_) const
{
  int val = 0;
  ConfigPath path(ConfigPath::ConfigPhyIndexPath);
  if (!this->GetData()->GetValue(path, val))
  {
    val = index_;
  }
  return (val);
}

bool
ConfigData::SetPhyIndex(const int index_)
{
  ConfigPath path(ConfigPath::ConfigPhyIndexPath);
  return (this->GetData()->PutValue(path, index_));
}

std::string
ConfigData::GetPhyName(const std::string& name_) const
{
  std::string str;
  ConfigPath path(ConfigPath::ConfigPhyNamePath);
  if (!this->GetData()->GetValue(path, str))
  {
    str = name_;
  }
  return (str);
}

bool
ConfigData::SetPhyName(const std::string& name_)
{
  ConfigPath path(ConfigPath::ConfigPhyNamePath);
  return (this->GetData()->PutValue(path, name_));
}

ConfigData::HWMODE
ConfigData::GetHwMode(const ConfigData::HWMODE mode_) const
{
  ConfigData::HWMODE mode = mode_;
  ConfigPath path(ConfigPath::ConfigHwModePath);
  std::string str;
  if (this->GetData()->GetValue(path, str))
  {
    mode = _str2hwmode(str);
  }
  return (mode);
}

bool
ConfigData::SetHwMode(const ConfigData::HWMODE mode_)
{
  ConfigPath path(ConfigPath::ConfigHwModePath);
  std::string str = _hwmode2str(mode_);
  return (this->GetData()->PutValue(path, str));
}

ConfigData::HTMODE
ConfigData::GetHtMode(const ConfigData::HTMODE mode_) const
{
  ConfigData::HTMODE mode = mode_;
  ConfigPath path(ConfigPath::ConfigHtModePath);
  std::string str;
  if (this->GetData()->GetValue(path, str))
  {
    mode = _str2htmode(str);
  }
  return (mode);
}

bool
ConfigData::SetHtMode(const ConfigData::HTMODE mode_)
{
  ConfigPath path(ConfigPath::ConfigHtModePath);
  std::string str = _htmode2str(mode_);
  return (this->GetData()->PutValue(path, str));
}

ConfigData::OPMODE
ConfigData::GetOpMode(const ConfigData::OPMODE mode_) const
{
  ConfigData::OPMODE mode = mode_;
  ConfigPath path(ConfigPath::ConfigOpModePath);
  std::string str;
  if (this->GetData()->GetValue(path, str))
  {
    mode = _str2opmode(str);
  }
  return (mode);
}

bool
ConfigData::SetOpMode(const ConfigData::OPMODE mode_)
{
  ConfigPath path(ConfigPath::ConfigOpModePath);
  std::string str = _opmode2str(mode_);
  return (this->GetData()->PutValue(path, str));
}

std::string
ConfigData::GetSsid(const std::string& ssid_) const
{
  std::string str;
  ConfigPath path(ConfigPath::ConfigSsidPath);
  if (!this->GetData()->GetValue(path, str))
  {
    str = ssid_;
  }
  return (str);
}

bool
ConfigData::SetSsid(const std::string& ssid_)
{
  ConfigPath path(ConfigPath::ConfigSsidPath);
  return (this->GetData()->PutValue(path, ssid_));
}

std::string
ConfigData::GetBssid(const std::string& bssid_) const
{
  std::string str;
  ConfigPath path(ConfigPath::ConfigBssidPath);
  if (!this->GetData()->GetValue(path, str))
  {
    str = bssid_;
  }
  return (str);
}

bool
ConfigData::SetBssid(const std::string& bssid_)
{
  ConfigPath path(ConfigPath::ConfigBssidPath);
  return (this->GetData()->PutValue(path, bssid_));
}

unsigned int
ConfigData::GetChannel(const unsigned int channel_) const
{
  unsigned int val = 0;
  ConfigPath path(ConfigPath::ConfigChannelPath);
  if (!this->GetData()->GetValue(path, val))
  {
    val = channel_;
  }
  return (val);
}

bool
ConfigData::SetChannel(const unsigned int channel_)
{
  ConfigPath path(ConfigPath::ConfigChannelPath);
  return (this->GetData()->PutValue(path, channel_));
}

unsigned int
ConfigData::GetTxPower(const unsigned int power_) const
{
  unsigned int val = 0;
  ConfigPath path(ConfigPath::ConfigTxPowerPath);
  if (!this->GetData()->GetValue(path, val))
  {
    val = power_;
  }
  return (val);
}

bool
ConfigData::SetTxPower(const unsigned int power_)
{
  ConfigPath path(ConfigPath::ConfigTxPowerPath);
  return (this->GetData()->PutValue(path, power_));
}

}
}

