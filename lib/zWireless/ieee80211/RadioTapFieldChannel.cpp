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

static uint16_t
_freq2chan(uint16_t freq_)
{

  uint16_t channel = 0;
  // Channels 1 - 13
  if ((freq_ >= 2412) && (freq_ <= 2472))
  {
    channel = (1 + ((freq_ - 2412) / 5));
  }
  // Channels 36 - 64
  else if ((freq_ >= 5170) && (freq_ <= 5320))
  {
    channel = (34 + ((freq_ - 5170) / 5));
  }
  // Channels 100 - 144
  else if ((freq_ >= 5500) && (freq_ <= 5720))
  {
    channel = (100 + ((freq_ - 5500) / 5));
  }
  // Channels 149 - 161
  else if ((freq_ >= 5745) && (freq_ <= 5805))
  {
    channel = (149 + ((freq_ - 5745) / 5));
  }
  // Channel 165
  else if (freq_ == 5825)
  {
    channel = 165;
  }

  return (channel);

}

static uint16_t
_chan2freq(uint16_t chan_)
{
  uint16_t freq = 0;
  if ((chan_ >= 1) && (chan_ <=13))
  {
    freq = (((chan_ - 1) * 5) + 2412);
  }
  else if ((chan_ >= 36) && (chan_ <= 64))
  {
    freq = (((chan_ - 34) * 5) + 5170);
  }
  else if ((chan_ >= 100) && (chan_ <= 144))
  {
    freq = (((chan_ - 100) * 5) + 5500);
  }
  else if ((chan_ >= 149) && (chan_ <= 161))
  {
    freq = (((chan_ - 149) * 5) + 5745);
  }
  else if (chan_ == 165)
  {
    freq = 5825;
  }
  return (freq);
}

//*****************************************************************************
// Class: RadioTapFieldChannel
//*****************************************************************************

RadioTapFieldChannel::RadioTapFieldChannel() :
    RadioTapField(RadioTapField::ID_CHANNEL)
{
}

RadioTapFieldChannel::~RadioTapFieldChannel()
{
}

uint32_t
RadioTapFieldChannel::operator()() const
{
  uint32_t val = 0;
  this->GetValue(val);
  return (val);
}

bool
RadioTapFieldChannel::operator ()(const uint32_t val_)
{
  return(this->PutValue(val_));
}

uint16_t
RadioTapFieldChannel::Frequency() const
{
  return ((uint16_t) (this->operator ()() >> 0));
}

bool
RadioTapFieldChannel::Frequency(const uint16_t val_)
{
  uint32_t val = this->operator ()();
  val = ((val & ~0x0000ffff) | (val_ << 0));
  return (this->operator ()(val));
}

uint16_t
RadioTapFieldChannel::Channel() const
{
  return (_freq2chan(this->Frequency()));
}

bool
RadioTapFieldChannel::Channel(const uint16_t val_)
{
  return (this->Frequency(_chan2freq(val_)));
}

uint16_t
RadioTapFieldChannel::Flags() const
{
  return ((uint16_t) (this->operator ()() >> 16));
}

bool
RadioTapFieldChannel::Flags(const uint16_t val_)
{
  uint32_t val = this->operator ()();
  val = ((val & ~0xffff0000) | (val_ << 16));
  return (this->operator ()(val));
}

bool
RadioTapFieldChannel::Turbo() const
{
  return (!!(this->Flags() & FLAGS_TURBO));
}

bool
RadioTapFieldChannel::Turbo(const bool flag_)
{
  uint8_t flags = this->Flags();
  if (flag_)
    flags |= FLAGS_TURBO;
  else
    flags &= ~FLAGS_TURBO;
  this->Flags(flags);
  return (true);
}

bool
RadioTapFieldChannel::CCK() const
{
  return (!!(this->Flags() & FLAGS_CCK));
}

bool
RadioTapFieldChannel::CCK(const bool flag_)
{
  uint8_t flags = this->Flags();
  if (flag_)
    flags |= FLAGS_CCK;
  else
    flags &= ~FLAGS_CCK;
  this->Flags(flags);
  return (true);
}

bool
RadioTapFieldChannel::OFDM() const
{
  return (!!(this->Flags() & FLAGS_OFDM));
}

bool
RadioTapFieldChannel::OFDM(const bool flag_)
{
  uint8_t flags = this->Flags();
  if (flag_)
    flags |= FLAGS_OFDM;
  else
    flags &= ~FLAGS_OFDM;
  this->Flags(flags);
  return (true);
}

bool
RadioTapFieldChannel::Spectrum2G() const
{
  return (!!(this->Flags() & FLAGS_2GHZ));
}

bool
RadioTapFieldChannel::Spectrum2G(const bool flag_)
{
  uint8_t flags = this->Flags();
  if (flag_)
    flags |= FLAGS_2GHZ;
  else
    flags &= ~FLAGS_2GHZ;
  this->Flags(flags);
  return (true);
}

bool
RadioTapFieldChannel::Spectrum5G() const
{
  return (!!(this->Flags() & FLAGS_5GHZ));
}

bool
RadioTapFieldChannel::Spectrum5G(const bool flag_)
{
  uint8_t flags = this->Flags();
  if (flag_)
    flags |= FLAGS_5GHZ;
  else
    flags &= ~FLAGS_5GHZ;
  this->Flags(flags);
  return (true);
}

bool
RadioTapFieldChannel::Passive() const
{
  return (!!(this->Flags() & FLAGS_PASSIVE));
}

bool
RadioTapFieldChannel::Passive(const bool flag_)
{
  uint8_t flags = this->Flags();
  if (flag_)
    flags |= FLAGS_PASSIVE;
  else
    flags &= ~FLAGS_PASSIVE;
  this->Flags(flags);
  return (true);
}

bool
RadioTapFieldChannel::DynamicCCK() const
{
  return (!!(this->Flags() & FLAGS_DYN));
}

bool
RadioTapFieldChannel::DynamicCCK(const bool flag_)
{
  uint8_t flags = this->Flags();
  if (flag_)
    flags |= FLAGS_DYN;
  else
    flags &= ~FLAGS_DYN;
  this->Flags(flags);
  return (true);
}

bool
RadioTapFieldChannel::GFSK() const
{
  return (!!(this->Flags() & FLAGS_GFSK));
}

bool
RadioTapFieldChannel::GFSK(const bool flag_)
{
  uint8_t flags = this->Flags();
  if (flag_)
    flags |= FLAGS_GFSK;
  else
    flags &= ~FLAGS_GFSK;
  this->Flags(flags);
  return (true);
}

bool
RadioTapFieldChannel::GSM() const
{
  return (!!(this->Flags() & FLAGS_GSM));
}

bool
RadioTapFieldChannel::GSM(const bool flag_)
{
  uint8_t flags = this->Flags();
  if (flag_)
    flags |= FLAGS_GSM;
  else
    flags &= ~FLAGS_GSM;
  this->Flags(flags);
  return (true);
}

bool
RadioTapFieldChannel::StaticTurbo() const
{
  return (!!(this->Flags() & FLAGS_STURBO));
}

bool
RadioTapFieldChannel::StaticTurbo(const bool flag_)
{
  uint8_t flags = this->Flags();
  if (flag_)
    flags |= FLAGS_STURBO;
  else
    flags &= ~FLAGS_STURBO;
  this->Flags(flags);
  return (true);
}

bool
RadioTapFieldChannel::HalfRate() const
{
  return (!!(this->Flags() & FLAGS_HALF));
}

bool
RadioTapFieldChannel::HalfRate(const bool flag_)
{
  uint8_t flags = this->Flags();
  if (flag_)
    flags |= FLAGS_HALF;
  else
    flags &= ~FLAGS_HALF;
  this->Flags(flags);
  return (true);
}

bool
RadioTapFieldChannel::QuarterRate() const
{
  return (!!(this->Flags() & FLAGS_QUARTER));
}

bool
RadioTapFieldChannel::QuarterRate(const bool flag_)
{
  uint8_t flags = this->Flags();
  if (flag_)
    flags |= FLAGS_QUARTER;
  else
    flags &= ~FLAGS_QUARTER;
  this->Flags(flags);
  return (true);
}

void
RadioTapFieldChannel::Display() const
{
  RadioTapField::Display();
}

}
