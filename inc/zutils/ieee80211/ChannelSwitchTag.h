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

#ifndef __IEEE80211_CHANNELSWITCHTAG_H__
#define __IEEE80211_CHANNELSWITCHTAG_H__

#include <vector>
using namespace std;

#include <zutils/ieee80211/Tag.h>

namespace zUtils
{
namespace zWireless
{
namespace ieee80211
{

//*****************************************************************************
// Class: ChannelSwitchTag
//*****************************************************************************

class ChannelSwitchTag : public Tag
{

public:

  struct channel_switch_tag
  {
    uint8_t mode;
    uint8_t channel;
    uint8_t count;
  };

  ChannelSwitchTag() :
    Tag(Tag::ID_CHANNEL_SWITCH, sizeof(struct channel_switch_tag))
  {
  }

  virtual
  ~ChannelSwitchTag()
  {
  }

  struct channel_switch_tag
  operator()() const
  {
    struct channel_switch_tag chan_switch;
    this->GetValue(chan_switch);
    return (chan_switch);
  }

  bool
  operator()(const struct channel_switch_tag& chan_switch_)
  {
    return (this->PutValue(chan_switch_));
  }

  uint8_t
  Mode() const
  {
    return (this->operator ()().mode);
  }

  bool
  Mode(const uint8_t mode_)
  {
    struct channel_switch_tag chan_switch = this->operator ()();
    chan_switch.mode = mode_;
    return (this->operator ()(chan_switch));
  }

  uint8_t
  Channel() const
  {
    return (this->operator ()().channel);
  }

  bool
  Channel(const uint8_t channel_)
  {
    struct channel_switch_tag chan_switch = this->operator ()();
    chan_switch.channel = channel_;
    return (this->operator ()(chan_switch));
  }

  uint8_t
  Count() const
  {
    return (this->operator ()().count);
  }

  bool
  Count(const uint8_t count_)
  {
    struct channel_switch_tag chan_switch = this->operator ()();
    chan_switch.count = count_;
    return (this->operator ()(chan_switch));
  }

  virtual void
  Display() const
  {
    struct channel_switch_tag chan_switch = this->operator()();
    Tag::Display();
    std::cout << "\tMode: " << int(chan_switch.mode) << ", Channel: " << int(chan_switch.channel) << ", Count: " << int(chan_switch.count) << std::endl;
  }

protected:

private:

};

}
}
}

#endif /* __IEEE80211_CHANNELSWITCHTAG_H__ */
