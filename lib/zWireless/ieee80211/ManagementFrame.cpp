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
#include <iostream>

// libzutils includes
#include <zutils/zLog.h>

// local includes
#include "ManagementFrame.h"

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
  if (!frame_ || this->Address(ADDRESS_1).empty() || !this->str2mac(this->Address(ADDRESS_1), f->ra))
  {
    return (NULL);
  }

  frame_ = this->chklen(frame_, sizeof(f->ta), rem_);
  if (!frame_ || this->Address(ADDRESS_2).empty() || !this->str2mac(this->Address(ADDRESS_2), f->ta))
  {
    return (NULL);
  }

  frame_ = this->chklen(frame_, sizeof(f->bssid), rem_);
  if (!frame_ || this->Address(ADDRESS_3).empty() || !this->str2mac(this->Address(ADDRESS_3), f->bssid))
  {
    return (NULL);
  }

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

  p_ = Frame::Disassemble(p_, rem_, fcs_);
  ieee80211_mgmthdr* f = (ieee80211_mgmthdr*) p_;

  if (f == NULL)
  {
    ZLOG_WARN("Error disassembling management frame: " + ZLOG_INT(rem_));
    return (NULL);
  }

  if (this->Type() != Frame::TYPE_MGMT)
  {
    ZLOG_WARN("Frame type not management");
    return (NULL);
  }

  p_ = this->chklen(p_, sizeof(f->ra), rem_);
  if (!p_ || !this->Address(ADDRESS_1, f->ra))
  {
    return (NULL);
  }

  p_ = this->chklen(p_, sizeof(f->ta), rem_);
  if (!p_ || !this->Address(ADDRESS_2, f->ta))
  {
    return (NULL);
  }

  p_ = this->chklen(p_, sizeof(f->bssid), rem_);
  if (!p_ || !this->Address(ADDRESS_3, f->bssid))
  {
    return (NULL);
  }

  p_ = this->chklen(p_, sizeof(f->seqcntl), rem_);
  if (!p_ || !this->SequenceControl(le16toh(f->seqcntl)))
  {
    return(NULL);
  }

  return (p_);
}

std::string
ManagementFrame::ReceiverAddress() const
{
  return(this->Address(Frame::ADDRESS_1));
}

bool
ManagementFrame::ReceiverAddress(const std::string& address_)
{
  return(this->Address(Frame::ADDRESS_1, address_));
}

std::string
ManagementFrame::DestinationAddress() const
{
  return(this->Address(Frame::ADDRESS_1));
}

bool
ManagementFrame::DestinationAddress(const std::string& address_)
{
  return(this->Address(Frame::ADDRESS_1, address_));
}

std::string
ManagementFrame::TransmitterAddress() const
{
  return(this->Address(Frame::ADDRESS_2));
}

bool
ManagementFrame::TransmitterAddress(const std::string& address_)
{
  return(this->Address(Frame::ADDRESS_2, address_));
}

std::string
ManagementFrame::SourceAddress() const
{
  return(this->Address(Frame::ADDRESS_2));
}

bool
ManagementFrame::SourceAddress(const std::string& address_)
{
  return(this->Address(Frame::ADDRESS_2, address_));
}

std::string
ManagementFrame::Bssid() const
{
  return(this->Address(Frame::ADDRESS_3));
}

bool
ManagementFrame::Bssid(const std::string& address_)
{
  return(this->Address(Frame::ADDRESS_3, address_));
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
  std::cout << "\tSeq:      \t" << (int) this->SequenceControl() << std::endl;
}

}
}
}
