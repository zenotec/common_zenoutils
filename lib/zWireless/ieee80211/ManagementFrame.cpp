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
#include <endian.h>

// libc++ includes

// libzutils includes

#include <zutils/zLog.h>
#include <zutils/zSocket.h>
#include <zutils/ieee80211/ManagementFrame.h>
using namespace zUtils;

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
// Class: ManagementFrame
//*****************************************************************************

ManagementFrame::ManagementFrame(const ManagementFrame::SUBTYPE subtype_) :
    Frame(Frame::TYPE_MGMT)
{
  this->Subtype(subtype_);
}

ManagementFrame::~ManagementFrame()
{
}

bool
ManagementFrame::Assemble(zSocket::Buffer& sb_)
{

  // NOTE: Assumes caller's socket buffer data and tail are set to start of frame (empty)

  // Initialize frame pointer to start of data
  struct ieee80211_hdr* f = (struct ieee80211_hdr*) sb_.Data();

  // Assemble lower level frame and validate
  if (!Frame::Assemble(sb_) || (this->Type() != Frame::TYPE_MGMT))
  {
    ZLOG_ERR("Error assembling frame");
    return (false);
  }

  // Write address 3 (BSSID)
  if (!this->GetAddress(Frame::ADDRESS_3).empty())
  {
    if (sb_.Put(sizeof(f->u.data.addr3))
        && this->str2mac(this->GetAddress(Frame::ADDRESS_3), f->u.data.addr3))
    {
      sb_.Pull(sizeof(f->u.data.addr3));
    }
    else
    {
      ZLOG_ERR("Error assembling frame");
      return (false);
    }
  }
  else
  {
    ZLOG_ERR("Missing address field: 3");
    return (false);
  }

  // Write sequence control field
  if (sb_.Put(sizeof(f->u.data.seqcntl)))
  {
    f->u.data.seqcntl = htole16(this->SequenceControl());
    sb_.Pull(sizeof(f->u.data.seqcntl));
  }
  else
  {
    ZLOG_ERR("Error assembling frame");
    return (false);
  }

  return (true);

}

bool
ManagementFrame::Disassemble(zSocket::Buffer& sb_)
{

  // NOTE: Assumes caller's socket buffer data is set to start of frame and
  //   tail is set to end of frame (full)

  // Initialize frame pointer to start of data
  struct ieee80211_hdr* f = (struct ieee80211_hdr*) sb_.Data();

  // Disassemble lower level frame and validate
  if (!Frame::Disassemble(sb_) || (this->Type() != Frame::TYPE_MGMT))
  {
    ZLOG_ERR("Error disassembling frame: " + ZLOG_INT(this->Type()));
    return (false);
  }

  // Read address 3 (BSSID) field
  if (sb_.Pull(sizeof(f->u.data.addr3)))
  {
    this->SetAddress(ADDRESS_3, f->u.data.addr3);
  }
  else
  {
    ZLOG_ERR("Missing address field: 3");
    return (false);
  }

  // Read sequence control field
  if (sb_.Pull(sizeof(f->u.data.seqcntl)))
  {
    this->SequenceControl(le16toh(f->u.data.seqcntl));
  }
  else
  {
    ZLOG_ERR("Error disassembling frame");
    return (false);
  }

  return (true);

}

uint8_t*
ManagementFrame::Assemble(uint8_t* frame_, size_t& rem_, bool fcs_)
{

  frame_ = Frame::Assemble(frame_, rem_, fcs_);
  ieee80211_mgmthdr* f = (ieee80211_mgmthdr*) frame_;

  if (f == NULL)
  {
    ZLOG_WARN("Error assembling management frame: " + ZLOG_INT(rem_));
    return (NULL);
  }

  if (this->Type() != Frame::TYPE_MGMT)
  {
    ZLOG_WARN("Frame type not management");
    return (NULL);
  }

  frame_ = this->chklen(frame_, sizeof(f->ra), rem_);
  if (!frame_)
  {
    return (NULL);
  }
  this->str2mac(this->GetAddress(ADDRESS_1), f->ra);

  frame_ = this->chklen(frame_, sizeof(f->ta), rem_);
  if (!frame_)
  {
    return (NULL);
  }
  this->str2mac(this->GetAddress(ADDRESS_2), f->ta);

  frame_ = this->chklen(frame_, sizeof(f->bssid), rem_);
  if (!frame_)
  {
    return (NULL);
  }
  this->str2mac(this->GetAddress(ADDRESS_3), f->bssid);

  frame_ = this->chklen(frame_, sizeof(f->seqcntl), rem_);
  if (frame_)
  {
    f->seqcntl = htole16(this->SequenceControl());
  }

  return (frame_);
}

uint8_t*
ManagementFrame::Disassemble(uint8_t* p_, size_t& rem_, bool fcs_)
{

  ieee80211_hdr* f = (ieee80211_hdr*) p_;
  p_ = Frame::Disassemble(p_, rem_, fcs_);

  if (f == NULL)
  {
    ZLOG_ERR("Error disassembling management frame: " + ZLOG_INT(rem_));
    return (NULL);
  }

  if (this->Type() != Frame::TYPE_MGMT)
  {
    ZLOG_ERR("Frame type not management");
    return (NULL);
  }

  p_ = this->chklen(p_, sizeof(f->u.mgmt.bssid), rem_);
  if (!p_ || !this->SetAddress(ADDRESS_3, f->u.mgmt.bssid))
  {
    ZLOG_ERR("Missing address field: 3");
    return (NULL);
  }

  p_ = this->chklen(p_, sizeof(f->u.mgmt.seqcntl), rem_);
  if (!p_ || !this->SequenceControl(le16toh(f->u.mgmt.seqcntl)))
  {
    ZLOG_ERR("Missing sequence control field");
    return (NULL);
  }

  return (p_);
}

bool
ManagementFrame::AssembleTags(zSocket::Buffer& sb_, uint8_t tagtype_)
{
  struct ieee80211_tag* t = NULL;
  std::map<int, std::vector<Tag> >* tags;

  int type_begin = (tagtype_ & TAGTYPE_HEAD) ? Tag::TYPE_HEAD : Tag::TYPE_TAIL;
  int type_end = (tagtype_ & TAGTYPE_TAIL) ? Tag::TYPE_LAST : Tag::TYPE_TAIL;

  for (int i = type_begin; i < type_end; i++)
  {
    FOREACH(auto& tag, this->_tags[i]) {
    if (tag.Valid())
    {
      t = (struct ieee80211_tag*) sb_.Data();

      // Some tags have zero length data

      if (not sb_.Put(sizeof(t->tag) + sizeof(t->len) + tag.Length()))
      {
        ZLOG_ERR("Buffer overrun assembling tags");
        return false;
      }
      t->tag = tag.Id();
      t->len = tag.Length();
      tag.GetValue(t->val, t->len);
      sb_.Pull(sizeof(t->tag) + sizeof(t->len) + tag.Length());
    }
  }
}
  return (true);
}

bool
ManagementFrame::DisassembleTags(zSocket::Buffer& sb_, uint8_t tagtype_)
{

  struct ieee80211_tag* t = (struct ieee80211_tag*) sb_.Data();

  // Clear out any previously "put/added" tags
  this->ClearTags(tagtype_);

  // Loop until the end of the specified buffer
  while (sb_.Length() > 0)
  {
    if (sb_.Pull(sizeof(t->tag) + sizeof(t->len) + t->len))
    {
      Tag tag((Tag::ID) t->tag, t->len);
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
      if (type != TAGTYPE_NONE and tagtype_ != TAGTYPE_NONE)
      {
        tag.PutValue(t->val, t->len);
        this->_tags[tag.Type()].push_back(tag);
        t = (struct ieee80211_tag*) sb_.Data();
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

  return true;
}

std::string
ManagementFrame::ReceiverAddress() const
{
  return (this->GetAddress(Frame::ADDRESS_1));
}

bool
ManagementFrame::ReceiverAddress(const std::string& address_)
{
  return (this->SetAddress(Frame::ADDRESS_1, address_));
}

std::string
ManagementFrame::DestinationAddress() const
{
  return (this->GetAddress(Frame::ADDRESS_1));
}

bool
ManagementFrame::DestinationAddress(const std::string& address_)
{
  return (this->SetAddress(Frame::ADDRESS_1, address_));
}

std::string
ManagementFrame::TransmitterAddress() const
{
  return (this->GetAddress(Frame::ADDRESS_2));
}

bool
ManagementFrame::TransmitterAddress(const std::string& address_)
{
  return (this->SetAddress(Frame::ADDRESS_2, address_));
}

std::string
ManagementFrame::SourceAddress() const
{
  return (this->GetAddress(Frame::ADDRESS_2));
}

bool
ManagementFrame::SourceAddress(const std::string& address_)
{
  return (this->SetAddress(Frame::ADDRESS_2, address_));
}

std::string
ManagementFrame::Bssid() const
{
  return (this->GetAddress(Frame::ADDRESS_3));
}

bool
ManagementFrame::Bssid(const std::string& address_)
{
  return (this->SetAddress(Frame::ADDRESS_3, address_));
}

void
ManagementFrame::Display() const
{
  Frame::Display();
  std::cout << "----- IEEE802.11 Mgmt Header -------------" << std::endl;
  std::cout << "\tRA:       \t" << this->ReceiverAddress() << std::endl;
  std::cout << "\tDA:       \t" << this->DestinationAddress() << std::endl;
  std::cout << "\tTA:       \t" << this->TransmitterAddress() << std::endl;
  std::cout << "\tSA:       \t" << this->SourceAddress() << std::endl;
  std::cout << "\tBSSID:    \t" << this->Bssid() << std::endl;
  std::cout << "\tFrag:     \t" << (int) this->FragmentNum() << std::endl;
  std::cout << "\tSeq:      \t" << (int) this->SequenceNum() << std::endl;
}

}
}
}
