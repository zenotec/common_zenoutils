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

#ifndef __NL80211_STASUPPORTEDRATESATTRIBUTE_H__
#define __NL80211_STASUPPORTEDRATESATTRIBUTE_H__

// libc includes
#include <stdint.h>
#include <linux/nl80211.h>

// libc++ includes
#include <vector>
using namespace std;

// libzutils includes
#include <zutils/netlink/Attribute.h>

// local includes

namespace nl80211
{

#define NL80211_MAX_SUPP_RATES  32

//*****************************************************************************
// Class: StaSupportedRatesAttribute
//*****************************************************************************

class StaSupportedRatesAttribute : public AttributeValue
{

public:

  StaSupportedRatesAttribute() :
      AttributeValue(NL80211_ATTR_STA_SUPPORTED_RATES)
  {
  }

  virtual
  ~StaSupportedRatesAttribute()
  {
  }

  vector<uint8_t>
  operator()() const
  {
	vector<uint8_t> rates;
    size_t len = this->GetLength();
    rates.resize(len);
    this->Get((uint8_t*) (&rates[0]), len);
    return (rates);
  }

  bool
  operator()(const vector<uint8_t> rates)
  {
	bool status = false;
	if (rates.size() <= NL80211_MAX_SUPP_RATES)
	{
	  status = this->Set((const uint8_t*) (&rates[0]), rates.size());
	}
	return (status);
  }

protected:

private:

  uint8_t _buf[NL80211_MAX_SUPP_RATES + 1];

};

}

#endif /* __NL80211_STASUPPORTEDRATESATTRIBUTE_H__ */
