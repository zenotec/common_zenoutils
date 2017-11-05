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

// local includes

namespace nl80211
{

static uint16_t
_freq2chan(uint16_t freq)
{

  uint16_t channel = 0;
  if ((freq >= 2412) && (freq <= 2472))
  {
    channel = (1 + ((freq - 2412) / 5));
  }
  else if ((freq >= 5170) && (freq <= 5320))
  {
    channel = (34 + ((freq - 5170) / 5));
  }
  else if ((freq >= 5500) && (freq <= 5720))
  {
    channel = (100 + ((freq - 5500) / 5));
  }
  else if ((freq >= 5745) && (freq <= 5805))
  {
    channel = (149 + ((freq - 5745) / 5));
  }
  else if (freq == 5825)
  {
    channel = 165;
  }

  return (channel);

}

//*****************************************************************************
// Class: FrequencyAttribute
//*****************************************************************************

class FrequencyAttribute : public Attribute<uint32_t>
{

public:

  FrequencyAttribute() :
      Attribute(NL80211_ATTR_WIPHY_FREQ)
  {
    this->SetValue(0);
  }

  virtual
  ~FrequencyAttribute()
  {
  }

  uint32_t
  GetChannel() const
  {
    return(_freq2chan(this->GetValue()));
  }

  bool
  SetChannel(const uint32_t channel_)
  {
    return(false);
  }

protected:

private:

};

}

#endif /* __NL80211_FREQATTRIBUTE_H__ */
