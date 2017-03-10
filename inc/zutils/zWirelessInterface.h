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

namespace zUtils
{
namespace zInterface
{

// ****************************************************************************
// Class: WirelessConfigPath
// ****************************************************************************

class WirelessConfigPath : public InterfaceConfigPath
{

public:

  static const std::string ConfigPhyNamePath;
  static const std::string ConfigBssidPath;
  static const std::string ConfigEssidPath;
  static const std::string ConfigSsidPath;
  static const std::string ConfigChannelPath;
  static const std::string ConfigTxPowerPath;

  WirelessConfigPath();

  virtual
  ~WirelessConfigPath();

  zConfig::ConfigPath
  PhyName() const;

  zConfig::ConfigPath
  Bssid() const;

  zConfig::ConfigPath
  Essid() const;

  zConfig::ConfigPath
  Ssid() const;

  zConfig::ConfigPath
  Channel() const;

  zConfig::ConfigPath
  TxPower() const;

protected:

private:

};

// ****************************************************************************
// Class: WirelessInterface
// ****************************************************************************

class WirelessInterface : public Interface
{
public:

  static const std::string ConfigPhyNameDefault;
  static const std::string ConfigBssidDefault;
  static const std::string ConfigEssidDefault;
  static const std::string ConfigSsidDefault;
  static const float ConfigChannelDefault;
  static const int ConfigTxPowerDefault;

  WirelessInterface(const std::string &name_);

  WirelessInterface(const InterfaceConfigData& config_);

  virtual
  ~WirelessInterface();

  virtual bool
  Refresh();

  std::string
  IwName();

  std::string
  GetPhyName() const;

  bool
  SetPhyName(const std::string& phy_);

  std::string
  GetBssid() const;

  bool
  SetBssid(const std::string& bssid_);

  std::string
  GetEssid() const;

  bool
  SetEssid(const std::string& essid_);

  std::string
  GetSsid() const;

  bool
  SetSsid(const std::string& ssid_);

  float
  GetChannel() const;

  bool
  SetChannel(const float chnl_);

  int
  GetTxPower() const;

  bool
  SetTxPower(int txpow_);

  bool
  IsAssociated();

  bool
  Associate(const std::string &essid_, const unsigned int channel_);

  bool
  Disassociate();

  int
  LinkQuality();

  int
  SignalLevel();

  int
  NoiseLevel();

  int
  BitRate();

  virtual void
  Display(const std::string &prefix_);

protected:

  int _link_quality;
  int _signal_level;
  int _noise_level;
  int _bit_rate;

  virtual int
  _get_link_quality();

  virtual int
  _get_signal_level();

  virtual int
  _get_noise_level();

  virtual int
  _get_bit_rate();

  virtual float
  _get_channel();

  virtual bool
  _set_channel(float chnl_);

  virtual int
  _get_tx_power();

  virtual bool
  _set_tx_power(int txpow_);

private:

  zSem::Mutex _lock;
  std::string _iw_name;
  bool _associated;

};

}
}
#endif /* __ZWIRELESSINTERFACE_H__ */
