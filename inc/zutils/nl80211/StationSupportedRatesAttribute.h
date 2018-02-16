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

#ifndef __NL80211_STATIONSUPPORTEDRATESATTRIBUTE_H__
#define __NL80211_STATIONSUPPORTEDRATESATTRIBUTE_H__

// libc includes
#include <stdint.h>
#include <linux/nl80211.h>

// libc++ includes

// libzutils includes
#include <zutils/netlink/Attribute.h>

// local includes

namespace nl80211
{

//*****************************************************************************
// Class: StationListenIntervalAttribute
//*****************************************************************************

class StationSupportedRatesAttribute : public Attribute<AttributeBuffer>
{

public:

  StationSupportedRatesAttribute() :
      Attribute(NL80211_ATTR_STA_SUPPORTED_RATES)
  {
  }

  virtual
  ~StationSupportedRatesAttribute()
  {
  }

  virtual std::vector<uint8_t>
  operator()() const
  {
    return(this->_rates);
  }

  virtual bool
  operator()(const uint8_t rate_)
  {
    bool status = false;
    if (this->_rates.size() < NL80211_MAX_SUPP_RATES)
    {
      this->_rates.push_back(rate_);
      status = true;
    }
    return(status);
  }

  virtual bool
  operator()(const std::vector<uint8_t>& rates_)
  {
    bool status = false;
    if (rates_.size() < NL80211_MAX_SUPP_RATES)
    {
      this->_rates = rates_;
      status = true;
    }
    return(status);
  }

  virtual bool
  AddRateKbsp(const size_t rate_)
  {
    bool status = false;
    if (rate_ < 127500)
    {
      status = this->operator ()(rate_ / 500);
    }
    return (status);
  }

  virtual  bool
  AddRateMbsp(const size_t rate_)
  {
    bool status = false;
    if (rate_ < 127)
    {
      status = this->operator ()(rate_ * 2);
    }
    return (status);
  }

protected:

private:

  std::vector<uint8_t> _rates;

};

}

#endif /* __NL80211_STATIONSUPPORTEDRATESATTRIBUTE_H__ */
