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

  struct rx_mcs
  {
    uint64_t rx_mcs_bitmask;
    uint16_t rx_highest_data_rate;
    uint8_t rx_tx_mcs;

  } __attribute__ ((packed));

  struct ht_info
  {
    uint8_t ht_primary_channel;
    uint8_t ht_subset_1;
    uint16_t ht_subset_2;
    uint16_t ht_subset_3;
	struct rx_mcs ht_rx_mcs;
  } __attribute__ ((packed));

  HtInfoTag(const Tag::ID id_ = ID_HT_INFORMATION, const size_t len_ = 0) :
    Tag(id_)
  {
  }

  virtual
  ~HtInfoTag()
  {
  }

  virtual std::vector<uint8_t>
  operator()() const
  {
    std::vector<uint8_t> info;
    info.resize(this->Length());
    this->GetValue(info.data(), info.size());
    return(info);
  }

  virtual bool
  operator()(const std::vector<uint8_t>& info_)
  {
    return(this->PutValue(info_.data(), info_.size()));
  }

  bool
  operator()(ht_info& info_)
  {
    return(this->PutValue<ht_info>(info_));
  }

  ht_info
  HtInfo() const
  {
    ht_info info;
    GetValue<ht_info>(info);
    return info;
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
