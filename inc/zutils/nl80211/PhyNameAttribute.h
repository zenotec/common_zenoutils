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

#ifndef __NL80211_PHYNAMEATTRIBUTE_H__
#define __NL80211_PHYNAMEATTRIBUTE_H__

// libc includes
#include <stdint.h>
#include <linux/nl80211.h>

// libc++ includes

// libzutils includes
#include <zutils/netlink/Attribute.h>
using namespace netlink;

// local includes

namespace nl80211
{

//*****************************************************************************
// Class: PhyNameAttribute
//*****************************************************************************

class PhyNameAttribute :
    public AttributeValue
{

public:

  PhyNameAttribute() :
    AttributeValue(NL80211_ATTR_WIPHY_NAME)
  {
  }

  virtual
  ~PhyNameAttribute()
  {
  }

  std::string
  operator()() const
  {
    std::string str;
    this->Get(str);
    return (str);
  }

  bool
  operator()(const std::string& ifname_)
  {
    return (this->Set(ifname_));
  }

protected:

private:

};

}

#endif /* __NL80211_PHYNAMEATTRIBUTE_H__ */
