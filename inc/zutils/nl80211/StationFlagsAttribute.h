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

#ifndef __NL80211_STATIONFLAGSATTRIBUTE_H__
#define __NL80211_STATIONFLAGSATTRIBUTE_H__

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
// Class: StaionFlagsAttribute
//*****************************************************************************

class StaionFlagsAttribute : public AttributeValue
{

public:

  StaionFlagsAttribute() :
      AttributeValue(NL80211_ATTR_STA_FLAGS2)
  {
  }

  virtual
  ~StaionFlagsAttribute()
  {
  }

  struct nl80211_sta_flag_update
  operator()() const
  {
	struct nl80211_sta_flag_update flags;
	size_t len = this->GetLength();
    //this->Get(&flags, sizeof(nl80211_sta_flag_update));
    this->Get((uint8_t*) (&flags), len);
    return (flags);
  }

  bool
  operator()(const struct nl80211_sta_flag_update *sta_flags)
  {
    bool status = false;
    //status = this->Set(sta_flags, sizeof(struct nl80211_sta_flag_update));
    status = this->Set((const uint8_t*) sta_flags, sizeof(struct nl80211_sta_flag_update));
    return (status);
  }

protected:

private:

  //struct nl80211_sta_flag_update _buf;
  uint8_t _buf[sizeof(struct nl80211_sta_flag_update)];

};

}

#endif /* __NL80211_STATIONFLAGSATTRIBUTE_H__ */
