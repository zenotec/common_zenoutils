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
#include <zutils/zLog.h>
#include <zutils/zInterface.h>
#include <zutils/zWireless.h>

// local includes

ZLOG_MODULE_INIT(zUtils::zLog::Log::MODULE_WIRELESS);

namespace zUtils
{
namespace zWireless
{


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
const std::string ConfigPath::ConfigPhyDevPath("PhyDev");
const std::string ConfigPath::ConfigPhyNamePath("PhyName");
const std::string ConfigPath::ConfigOpModePath("OpMode");
const std::string ConfigPath::ConfigSsidPath("Ssid");
const std::string ConfigPath::ConfigBssidPath("Bssid");
const std::string ConfigPath::ConfigHtModePath("HtMode");
const std::string ConfigPath::ConfigFrequencyPath("Frequency");
const std::string ConfigPath::ConfigCenterFrequency1Path("CenterFrequency1");
const std::string ConfigPath::ConfigCenterFrequency2Path("CenterFrequency2");
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
const int ConfigData::ConfigPhyDevDefault(-1);

const std::string ConfigData::ConfigPhyNameDefault("");

const std::string ConfigData::ConfigOpModeNone("");
const std::string ConfigData::ConfigOpModeSTA("STATION");
const std::string ConfigData::ConfigOpModeAP("AP");
const std::string ConfigData::ConfigOpModeAdhoc("ADHOC");
const std::string ConfigData::ConfigOpModeMonitor("MONITOR");
const std::string ConfigData::ConfigOpModeMesh("MESH");
const std::string ConfigData::ConfigOpModeDefault(ConfigOpModeNone);

const std::string ConfigData::ConfigSsidDefault("");

const std::string ConfigData::ConfigBssidDefault("");

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

const unsigned int ConfigData::ConfigFrequencyDefault(0);
const unsigned int ConfigData::ConfigCenterFrequency1Default(0);
const unsigned int ConfigData::ConfigCenterFrequency2Default(0);

const unsigned int ConfigData::ConfigTxPowerDefault(0);

ConfigData::ConfigData(const std::string& name_) :
    zInterface::ConfigData(name_)
{
  ZLOG_DEBUG("zWireless::ConfigData::ConfigData(name_)");
  this->SetIfType(zInterface::ConfigData::IFTYPE_IEEE80211);
}

ConfigData::ConfigData(zInterface::ConfigData config_) :
    zInterface::ConfigData(config_)
{
  ZLOG_DEBUG("zWireless::ConfigData::ConfigData(config_)");
  this->SetIfType(zInterface::ConfigData::IFTYPE_IEEE80211);
}

ConfigData::ConfigData(SHARED_PTR(zConfig::ConfigData) data_) :
    zInterface::ConfigData(data_)
{
  ZLOG_DEBUG("zWireless::ConfigData::ConfigData(data_)");
  this->SetIfType(zInterface::ConfigData::IFTYPE_IEEE80211);
}

ConfigData::~ConfigData()
{
}

int
ConfigData::GetPhyIndex(const int index_) const
{
  int val = -1;
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

int
ConfigData::GetPhyDev(const int index_) const
{
  int val = -1;
  ConfigPath path(ConfigPath::ConfigPhyDevPath);
  if (!this->GetData()->GetValue(path, val))
  {
    val = index_;
  }
  return (val);
}

bool
ConfigData::SetPhyDev(const int dev_)
{
  ConfigPath path(ConfigPath::ConfigPhyDevPath);
  return (this->GetData()->PutValue(path, dev_));
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

unsigned int
ConfigData::GetFrequency(const unsigned int freq_) const
{
  unsigned int val = 0;
  ConfigPath path(ConfigPath::ConfigFrequencyPath);
  if (!this->GetData()->GetValue(path, val))
  {
    val = freq_;
  }
  return (val);
}

bool
ConfigData::SetFrequency(const unsigned int freq_)
{
  ConfigPath path(ConfigPath::ConfigFrequencyPath);
  return (this->GetData()->PutValue(path, freq_));
}

unsigned int
ConfigData::GetCenterFrequency1(const unsigned int freq_) const
{
  unsigned int val = 0;
  ConfigPath path(ConfigPath::ConfigCenterFrequency1Path);
  if (!this->GetData()->GetValue(path, val))
  {
    val = freq_;
  }
  return (val);
}

bool
ConfigData::SetCenterFrequency1(const unsigned int freq_)
{
  ConfigPath path(ConfigPath::ConfigCenterFrequency1Path);
  return (this->GetData()->PutValue(path, freq_));
}

unsigned int
ConfigData::GetCenterFrequency2(const unsigned int freq_) const
{
  unsigned int val = 0;
  ConfigPath path(ConfigPath::ConfigCenterFrequency1Path);
  if (!this->GetData()->GetValue(path, val))
  {
    val = freq_;
  }
  return (val);
}

bool
ConfigData::SetCenterFrequency2(const unsigned int freq_)
{
  ConfigPath path(ConfigPath::ConfigCenterFrequency2Path);
  return (this->GetData()->PutValue(path, freq_));
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

