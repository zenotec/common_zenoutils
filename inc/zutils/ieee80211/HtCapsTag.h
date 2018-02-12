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

  HtCapsTag(const Tag::ID id_ = ID_HT_CAPS, const size_t len_ = 0) :
    Tag(id_)
  {
  }

  virtual
  ~HtCapsTag()
  {
  }

  virtual std::vector<uint8_t>
  operator()() const
  {
    std::vector<uint8_t> caps;
    caps.resize(this->Length());
    this->GetValue(caps.data(), caps.size());
    return(caps);
  }

  virtual bool
  operator()(const std::vector<uint8_t>& caps_)
  {
    return(this->PutValue(caps_.data(), caps_.size()));
  }

  bool
  operator()(ht_caps& caps_)
  {
    return(this->PutValue<ht_caps>(caps_));
  }

  ht_caps
  HtCaps() const
  {
    ht_caps caps;
    GetValue<ht_caps>(caps);
    return caps;
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
