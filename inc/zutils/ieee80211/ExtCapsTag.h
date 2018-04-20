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

#ifndef __IEEE80211_EXTENDEDCAPSTAG_H__
#define __IEEE80211_EXTENDEDCAPSTAG_H__

#include <zutils/ieee80211/Tag.h>

namespace zUtils
{
namespace zWireless
{
namespace ieee80211
{

//*****************************************************************************
// Class: ExtCapsTag
//*****************************************************************************

class ExtCapsTag : public Tag
{

public:

  //	The the first 4 bits indicate which byte, the second which bit in the byte
  enum EXTCAPSBIT 
  { 
    EXCAP_20_40_BSS_COEXISTENCE_MANAGEMENT_SUPPORT = 0x00,
    EXCAP_ON_DEMAND_BEACON = 0x01,
    EXCAP_EXTENDED_CHANNEL_SWITCHING = 0x02,
    EXCAP_WAVE_INDICATION = 0x03,
    EXCAP_PSMP_CAPABILITY = 0x04,
    EXCAP_S_PSMP_SUPPORT = 0x06,
    EXCAP_EVENT = 0x07,
    EXCAP_DIAGNOSTICS = 0x10,
    EXCAP_MULTICAST_DIAGNOSTICS = 0x11,
    EXCAP_LOCATION_TRACKING = 0x12,
    EXCAP_FMS = 0x13,
    EXCAP_PROXY_ARP_SERVICE = 0x14,
    EXCAP_COLLOCATED_INTERFERENCE_REPORTING = 0x15,
    EXCAP_CIVIC_LOCATION = 0x16,
    EXCAP_GEOSPATIAL_LOCATION = 0x17,
    EXCAP_TFS = 0x20,
    EXCAP_WNM_SLEEP_MODE = 0x21,
    EXCAP_TIM_BROADCAST = 0x22,
    EXCAP_BSS_TRANSITION = 0x23,
    EXCAP_QOS_TRAFFIC_CAPABILITY = 0x24,
    EXCAP_AC_STATION_COUNT = 0x25,
    EXCAP_MULTIPLE_BSSID = 0x26,
    EXCAP_TIMING_MEASUREMENT = 0x27,
    EXCAP_CHANNEL_USAGE = 0x30,
    EXCAP_SSID_LIST = 0x31,
    EXCAP_DMS = 0x32,
    EXCAP_UTC_TSF_OFFSET = 0x33,
    EXCAP_PEER_U_APSD_BUFFER_STA_SUPPORT = 0x34,
    EXCAP_TDLS_PEER_PSM_SUPPORT = 0x35,
    EXCAP_TDLS_CHANNEL_SWITCHING = 0x36,
    EXCAP_INTERWORKING = 0x37,
    EXCAP_QOS_MAP = 0x40,
    EXCAP_EBR = 0x41,
    EXCAP_SSPN_INTERFACE = 0x42,
    EXCAP_MSGCF_CAPABILITY = 0x44,
    EXCAP_TDLS_SUPPORT = 0x45,
    EXCAP_TDLS_PROHIBITED = 0x46,
    EXCAP_TDLS_CHANNEL_SWITCHING_PROHIBITED = 0x47,
    EXCAP_REJECT_UNADMITTED_FRAME = 0x50,
    EXCAP_IDENTIFIER_LOCATION = 0x04,
    EXCAP_U_APSD_COEXISTENCE = 0x05,
    EXCAP_WNM_NOTIFICATION = 0x06,
    EXCAP_QAB_CAPABILITY = 0x07,
    EXCAP_UTF_8_SSID = 0x60,
    EXCAP_TDLS_WIDER_BANDWIDTH = 0x75,
    EXCAP_OPERATING_MODE_NOTIFICATION = 0x76,
    EXCAP_MAX_NUMBER_OF_MSDUS_IN_A_MSDU = 0x77
  };

  struct ext_caps_tag
  {
    uint8_t caps[8];
  };

  ExtCapsTag() :
    Tag(Tag::ID_EXT_CAPS, sizeof(struct ext_caps_tag))
  {
  }

  virtual
  ~ExtCapsTag()
  {
  }

  struct ext_caps_tag
  operator()() const
  {
    struct ext_caps_tag val = { 0 };
    this->GetValue(val);
    return(val);
  }

  bool
  operator()(const struct ext_caps_tag caps_)
  {
    return(this->PutValue(caps_));
  }

  virtual void
  Display() const
  {
    Tag::Display();
  }

  bool
  GetFlag(const ExtCapsTag::EXTCAPSBIT bit_)
  {
      struct ext_caps_tag val = { 0 };
      this->GetValue(val);
      return( val.caps[(bit_ >> 4)] >> (bit_ & 0xf) );
  }

  bool
  SetFlag(const ExtCapsTag::EXTCAPSBIT bit_)
  {
    struct ext_caps_tag val = { 0 };
    this->GetValue(val);
    val.caps[(bit_ >> 4)] |= (1 << (bit_ & 0xf));
    return(this->PutValue(val));
  }

  bool
  ClearFlag(const ExtCapsTag::EXTCAPSBIT bit_)
  {
    struct ext_caps_tag val = { 0 };
    this->GetValue(val);
    val.caps[(bit_ >> 4)] &= ~(1 << (bit_ & 0xf));
    return(this->PutValue(val));
  }

protected:

private:

};

}
}
}

#endif /* __IEEE80211_EXTENDEDCAPSTAG_H__ */
