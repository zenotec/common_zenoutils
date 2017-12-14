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

#ifndef __NL80211_TXPOWERATTRIBUTE_H__
#define __NL80211_TXPOWERATTRIBUTE_H__

// libc includes
#include <stdint.h>
#include <linux/nl80211.h>

// libc++ includes

// local includes
#include "Attribute.h"

namespace nl80211
{

//*****************************************************************************
// Class: TxPowerAttribute
//*****************************************************************************

class TxPowerAttribute : public Attribute<uint32_t>
{

public:

  TxPowerAttribute() :
      Attribute(NL80211_ATTR_WIPHY_TX_POWER_LEVEL)
  {
    this->SetValue(0);
  }

  virtual
  ~TxPowerAttribute()
  {
  }

protected:

private:

};

}

#endif /* __NL80211_PHYINDEXATTRIBUTE_H__ */
