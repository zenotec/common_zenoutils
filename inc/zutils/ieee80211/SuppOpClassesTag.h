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

#ifndef __IEEE80211_SUPPOPCLASSESTAG_H__
#define __IEEE80211_SUPPOPCLASSESTAG_H__

#include <zutils/ieee80211/Tag.h>

namespace zUtils
{
namespace zWireless
{
namespace ieee80211
{

//*****************************************************************************
// Class: SuppOpClassesTag
//*****************************************************************************

class SuppOpClassesTag :
    public Tag
{

public:

  SuppOpClassesTag() :
    Tag(Tag::ID_REG_CLASSES, sizeof(uint16_t))
  {
  }

  virtual
  ~SuppOpClassesTag()
  {
  }

  uint16_t
  operator()() const
  {
	  uint16_t val = 0;
    this->GetValue(val);
    return(val);
  }

  bool
  operator()(const uint16_t class_)
  {
    return(this->PutValue(class_));
  }

  uint8_t
  CurrentOpClass() const
  {
    return (this->operator ()());
  }

  bool
  CurrentOpClass(const uint16_t class_)
  {
    return (this->operator ()(class_));
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

#endif /* __IEEE80211_SUPPOPCLASSESTAG_H__ */
