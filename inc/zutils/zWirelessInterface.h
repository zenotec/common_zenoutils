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

namespace nl80211
{
 class GetPhyCommand;
 class GetInterfaceCommand;
 class SetInterfaceCommand;
 class NewInterfaceCommand;
 class DelInterfaceCommand;
}
using namespace nl80211;

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

  WirelessInterfaceConfigPath(const std::string& root_ = std::string(""));

  WirelessInterfaceConfigPath(const zData::DataPath& path_);

  WirelessInterfaceConfigPath(const WirelessInterfaceConfigPath& other_);

  virtual
  ~WirelessInterfaceConfigPath();

protected:

private:

};

// ****************************************************************************
// Class: WirelessInterfaceConfigData
// ****************************************************************************

class WirelessInterfaceConfigData : public zConfig::ConfigData
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

  static const std::string ConfigHtModeNone;
  static const std::string ConfigHtModeDefault;

  static const std::string ConfigOpModeNone;
  static const std::string ConfigOpModeSTA;
  static const std::string ConfigOpModeAP;
  static const std::string ConfigOpModeAdhoc;
  static const std::string ConfigOpModeMonitor;
  static const std::string ConfigOpModeMesh;
  static const std::string ConfigOpModeDefault;

  WirelessInterfaceConfigData();

  WirelessInterfaceConfigData(const zData::Data& data_);

  WirelessInterfaceConfigData(const zConfig::ConfigData& config_);

  WirelessInterfaceConfigData(const WirelessInterfaceConfigData& other_);

  virtual
  ~WirelessInterfaceConfigData();

  unsigned int
  PhyIndex() const;

  bool
  PhyIndex(const unsigned int index_);

  std::string
  PhyName() const;

  bool
  PhyName(const std::string& name_);

  std::string
  HwMode() const;

  bool
  HwMode(const std::string& mode_);

  std::string
  HtMode() const;

  bool
  HtMode(const std::string& mode_);

  std::string
  OpMode() const;

  bool
  OpMode(const std::string& mode_);

protected:

private:

};


// ****************************************************************************
// Class: WirelessInterface
// ****************************************************************************

class WirelessInterface : public Interface
{
public:

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

  enum OPMODE
  {
    OPMODE_ERR = -1,
    OPMODE_NONE = 0,
    OPMODE_STA = 1,
    OPMODE_AP = 2,
    OPMODE_ADHOC = 3,
    OPMODE_MONITOR = 4,
    OPMODE_MESH = 5,
    OPMODE_LAST
  };

  WirelessInterfaceConfigData WiConfig;

  WirelessInterface(const int index_ = 0);

  WirelessInterface(const std::string& name_);

  WirelessInterface(const zConfig::ConfigData& config_);

  virtual
  ~WirelessInterface();

  unsigned int
  GetPhyIndex() const;

  std::string
  GetPhyName() const;

  WirelessInterface::HWMODE
  GetHwMode() const;

  bool
  SetHwMode(const WirelessInterface::HWMODE mode_);

  WirelessInterface::HTMODE
  GetHtMode() const;

  bool
  SetHtMode(const WirelessInterface::HTMODE mode_);

  WirelessInterface::OPMODE
  GetOpMode() const;

  bool
  SetOpMode(const WirelessInterface::OPMODE mode_);

  virtual bool
  Refresh();

  virtual bool
  Create();

  virtual bool
  Destroy();

//  std::string
//  GetBssid() const;
//
//  bool
//  SetBssid(const std::string& bssid_);
//
//  std::string
//  GetEssid() const;
//
//  bool
//  SetEssid(const std::string& essid_);
//
//  std::string
//  GetSsid() const;
//
//  bool
//  SetSsid(const std::string& ssid_);
//
//  float
//  GetChannel() const;
//
//  bool
//  SetChannel(const float chnl_);
//
//  int
//  GetTxPower() const;
//
//  bool
//  SetTxPower(int txpow_);
//
//  bool
//  IsAssociated();
//
//  bool
//  Associate(const std::string &essid_, const unsigned int channel_);
//
//  bool
//  Disassociate();
//
//  int
//  LinkQuality();
//
//  int
//  SignalLevel();
//
//  int
//  NoiseLevel();
//
//  int
//  BitRate();
//
  virtual void
  Display(const std::string &prefix_ = std::string(""));

protected:

  GetPhyCommand* _getphycmd;
  GetInterfaceCommand* _getifacecmd;
  SetInterfaceCommand* _setifacecmd;
  NewInterfaceCommand* _newifacecmd;
  DelInterfaceCommand* _delifacecmd;

private:

};

}
}
#endif /* __ZWIRELESSINTERFACE_H__ */
