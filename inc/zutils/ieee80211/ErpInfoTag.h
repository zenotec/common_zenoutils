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

#ifndef __IEEE80211_ERPINFOTAG_H__
#define __IEEE80211_ERPINFOTAG_H__

#include <zutils/ieee80211/Tag.h>

namespace zUtils
{
namespace zWireless
{
namespace ieee80211
{

//*****************************************************************************
// Class: ErpInfoTag
//*****************************************************************************

class ErpInfoTag : public Tag
{

public:

  ErpInfoTag() :
      Tag(Tag::ID_ERP, sizeof(uint8_t))
  {
  }

  virtual
  ~ErpInfoTag()
  {
  }

  uint8_t
  operator()() const
  {
    uint8_t val = 0;
    this->GetValue(val);
    return (val);
  }

  bool
  operator()(const uint8_t params_)
  {
    return (this->PutValue(params_));
  }

  bool
  NonErpPresent() const
  {
    return (!!(this->operator ()() & 0x01));
  }

  bool
  NonErpPresent(const bool flag_)
  {
    uint8_t val = this->operator ()();
    if (flag_)
    {
      val |= 0x01;
    }
    else
    {
      val &= ~0x01;
    }
    return (this->operator ()(val));
  }

  bool
  UseProtection() const
  {
    return (!!(this->operator ()() & 0x02));
  }

  bool
  UseProtection(const bool flag_)
  {
    uint8_t val = this->operator ()();
    if (flag_)
    {
      val |= 0x02;
    }
    else
    {
      val &= ~0x02;
    }
    return (this->operator ()(val));
  }

  bool
  BarkerPreambleMode() const
  {
    return (!!(this->operator ()() & 0x04));
  }

  bool
  BarkerPreambleMode(const bool flag_)
  {
    uint8_t val = this->operator ()();
    if (flag_)
    {
      val |= 0x04;
    }
    else
    {
      val &= ~0x04;
    }
    return (this->operator ()(val));
  }

  virtual void
  Display() const
  {
    Tag::Display();
    printf("\t0x%02x\n", this->operator()());
  }

protected:

private:

};

}
}
}

#endif /* __IEEE80211_DSSSTAG_H__ */
