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
#include <zutils/ieee80211/ieee80211.h>

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
    _interval(0), _currentApMac("")
{
}

ReassociationRequest::~ReassociationRequest()
{
}

bool
ReassociationRequest::Assemble(zSocket::Buffer& sb_, bool fcs_)
{
  if (not ManagementFrame::Assemble(sb_, fcs_) || this->Subtype() != Frame::SUBTYPE_RASSREQ)
  {
    ZLOG_ERR("Error assembling ReassociationRequest frame header");
    return (false);
  }

  struct ieee80211_rassreq* f = (struct ieee80211_rassreq*) sb_.Data();

  if (sb_.Put(sizeof(f->capabilities)))
  {
    f->capabilities = htole16(this->Capabilities());
    sb_.Pull(sizeof(f->capabilities));
  }
  else
  {
    ZLOG_ERR("Error assembling ReassociationRequest frame");
    return (false);
  }

  if (sb_.Put(sizeof(f->interval)))
  {
    f->interval = htole16(this->Interval());
    sb_.Pull(sizeof(f->interval));
  }
  else
  {
    ZLOG_ERR("Error assembling ReassociationRequest frame");
    return (false);
  }

  if (sb_.Put(sizeof(f->currentApMac)))
  {
    this->str2mac(this->CurrentApMac(), f->currentApMac);
    sb_.Pull(sizeof(f->currentApMac));
  }
  else
  {
    ZLOG_ERR("Error assembling ReassociationRequest frame");
    return (false);
  }

  if (!this->PutTag(this->Ssid))
  {
    ZLOG_ERR("Error assembling ReassociationRequest frame: Missing SSID");
    return (false);
  }

  if (!this->PutTag(this->Rates))
  {
    ZLOG_ERR("Error assembling ReassociationRequest frame: Missing Rates");
    return (false);
  }

  //ORDER MATTERS - Declare in the order they are rendered
  this->PutTag(this->ExtRates);
  this->PutTag(this->PowerCaps);
  this->PutTag(this->Channels);
  this->PutTag(this->HtCaps);
  this->PutTag(this->WmmWme);

  if (not AssembleTags(sb_))
  {
    ZLOG_ERR("Error assembling ReassociationRequest frame tags");
    return (false);
  }

  return true;
}

bool
ReassociationRequest::Disassemble(zSocket::Buffer& sb_, bool fcs_)
{
  struct ieee80211_rassreq* f = (ieee80211_rassreq*) sb_.Data();

  // Disassemble base and verify
  if (not ManagementFrame::Disassemble(sb_, fcs_))
  {
    ZLOG_ERR("Error disassembling ReassociationRequest frame header");
    return false;
  }

  // Validate frame is proper type/subtype
  if (this->Subtype() != ManagementFrame::SUBTYPE_RASSREQ)
  {
    ZLOG_ERR("Invalid subtype: " + ZLOG_INT(this->Subtype()));
    return (false);
  }

  f = (ieee80211_rassreq*) sb_.Data();

  if (sb_.Pull(sizeof(f->capabilities)))
  {
    this->Capabilities(le16toh(f->capabilities));
  }
  else
  {
    ZLOG_ERR("Missing capabilities field");
    return (false);
  }

  if (sb_.Pull(sizeof(f->interval)))
  {
    this->Interval(le16toh(f->interval));
  }
  else
  {
    ZLOG_ERR("Missing interval field");
    return (false);
  }

  if (sb_.Pull(sizeof(f->currentApMac)))
  {
    this->CurrentApMac(f->currentApMac);
  }
  else
  {
    ZLOG_ERR("Missing currentApMac field");
    return (false);
  }

  if (not this->DisassembleTags(sb_))
  {
    ZLOG_ERR("Error disassembling AssociationRequest frame tags");
    return (false);
  }

  if (!this->GetTag(this->Ssid))
  {
    ZLOG_ERR("Error disassembling AssociationRequest frame tags: missing SSID");
    return (false);
  }

  if (!this->GetTag(this->Rates))
  {
    ZLOG_ERR("Error disassembling AssociationRequest frame tags: missing Rates");
    return (false);
  }

  //ORDER MATTERS - Declare in the order they are rendered - don't think it matters here since GetTag itterates
  this->GetTag(ExtRates);
  this->GetTag(PowerCaps);
  this->GetTag(Channels);
  this->GetTag(HtCaps);
  this->GetTag(WmmWme);

  return true;
}

uint16_t
ReassociationRequest::Capabilities() const
{
  return (this->_capabilities);
}

bool
ReassociationRequest::Capabilities(const uint16_t capabilities_)
{
  this->_capabilities = capabilities_;
  return (true);
}

uint16_t
ReassociationRequest::Interval() const
{
  return (this->_interval);
}

bool
ReassociationRequest::Interval(const uint16_t interval_)
{
  this->_interval = interval_;
  return (true);
}

std::string
ReassociationRequest::CurrentApMac() const
{
  return (this->_currentApMac);
}

bool
ReassociationRequest::CurrentApMac(const std::string mac_)
{
  this->_currentApMac = mac_;
  return (true);
}

bool
ReassociationRequest::CurrentApMac(const uint8_t* mac_)
{
  bool status = false;
  std::string addr;
  if (this->mac2str(mac_, addr))
  {
    status = this->CurrentApMac(addr);
  }
  return (status);
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
    ManagementFrame(ManagementFrame::SUBTYPE_RASSRESP), _capabilities(0), _status(0), _aid(0)
{
}

ReassociationResponse::~ReassociationResponse()
{
}

bool
ReassociationResponse::Assemble(zSocket::Buffer& sb_, bool fcs_)
{
  if (not ManagementFrame::Assemble(sb_, fcs_) || this->Subtype() != Frame::SUBTYPE_RASSRESP)
  {
    ZLOG_ERR("Error assembling ReassociationResponse frame header");
    return (false);
  }

  struct ieee80211_rassresp* f = (struct ieee80211_rassresp*) sb_.Data();

  if (sb_.Put(sizeof(f->capabilities)))
  {
    f->capabilities = htole16(this->Capabilities());
    sb_.Pull(sizeof(f->capabilities));
  }
  else
  {
    ZLOG_ERR("Error assembling ReassociationResponse frame");
    return (false);
  }

  if (sb_.Put(sizeof(f->status)))
  {
    f->status = htole16(this->Status());
    sb_.Pull(sizeof(f->status));
  }
  else
  {
    ZLOG_ERR("Error assembling ReassociationResponse frame");
    return (false);
  }

  if (sb_.Put(sizeof(f->aid)))
  {
    f->aid = htole16(this->ReassociationIdentifier());
    sb_.Pull(sizeof(f->aid));
  }
  else
  {
    ZLOG_ERR("Error assembling ReassociationResponse frame");
    return (false);
  }

  // Order matters - check 802.11 spec
  this->PutTag(this->Rates);
  this->PutTag(this->ExtRates);
  this->PutTag(this->HtCaps);
  this->PutTag(this->HtInfo);
  this->PutTag(this->ExtCaps);
#if 0
  this->PutTag(this->WmmWme);
#endif

  if (not AssembleTags(sb_))
  {
    ZLOG_ERR("Error assembling ReassociationResponse frame tags");
    return (false);
  }

  return true;
}

bool
ReassociationResponse::Disassemble(zSocket::Buffer& sb_, bool fcs_)
{
  struct ieee80211_rassresp* f = (ieee80211_rassresp*) sb_.Data();

  // Disassemble base and verify
  if (not ManagementFrame::Disassemble(sb_, fcs_))
  {
    ZLOG_ERR("Error disassembling ReassociationResponse frame header");
    return false;
  }

  // Validate frame is proper type/subtype
  if (this->Subtype() != ManagementFrame::SUBTYPE_RASSRESP)
  {
    ZLOG_ERR("Invalid subtype: " + ZLOG_INT(this->Subtype()));
    return (false);
  }

  f = (ieee80211_rassresp*) sb_.Data();

  if (sb_.Pull(sizeof(f->capabilities)))
  {
    this->Capabilities(le16toh(f->capabilities));
  }
  else
  {
    ZLOG_ERR("Missing capabilities field");
    return (false);
  }

  if (sb_.Pull(sizeof(f->status)))
  {
    this->Status(le16toh(f->status));
  }
  else
  {
    ZLOG_ERR("Missing status field");
    return (false);
  }

  if (sb_.Pull(sizeof(f->aid)))
  {
    this->ReassociationIdentifier(le16toh(f->aid));
  }
  else
  {
    ZLOG_ERR("Missing aid field");
    return (false);
  }

  if (not this->DisassembleTags(sb_))
  {
    ZLOG_ERR("Error disassembling ReassociationResponse frame tags");
    return (false);
  }

  this->GetTag(this->Rates);
  this->GetTag(this->ExtRates);
  this->GetTag(this->HtCaps);
  this->GetTag(this->HtInfo);
  this->GetTag(this->ExtCaps);
  this->GetTag(this->WmmWme);

  return true;
}

uint16_t
ReassociationResponse::Capabilities() const
{
  return (this->_capabilities);
}

bool
ReassociationResponse::Capabilities(const uint16_t capabilities_)
{
  this->_capabilities = capabilities_;
  return (true);
}

uint16_t
ReassociationResponse::Status() const
{
  return (this->_status);
}

bool
ReassociationResponse::Status(const uint16_t status_)
{
  this->_status = status_;
  return (true);
}

uint16_t
ReassociationResponse::ReassociationIdentifier() const
{
  return (this->_aid);
}

bool
ReassociationResponse::ReassociationIdentifier(const uint16_t aid_)
{
  this->_aid = aid_;
  return (true);
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
