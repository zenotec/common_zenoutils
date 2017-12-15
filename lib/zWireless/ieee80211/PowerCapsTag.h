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

#ifndef INC_IEEE80211_POWERCAPSTAG_H_
#define INC_IEEE80211_POWERCAPSTAG_H_

#include <vector>
using namespace std;

namespace zUtils
{
namespace ieee80211
{

//*****************************************************************************
// Class: PowerCapsTag
//*****************************************************************************

class PowerCapsTag : public Tag
{

public:

  PowerCapsTag() :
      Tag(Tag::ID_POWER_CAPS)
  {
  }

  virtual
  ~PowerCapsTag()
  {
  }

  vector<uint8_t>
  operator()() const
  {
    vector<uint8_t> minmax;
    minmax.resize(this->Length());
    this->GetValue(minmax.data(), minmax.size());
    return (minmax);
  }

  bool
  operator()(const uint8_t min_, const uint8_t max_)
  {
    std::vector<uint8_t> minmax;
    minmax.push_back(min_);
    minmax.push_back(max_);
    return (this->PutValue(minmax.data(), 2));
  }

  virtual void
  Display() const
  {
    Tag::Display();
    vector<uint8_t> minmax = this->operator()();
    std::cout << "\tMin Pwr: " << int(minmax.front()) << ", Max Pwr: " << int(minmax.back())
        << endl;
  }

protected:

private:

};

}
}

#endif /* INC_IEEE80211_POWERCAPSTAG_H_ */
