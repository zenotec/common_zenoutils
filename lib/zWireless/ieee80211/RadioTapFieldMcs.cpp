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

uint32_t datarates[4][32] =
{
    { // 20 MHz (GI = 800 ns)
        6500, 13000, 19500, 26000, 39000, 52000, 58500, 65000,
        13000, 26000, 39000, 52000, 78000, 104000, 117000, 130000,
        19500, 39000, 58500, 78000, 117000, 156000, 175500, 195000,
        26000, 52000, 78000, 104000, 156000, 208000, 234000, 260000
    },
    { // 20 MHz (SGI = 400 ns) (TODO - need to set actual values)
        6500, 13000, 19500, 26000, 39000, 52000, 58500, 65000,
        13000, 26000, 39000, 52000, 78000, 104000, 117000, 130000,
        19500, 39000, 58500, 78000, 117000, 156000, 175500, 195000,
        26000, 52000, 78000, 104000, 156000, 208000, 234000, 260000
    },
    { // 40 MHz (GI = 800 ns)
        13500, 27000, 40500, 54000, 81000, 108000, 121500, 135000,
        27000, 54000, 81000, 108000, 162000, 216000, 243000, 270000,
        40500, 81000, 121500, 162000, 243000, 324000, 364500, 405000,
        54000, 108000, 162000, 216000, 324000, 432000, 486000, 540000
    },
    { // 40 MHz (SGI = 400 ns) (TODO - need to set actual values)
        13500, 27000, 40500, 54000, 81000, 108000, 121500, 135000,
        27000, 54000, 81000, 108000, 162000, 216000, 243000, 270000,
        40500, 81000, 121500, 162000, 243000, 324000, 364500, 405000,
        54000, 108000, 162000, 216000, 324000, 432000, 486000, 540000
    }
};

struct mcs
{
  uint8_t known;
  uint8_t flags;
  uint8_t index;
};

static uint32_t
_i2rate(RadioTapFieldMcs::GUARD_INT gi_, RadioTapFieldMcs::WIDTH width_, uint8_t index_)
{

  int i = 0;

  switch (gi_)
  {
  case RadioTapFieldMcs::GUARD_INT_LONG:
    i += 0;
    break;
  case RadioTapFieldMcs::GUARD_INT_SHORT:
    i += 1; // TODO
    break;
  default:
    return (0);
  }

  switch (width_)
  {
  case RadioTapFieldMcs::WIDTH_20:
    // no break
  case RadioTapFieldMcs::WIDTH_20L:
    // no break
  case RadioTapFieldMcs::WIDTH_20U:
    i += 0;
    break;
  case RadioTapFieldMcs::WIDTH_40:
    i += 2;
    break;
  default:
    return (0);
  }

  return (datarates[i][index_]);
}

//*****************************************************************************
// Class: RadioTapFieldMcs
//*****************************************************************************

RadioTapFieldMcs::RadioTapFieldMcs() :
    RadioTapField(RadioTapField::ID_MCS)
{
}

RadioTapFieldMcs::~RadioTapFieldMcs()
{
}

struct mcs
RadioTapFieldMcs::operator()() const
{
  struct mcs val = { 0 };
  this->GetValue(val);
  return (val);
}

bool
RadioTapFieldMcs::operator ()(const struct mcs val_)
{
  return(this->PutValue(val_));
}

RadioTapFieldMcs::WIDTH
RadioTapFieldMcs::Width() const
{
  RadioTapFieldMcs::WIDTH width = WIDTH_NONE;
  uint8_t known = this->operator ()().known;
  uint8_t flags = this->operator ()().flags;
  if (known & 0x01)
  {
    switch (flags & 0x03)
    {
    case 0:
      width = WIDTH_20;
      break;
    case 1:
      width = WIDTH_40;
      break;
    case 2:
      width = WIDTH_20L;
      break;
    case 3:
      width = WIDTH_20U;
      break;
    default:
      width = WIDTH_ERR;
      break;
    }
  }
  return (width);
}

bool
RadioTapFieldMcs::Width(const RadioTapFieldMcs::WIDTH width_)
{
  struct mcs mcs = this->operator ()();
  mcs.known |= 0x01;
  mcs.flags &= ~0x03;
  switch (width_)
  {
  case WIDTH_20:
    mcs.flags |= 0x00;
    break;
  case WIDTH_40:
    mcs.flags |= 0x01;
    break;
  case WIDTH_20L:
    mcs.flags |= 0x02;
    break;
  case WIDTH_20U:
    mcs.flags |= 0x03;
    break;
  default:
    return (false);
  }
  return (this->operator ()(mcs));
}

RadioTapFieldMcs::GUARD_INT
RadioTapFieldMcs::GuardInterval() const
{
  RadioTapFieldMcs::GUARD_INT gi = GUARD_INT_NONE;
  uint8_t known = this->operator ()().known;
  uint8_t flags = this->operator ()().flags;

  if (known & 0x04)
  {
    switch ((flags >> 2) & 0x01)
    {
    case 0:
      gi = GUARD_INT_LONG;
      break;
    case 1:
      gi = GUARD_INT_SHORT;
      break;
    default:
      gi = GUARD_INT_ERR;
      break;
    }
  }

  return (gi);
}

bool
RadioTapFieldMcs::GuardInterval(const RadioTapFieldMcs::GUARD_INT gi_)
{
  struct mcs mcs = this->operator ()();
  mcs.known |= 0x04;
  mcs.flags &= ~0x04;
  switch (gi_)
  {
  case GUARD_INT_LONG:
    mcs.flags |= (0x00 << 2);
    break;
  case GUARD_INT_SHORT:
    mcs.flags |= (0x01 << 2);
    break;
  default:
    return (false);
  }
  return (this->operator ()(mcs));
}

uint8_t
RadioTapFieldMcs::Index() const
{
  uint8_t index = 0;
  uint8_t known = this->operator ()().known;

  if (known & 0x02)
  {
    index = this->operator ()().index;
  }

  return (index);
}

bool
RadioTapFieldMcs::Index( const uint8_t index_)
{
  struct mcs mcs = this->operator ()();
  mcs.known |= 0x02;
  mcs.index = index_;
  return (this->operator ()(mcs));
}

float
RadioTapFieldMcs::RateBps() const
{
  return (float(_i2rate(this->GuardInterval(), this->Width(), this->Index())) * 1000.0);
}

float
RadioTapFieldMcs::RateKbps() const
{
  return (float(_i2rate(this->GuardInterval(), this->Width(), this->Index())));
}

float
RadioTapFieldMcs::RateMbps() const
{
  return (float(_i2rate(this->GuardInterval(), this->Width(), this->Index())) / 1000.0);
}

void
RadioTapFieldMcs::Display() const
{
  RadioTapField::Display();
}

}
}
}
