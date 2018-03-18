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

#ifndef __NL80211_TXPOWERLEVELATTRIBUTE_H__
#define __NL80211_TXPOWERLEVELATTRIBUTE_H__

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
// Class: TxPowerLevelAttribute
//*****************************************************************************

class TxPowerLevelAttribute : public Attribute
{

public:

  TxPowerLevelAttribute() :
      Attribute(TYPE_U32, NL80211_ATTR_WIPHY_TX_POWER_LEVEL)
  {
    this->SetValue(0);
    this->ClrValid();
  }

  virtual
  ~TxPowerLevelAttribute()
  {
  }

  uint32_t
  operator()() const
  {
    return (this->GetValue<uint32_t>());
  }

  bool
  operator()(const uint32_t ifindex_)
  {
    return (this->SetValue(ifindex_));
  }

protected:

private:

};

}

#endif /* __NL80211_TXPOWERLEVELATTRIBUTE_H__ */
