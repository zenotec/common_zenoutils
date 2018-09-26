/*
 * Copyright (c) 2017 Cable Television Laboratories, Inc. ("CableLabs")
 *                    and others.  All rights reserved.
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

// libc includes
#include <stdlib.h>
#include <string.h>
#include <endian.h>

// libc++ includes
#include <iostream>

// libzutils includes
#include <zutils/zLog.h>
using namespace zUtils;
#include <zutils/ieee80211/Frame.h>

// local includes
#include "ieee80211.h"

ZLOG_MODULE_INIT(zLog::Log::MODULE_WIRELESS);

namespace zUtils
{
namespace zWireless
{
namespace ieee80211
{

//*****************************************************************************
// Class: Frame
//*****************************************************************************

Frame::Frame(const Frame::TYPE type_) :
    zSocket::Frame(zSocket::Frame::TYPE_80211),_fccntl(0), _durationid(0),
    _seqcntl(0), _qoscntl(0), _htcntl(0), _payload{ 0 }, _psize(0), _fcs(0)
{
   if (!this->Type(type_))
   {
     ZLOG_WARN("Error constructing frame: Unknown frame type: " + ZLOG_INT(type_));
   }
}

Frame::~Frame()
{
}

bool
Frame::Assemble(zSocket::Buffer& sb_, bool fcs_)
{

  // NOTE: Assumes caller's socket buffer data and tail are set to start of frame (empty)
  // NOTE: Returns socket buffer data pointer set to end of transmitter address field

  // Initialize frame pointer to start of data
  struct ieee80211_hdr* f = (struct ieee80211_hdr*)sb_.Data();

  // Write frame control field
  if (sb_.Put(sizeof(f->fc)))
  {
    f->fc = htole16(this->_fccntl);
    sb_.Pull(sizeof(f->fc));
  }
  else
  {
    ZLOG_ERR("Error assembling frame");
    return(false);
  }

  // Write duration field
  if (sb_.Put(sizeof(f->duration)))
  {
    f->duration = htole16(this->_durationid);
    sb_.Pull(sizeof(f->duration));
  }
  else
  {
    ZLOG_ERR("Error assembling frame");
    return(false);
  }

  // Write address 1 (receiver address)
  if (sb_.Put(sizeof(f->u.gen.addr1)) && this->str2mac(this->GetDestination(), f->u.gen.addr1))
  {
    sb_.Pull(sizeof(f->u.gen.addr1));
  }
  else
  {
    ZLOG_ERR("Missing address field: 1");
    return(false);
  }

  // Conditionally write address 2 (transmitter address)
  if (!this->GetSource().empty())
  {
    if (sb_.Put(sizeof(f->u.gen.addr2)) && this->str2mac(this->GetSource(), f->u.gen.addr2))
    {
      sb_.Pull(sizeof(f->u.gen.addr2));
    }
    else
    {
      ZLOG_ERR("Missing address field: 2");
      return (false);
    }
  }

  return (true);

}

bool
Frame::Disassemble(zSocket::Buffer& sb_, bool fcs_)
{

  // NOTE: Assumes caller's socket buffer data is set to start of frame and
  //   tail is set to end of frame (full)
  // NOTE: Returns socket buffer data pointer set to end of transmitter address field

  // Initialize frame pointer to start of data
  struct ieee80211_hdr* f = (struct ieee80211_hdr*)sb_.Data();

  // Check for FCS
  if (fcs_ && sb_.Pop(sizeof(this->_fcs)))
  {
    this->_fcs = *(uint32_t*)(sb_.Tail());

  }

  // Read frame control field
  if (sb_.Pull(sizeof(f->fc)))
  {
    this->_fccntl = le16toh(f->fc);
  }
  else
  {
    ZLOG_ERR("Error disassembling frame");
    return(false);
  }

  // Perform sanity check on version to determine if this is a valid frame
  if (this->Version())
  {
    ZLOG_ERR("Invalid header version: " + ZLOG_INT(this->Version()))
    return (NULL);
  }

  // Read duration field
  if (sb_.Pull(sizeof(f->duration)))
  {
    this->DurationId(le16toh(f->duration));
  }
  else
  {
    ZLOG_ERR("Error disassembling frame");
    return(false);
  }

  // Read address 1 (Receiver address) field
  if (sb_.Pull(sizeof(f->u.gen.addr1)))
  {
    this->SetAddress(ADDRESS_1, f->u.gen.addr1);
    this->SetDestination(this->GetAddress(Frame::ADDRESS_1));
  }
  else
  {
    ZLOG_ERR("Missing address field: 1");
    return(false);
  }

  // Verify presence of address 2 (transmitter address) field
  if (sb_.Length())
  {
    if (sb_.Pull(sizeof(f->u.gen.addr2)))
    {
      this->SetAddress(ADDRESS_2, f->u.gen.addr2);
      this->SetSource(this->GetAddress(Frame::ADDRESS_2));
    }
    else
    {
      fprintf(stderr, "Frame::Disassemble(%d)\n", fcs_);
      ZLOG_ERR("Missing address field: 2");
      return(false);
    }
  }

  return (true);

}

bool
Frame::Peek(const zSocket::Buffer& sb_, bool fcs_)
{

  // NOTE: Assumes caller's socket buffer data is set to start of frame and
  //   tail is set to end of frame (full)
  // NOTE: Does not alter caller's buffer

  // Create copy of caller's buffer to avoid modifying pointers
  // NOTE: Socket buffer copy only copies the pointers, not the buffer memory
  zSocket::Buffer tmp(sb_);

  // Disassemble frame using temporary local buffer
  return (this->Disassemble(tmp, fcs_));

}

// Deprecated - DO NOT USE
uint8_t*
Frame::Assemble(uint8_t* p_, size_t& rem_, bool fcs_)
{

  struct ieee80211_hdr* f = (struct ieee80211_hdr*) p_;

  p_ = this->chklen(p_, (sizeof(f->fc) + sizeof(f->duration)), rem_);
  if (!p_)
  {
    ZLOG_WARN("Error assembling frame: " + ZLOG_INT(rem_));
    return(NULL);
  }

  // Fill in the frame control field
  f->fc = htole16(this->_fccntl);
  f->duration = htole16(this->_durationid);

  return (p_);
}

// Deprecated - DO NOT USE
uint8_t*
Frame::Disassemble(uint8_t* p_, size_t& rem_, bool fcs_)
{

  size_t len = 0;
  struct ieee80211_hdr* f = (struct ieee80211_hdr*) p_;

  if (p_ && rem_ && fcs_)
  {
    if (!(this->_fcs = this->chkfcs(p_, rem_)))
    {
//      ZLOG_WARN("FCS check failed"); // TODO:
    }
  }

  p_ = this->chklen(p_, (sizeof(f->fc) + sizeof(f->duration)), rem_);
  if (!p_)
  {
    ZLOG_WARN("Error disassembling frame: " + ZLOG_INT(rem_));
    return(NULL);
  }

  this->_fccntl = le16toh(f->fc);

  // Perform sanity check on version to determine if this is a valid frame
  if (this->Version())
  {
    ZLOG_ERR("Invalid header version: " + ZLOG_INT(this->Version()))
    return (NULL);
  }

  this->_durationid = le16toh(f->duration);

  // Address 1 (Receiver address) field
  p_ = this->chklen(p_, sizeof(f->u.gen.addr1), rem_);
  if (!p_ || !this->SetAddress(ADDRESS_1, f->u.gen.addr1))
  {
    ZLOG_ERR("Missing address field: 1");
    return (NULL);
  }
  this->SetDestination(this->GetAddress(Frame::ADDRESS_1));


  // Address 2 (Transmitter address) field
  if (rem_)
  {
    p_ = this->chklen(p_, sizeof(f->u.gen.addr2), rem_);
    if (!p_ || !this->SetAddress(ADDRESS_2, f->u.gen.addr2))
    {
      ZLOG_ERR("Missing address field: 2");
      return (NULL);
    }
    this->SetSource(this->GetAddress(Frame::ADDRESS_2));
  }

  return (p_);
}

uint8_t*
Frame::Peek(uint8_t* p_, size_t len_, bool fcs_)
{
  size_t rem = len_;
  uint8_t* p = this->Disassemble(p_, rem, fcs_);
  return((p == NULL) ? NULL : p_);
}


// return pointer to next location in buffer; NULL on error
uint8_t*
Frame::AssembleTags(uint8_t* p_, size_t& rem_, uint8_t tagtype_)
{
  struct ieee80211_tag* t = (struct ieee80211_tag*) p_;
  std::map<int, std::vector<Tag> >* tags;

  int type_begin = (tagtype_ & TAGTYPE_HEAD) ? Tag::TYPE_HEAD : Tag::TYPE_TAIL;
  int type_end = (tagtype_ & TAGTYPE_TAIL) ? Tag::TYPE_LAST : Tag::TYPE_TAIL;

  for (int i = type_begin; i < type_end; i++)
  {
      FOREACH(auto& tag, this->_tags[i])
      {
        if (tag.Valid())
        {
          // Some tags have zero length data
          p_ = this->chklen(p_, (sizeof(t->tag) + sizeof(t->len) + tag.Length()), rem_);
          if (!p_)
          {
            break;
          }
          t->tag = tag.Id();
          t->len = tag.Length();
          tag.GetValue(t->val, t->len);
          t = (struct ieee80211_tag*) p_;
        }
      }
  }
  return (p_);
}

uint8_t*
Frame::DisassembleTags(uint8_t* p_, size_t& rem_, uint8_t tagtype_)
{

  struct ieee80211_tag* t = (struct ieee80211_tag*)p_;

  // Clear out any previously "put/added" tags
  this->ClearTags(tagtype_);

  // Loop until the end of the specified buffer; rem_ should be zero when finished
  while (p_ && rem_)
  {
    size_t tmp_rem = rem_;
    uint8_t* tmp_p = this->chklen(p_, (sizeof(t->tag) + sizeof(t->len) + t->len), tmp_rem);
    if (tmp_p)
    {
      Tag tag((Tag::ID)t->tag, t->len);
      uint8_t type = TAGTYPE_NONE;
      switch (tag.Type())
      {
      case Tag::TYPE_HEAD:
        type = TAGTYPE_HEAD;
        break;
      case Tag::TYPE_TAIL:
        type = TAGTYPE_TAIL;
        break;
      default:
        break;
      }
      if (type & tagtype_)
      {
        tag.PutValue(t->val, t->len);
        this->_tags[tag.Type()].push_back(tag);
        p_ = tmp_p;
        rem_ = tmp_rem;
        t = (struct ieee80211_tag*)p_;
      }
      else
      {
        break;
      }
    }
    else
    {
      break; // TODO: KEM: Not sure what effect this will have yet
    }
  }

  // Return pointer to end of buffer; NULL on error
  return(p_);
}

void
Frame::ClearTags(uint8_t tagtype_)
{

  // Clear out any previously "put/added" tags
  int type_begin = (tagtype_ & TAGTYPE_HEAD) ? Tag::TYPE_HEAD : Tag::TYPE_TAIL;
  int type_end = (tagtype_ & TAGTYPE_TAIL) ? Tag::TYPE_LAST : Tag::TYPE_TAIL;
  for (int i = type_begin; i < type_end; i++)
  {
    this->_tags[i].clear();
  }

}

uint8_t
Frame::Version() const
{
  return ((this->_fccntl >> 0) & 0x03);
}

bool
Frame::Version(const uint8_t version_)
{
  this->_fccntl &= ~(0x0003 << 0);
  this->_fccntl |= ((version_ & 0x0003) << 0);
  return (true);
}

Frame::TYPE
Frame::Type() const
{
  return (Frame::TYPE((this->_fccntl >> 2) & 0x0003));
}

bool
Frame::Type(const Frame::TYPE type_)
{
  bool status = false;
  if ((type_ > TYPE_ERR) && (type_ < TYPE_LAST))
  {
    this->_fccntl &= ~(0x0003 << 2);
    this->_fccntl |= ((type_ & 0x0003) << 2);
    status = true;
  }
  return (status);
}

Frame::SUBTYPE
Frame::Subtype() const
{
  return (Frame::SUBTYPE((this->_fccntl >> 4) & 0x000f));
}

bool
Frame::Subtype(const Frame::SUBTYPE stype_)
{
  bool status = false;
  if ((stype_ > SUBTYPE_ERR) && (stype_ < SUBTYPE_LAST))
  {
    this->_fccntl &= ~(0x000f << 4);
    this->_fccntl |= ((stype_ & 0x000f) << 4);
    status = true;
  }
  return (status);
}

bool
Frame::ToDS() const
{
  return (!!(this->_fccntl & (0x0001 << 8)));
}

bool
Frame::ToDS(const bool flag_)
{
  this->_fccntl &= ~(0x0001 << 8);
  this->_fccntl |= ((flag_ & 0x0001) << 8);
  return (true);
}

bool
Frame::FromDS() const
{
  return (!!(this->_fccntl & (0x0001 << 9)));
}

bool
Frame::FromDS(const bool flag_)
{
  this->_fccntl &= ~(0x0001 << 9);
  this->_fccntl |= ((flag_ & 0x0001) << 9);
  return (true);
}

bool
Frame::MoreFragments() const
{
  return (!!(this->_fccntl & (0x0001 << 10)));
}

bool
Frame::MoreFragments(const bool flag_)
{
  this->_fccntl &= ~(0x0001 << 10);
  this->_fccntl |= ((flag_ & 0x0001) << 10);
  return (true);
}

bool
Frame::Retry() const
{
  return (!!(this->_fccntl & (0x0001 << 11)));
}

bool
Frame::Retry(const bool flag_)
{
  this->_fccntl &= ~(0x0001 << 11);
  this->_fccntl |= ((flag_ & 0x0001) << 11);
  return (true);
}

bool
Frame::PowerManagement() const
{
  return (!!(this->_fccntl & (0x0001 << 12)));
}

bool
Frame::PowerManagement(const bool flag_)
{
  this->_fccntl &= ~(0x0001 << 12);
  this->_fccntl |= ((flag_ & 0x0001) << 12);
  return (true);
}

bool
Frame::MoreData() const
{
  return (!!(this->_fccntl & (0x0001 << 13)));
}

bool
Frame::MoreData(const bool flag_)
{
  this->_fccntl &= ~(0x0001 << 13);
  this->_fccntl |= ((flag_ & 0x0001) << 13);
  return (true);
}

bool
Frame::Protected() const
{
  return (!!(this->_fccntl & (0x0001 << 14)));
}

bool
Frame::Protected(const bool flag_)
{
  this->_fccntl &= ~(0x0001 << 14);
  this->_fccntl |= ((flag_ & 0x0001) << 14);
  return (true);
}

bool
Frame::Order() const
{
  return (!!(this->_fccntl & (0x0001 << 15)));
}

bool
Frame::Order(const bool flag_)
{
  this->_fccntl &= ~(0x0001 << 15);
  this->_fccntl |= ((flag_ & 0x0001) << 15);
  return (true);
}

uint16_t
Frame::DurationId() const
{
  return (this->_durationid);
}

bool
Frame::DurationId(const uint16_t durid_)
{
  this->_durationid = durid_;
  return (true);
}

std::string
Frame::GetAddress(const ADDRESS_ID id_) const
{
  std::string addr = "Err";
  if ((id_ > Frame::ADDRESS_ERR) && (id_ < Frame::ADDRESS_LAST))
  {
    addr = this->_address[id_];
  }
  return (addr);
}

bool
Frame::GetAddress(const ADDRESS_ID id_, uint8_t* address_)
{
  bool status = false;
  std::string addr;
  if ((id_ > Frame::ADDRESS_ERR) && (id_ < Frame::ADDRESS_LAST))
  {
    status = this->str2mac(this->_address[id_], address_);
  }
  return (status);
}

bool
Frame::SetAddress(const ADDRESS_ID id_, const std::string& address_)
{
  bool status = false;
  switch (id_)
  {
  case Frame::ADDRESS_1:
    this->SetDestination(address_); // TODO: this will go away
    this->_address[id_] = address_;
    status = true;
    break;
  case Frame::ADDRESS_2:
    this->SetSource(address_);
    this->_address[id_] = address_;
    status = true;
  case Frame::ADDRESS_3:
    // no break
  case Frame::ADDRESS_4:
    this->_address[id_] = address_;
    status = true;
  default:
    break;
  }
  return (status);
}

bool
Frame::SetAddress(const ADDRESS_ID id_, const uint8_t* address_)
{
  bool status = false;
  std::string addr;
  if (this->mac2str(address_, addr))
  {
    status = this->SetAddress(id_, addr);
  }
  return (status);
}

uint8_t
Frame::FragmentNum() const
{
  return ((this->_seqcntl & 0x000f));
}

bool
Frame::FragmentNum(const uint8_t num_)
{
  this->_seqcntl &= 0xfff0;
  this->_seqcntl |= (num_ & 0x000f);
  return (true);
}

uint16_t
Frame::SequenceNum() const
{
  return ((this->_seqcntl >> 4));
}

bool
Frame::SequenceNum(const uint16_t num_)
{
  this->_seqcntl &= 0x000f;
  this->_seqcntl |= ((num_ << 4) & 0xfff0);
  return (true);
}

uint16_t
Frame::SequenceControl() const
{
  return (this->_seqcntl);
}

bool
Frame::SequenceControl(const uint16_t cntl_)
{
  this->_seqcntl = cntl_;
  return (true);
}

uint16_t
Frame::QosControl() const
{
  return (this->_qoscntl);
}

bool
Frame::QosControl(const uint16_t cntl_)
{
  this->_qoscntl = cntl_;
  return (true);
}

uint32_t
Frame::HtControl() const
{
  return (this->_htcntl);
}

bool
Frame::HtControl(const uint32_t cntl_)
{
  this->_htcntl = cntl_;
  return (true);
}

bool
Frame::GetTag(Tag& tag_, const int index_)
{
  bool status = false;
  int cnt = 0;

  std::list<Tag>::iterator it = this->_tags[tag_.Type()].begin();
  std::list<Tag>::iterator end = this->_tags[tag_.Type()].end();
  for (; it != end; ++it)
  {
    if (it->Id() == tag_.Id())
    {
      if (cnt++ == index_)
      {
        tag_ = *it;
        status = true;
        break;
      }
    }
  }

  return (status);
}

bool
Frame::PutTag(const Tag& tag_, const int index_)
{
  bool status = false;
  int cnt = 0;

  std::list<Tag>::iterator it = this->_tags[tag_.Type()].begin();
  std::list<Tag>::iterator end = this->_tags[tag_.Type()].end();
  for (; it != end; ++it)
  {
    if (it->Id() == tag_.Id())
    {
      if (cnt++ == index_)
      {
        *it = tag_;
        status = true;
        break;
      }
    }
  }

  if (!status)
  {
    // Create new tag at end of list
    this->_tags[tag_.Type()].emplace_back(tag_);
    status = true;
  }

  return (status);
}

uint32_t
Frame::FrameCheck() const
{
  return(this->_fcs);
}

bool
Frame::FrameCheck(const uint32_t fcs_)
{
  this->_fcs = fcs_;
  return(true);
}

size_t
Frame::GetPayload(uint8_t* buf_, const size_t len_) const
{
  size_t cnt = std::min(this->_psize, len_);
  memcpy(buf_, this->_payload, cnt);
  return(cnt);
}

size_t
Frame::GetPayloadLength() const
{
  return(this->_psize);
}

bool
Frame::PutPayload(const uint8_t* buf_, const size_t len_)
{
  bool status = false;

  if (buf_ && (len_ < FRAME_PAYLOAD_MAXLEN))
  {
    this->_psize = len_;
    status = (memcpy(this->_payload, buf_, len_) == this->_payload);
  }
  return (status);
}

void
Frame::Display() const
{
  std::cout << "------------------------------------------" << std::endl;
  std::cout << "----- IEEE802.11 Header ------------------" << std::endl;
  std::cout << "\tVersion:  \t" << (int) this->Version() << std::endl;
  std::cout << "\tType:     \t" << std::hex << (int) this->Type() << std::dec << std::endl;
  std::cout << "\tSubtype:  \t" << std::hex << (int) this->Subtype() << std::dec << std::endl;
  std::cout << "\tToDS:     \t" << (this->ToDS() ? "true" : "false") << std::endl;
  std::cout << "\tFromDS:   \t" << (this->FromDS() ? "true" : "false") << std::endl;
  std::cout << "\tFrag:     \t" << (this->MoreFragments() ? "true" : "false") << std::endl;
  std::cout << "\tRetry:    \t" << (this->Retry() ? "true" : "false") << std::endl;
  std::cout << "\tProtect:  \t" << (this->Protected() ? "true" : "false") << std::endl;
  std::cout << "\tOrder:    \t" << (this->Order() ? "true" : "false") << std::endl;
  std::cout << "\tDuration: \t" << (int) this->DurationId() << std::endl;
  printf("fccntl: 0x%04x\n", this->_fccntl);
}

bool
Frame::str2mac(const std::string& addr_, uint8_t* hwaddr_)
{
  return (sscanf(addr_.c_str(), "%hhx:%hhx:%hhx:%hhx:%hhx:%hhx", &hwaddr_[0], &hwaddr_[1],
      &hwaddr_[2], &hwaddr_[3], &hwaddr_[4], &hwaddr_[5]) == ETH_ALEN);
}

bool
Frame::mac2str(const uint8_t* hwaddr_, std::string& addr_)
{
  bool status = false;
  char str[256] = { 0 };
  if (sprintf(str, "%02hhx:%02hhx:%02hhx:%02hhx:%02hhx:%02hhx", hwaddr_[0], hwaddr_[1], hwaddr_[2],
      hwaddr_[3], hwaddr_[4], hwaddr_[5]) > 0)
  {
    addr_ = std::string(str);
    status = true;
  }
  return (status);
}

uint8_t*
Frame::chklen(uint8_t* f_, size_t len_, size_t& rem_)
{
  if (!f_ || (len_ > rem_))
  {
    return(NULL);
  }
  rem_ -= len_;
  return(f_ + len_);
}

uint32_t
Frame::chkfcs(uint8_t* f_, size_t& rem_)
{
  rem_ -= sizeof(uint32_t);
  return(0); // TODO: implement FCS computation
}

}
}
}
