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
#include <zutils/ieee80211/ControlFrame.h>
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

ControlFrame::ControlFrame(const Frame::SUBTYPE subtype_) :
    Frame(Frame::TYPE_CNTL)
{
  this->Subtype(subtype_);
}

ControlFrame::~ControlFrame()
{
}

uint8_t*
ControlFrame::Assemble(uint8_t* p_, size_t& rem_, bool fcs_)
{

  p_ = Frame::Assemble(p_, rem_, fcs_);
  ieee80211_cntlhdr* f = (ieee80211_cntlhdr*) p_;

  if (f == NULL)
  {
    ZLOG_WARN("Error assembling control frame: " + ZLOG_INT(rem_));
    return (NULL);
  }

  if (this->Type() != Frame::TYPE_CNTL)
  {
    ZLOG_WARN("Frame type not control");
    return (NULL);
  }

  return (p_);
}

uint8_t*
ControlFrame::Disassemble(uint8_t* p_, size_t& rem_, bool fcs_)
{

  p_ = Frame::Disassemble(p_, rem_, fcs_);
  ieee80211_cntlhdr* f = (ieee80211_cntlhdr*) p_;

  if (f == NULL)
  {
    ZLOG_WARN("Error disassembling control frame: " + ZLOG_INT(rem_));
    return (NULL);
  }

  if (this->Type() != Frame::TYPE_CNTL)
  {
    ZLOG_WARN("Frame type not control");
    return (NULL);
  }

  p_ = this->chklen(p_, sizeof(f->ra), rem_);
  if (!p_ || !this->Address(ADDRESS_1, f->ra))
  {
    return (NULL);
  }

  if (this->Subtype() == Frame::SUBTYPE_RTS)
  {
    p_ = this->chklen(p_, sizeof(f->ta), rem_);
    if (!p_ || !this->Address(ADDRESS_2, f->ta))
    {
      return (NULL);
    }
  }

  return (p_);
}

std::string
ControlFrame::ReceiverAddress() const
{
  return(this->Address(Frame::ADDRESS_1));
}

bool
ControlFrame::ReceiverAddress(const std::string& address_)
{
  return(this->Address(Frame::ADDRESS_1, address_));
}

std::string
ControlFrame::TransmitterAddress() const
{
  return(this->Address(Frame::ADDRESS_2));
}

bool
ControlFrame::TransmitterAddress(const std::string& address_)
{
  return(this->Address(Frame::ADDRESS_2, address_));
}

void
ControlFrame::Display() const
{
  Frame::Display();
  std::cout << "----- IEEE802.11 Control Header ----------" << std::endl;
  std::cout << "\tRA:       \t" << this->ReceiverAddress() << std::endl;
  std::cout << "\tTA:       \t" << this->TransmitterAddress() << std::endl;
}

}
}
}
