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

#ifndef __IEEE80211_ASSOCIATION_H__
#define __IEEE80211_ASSOCIATION_H__

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
// Class: AssociationRequest
//*****************************************************************************

class AssociationRequest : public ManagementFrame
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

  AssociationRequest();

  virtual
  ~AssociationRequest();

  virtual bool
  Assemble(zSocket::Buffer& sb_, bool fcs_e);

  virtual bool
  Disassemble(zSocket::Buffer& sb_, bool fcs_);

  uint16_t
  Capabilities() const;

  bool
  Capabilities(const uint16_t capabilities_);

  uint16_t
  Interval() const;

  bool
  Interval(const uint16_t int_);

  virtual void
  Display() const;

protected:

private:
  uint16_t _capabilities;
  uint16_t _interval;

};

//*****************************************************************************
// Class: AssociationResponse
//*****************************************************************************

class AssociationResponse : public ManagementFrame
{

public:
  //ORDER MATTERS - Declare in the order they are rendered
  RatesTag Rates;
  ExtRatesTag ExtRates;
  HtCapsTag HtCaps;
  HtInfoTag HtInfo;
  ExtCapsTag ExtCaps;
  WmmWmeTag WmmWme;

  AssociationResponse();

  virtual
  ~AssociationResponse();

  virtual bool
  Assemble(zSocket::Buffer& sb_, bool fcs_);

  virtual bool
  Disassemble(zSocket::Buffer& sb_, bool fcs_);

  uint16_t
  Capabilities() const;

  bool
  Capabilities(const uint16_t capabilities_);

  uint16_t
  Status() const;

  bool
  Status(const uint16_t status_);

  uint16_t
  AssociationIdentifier() const;

  bool
  AssociationIdentifier(const uint16_t status_);

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

#endif /* __IEEE80211_ASSOCIATION_H__ */
