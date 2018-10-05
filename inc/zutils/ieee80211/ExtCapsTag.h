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

#ifndef __IEEE80211_EXTENDEDCAPSTAG_H__
#define __IEEE80211_EXTENDEDCAPSTAG_H__

#include <zutils/ieee80211/Tag.h>

namespace zUtils
{
namespace zWireless
{
namespace ieee80211
{

//*****************************************************************************
// Class: ExtCapsTag
//*****************************************************************************

class ExtCapsTag : public Tag
{

public:

  ExtCapsTag() :
    Tag(Tag::ID_EXTCAP, sizeof(struct ext_caps_tag))
  {
  }

  virtual
  ~ExtCapsTag()
  {
  }

  struct ext_caps_tag
  operator()() const
  {
    struct ext_caps_tag val = { 0 };
    this->GetValue(val);
    return(val);
  }

  bool
  operator()(const struct ext_caps_tag caps_)
  {
    return(this->PutValue(caps_));
  }

  virtual void
  Display() const
  {
    Tag::Display();
  }

  bool
  GetFlag(const EXTCAPSBIT bit_)
  {
      struct ext_caps_tag val = { 0 };
      this->GetValue(val);
      return( val.caps[(bit_ >> 4)] >> (bit_ & 0xf) & 1 );
  }

  bool
  SetFlag(const EXTCAPSBIT bit_)
  {
    struct ext_caps_tag val = { 0 };
    this->GetValue(val);
    val.caps[(bit_ >> 4)] |= (1 << (bit_ & 0xf));
    return(this->PutValue(val));
  }

  bool
  ClearFlag(const EXTCAPSBIT bit_)
  {
    struct ext_caps_tag val = { 0 };
    this->GetValue(val);
    val.caps[(bit_ >> 4)] &= ~(1 << (bit_ & 0xf));
    return(this->PutValue(val));
  }

protected:

private:

};

}
}
}

#endif /* __IEEE80211_EXTENDEDCAPSTAG_H__ */
