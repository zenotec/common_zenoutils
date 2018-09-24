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

#ifndef __IEEE80211_HTCAPSTAG_H__
#define __IEEE80211_HTCAPSTAG_H__
#include <array>
using namespace std;

#include <zutils/ieee80211/Tag.h>

namespace zUtils
{
namespace zWireless
{
namespace ieee80211
{

//*****************************************************************************
// Class: HtCapsTag
//*****************************************************************************

class HtCapsTag : public Tag
{

public:

  struct tx_mcs
  {
    uint8_t tx_bits;
    uint16_t reserved1;
    uint8_t reserved2;
  } __attribute__ ((packed));

  struct mcs_set
  {
    std::array<uint8_t,10> rx_mcs_bitmask;
    uint16_t rx_highest_rate;
    tx_mcs tx_mcs_fields;
  } __attribute__ ((packed));

  struct ht_caps
  {
    uint16_t ht_cap_info;
    uint8_t ampdu_parms;
    mcs_set supported_mcs_set;
    uint16_t ht_ext_cap;
    uint32_t trans_beam_cap;
    uint8_t asel_cap;
  } __attribute__ ((packed));

  HtCapsTag() :
    Tag(Tag::ID_HT_CAPS, sizeof(struct ht_caps))
  {
  }

  virtual
  ~HtCapsTag()
  {
  }

  struct ht_caps
  operator()() const
  {
    ht_caps caps;
    this->GetValue(caps);
    return(caps);
  }

  virtual bool
  operator()(const struct ht_caps& caps_)
  {
    return(this->PutValue(caps_));
  }

  virtual HtCapsTag&
  operator=(const struct ht_caps& caps_)
  {
    this->PutValue(caps_);
    return(*this);
  }

  uint16_t
  HtCapInfo() const
  {
    return (this->operator ()().ht_cap_info);
  }

  bool
  HtCapInfo(const uint16_t HtCapInfo_)
  {
    struct ht_caps caps = this->operator ()();
    caps.ht_cap_info = HtCapInfo_;
    return (this->operator ()(caps));
  }

  uint8_t
  HtAmpduParms() const
  {
    return (this->operator ()().ampdu_parms);
  }

  bool
  HtAmpduParms(const uint8_t HtAmpduParms_)
  {
    struct ht_caps caps = this->operator ()();
    caps.ampdu_parms = HtAmpduParms_;
    return (this->operator ()(caps));
  }

  mcs_set
  HtSupportedMcsSet() const
  {
    return (this->operator ()().supported_mcs_set);
  }

  bool
  HtSupportedMcsSet(const mcs_set & HtSupportedMcsSet_)
  {
    struct ht_caps caps = this->operator ()();
    caps.supported_mcs_set = HtSupportedMcsSet_;
    return (this->operator ()(caps));
  }

  uint16_t
  HtExtCaps() const
  {
    return (this->operator ()().ht_ext_cap);
  }

  bool
  HtExtCaps(const uint16_t HtExtCaps_)
  {
    struct ht_caps caps = this->operator ()();
    caps.ht_ext_cap = HtExtCaps_;
    return (this->operator ()(caps));
  }

  uint32_t
  HtTransBeamCap() const
  {
    return (this->operator ()().trans_beam_cap);
  }

  bool
  HtAselCap(const uint32_t HtAselCap_)
  {
    struct ht_caps caps = this->operator ()();
    caps.asel_cap = HtAselCap_;
    return (this->operator ()(caps));
  }

  uint32_t
  HtAselCap() const
  {
    return (this->operator ()().trans_beam_cap);
  }

  bool
  HtTransBeamCap(const uint32_t HtTransBeamCap_)
  {
    struct ht_caps caps = this->operator ()();
    caps.asel_cap = HtTransBeamCap_;
    return (this->operator ()(caps));
  }

  virtual void
  Display() const;

protected:

private:

};

}
}
}

#endif /* __IEEE80211_HTCAPSTAG_H__ */
