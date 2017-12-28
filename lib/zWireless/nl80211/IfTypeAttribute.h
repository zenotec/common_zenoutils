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

#ifndef __NL80211_IFTYPEATTRIBUTE_H__
#define __NL80211_IFTYPEATTRIBUTE_H__

namespace nl80211
{

//*****************************************************************************
// Class: IfTypeAttribute
//*****************************************************************************

class IfTypeAttribute : public Attribute<uint32_t>
{

public:

  IfTypeAttribute() :
      Attribute(NL80211_ATTR_IFTYPE)
  {
    this->SetValue(0);
    this->ClrValid();
  }

  virtual
  ~IfTypeAttribute()
  {
  }

  std::string
  GetString() const
  {
    std::string str;
    switch (this->GetValue())
    {
    case NL80211_IFTYPE_UNSPECIFIED:
      str = "Unspecified";
      break;
    case NL80211_IFTYPE_ADHOC:
      str = "AdHoc";
      break;
    case NL80211_IFTYPE_STATION:
      str = "Station";
      break;
    case NL80211_IFTYPE_AP:
      str = "AP";
      break;
    case NL80211_IFTYPE_AP_VLAN:
      str = "AP VLAN";
      break;
    case NL80211_IFTYPE_WDS:
      str = "WDS";
      break;
    case NL80211_IFTYPE_MONITOR:
      str = "Monitor";
      break;
    case NL80211_IFTYPE_MESH_POINT:
      str = "Mesh";
      break;
    case NL80211_IFTYPE_P2P_CLIENT:
      str = "P2P Client";
      break;
    case NL80211_IFTYPE_P2P_GO:
      str = "P2P Go";
      break;
    case NL80211_IFTYPE_P2P_DEVICE:
      str = "P2P Device";
      break;
    case NL80211_IFTYPE_OCB:
      str = "OCB";
      break;
    default:
      str = "Unknown";
      break;
    }
    return (str);
  }

protected:

private:

};

}

#endif /* __NL80211_IFTYPEATTRIBUTE_H__ */
