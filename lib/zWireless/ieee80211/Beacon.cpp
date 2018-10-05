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
using namespace zUtils;
#include <zutils/ieee80211/Beacon.h>

// local includes
#include <zutils/ieee80211/ieee80211.h>

ZLOG_MODULE_INIT(zLog::Log::MODULE_WIRELESS);

namespace zUtils
{
namespace zWireless
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

bool
Beacon::Assemble(zSocket::Buffer& sb_, bool fcs_)
{

  // latch ptrs to beginning of data (_head), end of HEAD tags, start of TAIL tags (_tail) and end of TAIL tags (_end)
  _head = sb_.Head();
  _tail = sb_.Head();
  _end = sb_.Head();

  if (not ManagementFrame::Assemble(sb_, fcs_) || this->Subtype() != Frame::SUBTYPE_BEACON)
  {
    ZLOG_ERR("Error assembling beacon frame header");
    return (false);
  }

  struct ieee80211_beacon* f = (struct ieee80211_beacon*) sb_.Data();

  if (sb_.Put(sizeof(f->timestamp)))
  {
    f->timestamp = htole64(this->Timestamp());
    sb_.Pull(sizeof(f->timestamp));
  }
  else
  {
    ZLOG_ERR("Error assembling beacon frame");
    return (false);
  }

  _tail = sb_.Data();
  _end = sb_.Data();

  if (sb_.Put(sizeof(f->interval)))
  {
    f->interval = htole16(this->Interval());
    sb_.Pull(sizeof(f->interval));
  }
  else
  {
    ZLOG_ERR("Error assembling beacon frame");
    return (false);
  }

  _tail = sb_.Data();
  _end = sb_.Data();

  if (sb_.Put(sizeof(f->capabilities)))
  {
    f->capabilities = htole16(this->Capabilities());
    sb_.Pull(sizeof(f->capabilities));
  }
  else
  {
    ZLOG_ERR("Error assembling beacon frame");
    return (false);
  }

  _tail = sb_.Data();
  _end = sb_.Data();

  if (!this->PutTag(this->Ssid))
  {
    ZLOG_ERR("Error assembling beacon frame: Missing SSID");
    return (false);
  }

  if (!this->PutTag(this->Rates))
  {
    ZLOG_ERR("Error assembling beacon frame: Missing Rates");
    return (false);
  }

  // NOTE: ORDER MATTERS!!!
  this->PutTag(this->Dsss);
  this->PutTag(this->Tim);
  this->PutTag(this->Country);
  this->PutTag(this->ChannelSwitch);
  this->PutTag(this->ErpInfo);
  this->PutTag(this->ExtRates);
  this->PutTag(this->SuppOpClass);
  this->PutTag(this->HtCaps);
  this->PutTag(this->HtInfo);
  this->PutTag(this->ExtCaps);
  this->PutTag(this->WmmWme);

  if (not AssembleTags(sb_, TAGTYPE_HEAD))
  {
    ZLOG_ERR("Error assembling beacon frame tags");
    return (false);
  }

  _tail = sb_.Data();
  _end = sb_.Data();

  if (not AssembleTags(sb_, TAGTYPE_TAIL))
  {
    ZLOG_ERR("Error assembling beacon frame tags");
    return (false);
  }

  _end = sb_.Data();

  return true;
}

bool
Beacon::Disassemble(zSocket::Buffer& sb_, bool fcs_)
{
  // latch ptrs to beginning of data (_head), end of HEAD tags, start of TAIL tags (_tail) and end of TAIL tags (_end)
  _head = sb_.Head();
  _tail = sb_.Head();
  _end = sb_.Head();

  struct ieee80211_beacon* f = (ieee80211_beacon*) sb_.Data();

  // Disassemble base and verify
  if (not ManagementFrame::Disassemble(sb_, fcs_))
  {
    ZLOG_ERR("Error disassembling beacon frame header");
    return false;
  }

  // Validate frame is proper type/subtype
  if (this->Subtype() != ManagementFrame::SUBTYPE_BEACON)
  {
    ZLOG_ERR("Invalid subtype: " + ZLOG_INT(this->Subtype()));
    return (false);
  }

  f = (ieee80211_beacon*) sb_.Data();

  if (sb_.Pull(sizeof(f->timestamp)))
  {
    this->Timestamp(le64toh(f->timestamp));
  }
  else
  {
    ZLOG_ERR("Missing timestamp field");
    return (false);
  }

  _tail = sb_.Data();
  _end = sb_.Data();

  if (sb_.Pull(sizeof(f->interval)))
  {
    this->Interval(le16toh(f->interval));
  }
  else
  {
    ZLOG_ERR("Missing interval field");
    return (false);
  }

  _tail = sb_.Data();
  _end = sb_.Data();

  if (sb_.Pull(sizeof(f->capabilities)))
  {
    this->Capabilities(le16toh(f->capabilities));
  }
  else
  {
    ZLOG_ERR("Missing capabilities field");
    return (false);
  }

  _tail = sb_.Data();
  _end = sb_.Data();

  if (not this->DisassembleTags(sb_, TAGTYPE_HEAD))
  {
    ZLOG_ERR("Error disassembling beacon frame tags");
    return (false);
  }

  _tail = sb_.Data();
  _end = sb_.Data();

  if (not this->DisassembleTags(sb_, TAGTYPE_TAIL))
  {
    ZLOG_ERR("Error disassembling beacon frame tags");
    return (false);
  }

  _end = sb_.Data();

  if (!this->GetTag(this->Ssid))
  {
    ZLOG_ERR("Error disassembling beacon frame: Missing SSID");
    return (false);
  }

  if (!this->GetTag(this->Rates))
  {
    ZLOG_ERR("Error disassembling beacon frame: Missing Rates");
    return (false);
  }

  // NOTE: ORDER MATTERS!!!	  //RKB Frame::GetTag(Tag& tag_, const int index_) appears to iterate all tags on each call - how do we verify order?
  this->GetTag(this->Dsss);
  this->GetTag(this->Tim);
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
  std::cout << "\tTS:       \t" << std::hex << this->Timestamp() << std::dec << std::endl;
  std::cout << "\tInterval: \t" << (int) this->Interval() << std::endl;
  std::cout << "\tCap:      \t" << std::hex << this->Capabilities() << std::dec << std::endl;
  if (this->Ssid.Valid()) this->Ssid.Display();
  if (this->Rates.Valid()) this->Rates.Display();
  if (this->Dsss.Valid()) this->Dsss.Display();
  if (this->Tim.Valid()) this->Tim.Display();
  if (this->Country.Valid()) this->Country.Display();
  if (this->ChannelSwitch.Valid()) this->ChannelSwitch.Display();
  if (this->ErpInfo.Valid()) this->ErpInfo.Display();
  if (this->ExtRates.Valid()) this->ExtRates.Display();
  if (this->SuppOpClass.Valid()) this->SuppOpClass.Display();
  if (this->HtCaps.Valid()) this->HtCaps.Display();
  if (this->HtInfo.Valid()) this->HtInfo.Display();
  if (this->ExtCaps.Valid()) this->ExtCaps.Display();
  if (this->WmmWme.Valid()) this->WmmWme.Display();
}

}
}
}
