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

#ifndef __ZWIRELESSINTERFACE_H__
#define __ZWIRELESSINTERFACE_H__

#include <zutils/zConfig.h>
#include <zutils/zInterface.h>

namespace zUtils
{
namespace zInterface
{

// ****************************************************************************
// Class: WirelessInterfaceConfigPath
// ****************************************************************************

class WirelessInterfaceConfigPath : public zInterface::ConfigPath
{

public:

  static const std::string ConfigPhyIndexPath;
  static const std::string ConfigPhyNamePath;
  static const std::string ConfigHwModePath;
  static const std::string ConfigHtModePath;
  static const std::string ConfigOpModePath;
  static const std::string ConfigSsidPath;
  static const std::string ConfigBssidPath;
  static const std::string ConfigChannelPath;
  static const std::string ConfigTxPowerPath;
  static const std::string ConfigHiddenSsidPath;
  static const std::string ConfigBeaconIntervalPath;
  static const std::string ConfigDtimIntervalPath;

  WirelessInterfaceConfigPath(const std::string& root_ = std::string(""));

  virtual
  ~WirelessInterfaceConfigPath();

protected:

private:

};

// ****************************************************************************
// Class: WirelessInterfaceConfigData
// ****************************************************************************

class WirelessInterfaceConfigData
{

public:
  static const unsigned int ConfigPhyIndexDefault;

  static const std::string ConfigPhyNameDefault;

  static const std::string ConfigHwModeNone;
  static const std::string ConfigHwModeA;
  static const std::string ConfigHwModeB;
  static const std::string ConfigHwModeG;
  static const std::string ConfigHwModeN;
  static const std::string ConfigHwModeAC;
  static const std::string ConfigHwModeAD;
  static const std::string ConfigHwModeAX;
  static const std::string ConfigHwModeDefault;

  enum HWMODE
  {
    HWMODE_ERR = -1,
    HWMODE_NONE = 0,
    HWMODE_DEF = 0,
    HWMODE_A = 1,
    HWMODE_B = 2,
    HWMODE_G = 3,
    HWMODE_N = 4,
    HWMODE_AC = 5,
    HWMODE_AD = 6,
    HWMODE_AX = 7,
    HWMODE_LAST
  };

  static const std::string ConfigHtModeNone;
  static const std::string ConfigHtModeDefault;

  enum HTMODE
  {
    HTMODE_ERR = -1,
    HTMODE_NONE = 0,
    HTMODE_DEF = 0,
    HTMODE_NOHT = 1,
    HTMODE_HT20 = 2,
    HTMODE_HT40MINUS = 3,
    HTMODE_HT40PLUS = 4,
    HTMODE_VHT20 = 5,
    HTMODE_VHT40 = 6,
    HTMODE_VHT80 = 7,
    HTMODE_VHT80PLUS80 = 8,
    HTMODE_VHT160 = 9,
    HTMODE_LAST
  };

  static const std::string ConfigOpModeNone;
  static const std::string ConfigOpModeSTA;
  static const std::string ConfigOpModeAP;
  static const std::string ConfigOpModeAdhoc;
  static const std::string ConfigOpModeMonitor;
  static const std::string ConfigOpModeMesh;
  static const std::string ConfigOpModeDefault;

  enum OPMODE
  {
    OPMODE_ERR = -1,
    OPMODE_NONE = 0,
    OPMODE_STA = 1,
    OPMODE_DEF = 1,
    OPMODE_AP = 2,
    OPMODE_ADHOC = 3,
    OPMODE_MONITOR = 4,
    OPMODE_MESH = 5,
    OPMODE_LAST
  };

  static const std::string ConfigSsidDefault;
  static const std::string ConfigBssidDefault;
  static const unsigned int ConfigChannelDefault;
  static const unsigned int ConfigTxPowerDefault;

  WirelessInterfaceConfigData(zInterface::ConfigData& config_);

  virtual
  ~WirelessInterfaceConfigData();

  unsigned int
  GetPhyIndex(const unsigned int index_ = ConfigPhyIndexDefault) const;

  bool
  SetPhyIndex(const unsigned int index_ = ConfigPhyIndexDefault);

  std::string
  GetPhyName(const std::string& name_ = ConfigPhyNameDefault) const;

  bool
  SetPhyName(const std::string& name_ = ConfigPhyNameDefault);

  WirelessInterfaceConfigData::HWMODE
  GetHwMode(const WirelessInterfaceConfigData::HWMODE mode_ = HWMODE_DEF) const;

  bool
  SetHwMode(const WirelessInterfaceConfigData::HWMODE mode_ = HWMODE_DEF);

  WirelessInterfaceConfigData::HTMODE
  GetHtMode(const WirelessInterfaceConfigData::HTMODE mode_ = HTMODE_DEF) const;

  bool
  SetHtMode(const WirelessInterfaceConfigData::HTMODE mode_ = HTMODE_DEF);

  WirelessInterfaceConfigData::OPMODE
  GetOpMode(const WirelessInterfaceConfigData::OPMODE mode_ = OPMODE_DEF) const;

  bool
  SetOpMode(const WirelessInterfaceConfigData::OPMODE mode_ = OPMODE_DEF);

  std::string
  GetSsid(const std::string& ssid_ = ConfigSsidDefault) const;

  bool
  SetSsid(const std::string& ssid_ = ConfigSsidDefault);

  std::string
  GetBssid(const std::string& bssid_ = ConfigBssidDefault) const;

  bool
  SetBssid(const std::string& bssid_ = ConfigBssidDefault);

  unsigned int
  GetChannel(const unsigned int channel_ = ConfigChannelDefault) const;

  bool
  SetChannel(const unsigned int channel_ = ConfigChannelDefault);

  unsigned int
  GetTxPower(const unsigned int power_ = ConfigTxPowerDefault) const;

  bool
  SetTxPower(const unsigned int power_ = ConfigTxPowerDefault);

protected:

private:

  zInterface::ConfigData& _config;

};

// ****************************************************************************
// Class: WirelessInterface
// ****************************************************************************

class WirelessInterface : public Interface
{
public:

  WirelessInterface(const std::string& name_);

  WirelessInterface(const zInterface::ConfigData& config_);

  virtual
  ~WirelessInterface();

  unsigned int
  GetPhyIndex() const;

  bool
  SetPhyIndex(const unsigned int index_);

  std::string
  GetPhyName() const;

  bool
  SetPhyName(const std::string& name_);

  WirelessInterfaceConfigData::HWMODE
  GetHwMode() const;

  bool
  SetHwMode(const WirelessInterfaceConfigData::HWMODE mode_);

  WirelessInterfaceConfigData::HTMODE
  GetHtMode() const;

  bool
  SetHtMode(const WirelessInterfaceConfigData::HTMODE mode_);

  WirelessInterfaceConfigData::OPMODE
  GetOpMode() const;

  bool
  SetOpMode(const WirelessInterfaceConfigData::OPMODE mode_);

  unsigned int
  GetChannel() const;

  bool
  SetChannel(const unsigned int channel_);

  unsigned int
  GetTxPower() const;

  bool
  SetTxPower(unsigned int txpower_);

  virtual bool
  Refresh();

  virtual bool
  Commit();

  virtual bool
  Create();

  virtual bool
  Destroy();

  virtual void
  Display(const std::string &prefix_ = std::string(""));

protected:

  WirelessInterfaceConfigData wconfig;

  virtual bool
  is_modified() const;

  virtual void
  set_modified();

  virtual void
  clr_modified();

private:

  bool _modified;

};

}
}

#endif /* __ZWIRELESSINTERFACE_H__ */
