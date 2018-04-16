/*
 * Copyright (c) 2017 Cable Television Laboratories, Inc. ("CableLabs")
 *                    and others.  All rights reserved.
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

#ifndef __IEEE80211_BEACON_PARAMETERS_H__
#define __IEEE80211_BEACON_PARAMETERS_H__

// libc includes

// libc++ includes

// libzutils includes
//#include <zutils/ieee80211/Frame.h>
//#include <zutils/ieee80211/ManagementFrame.h>
#include <zutils/ieee80211/SsidTag.h>
#include <zutils/ieee80211/RatesTag.h>
#include <zutils/ieee80211/DsssTag.h>
#include <zutils/ieee80211/TimTag.h>
#include <zutils/ieee80211/CountryTag.h>
#include <zutils/ieee80211/PowerCapsTag.h>
#include <zutils/ieee80211/HtCapsTag.h>
#include <zutils/ieee80211/ExtRatesTag.h>
#include <zutils/ieee80211/WmmWmeTag.h>

// local includes

namespace zUtils
{
namespace zWireless
{
namespace ieee80211
{

class BeaconParameters
{

public:

  SsidTag Ssid;
  RatesTag Rates;
  DsssTag Dsss;
  TimTag Tim;
  CountryTag Country;
  PowerCapsTag PowerCaps;
  HtCapsTag HtCaps;
  ExtRatesTag ExtRates;
//  WmmWmeTag WmmWme;

  BeaconParameters();
  
  virtual
  ~BeaconParameters();

};



}
}
}

#endif /* __IEEE80211_BEACON_PARAMETERS_H__ */
