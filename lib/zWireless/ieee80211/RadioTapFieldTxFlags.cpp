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
// Class: RadioTapFieldTxFlags
//*****************************************************************************

RadioTapFieldTxFlags::RadioTapFieldTxFlags() :
    RadioTapField(RadioTapField::ID_TX_FLAGS)
{
}

RadioTapFieldTxFlags::~RadioTapFieldTxFlags()
{
}

uint16_t
RadioTapFieldTxFlags::operator ()() const
{
  uint16_t val = 0;
  this->GetValue(val);
  return (val);
}

bool
RadioTapFieldTxFlags::operator ()(const uint16_t val_)
{
  return(this->PutValue(val_));
}

bool
RadioTapFieldTxFlags::Failed() const
{
  uint16_t flags = this->operator ()();
  return (!!(flags & 0x0001));
}

bool
RadioTapFieldTxFlags::Failed(const bool flag_)
{
  uint16_t flags = this->operator ()();
  if (flag_)
  {
    flags |= 0x0001;
  }
  else
  {
    flags &= ~0x0001;
  }
  return (this->operator ()(flags));
}

bool
RadioTapFieldTxFlags::UseRtsCts() const
{
  uint16_t flags = this->operator ()();
  return (!!(flags & 0x0004));
}

bool
RadioTapFieldTxFlags::UseRtsCts(const bool flag_)
{
  uint16_t flags = this->operator ()();
  if (flag_)
  {
    flags |= 0x0004;
  }
  else
  {
    flags &= ~0x0004;
  }
  return (this->operator ()(flags));
}

bool
RadioTapFieldTxFlags::NoAck() const
{
  uint16_t flags = this->operator ()();
  return (!!(flags & 0x0008));
}

bool
RadioTapFieldTxFlags::NoAck(const bool flag_)
{
  uint16_t flags = this->operator ()();
  if (flag_)
  {
    flags |= 0x0008;
  }
  else
  {
    flags &= ~0x0008;
  }
  return (this->operator ()(flags));
}

bool
RadioTapFieldTxFlags::NoSeqNum() const
{
  uint16_t flags = this->operator ()();
  return (!(flags & 0x0010));
}

bool
RadioTapFieldTxFlags::NoSeqNum(const bool flag_)
{
  uint16_t flags = this->operator ()();
  if (flag_)
  {
    flags &= ~0x0010;
  }
  else
  {
    flags |= 0x0010;
  }
  return (this->operator ()(flags));
}

void
RadioTapFieldTxFlags::Display() const
{
  RadioTapField::Display();
}

}
}
}
