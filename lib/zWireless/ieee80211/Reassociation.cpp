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
#include <zutils/ieee80211/Reassociation.h>
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
// Class: ReassociationRequest
//*****************************************************************************

ReassociationRequest::ReassociationRequest() :
    ManagementFrame(ManagementFrame::SUBTYPE_RASSREQ), _capabilities(0),
    _interval(0), _currentApMac(0)
{
}

ReassociationRequest::~ReassociationRequest()
{
}

uint8_t*
ReassociationRequest::Assemble(uint8_t* p_, size_t& rem_, bool fcs_)
{

  p_ = ManagementFrame::Assemble(p_, rem_, fcs_);
  struct ieee80211_rassreq* f = (ieee80211_rassreq*) p_;

  if (f == NULL)
  {
    ZLOG_WARN("Error assembling reassociation request frame: " + ZLOG_INT(rem_));
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

  p_ = this->chklen(p_, sizeof(f->currentApMac), rem_);
  if (!p_)
  {
    return (NULL);
  }
  f->currentApMac = htole64(this->CurrentApMac());

  if (!this->PutTag(this->Ssid))
  {
    return(NULL);
  }

  if (!this->PutTag(this->Rates))
  {
    return(NULL);
  }

  //ORDER MATTERS - Declare in the order they are rendered
  if (!this->PutTag(this->ExtRates))
  {
    return(NULL);
  }
  if (!this->PutTag(this->PowerCaps))
  {
    return(NULL);
  }
  if (!this->PutTag(this->Channels))
  {
    return(NULL);
  }
  if (!this->PutTag(this->HtCaps))
  {
    return(NULL);
  }
#if 0 //TODO
  if (!this->PutTag(this->WmmWme))
  {
    return(NULL);
  }
#endif

  p_ = this->AssembleTags(p_, rem_);
  if (!p_)
  {
    ZLOG_ERR("Error assembling reassociation request frame tags: " + ZLOG_INT(rem_));
    return(NULL);
  }

  this->PutTag(this->PowerCaps);

  return (p_);
}

uint8_t*
ReassociationRequest::Disassemble(uint8_t* p_, size_t& rem_, bool fcs_)
{

  p_ = ManagementFrame::Disassemble(p_, rem_, fcs_);
  struct ieee80211_rassreq* f = (ieee80211_rassreq*) p_;

  if (f == NULL)
  {
    ZLOG_WARN("Error disassembling reassociation request frame: " + ZLOG_INT(rem_));
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

  p_ = this->chklen(p_, sizeof(f->currentApMac), rem_);
  if (!p_ || !this->Interval(le64toh(f->currentApMac)))
  {
    return (NULL);
  }

  p_ = this->DisassembleTags((uint8_t*)&f->tags, rem_);
  if (!p_)
  {
    ZLOG_ERR("Error disassembling reassociation request frame tags: " + ZLOG_INT(rem_));
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

  //ORDER MATTERS - Declare in the order they are rendered - don't think it matters here since GetTag itterates
  this->GetTag(ExtRates);
  this->GetTag(PowerCaps);
  this->GetTag(Channels);
  this->GetTag(HtCaps);
  this->GetTag(WmmWme);

  return (p_);
}

uint16_t
ReassociationRequest::Capabilities() const
{
  return(this->_capabilities);
}

bool
ReassociationRequest::Capabilities(const uint16_t capabilities_)
{
  this->_capabilities = capabilities_;
  return(true);
}

uint16_t
ReassociationRequest::Interval() const
{
  return(this->_interval);
}

bool
ReassociationRequest::Interval(const uint16_t interval_)
{
  this->_interval = interval_;
  return(true);
}

uint64_t
ReassociationRequest::CurrentApMac() const
{
	return (this->_currentApMac);
}

bool
ReassociationRequest::CurrentApMac(const uint64_t mac_)
{
	this->_currentApMac = mac_;
	return (true);
}

void
ReassociationRequest::Display() const
{
  ManagementFrame::Display();
  std::cout << "----- IEEE802.11 Reassociation Request -----------" << std::endl;
  std::cout << "\tCap:      \t" << std::hex << this->Capabilities() << std::dec << std::endl;
  std::cout << "\tInterval: \t" << (int) this->Interval() << std::endl;
  if (this->Ssid.Valid()) this->Ssid.Display();
  if (this->PowerCaps.Valid()) this->PowerCaps.Display();
}

//*****************************************************************************
// Class: ReassociationResponse
//*****************************************************************************

ReassociationResponse::ReassociationResponse() :
    ManagementFrame(ManagementFrame::SUBTYPE_ASSRESP),
    _capabilities(0), _status(0), _aid(0)
{
}

ReassociationResponse::~ReassociationResponse()
{
}

uint8_t*
ReassociationResponse::Assemble(uint8_t* p_, size_t& rem_, bool fcs_)
{
  p_ = ManagementFrame::Assemble(p_, rem_, fcs_);
  struct ieee80211_assresp* f = (ieee80211_assresp*) p_;

  if (f == NULL)
  {
    ZLOG_WARN("Error assembling reassociation response frame: " + ZLOG_INT(rem_));
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
  f->aid = htole16(this->ReassociationIdentifier());

  // Order matters - check 802.11 spec
  this->PutTag(this->Rates);
  this->PutTag(this->ExtRates);
  this->PutTag(this->HtCaps);
  this->PutTag(this->HtInfo);
  this->PutTag(this->ExtCaps);
  this->PutTag(this->WmmWme);

  p_ = this->AssembleTags(p_, rem_);
  if (!p_)
  {
    ZLOG_ERR("Error assembling reassociation response frame tags: " + ZLOG_INT(rem_));
    return(NULL);
  }

  return (p_);
}

uint8_t*
ReassociationResponse::Disassemble(uint8_t* p_, size_t& rem_, bool fcs_)
{
  p_ = ManagementFrame::Disassemble(p_, rem_, fcs_);
  struct ieee80211_assresp* f = (ieee80211_assresp*) p_;

  if (f == NULL)
  {
    ZLOG_WARN("Error disassembling reassociation response frame: " + ZLOG_INT(rem_));
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
  if (!p_ || !this->ReassociationIdentifier(le16toh(f->aid)))
  {
    return (NULL);
  }

  p_ = this->DisassembleTags((uint8_t*)&f->tags, rem_);
  if (!p_)
  {
    ZLOG_ERR("Error disassembling reassociation response frame tags: " + ZLOG_INT(rem_));
    return (NULL);
  }

  this->GetTag(this->Rates);
  this->GetTag(this->ExtRates);
  this->GetTag(this->HtCaps);
  this->GetTag(this->HtInfo);
  this->GetTag(this->ExtCaps);
  this->GetTag(this->WmmWme);

  return (p_);
}

uint16_t
ReassociationResponse::Capabilities() const
{
  return(this->_capabilities);
}

bool
ReassociationResponse::Capabilities(const uint16_t capabilities_)
{
  this->_capabilities = capabilities_;
  return(true);
}

uint16_t
ReassociationResponse::Status() const
{
  return(this->_status);
}

bool
ReassociationResponse::Status(const uint16_t status_)
{
  this->_status = status_;
  return(true);
}

uint16_t
ReassociationResponse::ReassociationIdentifier() const
{
  return(this->_aid);
}

bool
ReassociationResponse::ReassociationIdentifier(const uint16_t aid_)
{
  this->_aid = aid_;
  return(true);
}

void
ReassociationResponse::Display() const
{
  ManagementFrame::Display();
  std::cout << "----- IEEE802.11 Reassociation Response ----------" << std::endl;
  std::cout << "\tCap:      \t" << std::hex << this->Capabilities() << std::dec << std::endl;
  std::cout << "\tStatus:   \t" << int(this->Status()) << std::endl;
  std::cout << "\tAID:      \t" << int(this->ReassociationIdentifier()) << std::endl;
  if (this->Rates.Valid()) this->Rates.Display();
  if (this->HtCaps.Valid()) this->HtCaps.Display();
  if (this->ExtRates.Valid()) this->ExtRates.Display();
  if (this->ExtCaps.Valid()) this->ExtCaps.Display();
  if (this->WmmWme.Valid()) this->WmmWme.Display();
}

}
}
}
