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
#include <zutils/zSocket.h>

#include "RadioTapField.h"
#include "RadioTap.h"

namespace ieee80211
{

struct radiotap_header
{
  uint8_t version;
  uint8_t pad;
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

uint8_t*
RadioTap::Assemble(uint8_t* frame_, size_t& rem_)
{

  struct radiotap_header* hdr = (struct radiotap_header*)frame_;
  size_t offset = ((unsigned long)frame_ & 0x7);
  size_t pad = 0;

  frame_ = this->_chklen(frame_, sizeof(struct radiotap_header), rem_);
  if (offset || !frame_)
  {
    return(NULL);
  }

  // Assemble header
  hdr->version = this->GetVersion();
  hdr->pad = 0;
  hdr->length = 0;
  this->_length = sizeof(struct radiotap_header);

  // Loop through present vector and update header
  int cnt = 0;
  FOREACH (auto& present, this->_present)
  {
    frame_ = this->_chklen(frame_, sizeof(present), rem_);
    if (!frame_)
    {
      return(NULL);
    }
    hdr->present[cnt++] = present;
//    hdr->present[cnt++] = htobe32(present);
    this->_length += sizeof(present);
  }

  // Loop through all fields and update header
  FOREACH (auto& field, this->_fields)
  {
    frame_ = field.second.Assemble(frame_, rem_, pad);
    if (!frame_)
    {
      return (NULL);
    }
    this->_length += (field.second.Size() + pad);
  }

  // Update header length
  hdr->length = this->_length;
//  hdr->length = htobe16(this->_length);

  return (frame_);
}

uint8_t*
RadioTap::Disassemble(uint8_t* frame_, size_t& rem_)
{

  int cnt = 0;
  radiotap_header *hdr = (radiotap_header *) frame_;
  size_t offset = ((unsigned long)frame_ & 0x7);
  size_t pad = 0;

//  printf("\nRadioTap::Disassemble(%p, %zd)\n", frame_, rem_);

  // Validate version and length
  if (offset || (hdr->version != 0) || (hdr->length > rem_))
  {
    return (NULL);
  }

  // Verify and increment frame pointer and remaining bytes
  frame_ = this->_chklen(frame_, sizeof(struct radiotap_header), rem_);
  if (!frame_)
  {
    return(NULL);
  }

//  printf("\nRadioTap::Disassemble(%p, %zd)\n", frame_, rem_);

  // Save version and length
  this->_version = hdr->version;
  this->_length = hdr->length;
//  this->_length = be16toh(hdr->length);

  // Loop through the present field(s) and save
  for (int cnt = 0; cnt < 8; cnt++)
  {
    // Verify and increment frame pointer and remaining bytes
    frame_ = this->_chklen(frame_, sizeof(uint32_t), rem_);
    if (!frame_)
    {
      return(NULL);
    }

    // Save present field
    uint32_t present = hdr->present[cnt];
//    uint32_t present = be32toh(hdr->present[cnt]);
    this->_present.push_back(present);

    // Test for additional present fields
    if (!(present & (1 << RadioTapField::ID_EXT)))
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
        frame_ = field.Disassemble(frame_, rem_, pad);
        if (!frame_ || !this->PutField(field, cnt))
        {
          return (NULL);
        }
      }
    }
  }

  return (frame_);
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
