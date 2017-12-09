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
// Class: RadioTapFieldFlags
//*****************************************************************************

RadioTapFieldFlags::RadioTapFieldFlags() :
    RadioTapField(RadioTapField::ID_FLAGS)
{
}

RadioTapFieldFlags::~RadioTapFieldFlags()
{
}

uint8_t
RadioTapFieldFlags::operator()() const
{
  uint8_t val = 0;
  this->GetValue(val);
  return (val);
}

bool
RadioTapFieldFlags::operator ()(const uint8_t val_)
{
  return(this->PutValue(val_));
}

bool
RadioTapFieldFlags::CFP() const
{
  return (!!(this->operator ()() & FLAGS_CFP));
}

bool
RadioTapFieldFlags::CFP(const bool flag_)
{
  uint8_t flags = this->operator ()();
  if (flag_)
    flags |= FLAGS_CFP;
  else
    flags &= ~FLAGS_CFP;
  this->operator ()(flags);
  return (true);
}

bool
RadioTapFieldFlags::ShortPreamble() const
{
  return (!!(this->operator ()() & FLAGS_SHORTPRE));
}

bool
RadioTapFieldFlags::ShortPreamble(const bool flag_)
{
  uint8_t flags = this->operator ()();
  if (flag_)
    flags |= FLAGS_SHORTPRE;
  else
    flags &= ~FLAGS_SHORTPRE;
  this->operator ()(flags);
  return (true);
}

bool
RadioTapFieldFlags::WEP() const
{
  return (!!(this->operator ()() & FLAGS_WEP));
}

bool
RadioTapFieldFlags::WEP(const bool flag_)
{
  uint8_t flags = this->operator ()();
  if (flag_)
    flags |= FLAGS_WEP;
  else
    flags &= ~FLAGS_WEP;
  this->operator ()(flags);
  return (true);
}

bool
RadioTapFieldFlags::Fragmentation() const
{
  return (!!(this->operator ()() & FLAGS_FRAG));
}

bool
RadioTapFieldFlags::Fragmentation(const bool flag_)
{
  uint8_t flags = this->operator ()();
  if (flag_)
    flags |= FLAGS_FRAG;
  else
    flags &= ~FLAGS_FRAG;
  this->operator ()(flags);
  return (true);
}

bool
RadioTapFieldFlags::FCS() const
{
  return (!!(this->operator ()() & FLAGS_FCS));
}

bool
RadioTapFieldFlags::FCS(const bool flag_)
{
  uint8_t flags = this->operator ()();
  if (flag_)
    flags |= FLAGS_FCS;
  else
    flags &= ~FLAGS_FCS;
  this->operator ()(flags);
  return (true);
}

bool
RadioTapFieldFlags::DataPad() const
{
  return (!!(this->operator ()() & FLAGS_DATAPAD));
}

bool
RadioTapFieldFlags::DataPad(const bool flag_)
{
  uint8_t flags = this->operator ()();
  if (flag_)
    flags |= FLAGS_DATAPAD;
  else
    flags &= ~FLAGS_DATAPAD;
  this->operator ()(flags);
  return (true);
}

bool
RadioTapFieldFlags::BadFCS() const
{
  return (!!(this->operator ()() & FLAGS_BADFCS));
}

bool
RadioTapFieldFlags::BadFCS(const bool flag_)
{
  uint8_t flags = this->operator ()();
  if (flag_)
    flags |= FLAGS_BADFCS;
  else
    flags &= ~FLAGS_BADFCS;
  this->operator ()(flags);
  return (true);
}

bool
RadioTapFieldFlags::ShortGI() const
{
  return (!!(this->operator ()() & FLAGS_SHORTGI));
}

bool
RadioTapFieldFlags::ShortGI(const bool flag_)
{
  uint8_t flags = this->operator ()();
  if (flag_)
    flags |= FLAGS_SHORTGI;
  else
    flags &= ~FLAGS_SHORTGI;
  this->operator ()(flags);
  return (true);
}

void
RadioTapFieldFlags::Display() const
{
  RadioTapField::Display();
}

}
