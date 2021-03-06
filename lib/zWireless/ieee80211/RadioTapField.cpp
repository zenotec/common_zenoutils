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

#include <zutils/zLog.h>
using namespace zUtils;

ZLOG_MODULE_INIT(zLog::Log::MODULE_WIRELESS);

namespace zUtils
{
namespace zWireless
{
namespace ieee80211
{

struct rtap_field
{
  uint8_t align;
  uint8_t size;
} __attribute__ ((packed));

static const struct rtap_field fieldtbl[] =
{
    [RadioTapField::ID_TSFT] = { .align = 8, .size = 8 },
    [RadioTapField::ID_FLAGS] = { .align = 1, .size = 1 },
    [RadioTapField::ID_RATE] = { .align = 1, .size = 1 },
    [RadioTapField::ID_CHANNEL] = { .align = 2, .size = 4 },
    [RadioTapField::ID_FHSS] = { .align = 2, .size = 2, },
    [RadioTapField::ID_DBM_ANTSIGNAL] = { .align = 1, .size = 1 },
    [RadioTapField::ID_DBM_ANTNOISE] = { .align = 1, .size = 1 },
    [RadioTapField::ID_LOCK_QUALITY] = { .align = 2, .size = 2 },
    [RadioTapField::ID_TX_ATTENUATION] = { .align = 2, .size = 2 },
    [RadioTapField::ID_DB_TX_ATTENUATION] = { .align = 2, .size = 2 },
    [RadioTapField::ID_DBM_TX_POWER] = { .align = 1, .size = 1 },
    [RadioTapField::ID_ANTENNA] = { .align = 1, .size = 1 },
    [RadioTapField::ID_DB_ANTSIGNAL] = { .align = 1, .size = 1, },
    [RadioTapField::ID_DB_ANTNOISE] = { .align = 1, .size = 1, },
    [RadioTapField::ID_RX_FLAGS] = { .align = 2, .size = 2, },
    [RadioTapField::ID_TX_FLAGS] = { .align = 2, .size = 2, },
    [RadioTapField::ID_RTS_RETRIES] = { .align = 1, .size = 1, },
    [RadioTapField::ID_DATA_RETRIES] = { .align = 1, .size = 1, },
    [RadioTapField::ID_RSVD1] = { .align = 0, .size = 0, },
    [RadioTapField::ID_MCS] = { .align = 1, .size = 3, },
    [RadioTapField::ID_AMPDU_STATUS] = { .align = 4, .size = 8, },
    [RadioTapField::ID_VHT] = { .align = 2, .size = 12, },
    [RadioTapField::ID_TIMESTAMP] = { .align = 8, .size = 12, }
};

static uint8_t*
_align(const uint8_t* hdr_, const uint8_t* addr_, const size_t align_, size_t& pad_)
{
  unsigned long mask = (align_ - 1);
  unsigned long offset = ((addr_ - hdr_) & mask);
  pad_ = ((offset > 0) ? (align_ - offset) : offset);
  return ((uint8_t*) (addr_ + pad_));
}

static size_t
_pad(const uint8_t* hdr_, const uint8_t* addr_, const size_t align_)
{
  unsigned long mask = (align_ - 1);
  unsigned long offset = ((addr_ - hdr_) & mask);
  return ((offset > 0) ? (align_ - offset) : offset);
}

//*****************************************************************************
// Class: RadioTapField
//*****************************************************************************

RadioTapField::RadioTapField(const RadioTapField::ID id_) :
    _id(id_)
{
  this->_value.resize(this->Size());
}

RadioTapField::~RadioTapField()
{
}

bool
RadioTapField::Assemble(zSocket::Buffer& sb_, const uint8_t* hdr_, size_t& pad_)
{
	// Sanity check the field attributes
	if (!this->Align() || !this->Size() || this->_value.empty())
	{
		return false;
	}

	// Calc amount of padding to align this field on it's size boundary, relative to the start of the hdr
	pad_ = _pad(hdr_, sb_.Data(), Align());

	// Verify there is enough room in the caller's buffer and copy the data to the aligned address
	if (sb_.Put(Size() + pad_))
	{
		sb_.Pull(pad_); // move data ptr to aligned address
		memcpy(sb_.Data(), this->_value.data(), this->Size());
		sb_.Pull(Size()); // move data ptr up to tail
	}
	else
	{
		ZLOG_ERR("Error assembling RadioTap field");
		return false;
	}


	return true;
}

bool
RadioTapField::Disassemble(zSocket::Buffer& sb_, const uint8_t* hdr_, size_t& pad_)
{
	// Sanity check the field attributes
	if (!this->Align() || !this->Size())
	{
		ZLOG_ERR("Error disassembling RadioTap field - undefined Align and/or Size");
		return false;
	}

	// Calc amount of padding to align this field on it's size boundary, relative to the start of the hdr
	pad_ = _pad(hdr_, sb_.Data(), Align());

	// advance to start of field with respect to pad
	if (not sb_.Pull(pad_))
	{
		ZLOG_ERR("Error disassembling RadioTap field - couldn't jump pad");
		return false;
	}

	uint8_t* field = sb_.Data();
	if (not sb_.Pull(this->Size()))
	{
		ZLOG_ERR("Error disassembling RadioTap field - couldn't Pull expected size");
		return false;
	}

	this->_value.resize(this->Size());

	memcpy(this->_value.data(), field, this->Size());

	return true;
}

RadioTapField::ID
RadioTapField::Id() const
{
  return (this->_id);
}

size_t
RadioTapField::Align() const
{
  return (fieldtbl[this->_id].align);
}

size_t
RadioTapField::Size() const
{
  return (fieldtbl[this->_id].size);
}

size_t
RadioTapField::GetValue(uint8_t* p_, size_t len_) const
{
  int i = 0;
  len_ = std::min(this->Size(), len_);
  if (p_ && (len_ <= this->_value.size()))
  {
    for (i = 0; i < len_; i++)
    {
      p_[i] = this->_value[i];
    }
  }
  return (i);
}

size_t
RadioTapField::PutValue(const uint8_t* p_, size_t len_)
{
  int i = 0;
  len_ = std::min(len_, (size_t) 32);
  this->_value.resize(len_, 0);
  for (i = 0; i < len_; i++)
  {
    this->_value[i] = p_[i];
  }
  return (i);
}

void
RadioTapField::Display() const
{
  printf("\t       ID: %d\n", this->Id());
  printf("\t    Align: %zd\n", this->Align());
  printf("\t     Size: %zd\n", this->Size());
  printf("\t    Value: %zd : ", this->_value.size());
  for (int i = 0; i < this->_value.size(); i++)
  {
    printf("%02X ", this->_value[i]);
  }
  printf("\n");
  return;
}

}
}
}
