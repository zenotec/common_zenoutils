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

bool
Deauthentication::Assemble(zSocket::Buffer& sb_)
{
  if (not ManagementFrame::Assemble(sb_) || this->Subtype() != Frame::SUBTYPE_DEAUTH)
  {
    ZLOG_ERR("Error assembling Deauthentication frame header");
    return (false);
  }

  struct ieee80211_deauth* f = (struct ieee80211_deauth*) sb_.Data();

  if (sb_.Put(sizeof(f->reasonCode)))
  {
    f->reasonCode = htole16(this->ReasonCode());
    sb_.Pull(sizeof(f->reasonCode));
  }
  else
  {
    ZLOG_ERR("Error assembling Deauthentication frame");
    return (false);
  }

  //TODO: Could have vendor specific elements
#if 0
  this->PutTag(this->WmmWme);
#endif

  if (not AssembleTags(sb_))
  {
    ZLOG_ERR("Error assembling Deauthentication frame tags");
    return (false);
  }
  return true;
}

bool
Deauthentication::Disassemble(zSocket::Buffer& sb_)
{
  struct ieee80211_deauth* f = (ieee80211_deauth*) sb_.Data();

  // Disassemble base and verify
  if (not ManagementFrame::Disassemble(sb_))
  {
    ZLOG_ERR("Error disassembling Deauthentication frame header");
    return false;
  }

  // Validate frame is proper type/subtype
  if (this->Subtype() != ManagementFrame::SUBTYPE_DEAUTH)
  {
    ZLOG_ERR("Invalid subtype: " + ZLOG_INT(this->Subtype()));
    return (false);
  }

  f = (ieee80211_deauth*) sb_.Data();

  if (sb_.Pull(sizeof(f->reasonCode)))
  {
    this->ReasonCode(le16toh(f->reasonCode));
  }
  else
  {
    ZLOG_ERR("Missing reasonCode field");
    return (false);
  }

  // Deauthentication doesn't implement any tags, but keep this for when we implement vendor specific tag
  if (not this->DisassembleTags(sb_))
  {
    ZLOG_ERR("Error disassembling Deauthentication frame tags");
    return (false);
  }

  return true;
}

uint8_t*
Deauthentication::Assemble(uint8_t* p_, size_t& rem_, bool fcs_)
{

  p_ = ManagementFrame::Assemble(p_, rem_, fcs_);
  struct ieee80211_deauth* f = (ieee80211_deauth*) p_;

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

  p_ = this->Frame::AssembleTags(p_, rem_);
  if (!p_)
  {
    ZLOG_ERR("Error assembling deauthentication request frame tags: " + ZLOG_INT(rem_));
    return (NULL);
  }

  return (p_);
}

uint8_t*
Deauthentication::Disassemble(uint8_t* p_, size_t& rem_, bool fcs_)
{

  p_ = ManagementFrame::Disassemble(p_, rem_, fcs_);
  struct ieee80211_deauth* f = (ieee80211_deauth*) p_;

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

  p_ = this->Frame::DisassembleTags((uint8_t*) &f->tags, rem_);
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
  return (this->_reasonCode);
}

bool
Deauthentication::ReasonCode(const uint16_t reasonCode_)
{
  this->_reasonCode = reasonCode_;
  return (true);
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
