/*
 * Copyright (c) 2017 Cable Television Laboratories, Inc. ("CableLabs")
 *                    and others.  All rights reserved.
 *
 * Copyright (c) 2014-2017 ZenoTec LLC (http://www.zenotec.net)
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

#ifndef __ZBASICSERVICESET_H__
#define __ZBASICSERVICESET_H__

#include <string>

#include <zutils/zAccessPointInterface.h>
#include <zutils/ieee80211/Beacon.h>
#include <zutils/ieee80211/Probe.h>
#include <zutils/ieee80211/Association.h>

namespace zUtils
{

namespace zWireless
{

namespace ieee80211
{
  class Beacon;
}

// ****************************************************************************
// Class: BasicServiceSet
// ****************************************************************************

class BasicServiceSet
{

public:

  BasicServiceSet(const std::string& iface_);

  virtual
  ~BasicServiceSet();

  std::string
  GetSsid() const;

  bool
  SetSsid(const std::string& ssid_);

  std::string
  GetBssid() const;

  bool
  SetBssid(const std::string& bssid_);

  ConfigData::HTMODE
  GetHtMode() const;

  bool
  SetHtMode(const ConfigData::HTMODE mode_);

  unsigned int
  GetChannel() const;

  bool
  SetChannel(const unsigned int channel_);

  bool
  AnnounceChannelSwitch(const unsigned int mode_, const unsigned int channel_, const unsigned int count_);

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

  ieee80211::Beacon&
  Beacon();

  ieee80211::ProbeResponse&
  ProbeResponse();

  ieee80211::AssociationResponse&
  AssociationResponse();

  bool
  Start();

  bool
  Update();

  bool
  Stop();

  bool
  AddStation(const std::string& addr_);

  bool
  DelStation(const std::string& addr_);

  virtual void
  Display(const std::string &prefix_ = std::string(""));

protected:

private:

  zWireless::AccessPointInterface _iface;
  zWireless::ConfigData _config;
  ieee80211::Beacon _beacon;
  ieee80211::ProbeResponse _probe;
  ieee80211::AssociationResponse _assoc;

  void
  _init_beacon();

  void
  _init_probe();

};

}
}
#endif /* __ZBASICSERVICESET_H__ */
