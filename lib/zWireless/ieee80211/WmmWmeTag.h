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

#ifndef INC_IEEE80211_WMMWMETAG_H_
#define INC_IEEE80211_WMMWMETAG_H_

#include <vector>
#include <array>
using namespace std;

namespace zUtils
{
namespace ieee80211
{

//*****************************************************************************
// Class: WmmWmeTag
//*****************************************************************************

class WmmWmeTag : public Tag
{

public:

  struct ac_parms
  {
    uint8_t aci_aifsn_field;
    uint8_t ecw;
    uint16_t txop_limit;
  };

  struct wmm_wme
  {
    array<uint8_t, 3> oui;
    uint8_t oui_type; // WMM/WME = 0x02
    uint8_t wme_subtype;
    uint8_t wme_version;
    uint8_t wme_qos_info;
    uint8_t reserved;
    ac_parms aci0; // best effort
    ac_parms aci1; // background
    ac_parms aci2; // video
    ac_parms aci3; // voice
  };

  WmmWmeTag() :
      Tag(Tag::ID_WMM_WME)
  {
  }

  virtual
  ~WmmWmeTag()
  {
  }

  vector<uint8_t>
  operator()() const
  {
    vector<uint8_t> wmmwme;
    wmmwme.resize(this->Length());
    this->GetValue(wmmwme.data(), wmmwme.size());
    return (wmmwme);
  }

  virtual bool
  operator()(const std::vector<uint8_t>& caps_)
  {
    return (this->PutValue(caps_.data(), caps_.size()));
  }

  bool
  operator()(wmm_wme& wmmwme_)
  {
    return (this->PutValue<wmm_wme>(wmmwme_));
  }

  wmm_wme
  WmmWme() const
  {
    wmm_wme wmmwme;
    GetValue<wmm_wme>(wmmwme);
    return wmmwme;
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

#endif /* INC_IEEE80211_WMMWMETAG_H_ */
