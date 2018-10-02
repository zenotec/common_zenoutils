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

#include <zutils/zUtils.h>
#include <zutils/zLog.h>
using namespace zUtils;
#include <zutils/ieee80211/RadioTap.h>

ZLOG_MODULE_INIT(zLog::Log::MODULE_WIRELESS);

namespace zUtils
{
namespace zWireless
{
namespace ieee80211
{

struct radiotap_header
{
  uint8_t version;
  uint8_t pad; // header pad is unused, per the spec (just aligns the header to 32 bits)
  uint16_t length;
  uint32_t present[];
} __attribute__ ((packed));

static uint32_t
_fieldid(const uint32_t id_, const int index_)
{
  return((index_ * 32) + id_);
}

//*****************************************************************************
// Class: RadioTap
//*****************************************************************************

RadioTap::RadioTap() :
    _version(0), _length(0)
{
}

RadioTap::~RadioTap()
{
}

bool
RadioTap::Assemble(zSocket::Buffer& sb_)
{
  struct radiotap_header* hdr = (struct radiotap_header*) sb_.Data();

  if (sb_.Put(sizeof(hdr->version)))
  {
    hdr->version = this->GetVersion();
    sb_.Pull(sizeof(hdr->version));
  }
  else
  {
    ZLOG_ERR("Error assembling RadioTap header - version");
    return (false);
  }

  if (sb_.Put(sizeof(hdr->pad)))
  {
    hdr->pad = 0;
    sb_.Pull(sizeof(hdr->pad)); // header pad is unused, per the spec (just aligns the header to 32 bits)
  }
  else
  {
    ZLOG_ERR("Error assembling RadioTap header - pad");
    return (false);
  }

  if (sb_.Put(sizeof(hdr->length)))
  {
    hdr->length = 0;
    sb_.Pull(sizeof(hdr->length));
  }
  else
  {
    ZLOG_ERR("Error assembling RadioTap header - length");
    return (false);
  }

  // Assemble header
  this->_length = sizeof(struct radiotap_header);

  // Loop through present vector and update header
  int cnt = 0;
  FOREACH (auto& present, this->_present) {
  if (sb_.Put(sizeof(present)))
  {
    hdr->present[cnt++] = htole32(present);
    this->_length += sizeof(present);
    sb_.Pull(sizeof(present));
  }
  else
  {
    ZLOG_ERR("Error assembling RadioTap header - present flag");
    return(false);
  }
}

// Loop through all fields and update header
  size_t pad = 0;
  FOREACH (auto& field, this->_fields) {
  if (field.second.Assemble(sb_, (uint8_t*)hdr, pad))
  {
    this->_length += (field.second.Size() + pad);
  }
  else
  {
    ZLOG_ERR("Error assembling RadioTap header - fields");
    return(false);
  }
}

// Update header length
  hdr->length = htole16(this->_length);

  return true;

}

bool
RadioTap::Disassemble(zSocket::Buffer& sb_)
{
  int cnt = 0;
  radiotap_header *hdr = (radiotap_header *) sb_.Data();
  size_t pad = 0;

  // Validate version and length
  if ((hdr->version != 0) || (le16toh(hdr->length) > sb_.Size()))
  {
    ZLOG_WARN("Invalid version or length: " + ZLOG_UINT(le16toh(hdr->length)));
    return false;
  }

  // Save version and length and pad
  if (sb_.Pull(sizeof(hdr->version)))
  {
    this->_version = hdr->version;
  }
  else
  {
    ZLOG_ERR("Missing version field");
    return (false);
  }

  if (sb_.Pull(sizeof(hdr->pad)))
  {
    pad = hdr->pad; // header pad is unused, per the spec (just aligns the header to 32 bits)
  }
  else
  {
    ZLOG_ERR("Missing pad field");
    return (false);
  }

  if (sb_.Pull(sizeof(hdr->length)))
  {
    this->_length = le16toh(hdr->length);
  }
  else
  {
    ZLOG_ERR("Missing length field");
    return (false);
  }

  // Loop through the present field(s) and save
  for (int cnt = 0; cnt < 8; cnt++)
  {
    uint32_t present = 0;
    if (sb_.Pull(sizeof(uint32_t)))
    {
      present = le32toh(hdr->present[cnt]);
      this->_present.push_back(present);
    }
    else
    {
      ZLOG_ERR("Missing present field");
      return (false);
    }

    // Test for additional present fields
    if (not (present & (1 << RadioTapField::ID_EXT)))
    {
      break;
    }

  }

  for (int cnt = 0; cnt < this->_present.size(); cnt++)
  {
    uint32_t present = this->_present[cnt];
    for (uint32_t bit = 1, id = 0; id < RadioTapField::ID_LAST; bit <<= 1, id++)
    {
      if (bit & present)
      {
        RadioTapField field((RadioTapField::ID) id);
        if (not field.Disassemble(sb_, (uint8_t*) hdr, pad))
        {
          ZLOG_ERR("Missing extra present field");
          return false;
        }
        if (not this->PutField(field, cnt))
        {
          ZLOG_ERR("Failed to PutField");
          return false;
        }
      }
    }
  }

  return true;

}


uint8_t
RadioTap::GetVersion() const
{
  return (this->_version);
}

uint16_t
RadioTap::GetLength() const
{
  return (this->_length);
}

uint32_t
RadioTap::GetPresent(const int index_) const
{
  uint32_t present = 0;
  if (index_ < this->_present.size())
  {
    present = this->_present[index_];
  }
  return (present);
}

bool
RadioTap::GetField(RadioTapField& field_, const int index_)
{
  bool status = false;
  if (this->_fields.count(_fieldid(field_.Id(), index_)))
  {
    field_ = this->_fields[_fieldid(field_.Id(), index_)];
    status = true;
  }
  return (status);
}

bool
RadioTap::PutField(const RadioTapField& field_, const int index_)
{
  uint32_t present = 0;

  // Conditionally increase present vector size
  if (index_ >= this->_present.size())
  {
    this->_present.resize(index_ + 1);
  }

  // Conditionally set previous present vector extra flags
  if (index_)
  {
    present = this->_present[index_ - 1];
    present |= (1 << RadioTapField::ID_EXT);
    present |= (1 << RadioTapField::ID_RADIOTAP_NAMESPACE);
    this->_present[index_ - 1] = present;
  }

  // Set present flag for current field identifier
  present = this->_present[index_];
  present |= (1 << field_.Id());
  this->_present[index_] = present;
  this->_fields[_fieldid(field_.Id(), index_)] = field_;

  // Return success
  return (true);

}

uint8_t*
RadioTap::_chklen(uint8_t* f_, size_t len_, size_t& rem_)
{
  if (!f_ || (len_ > rem_))
  {
    return(NULL);
  }
  rem_ -= len_;
  return(f_ + len_);
}

void
RadioTap::Display() const
{
  printf("----- RadioTap Frame ---------------------\n");
  printf("     Version: %hhu\n", this->GetVersion());
  printf("      Length: %hu\n", this->GetLength());
  int cnt = 0;
  FOREACH (auto& present, this->_present)
  {
    printf("  Present[%d]: 0x%08X\n", cnt++, present);
  }

  FOREACH (auto& field, this->_fields)
  {
    int index = 0;
    int id = field.first;
    if (id)
    {
      index = (id / 32);
      id %= 32;
    }
    printf("  Field[%d:%d]: %d\n", index, id, field.first);
    field.second.Display();
  }

  return;
}

}
}
}
