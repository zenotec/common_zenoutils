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

#ifndef __IEEE80211_TIMTAG_H__
#define __IEEE80211_TIMTAG_H__

#include <zutils/ieee80211/Tag.h>

namespace zUtils
{
namespace zWireless
{
namespace ieee80211
{

struct tim_tag
{
  uint8_t dtim_count;
  uint8_t dtim_period;
  uint8_t bitmap_cntl;
  uint8_t virt_bitmap[1];
} __attribute__ ((packed));

//*****************************************************************************
// Class: TimTag
//*****************************************************************************

class TimTag : public Tag
{

public:

  TimTag() :
    Tag(Tag::ID_TIM, sizeof(struct tim_tag))
  {
  }

  virtual
  ~TimTag()
  {
  }

  struct tim_tag
  operator()() const
  {
    struct tim_tag val = { 0 };
    this->GetValue(val);
    return(val);
  }

  bool
  operator()(const struct tim_tag dtim_)
  {
    return(this->PutValue(dtim_));
  }

  uint8_t
  Count() const
  {
    return (this->operator ()().dtim_count);
  }

  bool
  Count(const uint8_t count_)
  {
    struct tim_tag tim = this->operator ()();
    tim.dtim_count = count_;
    return (this->operator ()(tim));
  }

  uint8_t
  Period() const
  {
    return (this->operator ()().dtim_period);
  }

  bool
  Period(const uint8_t period_)
  {
    struct tim_tag tim = this->operator ()();
    tim.dtim_period = period_;
    return (this->operator ()(tim));
  }

  virtual void
  Display() const
  {
    Tag::Display();
  }

protected:

private:

};

}
}
}

#endif /* __IEEE80211_TIMTAG_H__ */
