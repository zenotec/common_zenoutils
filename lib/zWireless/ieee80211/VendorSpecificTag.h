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

#ifndef INC_IEEE80211_VENDORSPECIFICTAG_H_
#define INC_IEEE80211_VENDORSPECIFICTAG_H_

#include <vector>
#include <array>
#include <iomanip>
using namespace std;

namespace zUtils
{
namespace zWireless
{
namespace ieee80211
{

//*****************************************************************************
// Class: VendorSpecificTag
//*****************************************************************************

class VendorSpecificTag : public Tag
{

public:

  enum OUI_TYPE
  {
    OUI_TYPE_NONE = 0,
    OUI_TYPE_WPA_INFO = 1,
    OUI_TYPE_WMM_WME = 2,
    OUI_TYPE_WPS = 4
  };

  VendorSpecificTag(const uint8_t oui1_, const uint8_t oui2_, const uint8_t oui3_, const uint8_t oui_type_) :
    Tag(Tag::ID_VENDOR_SPECIFIC)
  {
    vector<uint8_t> vendor;
    vendor.push_back(uint8_t(oui1_));
    vendor.push_back(uint8_t(oui2_));
    vendor.push_back(uint8_t(oui3_));
    vendor.push_back(oui_type_);
    this->PutValue(vendor.data(), 4);
  }

  virtual
  ~VendorSpecificTag()
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

  virtual void
  Display() const
  {
    Tag::Display();
    vector<uint8_t> vendor = this->operator()();
    cout << "OUI: " << hex << setfill('0') << setw(2) << int(vendor[0]) << "-" << int(vendor[1]) << "-" << int(vendor[2]) << endl;
    cout << "Vendor Specific OUI Type: " << int(vendor[3]) << endl;
    cout << dec; // reset output stream to decimal
  }

protected:

private:

};

}
}
}

#endif /* INC_IEEE80211_VENDORSPECIFICTAG_H_ */
