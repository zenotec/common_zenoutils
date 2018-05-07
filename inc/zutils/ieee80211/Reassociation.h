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

#ifndef __IEEE80211_REASSOCIATION_H__
#define __IEEE80211_REASSOCIATION_H__

// libc includes

// libc++ includes

// libzutils includes
#include <zutils/ieee80211/Frame.h>
#include <zutils/ieee80211/ManagementFrame.h>
#include <zutils/ieee80211/SsidTag.h>
#include <zutils/ieee80211/RatesTag.h>
#include <zutils/ieee80211/DsssTag.h>
#include <zutils/ieee80211/TimTag.h>
#include <zutils/ieee80211/CountryTag.h>
#include <zutils/ieee80211/ErpInfoTag.h>
#include <zutils/ieee80211/HtCapsTag.h>
#include <zutils/ieee80211/SuppOpClassesTag.h>
#include <zutils/ieee80211/HtInfoTag.h>
#include <zutils/ieee80211/ExtRatesTag.h>
#include <zutils/ieee80211/ExtCapsTag.h>
#include <zutils/ieee80211/WmmWmeTag.h>
#include <zutils/ieee80211/ChannelsTag.h>
#include <zutils/ieee80211/PowerCapsTag.h>

// local includes

namespace zUtils
{
namespace zWireless
{
namespace ieee80211
{

//*****************************************************************************
// Class: ReassociationRequest
//*****************************************************************************

class ReassociationRequest : public ManagementFrame
{

public:
  //ORDER MATTERS - Declare in the order they are rendered
  SsidTag Ssid;
  RatesTag Rates;
  ExtRatesTag ExtRates;
  PowerCapsTag PowerCaps;
  ChannelsTag Channels;
  HtCapsTag HtCaps;
  WmmWmeTag WmmWme;

  ReassociationRequest();

  virtual
  ~ReassociationRequest();

  virtual uint8_t*
  Assemble(uint8_t* p_, size_t& len_, bool fcs_ = false);

  virtual uint8_t*
  Disassemble(uint8_t* p_, size_t& len_, bool fcs_ = false);

  uint16_t
  Capabilities() const;

  bool
  Capabilities(const uint16_t capabilities_);

  uint16_t
  Interval() const;

  bool
  Interval(const uint16_t int_);

  uint64_t
  CurrentApMac() const;

  bool
  CurrentApMac(const uint64_t mac_);

  virtual void
  Display() const;

protected:

private:
  uint16_t _capabilities;
  uint16_t _interval;
  uint64_t _currentApMac;

};

//*****************************************************************************
// Class: ReassociationResponse
//*****************************************************************************

class ReassociationResponse : public ManagementFrame
{

public:
  //ORDER MATTERS - Declare in the order they are rendered
  RatesTag Rates;
  ExtRatesTag ExtRates;
  HtCapsTag HtCaps;
  HtInfoTag HtInfo;
  ExtCapsTag ExtCaps;
  WmmWmeTag WmmWme;

  ReassociationResponse();

  virtual
  ~ReassociationResponse();

  virtual uint8_t*
  Assemble(uint8_t* p_, size_t& len_, bool fcs_ = false);

  virtual uint8_t*
  Disassemble(uint8_t* p_, size_t& len_, bool fcs_ = false);

  uint16_t
  Capabilities() const;

  bool
  Capabilities(const uint16_t capabilities_);

  uint16_t
  Status() const;

  bool
  Status(const uint16_t status_);

  uint16_t
  ReassociationIdentifier() const;

  bool
  ReassociationIdentifier(const uint16_t status_);

  virtual void
  Display() const;

protected:

private:

  uint16_t _capabilities;
  uint16_t _status;
  uint16_t _aid;

};

}
}
}

#endif /* __IEEE80211_REASSOCIATION_H__ */
