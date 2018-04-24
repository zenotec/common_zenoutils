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

#ifndef __IEEE80211_COUNTRYTAG_H__
#define __IEEE80211_COUNTRYTAG_H__

#include <zutils/ieee80211/Tag.h>

namespace zUtils
{
namespace zWireless
{
namespace ieee80211
{

//*****************************************************************************
// Class: CountryTag
//*****************************************************************************

struct country_tag
{
  uint8_t country_str[3];
  uint8_t first_channel;
  uint8_t num_channels;
  uint8_t max_power;
};

class CountryTag : public Tag
{

public:

  CountryTag() :
    Tag(Tag::ID_COUNTRY)
  {
  }

  virtual
  ~CountryTag()
  {
  }

  struct country_tag
  operator()() const
  {
    struct country_tag val = { 0 };
    this->GetValue(val);
    return (val);
  }

  bool
  operator()(const struct country_tag& tag_)
  {
    struct country_tag t = { 'U', 'S', 0x20, 1, 11, 30 }; // TODO:
    return(this->PutValue(t));
  }

  virtual void
  Display() const
  {
    struct country_tag tag = this->operator()();
    Tag::Display();
    printf("\t%c%c [%d:%d]\n", tag.country_str[0], tag.country_str[1],
        tag.first_channel, (tag.first_channel + tag.num_channels));
  }

protected:

private:

};

}
}
}

#endif /* __IEEE80211_COUNTRYTAG_H__ */
