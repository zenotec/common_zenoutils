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

#include "RadioTapField.h"

namespace ieee80211
{

//*****************************************************************************
// Class: RadioTapFieldTsft
//*****************************************************************************
RadioTapFieldTsft::RadioTapFieldTsft() :
    RadioTapField(RadioTapField::ID_TSFT)
{
}

RadioTapFieldTsft::~RadioTapFieldTsft()
{
}

uint64_t
RadioTapFieldTsft::operator()() const
{
  uint64_t val = 0;
  this->GetValue(val);
  return (val);
}

bool
RadioTapFieldTsft::operator ()(const uint64_t val_)
{
  return(this->PutValue(val_));
}

void
RadioTapFieldTsft::Display() const
{
  RadioTapField::Display();
}

}
