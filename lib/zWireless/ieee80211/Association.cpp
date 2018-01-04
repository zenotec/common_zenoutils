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
#include "Association.h"

namespace zUtils
{
namespace zWireless
{
namespace ieee80211
{

//*****************************************************************************
// Class: AssociationRequest
//*****************************************************************************

AssociationRequest::AssociationRequest() :
    ManagementFrame(ManagementFrame::SUBTYPE_ASSREQ), _capabilities(0),
    _interval(0)
{
}

AssociationRequest::~AssociationRequest()
{
}

uint8_t*
AssociationRequest::Assemble(uint8_t* p_, size_t& rem_, bool fcs_)
{

  p_ = ManagementFrame::Assemble(p_, rem_, fcs_);
  struct ieee80211_assreq* f = (ieee80211_assreq*) p_;

  if (f == NULL)
  {
    ZLOG_WARN("Error assembling association request frame: " + ZLOG_INT(rem_));
    return (NULL);
  }

  p_ = this->chklen(p_, sizeof(f->capabilities), rem_);
  if (!p_)
  {
    return (NULL);
  }
  f->capabilities = htole16(this->Capabilities());

  p_ = this->chklen(p_, sizeof(f->interval), rem_);
  if (!p_)
  {
    return (NULL);
  }
  f->interval = htole16(this->Interval());

  if (!this->PutTag(this->Ssid))
  {
    return(NULL);
  }

  if (!this->PutTag(this->Rates))
  {
    return(NULL);
  }

  p_ = this->AssembleTags(p_, rem_);
  if (!p_)
  {
    ZLOG_ERR("Error assembling association request frame tags: " + ZLOG_INT(rem_));
    return(NULL);
  }

  return (p_);
}

uint8_t*
AssociationRequest::Disassemble(uint8_t* p_, size_t& rem_, bool fcs_)
{

  p_ = ManagementFrame::Disassemble(p_, rem_, fcs_);
  struct ieee80211_assreq* f = (ieee80211_assreq*) p_;

  if (f == NULL)
  {
    ZLOG_WARN("Error disassembling association request frame: " + ZLOG_INT(rem_));
    return (NULL);
  }

  p_ = this->chklen(p_, sizeof(f->capabilities), rem_);
  if (!p_ || !this->Capabilities(le16toh(f->capabilities)))
  {
    return (NULL);
  }

  p_ = this->chklen(p_, sizeof(f->interval), rem_);
  if (!p_ || !this->Interval(le16toh(f->interval)))
  {
    return (NULL);
  }

  p_ = this->DisassembleTags((uint8_t*)&f->tags, rem_);
  if (!p_)
  {
    ZLOG_ERR("Error disassembling association request frame tags: " + ZLOG_INT(rem_));
    return (NULL);
  }

  if (!this->GetTag(this->Ssid))
  {
    return (NULL);
  }

  if (!this->GetTag(this->Rates))
  {
    return (NULL);
  }

  return (p_);
}

uint16_t
AssociationRequest::Capabilities() const
{
  return(this->_capabilities);
}

bool
AssociationRequest::Capabilities(const uint16_t capabilities_)
{
  this->_capabilities = capabilities_;
  return(true);
}

uint16_t
AssociationRequest::Interval() const
{
  return(this->_interval);
}

bool
AssociationRequest::Interval(const uint16_t interval_)
{
  this->_interval = interval_;
  return(true);
}

void
AssociationRequest::Display() const
{
  ManagementFrame::Display();
  std::cout << "----- IEEE802.11 Association Request -----------" << std::endl;
}

//*****************************************************************************
// Class: AssociationResponse
//*****************************************************************************

AssociationResponse::AssociationResponse() :
    ManagementFrame(ManagementFrame::SUBTYPE_ASSRESP),
    _capabilities(0), _status(0), _aid(0)
{
}

AssociationResponse::~AssociationResponse()
{
}

uint8_t*
AssociationResponse::Assemble(uint8_t* p_, size_t& rem_, bool fcs_)
{
  p_ = ManagementFrame::Assemble(p_, rem_, fcs_);
  struct ieee80211_assresp* f = (ieee80211_assresp*) p_;

  if (f == NULL)
  {
    ZLOG_WARN("Error assembling association response frame: " + ZLOG_INT(rem_));
    return (NULL);
  }

  p_ = this->chklen(p_, sizeof(f->capabilities), rem_);
  if (!p_)
  {
    return (NULL);
  }
  f->capabilities = htole16(this->Capabilities());

  p_ = this->chklen(p_, sizeof(f->status), rem_);
  if (!p_)
  {
    return (NULL);
  }
  f->status = htole16(this->Status());

  p_ = this->chklen(p_, sizeof(f->aid), rem_);
  if (!p_)
  {
    return (NULL);
  }
  f->aid = htole16(this->AssociationIdentifier());

  if (!this->PutTag(this->Rates))
  {
    return(NULL);
  }

  p_ = this->AssembleTags(p_, rem_);
  if (!p_)
  {
    ZLOG_ERR("Error assembling association response frame tags: " + ZLOG_INT(rem_));
    return(NULL);
  }

  return (p_);
}

uint8_t*
AssociationResponse::Disassemble(uint8_t* p_, size_t& rem_, bool fcs_)
{
  p_ = ManagementFrame::Disassemble(p_, rem_, fcs_);
  struct ieee80211_assresp* f = (ieee80211_assresp*) p_;

  if (f == NULL)
  {
    ZLOG_WARN("Error disassembling association response frame: " + ZLOG_INT(rem_));
    return (NULL);
  }

  p_ = this->chklen(p_, sizeof(f->capabilities), rem_);
  if (!p_ || !this->Capabilities(le16toh(f->capabilities)))
  {
    return (NULL);
  }

  p_ = this->chklen(p_, sizeof(f->status), rem_);
  if (!p_ || !this->Status(le16toh(f->status)))
  {
    return (NULL);
  }

  p_ = this->chklen(p_, sizeof(f->aid), rem_);
  if (!p_ || !this->AssociationIdentifier(le16toh(f->aid)))
  {
    return (NULL);
  }

  p_ = this->DisassembleTags((uint8_t*)&f->tags, rem_);
  if (!p_)
  {
    ZLOG_ERR("Error disassembling association response frame tags: " + ZLOG_INT(rem_));
    return (NULL);
  }

  if (!this->GetTag(this->Rates))
  {
    return (NULL);
  }

  return (p_);
}

uint16_t
AssociationResponse::Capabilities() const
{
  return(this->_capabilities);
}

bool
AssociationResponse::Capabilities(const uint16_t capabilities_)
{
  this->_capabilities = capabilities_;
  return(true);
}

uint16_t
AssociationResponse::Status() const
{
  return(this->_status);
}

bool
AssociationResponse::Status(const uint16_t status_)
{
  this->_status = status_;
  return(true);
}

uint16_t
AssociationResponse::AssociationIdentifier() const
{
  return(this->_aid);
}

bool
AssociationResponse::AssociationIdentifier(const uint16_t aid_)
{
  this->_aid = aid_;
  return(true);
}

void
AssociationResponse::Display() const
{
  ManagementFrame::Display();
  std::cout << "----- IEEE802.11 Association Response ----------" << std::endl;
  std::cout << "\tCap:      \t" << std::hex << this->Capabilities() << std::endl;
  std::cout << "\tStatus:   \t" << (int) this->Status() << std::endl;
}

}
}
}
