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

#ifndef __IEEE80211_POWERCAPSTAG_H__
#define __IEEE80211_POWERCAPSTAG_H__

#include <vector>
using namespace std;

#include <zutils/ieee80211/Tag.h>

namespace zUtils
{
namespace zWireless
{
namespace ieee80211
{

//*****************************************************************************
// Class: PowerCapsTag
//*****************************************************************************

class PowerCapsTag :
    public Tag
{

public:

  struct power_caps_tag
  {
    uint8_t min;
    uint8_t max;
  };

  PowerCapsTag() :
    Tag(Tag::ID_PWRCAP, sizeof(struct power_caps_tag))
  {
  }

  virtual
  ~PowerCapsTag()
  {
  }

  struct power_caps_tag
  operator()() const
  {
    struct power_caps_tag caps;
    this->GetValue(caps);
    return (caps);
  }

  bool
  operator()(const struct power_caps_tag& caps_)
  {
    return (this->PutValue(caps_));
  }

  uint8_t
  Min() const
  {
    return (this->operator ()().min);
  }

  bool
  Min(const uint8_t min_)
  {
    struct power_caps_tag caps = this->operator ()();
    caps.min = min_;
    return (this->operator ()(caps));
  }

  uint8_t
  Max() const
  {
    return (this->operator ()().max);
  }

  bool
  Max(const uint8_t max_)
  {
    struct power_caps_tag caps = this->operator ()();
    caps.max = max_;
    return (this->operator ()(caps));
  }

  virtual void
  Display() const
  {
    struct power_caps_tag caps = this->operator()();
    Tag::Display();
    std::cout << "\tMin Pwr: " << int(caps.min) << ", Max Pwr: " << int(caps.max) << std::endl;
  }

protected:

private:

};

}
}
}

#endif /* __IEEE80211_POWERCAPSTAG_H__ */
