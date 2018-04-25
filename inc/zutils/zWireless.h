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

#ifndef __ZWIRELESS_H__
#define __ZWIRELESS_H__

#include <zutils/zConfig.h>
#include <zutils/zInterface.h>

#include <zutils/ieee80211/PowerCapsTag.h>
#include <zutils/ieee80211/HtCapsTag.h>
#include <zutils/ieee80211/HtInfoTag.h>

namespace zUtils
{
namespace zWireless
{

// ****************************************************************************
// Class: Wireless::ConfigPath
// ****************************************************************************

class ConfigPath : public zInterface::ConfigPath
{

public:

  static const std::string ConfigPhyIndexPath;
  static const std::string ConfigPhyDevPath;
  static const std::string ConfigPhyNamePath;
  static const std::string ConfigOpModePath;
  static const std::string ConfigSsidPath;
  static const std::string ConfigBssidPath;
  static const std::string ConfigHiddenSsidPath;
  static const std::string ConfigBeaconIntervalPath;
  static const std::string ConfigDtimIntervalPath;
  static const std::string ConfigHtModePath;
  static const std::string ConfigFrequencyPath;
  static const std::string ConfigCenterFrequency1Path;
  static const std::string ConfigCenterFrequency2Path;
  static const std::string ConfigTxPowerPath;

  ConfigPath(const std::string& root_ = std::string(""));

  virtual
  ~ConfigPath();

protected:

private:

};

// ****************************************************************************
// Class: zWireless::ConfigData
// ****************************************************************************

class ConfigData : public zInterface::ConfigData
{

public:

  static const int ConfigPhyIndexDefault;
  static const int ConfigPhyDevDefault;

  static const std::string ConfigPhyNameDefault;

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

  static const std::string ConfigHtModeNone;
  static const std::string ConfigHtModeHT20;
  static const std::string ConfigHtModeHT40;
  static const std::string ConfigHtModeVHT20;
  static const std::string ConfigHtModeVHT40;
  static const std::string ConfigHtModeVHT80;
  static const std::string ConfigHtModeVHT80Plus80;
  static const std::string ConfigHtModeVHT160;
  static const std::string ConfigHtModeDefault;

  enum HTMODE
  {
    HTMODE_ERR = -1,
    HTMODE_NONE = 0,
    HTMODE_DEF = 0,
    HTMODE_NOHT = 1,
    HTMODE_HT20 = 2,
    HTMODE_HT40 = 3,
    HTMODE_VHT20 = 4,
    HTMODE_VHT40 = 5,
    HTMODE_VHT80 = 6,
    HTMODE_VHT80PLUS80 = 7,
    HTMODE_VHT160 = 8,
    HTMODE_LAST
  };

  static const unsigned int ConfigFrequencyDefault;
  static const unsigned int ConfigCenterFrequency1Default; // VHT only
  static const unsigned int ConfigCenterFrequency2Default; // VHT only
  static const unsigned int ConfigTxPowerDefault;

  ConfigData(const std::string& name_ = ConfigIfNameDefault);

  ConfigData(zInterface::ConfigData config_);

  ConfigData(SHARED_PTR(zConfig::ConfigData) data_);

  virtual
  ~ConfigData();

  int
  GetPhyIndex(const int index_ = ConfigPhyIndexDefault) const;

  bool
  SetPhyIndex(const int index_ = ConfigPhyIndexDefault);

  int
  GetPhyDev(const int index_ = ConfigPhyDevDefault) const;

  bool
  SetPhyDev(const int index_ = ConfigPhyDevDefault);

  std::string
  GetPhyName(const std::string& name_ = ConfigPhyNameDefault) const;

  bool
  SetPhyName(const std::string& name_ = ConfigPhyNameDefault);

  ConfigData::OPMODE
  GetOpMode(const ConfigData::OPMODE mode_ = OPMODE_DEF) const;

  bool
  SetOpMode(const ConfigData::OPMODE mode_ = OPMODE_DEF);

  std::string
  GetSsid(const std::string& ssid_ = ConfigSsidDefault) const;

  bool
  SetSsid(const std::string& ssid_ = ConfigSsidDefault);

  std::string
  GetBssid(const std::string& bssid_ = ConfigBssidDefault) const;

  bool
  SetBssid(const std::string& bssid_ = ConfigBssidDefault);

  ConfigData::HTMODE
  GetHtMode(const ConfigData::HTMODE mode_ = HTMODE_DEF) const;

  bool
  SetHtMode(const ConfigData::HTMODE mode_ = HTMODE_DEF);

  unsigned int
  GetFrequency(const unsigned int freq_ = ConfigFrequencyDefault) const;

  bool
  SetFrequency(const unsigned int freq_ = ConfigFrequencyDefault);

  unsigned int
  GetCenterFrequency1(const unsigned int freq_ = ConfigCenterFrequency1Default) const;

  bool
  SetCenterFrequency1(const unsigned int freq_ = ConfigCenterFrequency1Default);

  unsigned int
  GetCenterFrequency2(const unsigned int freq_ = ConfigCenterFrequency2Default) const;

  bool
  SetCenterFrequency2(const unsigned int freq_ = ConfigCenterFrequency2Default);

  unsigned int
  GetTxPower(const unsigned int power_ = ConfigTxPowerDefault) const;

  bool
  SetTxPower(const unsigned int power_ = ConfigTxPowerDefault);

protected:

private:

  SHARED_PTR(zConfig::ConfigData) _data;

};

// ****************************************************************************
// Class: zWireless::Capabilities
// ****************************************************************************

class Capabilities
{

public:

  enum BAND
  {
    BAND_ERR = -1,
    BAND_2_4 = 0,
    BAND_5 = 1,
    BAND_60 = 2,
    BAND_LAST
  };

  Capabilities();

  virtual
  ~Capabilities();

  bool
  IsValid() const;

  bool
  SetValid();

  bool
  ClrValid();

  ieee80211::PowerCapsTag::power_caps_tag
  GetPowerCaps() const;

  bool
  SetPowerCaps(const ieee80211::PowerCapsTag::power_caps_tag& caps_);

  ieee80211::HtCapsTag::ht_caps
  GetHtCaps() const;

  bool
  SetHtCaps(const ieee80211::HtCapsTag::ht_caps& caps_);

  ieee80211::HtInfoTag::ht_info
  GetHtInfo() const;

  bool
  SetHtInfo(const ieee80211::HtInfoTag::ht_info& info_);

  std::vector<uint8_t>
  GetChannels() const;

  bool
  SetChannels(const std::vector<uint8_t>& channels_);

  std::vector<uint8_t>
  GetBitRates() const;

  bool
  SetBitRates(const std::vector<uint8_t>& rates_);

  std::vector<uint8_t>
  GetExtBitRates() const;

  bool
  SetExtBitRates(const std::vector<uint8_t>& rates_);

  std::vector<uint8_t>
  GetMcsRates() const;

  bool
  SetMcsRates(const std::vector<uint8_t>& rates_);

  void
  Display() const;

protected:

private:

  bool _valid;
  ieee80211::PowerCapsTag::power_caps_tag _power_caps;
  ieee80211::HtCapsTag::ht_caps _ht_caps;
  ieee80211::HtInfoTag::ht_info _ht_info;
  std::vector <uint8_t> _channels;
  std::vector <uint8_t> _rates;
  std::vector <uint8_t> _ext_rates;
  std::vector <uint8_t> _mcs_rates;

};

// ****************************************************************************
// Class: zWireless::Interface
// ****************************************************************************

class Interface : public zInterface::Interface
{
public:

  Interface(const std::string& name_);

  virtual
  ~Interface();

  zWireless::ConfigData
  GetConfig() const;

  bool
  SetConfig(zWireless::ConfigData config_);

  int
  GetPhyIndex() const;

  bool
  SetPhyIndex(const int index_);

  int
  GetPhyDev() const;

  bool
  SetPhyDev(const int dev_);

  std::string
  GetPhyName() const;

  bool
  SetPhyName(const std::string& name_);

  ConfigData::OPMODE
  GetOpMode() const;

  bool
  SetOpMode(const ConfigData::OPMODE mode_);

  ConfigData::HTMODE
  GetHtMode() const;

  bool
  SetHtMode(const ConfigData::HTMODE mode_);

  unsigned int
  GetChannel() const;

  bool
  SetChannel(const unsigned int channel_);

  unsigned int
  GetFrequency() const;

  bool
  SetFrequency(const unsigned int freq_);

  unsigned int
  GetCenterFrequency1() const;

  bool
  SetCenterFrequency1(const unsigned int freq_);

  unsigned int
  GetCenterFrequency2() const;

  bool
  SetCenterFrequency2(const unsigned int freq_);

  unsigned int
  GetTxPower() const;

  bool
  SetTxPower(unsigned int txpower_);

  std::map<int, Capabilities>
  GetCapabilities() const;

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

  zWireless::ConfigData stagingConfig;
  zWireless::ConfigData workingConfig;

  static uint16_t
  freq2chan(const uint16_t freq_);

  static uint16_t
  chan2freq(const uint16_t chan_);

  static ConfigData::HTMODE
  nl2htmode(const uint32_t nl_);

  static uint32_t
  htmode2nl(const ConfigData::HTMODE mode_);

  static std::string
  htmode2str(ConfigData::HTMODE mode_);

  static ConfigData::OPMODE
  nl2opmode(const uint32_t iftype_);

  static uint32_t
  opmode2nl(const ConfigData::OPMODE mode_);

  static std::string
  opmode2str(const ConfigData::OPMODE mode_);

private:

  int
  _getPhyIndex() const;

  bool
  _setPhyIndex();

  int
  _getPhyDev() const;

  bool
  _setPhyDev();

  std::string
  _getPhyName() const;

  bool
  _setPhyName();

  ConfigData::OPMODE
  _getOpMode() const;

  bool
  _setOpMode();

  ConfigData::HTMODE
  _getHtMode() const;

  unsigned int
  _getFrequency() const;

  unsigned int
  _getCenterFrequency1() const;

  unsigned int
  _getCenterFrequency2() const;

  bool
  _setChannel();

  unsigned int
  _getTxPower() const;

  bool
  _setTxPower();

  std::map<int, Capabilities>
  _getCapabilities() const;

};

}
}

#endif /* __ZWIRELESS_H__ */
