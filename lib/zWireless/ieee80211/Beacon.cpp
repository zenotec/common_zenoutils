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
#include "Beacon.h"

namespace zUtils
{
namespace ieee80211
{

//*****************************************************************************
// Class: Beacon
//*****************************************************************************

Beacon::Beacon() :
    ManagementFrame(ManagementFrame::SUBTYPE_BEACON), _ts(0), _interval(0),
    _capabilities(0), _head(NULL), _tail(NULL), _end(NULL)
{
  this->ReceiverAddress("ff:ff:ff:ff:ff:ff");
}

Beacon::~Beacon()
{
}

uint64_t
Beacon::Timestamp() const
{
  return(this->_ts);
}

bool
Beacon::Timestamp(const uint64_t ts_)
{
  this->_ts = ts_;
  return(true);
}

uint16_t
Beacon::Interval() const
{
  return(this->_interval);
}

bool
Beacon::Interval(const uint16_t int_)
{
  this->_interval = int_;
  return(true);
}

uint16_t
Beacon::Capabilities() const
{
  return(this->_capabilities);
}

bool
Beacon::Capabilities(const uint16_t cap_)
{
  this->_capabilities = cap_;
  return(true);
}

uint8_t*
Beacon::Assemble(uint8_t* p_, size_t& rem_, bool fcs_)
{

  p_ = ManagementFrame::Assemble(p_, rem_, fcs_);
  struct ieee80211_beacon* f = (ieee80211_beacon*) p_;
  this->_head = NULL;
  this->_tail = NULL;
  this->_end = NULL;

  if (p_ == NULL)
  {
    ZLOG_ERR("Error assembling beacon frame header: " + ZLOG_INT(rem_));
    return (NULL);
  }

  if (this->Subtype() != ManagementFrame::SUBTYPE_BEACON)
  {
    ZLOG_ERR("Invalid subtype: " + ZLOG_INT(this->Subtype()));
    return (NULL);
  }

  this->_head = p_;
  p_ = this->chklen(p_, sizeof(f->timestamp), rem_);
  if (!p_)
  {
    return (NULL);
  }
  f->timestamp = htole64(this->Timestamp());
  this->_tail = p_;
  this->_end = p_;

  p_ = this->chklen(p_, sizeof(f->interval), rem_);
  if (!p_)
  {
    return (NULL);
  }
  f->interval = htole16(this->Interval());
  this->_tail = p_;
  this->_end = p_;

  p_ = this->chklen(p_, sizeof(f->capabilities), rem_);
  if (!p_)
  {
    return (NULL);
  }
  f->capabilities = htole16(this->Capabilities());
  this->_tail = p_;
  this->_end = p_;

  // Inserted in map sorted by ID
  this->PutTag(this->Ssid);
  this->PutTag(this->Rates);
  this->PutTag(this->Dsss);
  this->PutTag(this->Tim);
  this->PutTag(this->PowerCaps);
  this->PutTag(this->HtCaps);
  this->PutTag(this->ExtRates);
  this->PutTag(this->WmmWme);

  p_ = this->AssembleTags(p_, rem_, TAGTYPE_HEAD);
  if (!p_)
  {
    ZLOG_ERR("Error assembling beacon frame tags: " + ZLOG_INT(rem_));
    return(NULL);
  }
  this->_tail = p_;
  this->_end = p_;

  p_ = this->AssembleTags(p_, rem_, TAGTYPE_TAIL);
  if (!p_)
  {
    ZLOG_ERR("Error assembling beacon frame tags: " + ZLOG_INT(rem_));
    return(NULL);
  }
  this->_end = p_;

  return (p_);
}

uint8_t*
Beacon::Disassemble(uint8_t* p_, size_t& rem_, bool fcs_)
{

  p_ = ManagementFrame::Disassemble(p_, rem_, fcs_);
  struct ieee80211_beacon* f = (ieee80211_beacon*) p_;
  this->_head = NULL;
  this->_tail = NULL;
  this->_end = NULL;

  if (f == NULL)
  {
    ZLOG_ERR("Error disassembling beacon frame header: " + ZLOG_INT(rem_));
    return (NULL);
  }

  if (this->Subtype() != ManagementFrame::SUBTYPE_BEACON)
  {
    ZLOG_ERR("Invalid subtype: " + ZLOG_INT(this->Subtype()));
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

  p_ = this->DisassembleTags((uint8_t*)&f->tags, rem_);
  if (!p_)
  {
    ZLOG_ERR("Error disassembling beacon frame tags: " + ZLOG_INT(rem_));
    return (NULL);
  }

  this->GetTag(this->Ssid);
  this->GetTag(this->Rates);
  this->GetTag(this->Dsss);
  this->GetTag(this->Tim);
  this->GetTag(this->PowerCaps);
  this->GetTag(this->HtCaps);
  this->GetTag(this->ExtRates);
  this->GetTag(this->WmmWme);

  return (p_);
}

uint8_t*
Beacon::Head() const
{
  return (this->_head);
}

size_t
Beacon::HeadSize() const
{
  return (this->_tail - this->_head);
}

uint8_t*
Beacon::Tail() const
{
  return (this->_tail);
}

size_t
Beacon::TailSize() const
{
  return (this->_end - this->_tail);
}

void
Beacon::Display() const
{
  ManagementFrame::Display();
  std::cout << "----- IEEE802.11 Beacon ------------------" << std::endl;
  std::cout << "\tTS:       \t" << std::hex << this->Timestamp() << std::endl;
  std::cout << "\tInterval: \t" << (int) this->Interval() << std::endl;
  std::cout << "\tCap:      \t" << std::hex << this->Capabilities() << std::endl;
  if (this->Ssid.Valid()) this->Ssid.Display();
  if (this->Rates.Valid()) this->Rates.Display();
  if (this->Dsss.Valid()) this->Dsss.Display();
  if (this->Tim.Valid()) this->Tim.Display();
  if (this->PowerCaps.Valid()) this->PowerCaps.Display();
  if (this->HtCaps.Valid()) this->HtCaps.Display();
  if (this->ExtRates.Valid()) this->ExtRates.Display();
  if (this->WmmWme.Valid()) this->WmmWme.Display();
}

}
}
