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
#include <zutils/ieee80211/SsidTag.h>
#include <zutils/ieee80211/RatesTag.h>
#include <zutils/ieee80211/DsssTag.h>
#include <zutils/ieee80211/TimTag.h>
#include <zutils/ieee80211/CountryTag.h>
#include <zutils/ieee80211/PowerCapsTag.h>
#include <zutils/ieee80211/ErpInfoTag.h>
#include <zutils/ieee80211/HtCapsTag.h>
#include <zutils/ieee80211/ExtRatesTag.h>
#include <zutils/ieee80211/ExtCapsTag.h>
#include <zutils/ieee80211/HtInfoTag.h>
#include <zutils/ieee80211/WmmWmeTag.h>

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

class BasicServiceSet : public zWireless::ConfigData
{

public:

  ieee80211::RatesTag Rates;
  ieee80211::DsssTag Dsss;
  ieee80211::TimTag Tim;
  ieee80211::CountryTag Country;
  ieee80211::PowerCapsTag PowerCaps;
  ieee80211::ErpInfoTag ErpInfo;
  ieee80211::HtCapsTag HtCaps;
  ieee80211::SuppOpClassesTag SuppOpClass;
  ieee80211::HtInfoTag HtInfo;
  ieee80211::ExtRatesTag ExtRates;
  ieee80211::ExtCapsTag ExtCaps;
  ieee80211::WmmWmeTag WmmWme;

  BasicServiceSet(const std::string& ifname_, const std::string &ssid_);

  virtual
  ~BasicServiceSet();

  bool
  Start();

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

  AccessPointInterface _iface;
  ieee80211::Beacon _beacon;
  ieee80211::ProbeResponse _probe;

  void
  _update_beacon();

  void
  _update_probe();

};

}
}
#endif /* __ZBASICSERVICESET_H__ */
