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

namespace nl80211
{

//*****************************************************************************
// Class: SsidAttribute
//*****************************************************************************

class SsidAttribute : public Attribute<AttributeBuffer>
{

  static const unsigned int MAX_SSID_LEN = 32;

public:

  SsidAttribute() :
      Attribute(NL80211_ATTR_SSID), _buf{0}
  {
    this->SetValue(std::make_pair(this->_buf, sizeof(this->_buf)));
  }

  virtual
  ~SsidAttribute()
  {
  }

  std::string
  GetString() const
  {
    return(std::string(this->_buf));
  }

  bool
  SetString(const std::string& str)
  {
    bool status = false;
    if (str.size() <= MAX_SSID_LEN)
    {
      status = (memcpy(this->_buf, str.c_str(), str.size()) == this->_buf);
    }
    return(status);
  }

protected:

private:

  char _buf[MAX_SSID_LEN+1];

};

}

#endif /* __NL80211_SSIDATTRIBUTE_H__ */
