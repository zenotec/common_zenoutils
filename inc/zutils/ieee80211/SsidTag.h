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

#ifndef __IEEE80211_SSIDTAG_H__
#define __IEEE80211_SSIDTAG_H__

#include <zutils/ieee80211/Tag.h>

namespace zUtils
{
namespace zWireless
{
namespace ieee80211
{

//*****************************************************************************
// Class: SsidTag
//*****************************************************************************

class SsidTag : public Tag
{

public:

  SsidTag() :
    Tag(Tag::ID_SSID)
  {
  }

  virtual
  ~SsidTag()
  {
  }

  std::string
  operator()() const
  {
    std::string str;
    this->GetValue(str);
    return(str);
  }

  bool
  operator()(const std::string& ssid_)
  {
    return(this->PutValue(ssid_));
  }

  virtual void
  Display() const
  {
    Tag::Display();
    std::cout << "\t" << this->operator()() << std::endl;
  }

protected:

private:

};

}
}
}

#endif /* __IEEE80211_TAG_H__ */
