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

#ifndef __NL80211_CHANNELWIDTHATTRIBUTE_H__
#define __NL80211_CHANNELWIDTHATTRIBUTE_H__

// libc includes
#include <stdint.h>
#include <linux/nl80211.h>

// libc++ includes

// libzutils includes
#include <zutils/netlink/Attribute.h>

// local includes

/**
 * enum nl80211_chan_width - channel width definitions
 *
 * These values are used with the %NL80211_ATTR_CHANNEL_WIDTH
 * attribute.
 *
 * @NL80211_CHAN_WIDTH_20_NOHT: 20 MHz, non-HT channel
 * @NL80211_CHAN_WIDTH_20: 20 MHz HT channel
 * @NL80211_CHAN_WIDTH_40: 40 MHz channel, the %NL80211_ATTR_CENTER_FREQ1
 *  attribute must be provided as well
 * @NL80211_CHAN_WIDTH_80: 80 MHz channel, the %NL80211_ATTR_CENTER_FREQ1
 *  attribute must be provided as well
 * @NL80211_CHAN_WIDTH_80P80: 80+80 MHz channel, the %NL80211_ATTR_CENTER_FREQ1
 *  and %NL80211_ATTR_CENTER_FREQ2 attributes must be provided as well
 * @NL80211_CHAN_WIDTH_160: 160 MHz channel, the %NL80211_ATTR_CENTER_FREQ1
 *  attribute must be provided as well
 * @NL80211_CHAN_WIDTH_5: 5 MHz OFDM channel
 * @NL80211_CHAN_WIDTH_10: 10 MHz OFDM channel
 */
//enum nl80211_chan_width {
//    NL80211_CHAN_WIDTH_20_NOHT,
//    NL80211_CHAN_WIDTH_20,
//    NL80211_CHAN_WIDTH_40,
//    NL80211_CHAN_WIDTH_80,
//    NL80211_CHAN_WIDTH_80P80,
//    NL80211_CHAN_WIDTH_160,
//    NL80211_CHAN_WIDTH_5,
//    NL80211_CHAN_WIDTH_10,
//};

namespace nl80211
{

//*****************************************************************************
// Class: ChannelWidthAttribute
//*****************************************************************************

class ChannelWidthAttribute : public Attribute
{

public:

  ChannelWidthAttribute() :
      Attribute(TYPE_U32, NL80211_ATTR_CHANNEL_WIDTH)
  {
    this->SetValue(NL80211_CHAN_WIDTH_20_NOHT);
    this->ClrValid();
  }

  virtual
  ~ChannelWidthAttribute()
  {
  }

  std::string
  ToString() const
  {
    std::string str;
    uint32_t val = 0;
    this->GetValue(val);
    switch(val)
    {
    case NL80211_CHAN_WIDTH_20_NOHT:
      str += "NL80211_CHAN_WIDTH_20_NOHT";
      break;
    case NL80211_CHAN_WIDTH_20:
      str += "NL80211_CHAN_WIDTH_20";
      break;
    case NL80211_CHAN_WIDTH_40:
      str += "NL80211_CHAN_WIDTH_40";
      break;
    case NL80211_CHAN_WIDTH_80:
      str += "NL80211_CHAN_WIDTH_80";
      break;
    case NL80211_CHAN_WIDTH_80P80:
      str += "NL80211_CHAN_WIDTH_80P80";
      break;
    case NL80211_CHAN_WIDTH_160:
      str += "NL80211_CHAN_WIDTH_160";
      break;
    case NL80211_CHAN_WIDTH_5:
      str += "NL80211_CHAN_WIDTH_5";
      break;
    case NL80211_CHAN_WIDTH_10:
      str += "NL80211_CHAN_WIDTH_10";
      break;
    default:
      str += "unknown";
      break;
    }
    return(str);
  }

protected:

private:

};

}

#endif /* __NL80211_CHANNELWIDTHATTRIBUTE_H__ */
