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

#ifndef __NL80211_CHANNELTYPEATTRIBUTE_H__
#define __NL80211_CHANNELTYPEATTRIBUTE_H__

// libc includes
#include <stdint.h>
#include <linux/nl80211.h>

// libc++ includes

// libzutils includes
#include <zutils/netlink/Attribute.h>

// local includes

/**
 * enum nl80211_channel_type - channel type
 * @NL80211_CHAN_NO_HT: 20 MHz, non-HT channel
 * @NL80211_CHAN_HT20: 20 MHz HT channel
 * @NL80211_CHAN_HT40MINUS: HT40 channel, secondary channel
 *  below the control channel
 * @NL80211_CHAN_HT40PLUS: HT40 channel, secondary channel
 *  above the control channel
 */
//enum nl80211_channel_type {
//    NL80211_CHAN_NO_HT,
//    NL80211_CHAN_HT20,
//    NL80211_CHAN_HT40MINUS,
//    NL80211_CHAN_HT40PLUS
//};

namespace nl80211
{

//*****************************************************************************
// Class: ChannelTypeAttribute
//*****************************************************************************

class ChannelTypeAttribute : public Attribute
{

public:

  ChannelTypeAttribute() :
    Attribute(NL80211_ATTR_WIPHY_CHANNEL_TYPE)
  {
  }

  virtual
  ~ChannelTypeAttribute()
  {
  }

  uint32_t
  operator()() const
  {
    return (this->Get<uint32_t>());
  }

  bool
  operator()(const uint32_t interval_)
  {
    return (this->Set(interval_));
  }

  std::string
  ToString() const
  {
    std::string str;
    switch(this->operator()())
    {
    case NL80211_CHAN_NO_HT:
      str += "NL80211_CHAN_NO_HT";
      break;
    case NL80211_CHAN_HT20:
      str += "NL80211_CHAN_HT20";
      break;
    case NL80211_CHAN_HT40MINUS:
      str += "NL80211_CHAN_HT40MINUS";
      break;
    case NL80211_CHAN_HT40PLUS:
      str += "NL80211_CHAN_HT40PLUS";
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

#endif /* __NL80211_CHANNELTYPEATTRIBUTE_H__ */
