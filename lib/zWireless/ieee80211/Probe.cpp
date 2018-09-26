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
#include <zutils/ieee80211/Probe.h>
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
// Class: ProbeRequest
//*****************************************************************************

ProbeRequest::ProbeRequest() :
    ManagementFrame(ManagementFrame::SUBTYPE_PROBEREQ)
{
}

ProbeRequest::~ProbeRequest()
{
}

bool
ProbeRequest::Assemble(zSocket::Buffer& sb_, bool fcs_)
{
  if (not ManagementFrame::Assemble(sb_, fcs_) || this->Subtype() != Frame::SUBTYPE_PROBEREQ)
  {
    ZLOG_ERR("Error assembling ProbeRequest frame header");
    return (false);
  }

  struct ieee80211_probereq* f = (struct ieee80211_probereq*) sb_.Data();

  if (!this->PutTag(this->Ssid))
  {
    ZLOG_ERR("Error assembling probe request frame: Missing SSID");
    return (NULL);
  }

  if (!this->PutTag(this->Rates))
  {
    ZLOG_ERR("Error assembling probe request frame: Missing Rates");
    return (NULL);
  }

  this->PutTag(this->ExtRates);
  this->PutTag(this->Dsss);
  this->PutTag(this->HtCaps);
  this->PutTag(this->ExtCaps);

  if (not AssembleTags(sb_))
  {
    ZLOG_ERR("Error assembling ProbeRequest frame tags");
    return (false);
  }

  return true;
}

bool
ProbeRequest::Disassemble(zSocket::Buffer& sb_, bool fcs_)
{
  struct ieee80211_probereq* f = (ieee80211_probereq*) sb_.Data();

  // Disassemble base and verify
  if (not ManagementFrame::Disassemble(sb_, fcs_))
  {
    ZLOG_ERR("Error disassembling ProbeRequest frame header");
    return false;
  }

  // Validate frame is proper type/subtype
  if (this->Subtype() != ManagementFrame::SUBTYPE_PROBEREQ)
  {
    ZLOG_ERR("Invalid subtype: " + ZLOG_INT(this->Subtype()));
    return (false);
  }

  f = (ieee80211_probereq*) sb_.Data();

  if (not this->DisassembleTags(sb_))
  {
    ZLOG_ERR("Error disassembling ProbeRequest frame tags");
    return (false);
  }

  if (!this->GetTag(this->Rates))
  {
    ZLOG_ERR("Error disassembling ProbeRequest frame tags: missing Rates");
    return (false);
  }

  this->GetTag(this->Ssid); // Optional - ignore return value
  this->GetTag(this->Dsss); // Optional - ignore return value
  this->GetTag(this->ExtRates); // Optional - ignore return value
  this->GetTag(this->HtCaps); // Optional - ignore return value
  this->GetTag(this->ExtCaps); // Optional - ignore return value

  return true;
}

uint8_t*
ProbeRequest::Assemble(uint8_t* p_, size_t& rem_, bool fcs_)
{

  p_ = ManagementFrame::Assemble(p_, rem_, fcs_);
  struct ieee80211_probereq* f = (ieee80211_probereq*) p_;

  if (f == NULL)
  {
    ZLOG_WARN("Error assembling probe request frame: " + ZLOG_INT(rem_));
    return (NULL);
  }

  if (!this->PutTag(this->Ssid))
  {
    ZLOG_ERR("Error assembling probe request frame: Missing SSID");
    return (NULL);
  }

  if (!this->PutTag(this->Rates))
  {
    ZLOG_ERR("Error assembling probe request frame: Missing Rates");
    return (NULL);
  }

  this->PutTag(this->ExtRates);
  this->PutTag(this->Dsss);
  this->PutTag(this->HtCaps);
  this->PutTag(this->ExtCaps);

  p_ = this->Frame::AssembleTags(p_, rem_);
  if (!p_)
  {
    ZLOG_ERR("Error assembling probe request frame tags: " + ZLOG_INT(rem_));
    return (NULL);
  }

  return (p_);
}

uint8_t*
ProbeRequest::Disassemble(uint8_t* p_, size_t& rem_, bool fcs_)
{

  p_ = ManagementFrame::Disassemble(p_, rem_, fcs_);
  struct ieee80211_probereq* f = (ieee80211_probereq*) p_;

  if (f == NULL)
  {
    ZLOG_WARN("Error disassembling probe request frame: " + ZLOG_INT(rem_));
    return (NULL);
  }

  p_ = this->Frame::DisassembleTags((uint8_t*) &f->tags, rem_);
  if (!p_)
  {
    ZLOG_ERR("Error disassembling probe request frame tags: " + ZLOG_INT(rem_));
    return (NULL);
  }

  if (!this->GetTag(this->Rates))
  {
    ZLOG_ERR("Error disassembling probe request frame: Missing Rates");
    return (NULL);
  }

  this->GetTag(this->Ssid); // Optional - ignore return value
  this->GetTag(this->Dsss); // Optional - ignore return value
  this->GetTag(this->ExtRates); // Optional - ignore return value
  this->GetTag(this->HtCaps); // Optional - ignore return value
  this->GetTag(this->ExtCaps); // Optional - ignore return value

  return (p_);
}

void
ProbeRequest::Display() const
{
  ManagementFrame::Display();
  std::cout << "----- IEEE802.11 Probe Request -----------" << std::endl;
  if (this->Ssid.Valid())
    this->Ssid.Display();
  if (this->Rates.Valid())
    this->Rates.Display();
}

//*****************************************************************************
// Class: ProbeResponse
//*****************************************************************************

ProbeResponse::ProbeResponse() :
    ManagementFrame(ManagementFrame::SUBTYPE_PROBERESP), _ts(0), _interval(0),
    _capabilities(0)
{
}

ProbeResponse::~ProbeResponse()
{
}

bool
ProbeResponse::Assemble(zSocket::Buffer& sb_, bool fcs_)
{
  if (not ManagementFrame::Assemble(sb_, fcs_) || this->Subtype() != Frame::SUBTYPE_PROBERESP)
  {
    ZLOG_ERR("Error assembling ProbeResponse frame header");
    return (false);
  }

  struct ieee80211_proberesp* f = (struct ieee80211_proberesp*) sb_.Data();

  if (sb_.Put(sizeof(f->timestamp)))
  {
    f->timestamp = htole64(this->Timestamp());
    sb_.Pull(sizeof(f->timestamp));
  }
  else
  {
    ZLOG_ERR("Error assembling ProbeResponse frame");
    return (false);
  }

  if (sb_.Put(sizeof(f->interval)))
  {
    f->interval = htole16(this->Interval());
    sb_.Pull(sizeof(f->interval));
  }
  else
  {
    ZLOG_ERR("Error assembling ProbeResponse frame");
    return (false);
  }

  if (sb_.Put(sizeof(f->capabilities)))
  {
    f->capabilities = htole16(this->Capabilities());
    sb_.Pull(sizeof(f->capabilities));
  }
  else
  {
    ZLOG_ERR("Error assembling ProbeResponse frame");
    return (false);
  }

  if (!this->PutTag(this->Ssid))
  {
    ZLOG_ERR("Error assembling probe response frame: Missing SSID");
    return (NULL);
  }

  if (!this->PutTag(this->Rates))
  {
    ZLOG_ERR("Error assembling probe response frame: Missing Rates");
    return (NULL);
  }

  // NOTE: ORDER MATTERS!!!

  this->PutTag(this->Dsss);
  this->PutTag(this->Country);
  this->PutTag(this->ChannelSwitch);
  this->PutTag(this->ErpInfo);
  this->PutTag(this->ExtRates);
  this->PutTag(this->SuppOpClass);
  this->PutTag(this->HtCaps);
  this->PutTag(this->HtInfo);
  this->PutTag(this->ExtCaps);
#if 0
  this->PutTag(this->WmmWme);
#endif

  if (not AssembleTags(sb_))
  {
    ZLOG_ERR("Error assembling ProbeResponse frame tags");
    return (false);
  }

  return true;
}

bool
ProbeResponse::Disassemble(zSocket::Buffer& sb_, bool fcs_)
{
  struct ieee80211_proberesp* f = (ieee80211_proberesp*) sb_.Data();

  // Disassemble base and verify
  if (not ManagementFrame::Disassemble(sb_, fcs_))
  {
    ZLOG_ERR("Error disassembling ProbeResponse frame header");
    return false;
  }

  // Validate frame is proper type/subtype
  if (this->Subtype() != ManagementFrame::SUBTYPE_PROBERESP)
  {
    ZLOG_ERR("Invalid subtype: " + ZLOG_INT(this->Subtype()));
    return (false);
  }

  f = (ieee80211_proberesp*) sb_.Data();

  if (sb_.Pull(sizeof(f->timestamp)))
  {
    this->Timestamp(le64toh(f->timestamp));
  }
  else
  {
    ZLOG_ERR("Missing timestamp field");
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

  if (sb_.Pull(sizeof(f->capabilities)))
  {
    this->Capabilities(le16toh(f->capabilities));
  }
  else
  {
    ZLOG_ERR("Missing capabilities field");
    return (false);
  }

  if (not this->DisassembleTags(sb_))
  {
    ZLOG_ERR("Error disassembling ProbeResponse frame tags");
    return (false);
  }

  if (!this->GetTag(this->Ssid))
  {
    ZLOG_ERR("Error disassembling probe response frame: Missing SSID");
    return (false);
  }

  if (!this->GetTag(this->Rates))
  {
    ZLOG_ERR("Error disassembling probe response frame: Missing Rates");
    return (false);
  }

  // NOTE: ORDER MATTERS!!!
  this->GetTag(this->Dsss);
  this->GetTag(this->Country);
  this->GetTag(this->ChannelSwitch);
  this->GetTag(this->ErpInfo);
  this->GetTag(this->ExtRates);
  this->GetTag(this->SuppOpClass);
  this->GetTag(this->HtCaps);
  this->GetTag(this->HtInfo);
  this->GetTag(this->ExtCaps);
  this->GetTag(this->WmmWme);

  return true;
}

uint8_t*
ProbeResponse::Assemble(uint8_t* p_, size_t& rem_, bool fcs_)
{
  p_ = ManagementFrame::Assemble(p_, rem_, fcs_);
  struct ieee80211_proberesp* f = (ieee80211_proberesp*) p_;

  if (f == NULL)
  {
    ZLOG_WARN("Error assembling probe response frame: " + ZLOG_INT(rem_));
    return (NULL);
  }

  p_ = this->chklen(p_, sizeof(f->timestamp), rem_);
  if (!p_)
  {
    return (NULL);
  }
  f->timestamp = htole64(this->Timestamp());

  p_ = this->chklen(p_, sizeof(f->interval), rem_);
  if (!p_)
  {
    return (NULL);
  }
  f->interval = htole16(this->Interval());

  p_ = this->chklen(p_, sizeof(f->capabilities), rem_);
  if (!p_)
  {
    return (NULL);
  }
  f->capabilities = htole16(this->Capabilities());

  if (!this->PutTag(this->Ssid))
  {
    ZLOG_ERR("Error assembling probe response frame: Missing SSID");
    return (NULL);
  }

  if (!this->PutTag(this->Rates))
  {
    ZLOG_ERR("Error assembling probe response frame: Missing Rates");
    return (NULL);
  }

  // NOTE: ORDER MATTERS!!!

  this->PutTag(this->Dsss);
  this->PutTag(this->Country);
  this->PutTag(this->ChannelSwitch);
  this->PutTag(this->ErpInfo);
  this->PutTag(this->ExtRates);
  this->PutTag(this->SuppOpClass);
  this->PutTag(this->HtCaps);
  this->PutTag(this->HtInfo);
  this->PutTag(this->ExtCaps);
  this->PutTag(this->WmmWme);

  p_ = this->Frame::AssembleTags(p_, rem_);
  if (!p_)
  {
    ZLOG_ERR("Error assembling probe response frame tags: " + ZLOG_INT(rem_));
    return (NULL);
  }

  return (p_);
}

uint8_t*
ProbeResponse::Disassemble(uint8_t* p_, size_t& rem_, bool fcs_)
{
  p_ = ManagementFrame::Disassemble(p_, rem_, fcs_);
  struct ieee80211_proberesp* f = (ieee80211_proberesp*) p_;

  if (f == NULL)
  {
    ZLOG_WARN("Error disassembling probe response frame: " + ZLOG_INT(rem_));
    return (NULL);
  }

  p_ = this->chklen(p_, sizeof(f->timestamp), rem_);
  if (!p_ || !this->Timestamp(le64toh(f->timestamp)))
  {
    return (NULL);
  }

  p_ = this->chklen(p_, sizeof(f->interval), rem_);
  if (!p_ || !this->Interval(le16toh(f->interval)))
  {
    return (NULL);
  }

  p_ = this->chklen(p_, sizeof(f->capabilities), rem_);
  if (!p_ || !this->Capabilities(le16toh(f->capabilities)))
  {
    return (NULL);
  }

  p_ = this->Frame::DisassembleTags((uint8_t*) &f->tags, rem_);
  if (!p_)
  {
    ZLOG_ERR("Error disassembling probe response frame tags: " + ZLOG_INT(rem_));
    return (NULL);
  }

  if (!this->GetTag(this->Ssid))
  {
    ZLOG_ERR("Error disassembling probe response frame: Missing SSID");
    return (NULL);
  }

  if (!this->GetTag(this->Rates))
  {
    ZLOG_ERR("Error disassembling probe response frame: Missing Rates");
    return (NULL);
  }

  // NOTE: ORDER MATTERS!!!
  this->GetTag(this->Dsss);
  this->GetTag(this->Country);
  this->GetTag(this->ChannelSwitch);
  this->GetTag(this->ErpInfo);
  this->GetTag(this->ExtRates);
  this->GetTag(this->SuppOpClass);
  this->GetTag(this->HtCaps);
  this->GetTag(this->HtInfo);
  this->GetTag(this->ExtCaps);
  this->GetTag(this->WmmWme);

  return (p_);
}

uint64_t
ProbeResponse::Timestamp() const
{
  return (this->_ts);
}

bool
ProbeResponse::Timestamp(const uint64_t ts_)
{
  this->_ts = ts_;
  return (true);
}

uint16_t
ProbeResponse::Interval() const
{
  return (this->_interval);
}

bool
ProbeResponse::Interval(const uint16_t interval_)
{
  this->_interval = interval_;
  return (true);
}

uint16_t
ProbeResponse::Capabilities() const
{
  return (this->_capabilities);
}

bool
ProbeResponse::Capabilities(const uint16_t capabilities_)
{
  this->_capabilities = capabilities_;
  return (true);
}

void
ProbeResponse::Display() const
{
  ManagementFrame::Display();
  std::cout << "----- IEEE802.11 Probe Response ----------" << std::endl;
  std::cout << "\tTS:       \t" << std::hex << this->Timestamp() << std::dec << std::endl;
  std::cout << "\tInterval: \t" << (int) this->Interval() << std::endl;
  std::cout << "\tCap:      \t" << std::hex << this->Capabilities() << std::dec << std::endl;
  if (this->Ssid.Valid())
    this->Ssid.Display();
  if (this->Rates.Valid())
    this->Rates.Display();
  if (this->Dsss.Valid())
    this->Dsss.Display();
  if (this->Country.Valid())
    this->Country.Display();
  if (this->ChannelSwitch.Valid())
    this->ChannelSwitch.Display();
  if (this->ErpInfo.Valid())
    this->ErpInfo.Display();
  if (this->ExtRates.Valid())
    this->ExtRates.Display();
  if (this->SuppOpClass.Valid())
    this->SuppOpClass.Display();
  if (this->HtCaps.Valid())
    this->HtCaps.Display();
  if (this->HtInfo.Valid())
    this->HtInfo.Display();
  if (this->ExtCaps.Valid())
    this->ExtCaps.Display();
  if (this->WmmWme.Valid())
    this->WmmWme.Display();
}

}
}
}
