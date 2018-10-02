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
#include <iomanip>

// libc++ includes
#include <iostream>

// libzutils includes
#include <zutils/zLog.h>
#include <zutils/ieee80211/Authentication.h>
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
// Class: Authentication
//*****************************************************************************

Authentication::Authentication() :
    ManagementFrame(ManagementFrame::SUBTYPE_AUTH),
    _algorithm(0),
    _sequenceNumber(0),
    _statusCode(0)
{
}

Authentication::~Authentication()
{
}

bool
Authentication::Assemble(zSocket::Buffer& sb_, bool fcs_)
{
  if (not ManagementFrame::Assemble(sb_, fcs_) || this->Subtype() != Frame::SUBTYPE_AUTH)
  {
    ZLOG_ERR("Error assembling Authentication frame header");
    return (false);
  }

  struct ieee80211_auth* f = (struct ieee80211_auth*) sb_.Data();

  if (sb_.Put(sizeof(f->algorithm)))
  {
    f->algorithm = htole16(this->Algorithm());
    sb_.Pull(sizeof(f->algorithm));
  }
  else
  {
    ZLOG_ERR("Error assembling Authentication frame");
    return (false);
  }

  if (sb_.Put(sizeof(f->seqNumber)))
  {
    f->seqNumber = htole16(this->AuthSequenceNumber());
    sb_.Pull(sizeof(f->seqNumber));
  }
  else
  {
    ZLOG_ERR("Error assembling Authentication frame");
    return (false);
  }

  if (sb_.Put(sizeof(f->statusCode)))
  {
    f->statusCode = htole16(this->StatusCode());
    sb_.Pull(sizeof(f->statusCode));
  }
  else
  {
    ZLOG_ERR("Error assembling Authentication frame");
    return (false);
  }

  // Authentication doesn't implement any tags, but keep this for when we implement vendor specific tag
  if (not AssembleTags(sb_))
  {
    ZLOG_ERR("Error assembling AssociationResponse frame tags");
    return (false);
  }

  return true;
}

bool
Authentication::Disassemble(zSocket::Buffer& sb_, bool fcs_)
{
  struct ieee80211_auth* f = (ieee80211_auth*) sb_.Data();

  // Disassemble base and verify
  if (not ManagementFrame::Disassemble(sb_, fcs_))
  {
    ZLOG_ERR("Error disassembling Authentication frame header");
    return false;
  }

  // Validate frame is proper type/subtype
  if (this->Subtype() != ManagementFrame::SUBTYPE_AUTH)
  {
    ZLOG_ERR("Invalid subtype: " + ZLOG_INT(this->Subtype()));
    return (false);
  }

  f = (ieee80211_auth*) sb_.Data();

  if (sb_.Pull(sizeof(f->algorithm)))
  {
    this->Algorithm(le16toh(f->algorithm));
  }
  else
  {
    ZLOG_ERR("Missing algorithm field");
    return (false);
  }

  if (sb_.Pull(sizeof(f->seqNumber)))
  {
    this->SequenceNumber(le16toh(f->seqNumber));
  }
  else
  {
    ZLOG_ERR("Missing seqNumber field");
    return (false);
  }

  if (sb_.Pull(sizeof(f->statusCode)))
  {
    this->StatusCode(le16toh(f->statusCode));
  }
  else
  {
    ZLOG_ERR("Missing statusCode field");
    return (false);
  }

  // Authentication doesn't implement any tags, but keep this for when we implement vendor specific tag
  if (not this->DisassembleTags(sb_))
  {
    ZLOG_ERR("Error disassembling Authentication frame tags");
    return (false);
  }

  return true;
}

uint16_t
Authentication::Algorithm() const
{
  return(this->_algorithm);
}

bool
Authentication::Algorithm(const uint16_t algorithm_)
{
  this->_algorithm = algorithm_;
  return(true);
}

uint16_t
Authentication::AuthSequenceNumber() const
{
  return(this->_sequenceNumber);
}

bool
Authentication::SequenceNumber(const uint16_t sequenceNumber_)
{
  this->_sequenceNumber = sequenceNumber_;
  return(true);
}

uint16_t
Authentication::StatusCode() const
{
  return(this->_statusCode);
}

bool
Authentication::StatusCode(const uint16_t statusCode_)
{
  this->_statusCode = statusCode_;
  return(true);
}

void
Authentication::Display() const
{
  ManagementFrame::Display();
  std::cout << "----- IEEE802.11 Authentication  -----------" << std::endl;
  cout << "Algorithm: " << Authentication::ToString(ALGORITHM_TYPE(this->_algorithm)) << endl;
  cout << "Sequence Number: " << int(_sequenceNumber) << endl;
  cout << "Status Code: " << int(_statusCode) << " (0x" << std::hex << setw(2) << setfill('0') << int(_statusCode) << ")" << std::dec << endl;
}


}
}
}
