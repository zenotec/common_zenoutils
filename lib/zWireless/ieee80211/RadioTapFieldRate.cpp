/*
 * Copyright (c) 2014-2016 ZenoTec LLC (http://www.zenotec.net)
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

#include <stdint.h>
#include <stdio.h>
#include <string.h>
#include <endian.h>

#include <string>

#include "RadioTapField.h"

namespace ieee80211
{

//*****************************************************************************
// Class: RadioTapFieldRate
//*****************************************************************************

RadioTapFieldRate::RadioTapFieldRate() :
    RadioTapField(RadioTapField::ID_RATE)
{
}

RadioTapFieldRate::~RadioTapFieldRate()
{
}

uint8_t
RadioTapFieldRate::operator()() const
{
  uint8_t val = 0;
  this->GetValue(val);
  return (val);
}

bool
RadioTapFieldRate::operator ()(const uint8_t val_)
{
  return(this->PutValue(val_));
}

float
RadioTapFieldRate::RateBps() const
{
  return ((float) this->operator ()() * 500000);
}

bool
RadioTapFieldRate::RateBsp(const float rate_)
{
  return (this->operator ()(rate_ / 500000));
}

float
RadioTapFieldRate::RateKbps() const
{
  return ((float) this->operator ()() * 500);
}

bool
RadioTapFieldRate::RateKbps(const float rate_)
{
  return (this->operator ()(rate_ / 500));
}

float
RadioTapFieldRate::RateMbps() const
{
  return ((float) this->operator ()() * 0.5);
}

bool
RadioTapFieldRate::RateMbps(const float rate_)
{
  return (this->operator ()(rate_ * 2));
}

void
RadioTapFieldRate::Display() const
{
  RadioTapField::Display();
}

}
