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

#ifndef __IEEE80211_VHTCAPSTAG_H__
#define __IEEE80211_VHTCAPSTAG_H__
#include <array>
using namespace std;

#include <zutils/ieee80211/Tag.h>
#include <zutils/ieee80211/ieee80211.h>

namespace zUtils
{
namespace zWireless
{
namespace ieee80211
{

//*****************************************************************************
// Class: VhtCapsTag
//*****************************************************************************

class VhtCapsTag :
    public Tag
{

public:

  VhtCapsTag() :
    Tag(Tag::ID_VHTCAP, sizeof(struct vht_caps))
  {
  }

  virtual
  ~VhtCapsTag()
  {
  }

  struct vht_caps
  operator()() const
  {
    vht_caps caps;
    this->GetValue(caps);
    return(caps);
  }

  virtual bool
  operator()(const struct vht_caps& caps_)
  {
    return(this->PutValue(caps_));
  }

  virtual VhtCapsTag&
  operator=(const struct vht_caps& caps_)
  {
    this->PutValue(caps_);
    return(*this);
  }

  virtual void
  Display() const;

protected:

private:

};

}
}
}

#endif /* __IEEE80211_VHTCAPSTAG_H__ */
