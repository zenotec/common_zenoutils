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

#ifndef __NL80211_SSIDATTRIBUTE_H__
#define __NL80211_SSIDATTRIBUTE_H__

// libc includes
#include <stdint.h>
#include <linux/nl80211.h>

// libc++ includes

// libzutils includes
#include <zutils/netlink/Attribute.h>

// local includes

namespace nl80211
{

#define MAX_SSID_LEN 32

//*****************************************************************************
// Class: SsidAttribute
//*****************************************************************************

class SsidAttribute : public AttributeValue
{

public:

  SsidAttribute() :
      AttributeValue(NL80211_ATTR_SSID)
  {
  }

  virtual
  ~SsidAttribute()
  {
  }

  std::string
  operator()() const
  {
    std::string str;
    size_t len = this->GetLength();
    str.resize(len);
    this->Get((uint8_t*) str.c_str(), len);
    return (str);
  }

  bool
  operator()(const std::string& str_)
  {
    bool status = false;
    if (str_.size() <= MAX_SSID_LEN)
    {
      status = this->Set((const uint8_t*) str_.c_str(), str_.size());
    }
    return (status);
  }

protected:

private:

  uint8_t _buf[MAX_SSID_LEN + 1];

};

}

#endif /* __NL80211_SSIDATTRIBUTE_H__ */
