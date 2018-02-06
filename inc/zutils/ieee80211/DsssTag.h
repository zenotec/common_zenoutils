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

#ifndef __IEEE80211_DSSSTAG_H__
#define __IEEE80211_DSSSTAG_H__

#include <zutils/ieee80211/Tag.h>

namespace zUtils
{
namespace zWireless
{
namespace ieee80211
{

//*****************************************************************************
// Class: DsssTag
//*****************************************************************************

class DsssTag : public Tag
{

public:

  DsssTag() :
    Tag(Tag::ID_DSSS)
  {
  }

  virtual
  ~DsssTag()
  {
  }

  uint8_t
  operator()() const
  {
    uint8_t val = 0;
    this->GetValue(val);
    return(val);
  }

  bool
  operator()(const uint8_t chan_)
  {
    return(this->PutValue(chan_));
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

#endif /* __IEEE80211_DSSSTAG_H__ */
