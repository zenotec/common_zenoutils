/*
 * Copyright (c) 2018 Cable Television Laboratories, Inc. ("CableLabs")
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

#ifndef __NL80211_FRAMETYPEATTRIBUTE_H__
#define __NL80211_FRAMETYPEATTRIBUTE_H__

// libc includes
#include <stdint.h>
#include <linux/nl80211.h>

// libc++ includes

// libzutils includes
#include <zutils/netlink/Attribute.h>

// local includes

namespace nl80211
{

//*****************************************************************************
// Class: FrameTypeAttribute
//*****************************************************************************

class FrameTypeAttribute : public netlink::AttributeValue
{

public:

  FrameTypeAttribute() :
      netlink::AttributeValue(NL80211_ATTR_FRAME_TYPE)
  {
  }

  virtual
  ~FrameTypeAttribute()
  {
  }

  uint16_t
  operator()() const
  {
    return (this->Get(uint16_t(0)));
  }

  bool
  operator()(const uint16_t interval_)
  {
    return (this->Set(interval_));
  }

protected:

private:

};

}

#endif /* __NL80211_FRAMETYPEATTRIBUTE_H__ */
