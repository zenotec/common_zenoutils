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

#ifndef __NL80211_MACATTRIBUTE_H__
#define __NL80211_MACATTRIBUTE_H__

// libc includes
#include <stdint.h>
#include <linux/nl80211.h>

// libc++ includes

// libzutils includes
#include <zutils/netlink/Attribute.h>

// local includes

namespace nl80211
{

#ifndef ETH_ALEN
#define ETH_ALEN    6
#endif

static bool
_str2mac(const std::string& addr_, uint8_t* hwaddr_)
{
  return (sscanf(addr_.c_str(), "%hhx:%hhx:%hhx:%hhx:%hhx:%hhx",
      &hwaddr_[0], &hwaddr_[1], &hwaddr_[2],
      &hwaddr_[3], &hwaddr_[4], &hwaddr_[5]) == ETH_ALEN);
}

static bool
_mac2str(const uint8_t* hwaddr_, std::string& addr_)
{
  bool status = false;
  char str[256] = { 0 };
  if (sprintf(str, "%02hhx:%02hhx:%02hhx:%02hhx:%02hhx:%02hhx",
      hwaddr_[0], hwaddr_[1], hwaddr_[2],
      hwaddr_[3], hwaddr_[4], hwaddr_[5]) > 0)
  {
    addr_ = std::string(str);
    status = true;
  }
  return (status);
}

//*****************************************************************************
// Class: MacAttribute
//*****************************************************************************

class MacAttribute : public Attribute
{

public:

  MacAttribute() :
      Attribute(TYPE_BINARY, NL80211_ATTR_IFTYPE), _mac{0}
  {
    this->SetValue(this->_mac, sizeof(this->_mac));
    this->ClrValid();
  }

  virtual
  ~MacAttribute()
  {
  }

  std::string
  GetString() const
  {
    std::string str;
    if (this->IsValid())
    {
      _mac2str(this->_mac, str);
    }
    return(str);
  }

  bool
  SetString(const std::string& addr_)
  {
    bool status = false;
    if (_str2mac(addr_, this->_mac))
    {
      this->SetValid();
      status = true;
    }
    return(status);
  }

protected:

private:

  uint8_t _mac[ETH_ALEN];

};

}

#endif /* __NL80211_MACATTRIBUTE_H__ */
