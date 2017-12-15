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

#ifndef INC_IEEE80211_CHANNELSTAG_H_
#define INC_IEEE80211_CHANNELSTAG_H_

#include <vector>
using namespace std;

namespace zUtils
{
namespace ieee80211
{

//*****************************************************************************
// Class: ChannelsTag
//*****************************************************************************

class ChannelsTag : public Tag
{

public:

  ChannelsTag() :
    Tag(Tag::ID_CHANNELS)
  {
  }

  virtual
  ~ChannelsTag()
  {
  }

  vector<uint8_t>
  operator()() const
  {
    vector<uint8_t> firstcnt;
    firstcnt.resize(this->Length());
    this->GetValue(firstcnt.data(), firstcnt.size());
    return(firstcnt);
  }

  bool
  operator()(const uint8_t first_, const uint8_t cnt_)
  {
    std::vector<uint8_t> firstcnt;
    firstcnt.push_back(first_);
    firstcnt.push_back(cnt_);
    return(this->PutValue(firstcnt.data(), 2));
  }

  virtual void
  Display() const
  {
    Tag::Display();
    vector<uint8_t> firstcnt = this->operator()();
    std::cout << "\tFirst Chan: " << firstcnt.front() << ", Chan Cnt: " << firstcnt.back() << endl;
  }

protected:

private:

};

}
}

#endif /* INC_IEEE80211_CHANNELSTAG_H_ */
