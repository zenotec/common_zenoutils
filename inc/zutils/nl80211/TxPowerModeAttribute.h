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

#ifndef __NL80211_TXPOWERMODEATTRIBUTE_H__
#define __NL80211_TXPOWERMODEATTRIBUTE_H__

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
// Class: TxPowerModeAttribute
//*****************************************************************************

class TxPowerModeAttribute : public Attribute<uint32_t>
{

public:

  enum MODE
  {
    MODE_ERR = -1,
    MODE_NONE = 0,
    MODE_AUTO = 1,
    MODE_LIMITED = 2,
    MODE_FIXED = 3,
    MODE_LAST
  };

  TxPowerModeAttribute() :
      Attribute(NL80211_ATTR_WIPHY_TX_POWER_SETTING)
  {
    this->SetValue(NL80211_TX_POWER_AUTOMATIC);
    this->ClrValid();
  }

  virtual
  ~TxPowerModeAttribute()
  {
  }

  MODE
  GetMode() const
  {
    MODE mode = MODE_ERR;
    switch (this->GetValue())
    {
    case NL80211_TX_POWER_AUTOMATIC:
      mode = MODE_AUTO;
      break;
    case NL80211_TX_POWER_LIMITED:
      mode = MODE_LIMITED;
      break;
    case NL80211_TX_POWER_FIXED:
      mode = MODE_FIXED;
      break;
    default:
      break;
    }
    return (mode);
  }

  bool
  SetMode(const MODE mode_)
  {
    bool status = false;
    switch (mode_)
    {
    case MODE_AUTO:
      this->SetValue(NL80211_TX_POWER_AUTOMATIC);
      status = true;
      break;
    case MODE_LIMITED:
      this->SetValue(NL80211_TX_POWER_LIMITED);
      status = true;
      break;
    case MODE_FIXED:
      this->SetValue(NL80211_TX_POWER_FIXED);
      status = true;
      break;
    default:
      break;
    }
    return (status);
  }

protected:

private:

};

}

#endif /* __NL80211_TXPOWERMODEATTRIBUTE_H__ */
