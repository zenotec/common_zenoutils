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
#include <zutils/ieee80211/Disassociation.h>
using namespace zUtils;

// local includes
#include "ieee80211.h"

ZLOG_MODULE_INIT(zUtils::zLog::Log::MODULE_WIRELESS);

namespace zUtils
{
namespace zWireless
{
namespace ieee80211
{

//*****************************************************************************
// Class: Disassociation
//*****************************************************************************

Disassociation::Disassociation() :
    ManagementFrame(ManagementFrame::SUBTYPE_DISASS), _reasonCode(REASON_ERR)
{
}

Disassociation::~Disassociation()
{
}

uint8_t*
Disassociation::Assemble(uint8_t* p_, size_t& rem_, bool fcs_)
{

  p_ = ManagementFrame::Assemble(p_, rem_, fcs_);
  struct ieee80211_disass* f = (ieee80211_disass*) p_;

  if (f == NULL)
  {
    ZLOG_WARN("Error assembling disassociation request frame: " + ZLOG_INT(rem_));
    return (NULL);
  }

  p_ = this->chklen(p_, sizeof(f->reasonCode), rem_);
  if (!p_)
  {
    return (NULL);
  }
  f->reasonCode = htole16(this->ReasonCode());

  p_ = this->AssembleTags(p_, rem_);
  if (!p_)
  {
    ZLOG_ERR("Error assembling disassociation request frame tags: " + ZLOG_INT(rem_));
    return(NULL);
  }

  return (p_);
}

uint8_t*
Disassociation::Disassemble(uint8_t* p_, size_t& rem_, bool fcs_)
{

  p_ = ManagementFrame::Disassemble(p_, rem_, fcs_);
  struct ieee80211_disass* f = (ieee80211_disass*) p_;

  if (f == NULL)
  {
    ZLOG_WARN("Error disassembling disassociation frame: " + ZLOG_INT(rem_));
    return (NULL);
  }

  p_ = this->chklen(p_, sizeof(f->reasonCode), rem_);
  if (!p_ || !this->ReasonCode(le16toh(f->reasonCode)))
  {
    return (NULL);
  }

  p_ = this->DisassembleTags((uint8_t*)&f->tags, rem_);
  if (!p_)
  {
    ZLOG_ERR("Error disassembling disassociation frame tags: " + ZLOG_INT(rem_));
    return (NULL);
  }

  return (p_);
}


uint16_t
Disassociation::ReasonCode() const
{
  return(this->_reasonCode);
}

bool
Disassociation::ReasonCode(const uint16_t reasonCode_)
{
  this->_reasonCode = reasonCode_;
  return(true);
}

void
Disassociation::Display() const
{
  ManagementFrame::Display();
  std::cout << "----- IEEE802.11 Disassociation  -----------" << std::endl;
}

}
}
}
