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

#include <zutils/ieee80211/RadioTapField.h>

namespace zUtils
{
namespace zWireless
{
namespace ieee80211
{

//*****************************************************************************
// Class: RadioTapFieldTxPower
//*****************************************************************************

RadioTapFieldTxPower::RadioTapFieldTxPower() :
    RadioTapField(RadioTapField::ID_DBM_TX_POWER)
{
}

RadioTapFieldTxPower::~RadioTapFieldTxPower()
{
}

uint8_t
RadioTapFieldTxPower::operator()() const
{
  uint8_t val = 0;
  this->GetValue(val);
  return (val);
}

bool
RadioTapFieldTxPower::operator ()(const uint8_t val_)
{
  return(this->PutValue(val_));
}

float
RadioTapFieldTxPower::TxPower() const
{
  return ((float) this->operator ()());
}

bool
RadioTapFieldTxPower::TxPower(const float txPower_)
{
  return (this->operator ()(txPower_));
}

void
RadioTapFieldTxPower::Display() const
{
  RadioTapField::Display();
}

}
}
}
