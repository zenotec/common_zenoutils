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

#ifndef __NL80211_FREQATTRIBUTE_H__
#define __NL80211_FREQATTRIBUTE_H__

// libc includes
#include <stdint.h>
#include <linux/nl80211.h>

// libc++ includes

// libzutils includes
#include <zutils/netlink/Attribute.h>

// local includes

namespace nl80211
{

static uint16_t
_freq2chan(uint16_t freq_)
{

  uint16_t channel = 0;
  // Channels 1 - 13
  if ((freq_ >= 2412) && (freq_ <= 2472))
  {
    channel = (1 + ((freq_ - 2412) / 5));
  }
  // Channels 36 - 64
  else if ((freq_ >= 5170) && (freq_ <= 5320))
  {
    channel = (34 + ((freq_ - 5170) / 5));
  }
  // Channels 100 - 144
  else if ((freq_ >= 5500) && (freq_ <= 5720))
  {
    channel = (100 + ((freq_ - 5500) / 5));
  }
  // Channels 149 - 161
  else if ((freq_ >= 5745) && (freq_ <= 5805))
  {
    channel = (149 + ((freq_ - 5745) / 5));
  }
  // Channel 165
  else if (freq_ == 5825)
  {
    channel = 165;
  }

  return (channel);

}

static uint16_t
_chan2freq(uint16_t chan_)
{
  uint16_t freq = 0;
  if ((chan_ >= 1) && (chan_ <=13))
  {
    freq = (((chan_ - 1) * 5) + 2412);
  }
  else if ((chan_ >= 36) && (chan_ <= 64))
  {
    freq = (((chan_ - 34) * 5) + 5170);
  }
  else if ((chan_ >= 100) && (chan_ <= 144))
  {
    freq = (((chan_ - 100) * 5) + 5500);
  }
  else if ((chan_ >= 149) && (chan_ <= 161))
  {
    freq = (((chan_ - 149) * 5) + 5745);
  }
  else if (chan_ == 165)
  {
    freq = 5825;
  }
  return (freq);
}

//*****************************************************************************
// Class: FrequencyAttribute
//*****************************************************************************

class FrequencyAttribute : public AttributeValue
{

public:

  FrequencyAttribute() :
    AttributeValue(NL80211_ATTR_WIPHY_FREQ)
  {
  }

  virtual
  ~FrequencyAttribute()
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

  uint32_t
  GetChannel() const
  {
    return(_freq2chan(this->operator()()));
  }

  bool
  SetChannel(const uint32_t channel_)
  {
    return(this->operator()(_chan2freq(channel_)));
  }

protected:

private:

};

}

#endif /* __NL80211_FREQATTRIBUTE_H__ */
