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

#ifndef __IEEE80211_RATESTAG_H__
#define __IEEE80211_RATESTAG_H__

#include "Tag.h"

namespace zUtils
{
namespace ieee80211
{

//*****************************************************************************
// Class: RatesTag
//*****************************************************************************

class RatesTag : public Tag
{

public:

  RatesTag(const Tag::ID id_ = ID_RATES, const size_t len_ = 0) :
    Tag(id_)
  {
  }

  virtual
  ~RatesTag()
  {
  }

  virtual std::vector<uint8_t>
  operator()() const
  {
    std::vector<uint8_t> rates;
    rates.resize(this->Length());
    this->GetValue(rates.data(), rates.size());
    return(rates);
  }

  virtual bool
  operator()(const uint8_t rate_)
  {
    std::vector<uint8_t> rates = this->operator ()();
    rates.push_back(rate_);
    return(this->PutValue(rates.data(), rates.size()));
  }

  virtual bool
  operator()(const std::vector<uint8_t>& rates_)
  {
    return(this->PutValue(rates_.data(), rates_.size()));
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

  virtual void
  Display() const
  {
    int cnt = 0;
    Tag::Display();
    FOREACH(auto& rate, this->operator ()())
    {
      if (!(cnt % 4))
      {
        std::cout << "\t";
      }
      float f = (rate & 0x7f);
      f /= 2;
      printf("%.1f Mb/s ", f);
      if ((++cnt % 4))
      {
        std::cout << ", ";
      }
      else
      {
        std::cout << std::endl;
      }
    }
  }

protected:

private:

};

}
}

#endif /* __IEEE80211_RATESTAG_H__ */
