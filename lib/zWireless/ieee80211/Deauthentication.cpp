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
#include <zutils/ieee80211/Deauthentication.h>
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
// Class: Deauthentication
//*****************************************************************************

Deauthentication::Deauthentication() :
    ManagementFrame(ManagementFrame::SUBTYPE_DEAUTH), _reasonCode(REASON_ERR)
{
}

Deauthentication::~Deauthentication()
{
}

uint8_t*
Deauthentication::Assemble(uint8_t* p_, size_t& rem_, bool fcs_)
{

  p_ = ManagementFrame::Assemble(p_, rem_, fcs_);
  struct ieee80211_disass* f = (ieee80211_disass*) p_;

  if (f == NULL)
  {
    ZLOG_WARN("Error assembling deauthentication request frame: " + ZLOG_INT(rem_));
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
    ZLOG_ERR("Error assembling deauthentication request frame tags: " + ZLOG_INT(rem_));
    return(NULL);
  }

  return (p_);
}

uint8_t*
Deauthentication::Disassemble(uint8_t* p_, size_t& rem_, bool fcs_)
{

  p_ = ManagementFrame::Disassemble(p_, rem_, fcs_);
  struct ieee80211_disass* f = (ieee80211_disass*) p_;

  if (f == NULL)
  {
    ZLOG_WARN("Error disassembling deauthentication frame: " + ZLOG_INT(rem_));
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
    ZLOG_ERR("Error disassembling deauthentication frame tags: " + ZLOG_INT(rem_));
    return (NULL);
  }

  return (p_);
}


uint16_t
Deauthentication::ReasonCode() const
{
  return(this->_reasonCode);
}

bool
Deauthentication::ReasonCode(const uint16_t reasonCode_)
{
  this->_reasonCode = reasonCode_;
  return(true);
}

void
Deauthentication::Display() const
{
  ManagementFrame::Display();
  std::cout << "----- IEEE802.11 Deauthentication  -----------" << std::endl;
}

}
}
}
