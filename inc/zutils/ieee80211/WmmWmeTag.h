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
#include <iomanip>
#include <string.h> // memcpy
using namespace std;

#include <zutils/ieee80211/Tag.h>
#include <zutils/ieee80211/VendorSpecificTag.h>

namespace zUtils
{
namespace zWireless
{
namespace ieee80211
{

//*****************************************************************************
// Class: WmmWmeTag
//*****************************************************************************

class WmmWmeTag : public VendorSpecificTag
{

public:

  struct ac_parms
  {
    uint8_t aci_aifsn_field;
    uint8_t ecw;
    uint16_t txop_limit; // transmit opportunity
  } __attribute__ ((packed));

  struct wmm_wme
  {
    uint8_t wme_subtype;    // Parameter Element (1)
    uint8_t wme_version;
    uint8_t wme_qos_info;
    uint8_t reserved;
    ac_parms aci0; // best effort
    ac_parms aci1; // background
    ac_parms aci2; // video
    ac_parms aci3; // voice
  } __attribute__ ((packed));

  // Microsoft WMM/WME OUI = 00:50:F2, type 2
  WmmWmeTag() :
    VendorSpecificTag(0x00,0x50,0xF2, OUI_TYPE_WMM_WME)
  {
  }

  virtual
  ~WmmWmeTag()
  {
  }

  virtual vector<uint8_t>
  operator()() const
  {
    vector<uint8_t> vendor;
    vendor.resize(this->Length());
    this->GetValue(vendor.data(), vendor.size());
    return(vendor);
  }

  bool
  operator()(wmm_wme& wmmwme_)
  {
    return (this->AddValue<wmm_wme>(wmmwme_));
  }

  wmm_wme
  WmmWme() const
  {
    wmm_wme wmmwme;
    vector<uint8_t> value = {};
    value.resize(Length());
    GetValue(value.data(), Length());
    memcpy((uint8_t*)&wmmwme, value.data() + 4, sizeof(wmm_wme));
    return wmmwme;
  }

  virtual void
  Display() const
  {
    VendorSpecificTag::Display();
    wmm_wme wmmwme = this->WmmWme();
    cout << "WME Subtype: " << int(wmmwme.wme_subtype) << endl;
    cout << "WME Version: " << int(wmmwme.wme_version) << endl;
    cout << "WME QOS: " << int(wmmwme.wme_qos_info) << endl;
    cout << "AC Parameters AC0 (Best Effort): ";
    cout << "\t ACI/AIFSN: " << int(wmmwme.aci0.aci_aifsn_field);
    cout << " ECW: " << int(wmmwme.aci0.ecw);
    cout << " TXOP: " << int(wmmwme.aci0.txop_limit) << endl;
    cout << "AC Parameters AC1 (Background): ";
    cout << "\t ACI/AIFSN: " << int(wmmwme.aci1.aci_aifsn_field);
    cout << " ECW: " << int(wmmwme.aci1.ecw);
    cout << " TXOP: " << int(wmmwme.aci1.txop_limit) << endl;
    cout << "AC Parameters AC2 (Video): ";
    cout << "\t\t ACI/AIFSN: " << int(wmmwme.aci2.aci_aifsn_field);
    cout << " ECW: " << int(wmmwme.aci2.ecw);
    cout << " TXOP: " << int(wmmwme.aci2.txop_limit) << endl;
    cout << "AC Parameters AC3 (Voice): ";
    cout << "\t\t ACI/AIFSN: " << int(wmmwme.aci3.aci_aifsn_field);
    cout << " ECW: " << int(wmmwme.aci3.ecw);
    cout << " TXOP: " << int(wmmwme.aci3.txop_limit) << endl;
  }

protected:

private:

};

}
}
}

#endif /* INC_IEEE80211_WMMWMETAG_H_ */
