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

// local includes
#include "Frame.h"

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
    _version(0), _type(type_), _subtype(Frame::SUBTYPE_ERR), _tods(false), _fromds(false),
    _morefrag(false), _retry(false), _pwrmgmt(false), _moredata(false), _protected(false),
    _order(false), _durationid(0), _seqcntl(0), _qoscntl(0), _htcntl(0), _fcs(0)
{
}

Frame::~Frame()
{
}

uint8_t*
Frame::Assemble(uint8_t* p_, size_t& rem_, bool fcs_)
{

  struct ieee80211_hdr* f = (struct ieee80211_hdr*) p_;

  p_ = this->chklen(p_, (sizeof(f->fc) + sizeof(f->duration)), rem_);
  if (!p_)
  {
    return(NULL);
  }

  // Fill in the frame control field
  uint16_t fc = 0;
  fc = ((this->Version() & 0x03) << 0);
  fc |= ((this->Type() & 0x03) << 2);
  fc |= ((this->Subtype() & 0x0f) << 4);
  fc |= (this->ToDS() << 8);
  fc |= (this->FromDS() << 9);
  fc |= (this->MoreFragments() << 10);
  fc |= (this->Retry() << 11);
  fc |= (this->PowerManagement() << 12);
  fc |= (this->MoreData() << 13);
  fc |= (this->Protected() << 14);
  fc |= (this->Order()<< 15);
  f->fc = htole16(fc);

  f->duration = htole16(this->_durationid);

  return (p_);
}

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

  p_ = this->chklen(p_, sizeof(f->fc), rem_);
  if (!p_)
  {
    ZLOG_WARN("Error disassembling frame: " + ZLOG_INT(rem_));
    return(NULL);
  }

  // Decode the frame control field
  uint16_t fc = le16toh(f->fc);
  this->Version((fc >> 0) & 0x03);
  if (this->Version() != 0)
  {
    ZLOG_ERR("Invalid header version: " + ZLOG_INT(this->Version()))
    return (NULL);
  }
  if (!this->Type(Frame::TYPE(((fc >> 2) & 0x03))))
  {
    ZLOG_ERR("Invalid type: " + ZLOG_INT(this->Type()))
    return(NULL);
  }
  if(!this->Subtype(Frame::SUBTYPE(((fc >> 4) & 0x0f))))
  {
    ZLOG_ERR("Invalid subtype: " + ZLOG_INT(this->Subtype()))
    return(NULL);
  }
  if(!this->ToDS(!!(fc & (1 << 8))))
  {
    return(NULL);
  }
  if(!this->FromDS(!!(fc & (1 << 9))))
  {
    return(NULL);
  }
  if(!this->MoreFragments(!!(fc & (1 << 10))))
  {
    return(NULL);
  }
  if(!this->Retry(!!(fc & (1 << 11))))
  {
    return(NULL);
  }
  if(!this->PowerManagement(!!(fc & (1 << 12))))
  {
    return(NULL);
  }
  if(!this->MoreData(!!(fc & (1 << 13))))
  {
    return(NULL);
  }
  if(!this->Protected(!!(fc & (1 << 14))))
  {
    return(NULL);
  }
  if(!this->Order(!!(fc & (1 << 15))))
  {
    return(NULL);
  }

  p_ = this->chklen(p_, sizeof(f->duration), rem_);
  if (!p_)
  {
    ZLOG_ERR("Missing duration field");
    return (NULL);
  }
  this->DurationId(le16toh(f->duration));

  return (p_);
}

uint8_t*
Frame::Peek(uint8_t* p_, size_t len_, bool fcs_)
{
  size_t rem = len_;
  uint8_t* p = this->Disassemble(p_, rem);
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
    FOREACH(auto& tagmap, this->_tags[i])
    {
      FOREACH(auto& tag, tagmap.second)
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
  }
  return (p_);
}

uint8_t*
Frame::DisassembleTags(uint8_t* p_, size_t& rem_, uint8_t tagtype_)
{

  struct ieee80211_tag* t = (struct ieee80211_tag*)p_;

  // Clear out any previously "put/added" tags
  int type_begin = (tagtype_ & TAGTYPE_HEAD) ? Tag::TYPE_HEAD : Tag::TYPE_TAIL;
  int type_end = (tagtype_ & TAGTYPE_TAIL) ? Tag::TYPE_LAST : Tag::TYPE_TAIL;
  for (int i = type_begin; i < type_end; i++)
  {
    this->_tags[i].clear();
  }

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
        this->_tags[tag.Type()][tag.Id()].push_back(tag);
        p_ = tmp_p;
        rem_ = tmp_rem;
        t = (struct ieee80211_tag*)p_;
      }
      else
      {
        break;
      }
    }
  }

  // Return pointer to end of buffer; NULL on error
  return(p_);
}

uint8_t
Frame::Version() const
{
  return (this->_version);
}

bool
Frame::Version(const uint8_t version_)
{
  this->_version = version_;
  return (true);
}

Frame::TYPE
Frame::Type() const
{
  return (this->_type);
}

bool
Frame::Type(const Frame::TYPE type_)
{
  bool status = false;
  if (type_ < TYPE_LAST)
  {
    this->_type = type_;
    status = true;
  }
  return (status);
}

Frame::SUBTYPE
Frame::Subtype() const
{
  return (this->_subtype);
}

bool
Frame::Subtype(const Frame::SUBTYPE stype_)
{
  this->_subtype = stype_;
  return (true);
}

bool
Frame::ToDS() const
{
  return (this->_tods);
}

bool
Frame::ToDS(const bool flag_)
{
  this->_tods = flag_;
  return (true);
}

bool
Frame::FromDS() const
{
  return (this->_fromds);
}

bool
Frame::FromDS(const bool flag_)
{
  this->_fromds = flag_;
  return (true);
}

bool
Frame::MoreFragments() const
{
  return (this->_morefrag);
}

bool
Frame::MoreFragments(const bool flag_)
{
  this->_morefrag = flag_;
  return (true);
}

bool
Frame::Retry() const
{
  return (this->_retry);
}

bool
Frame::Retry(const bool flag_)
{
  this->_retry = flag_;
  return (true);
}

bool
Frame::PowerManagement() const
{
  return (this->_pwrmgmt);
}

bool
Frame::PowerManagement(const bool flag_)
{
  this->_pwrmgmt = flag_;
  return (true);
}

bool
Frame::MoreData() const
{
  return (this->_moredata);
}

bool
Frame::MoreData(const bool flag_)
{
  this->_moredata = flag_;
  return (true);
}

bool
Frame::Protected() const
{
  return (this->_protected);
}

bool
Frame::Protected(const bool flag_)
{
  this->_protected = flag_;
  return (true);
}

bool
Frame::Order() const
{
  return (this->_order);
}

bool
Frame::Order(const bool flag_)
{
  this->_order = flag_;
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
Frame::Address(const ADDRESS_ID id_) const
{
  std::string addr = "Err";
  if ((id_ > Frame::ADDRESS_ERR) && (id_ < Frame::ADDRESS_LAST))
  {
    addr = this->_address[id_];
  }
  return (addr);
}

bool
Frame::Address(const ADDRESS_ID id_, const std::string& address_)
{
  bool status = false;
  if ((id_ > Frame::ADDRESS_ERR) && (id_ < Frame::ADDRESS_LAST))
  {
    this->_address[id_] = address_;
    status = true;
  }
  return (status);
}

bool
Frame::Address(const ADDRESS_ID id_, const uint8_t* address_)
{
  bool status = false;
  std::string addr;
  if (this->mac2str(address_, addr))
  {
    status = this->Address(id_, addr);
  }
  return (status);
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

  if (this->_tags[tag_.Type()].count(tag_.Id()))
  {
    tag_ = this->_tags[tag_.Type()][tag_.Id()][index_];
    status = true;
  }

  return (status);
}

bool
Frame::PutTag(const Tag& tag_, const int index_)
{
  // Conditionally resize map to account for additional tag
  if (index_ >= this->_tags[tag_.Type()][tag_.Id()].size())
  {
    this->_tags[tag_.Type()][tag_.Id()].resize(index_ + 1);
  }
  // Copy tag
  this->_tags[tag_.Type()][tag_.Id()][index_] = tag_;
  return (true);
}

bool
Frame::AddTag(const Tag& tag_)
{
  this->_tags[tag_.Type()][tag_.Id()].push_back(tag_);
  return (true);
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
Frame::GetPayload(uint8_t*& buf_, const size_t len_) const
{
  size_t cnt = std::min(this->_payload.size(), len_);
  memcpy(buf_, this->_payload.data(), cnt);
  return(cnt);
}

size_t
Frame::GetPayloadLength() const
{
  return(this->_payload.size());
}

bool
Frame::PutPayload(const uint8_t* buf_, const size_t len_)
{
  bool status = false;

  if (!len_)
  {
    ZLOG_WARN("Zero length payload");
  }

  if (buf_ && (len_ < 7952))
  {
    this->_payload.clear();
    this->_payload.resize(len_);
    status = (memcpy(this->_payload.data(), buf_, len_) == this->_payload.data());
  }
  return (status);
}

void
Frame::Display() const
{
  std::cout << "------------------------------------------" << std::endl;
  std::cout << "----- IEEE802.11 Header ------------------" << std::endl;
  std::cout << "\tVersion:  \t" << (int) this->Version() << std::endl;
  std::cout << "\tType:     \t" << std::hex << (int) this->Type() << std::endl;
  std::cout << "\tSubtype:  \t" << std::hex << (int) this->Subtype() << std::endl;
  std::cout << "\tToDS:     \t" << (this->ToDS() ? "true" : "false") << std::endl;
  std::cout << "\tFromDS:   \t" << (this->FromDS() ? "true" : "false") << std::endl;
  std::cout << "\tFrag:     \t" << (this->MoreFragments() ? "true" : "false") << std::endl;
  std::cout << "\tRetry:    \t" << (this->Retry() ? "true" : "false") << std::endl;
  std::cout << "\tProtect:  \t" << (this->Protected() ? "true" : "false") << std::endl;
  std::cout << "\tOrder:    \t" << (this->Order() ? "true" : "false") << std::endl;
  std::cout << "\tDuration: \t" << (int) this->DurationId() << std::endl;
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
