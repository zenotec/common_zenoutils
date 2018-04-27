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

#ifndef __IEEE80211_HTINFOTAG_H__
#define __IEEE80211_HTINFOTAG_H__
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
// Class: HtInfoTag
//*****************************************************************************

class HtInfoTag : public Tag
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

  struct ht_info
  {
    uint8_t ht_primary_channel;
    uint8_t ht_subset_1;
    uint16_t ht_subset_2;
    uint16_t ht_subset_3;
	struct mcs_set ht_rx_mcs;
  } __attribute__ ((packed));

  HtInfoTag() :
    Tag(Tag::ID_HT_INFORMATION, sizeof(struct ht_info))
  {
  }

  virtual
  ~HtInfoTag()
  {
  }

  ht_info
  operator()() const
  {
    ht_info info;
    this->GetValue(info);
    return(info);
  }

  bool
  operator()(const ht_info& info_)
  {
    return(this->PutValue(info_));
  }

  uint8_t
  PrimaryChannel() const
  {
    return (this->operator ()().ht_primary_channel);
  }

  bool
  PrimaryChannel(const uint8_t PrimaryChannel_)
  {
    struct ht_info info = this->operator ()();
    info.ht_primary_channel = PrimaryChannel_;
    return (this->operator ()(info));
  }

  uint8_t
  HtSubset1() const
  {
    return (this->operator ()().ht_subset_1);
  }

  bool
  HtSubset1(const uint8_t HtSubset1_)
  {
    struct ht_info info = this->operator ()();
    info.ht_subset_1 = HtSubset1_;
    return (this->operator ()(info));
  }

  uint16_t
  HtSubset2() const
  {
    return (this->operator ()().ht_subset_2);
  }

  bool
  HtSubset2(const uint16_t HtSubset2_)
  {
    struct ht_info info = this->operator ()();
    info.ht_subset_2 = HtSubset2_;
    return (this->operator ()(info));
  }

  uint16_t
  HtSubset3() const
  {
    return (this->operator ()().ht_subset_3);
  }

  bool
  HtSubset3(const uint16_t HtSubset3_)
  {
    struct ht_info info = this->operator ()();
    info.ht_subset_3 = HtSubset3_;
    return (this->operator ()(info));
  }

  struct mcs_set
  RxMcs() const
  {
    return (this->operator ()().ht_rx_mcs);
  }

  bool
  RxMcs(const struct mcs_set mcs_)
  {
    struct ht_info info = this->operator ()();
    info.ht_rx_mcs = mcs_;
    return (this->operator ()(info));
  }

  virtual void
  Display() const;

protected:

private:

};

}
}
}

#endif /* __IEEE80211_HTINFOTAG_H__ */
