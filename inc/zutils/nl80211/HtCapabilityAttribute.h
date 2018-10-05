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

#ifndef __NL80211_HTCAPABILITYATTRIBUTE_H__
#define __NL80211_HTCAPABILITYATTRIBUTE_H__

// libc includes
#include <stdint.h>
#include <linux/nl80211.h>

// libc++ includes

// libzutils includes
#include <zutils/ieee80211/ieee80211.h>
#include <zutils/netlink/Attribute.h>

// local includes

namespace nl80211
{

//*****************************************************************************
// Class: HtCapabilityAttribute
//*****************************************************************************

class HtCapabilityAttribute :
    public netlink::AttributeValue
{

public:

  HtCapabilityAttribute() :
      AttributeValue(NL80211_ATTR_HT_CAPABILITY)
  {
  }

  virtual
  ~HtCapabilityAttribute()
  {
  }

  struct ht_caps
  operator()() const
  {
    struct ht_caps htcaps = { 0 };
    size_t len = sizeof(struct ht_caps);
    this->Get((uint8_t*)&htcaps, len);
    return (htcaps);
  }

  bool
  operator()(const struct ht_caps& capability_)
  {
    return (this->Set((uint8_t*)&capability_, sizeof(struct ht_caps)));
  }

protected:

private:

};

}

#endif /* __NL80211_HTCAPABILITYATTRIBUTE_H__ */
