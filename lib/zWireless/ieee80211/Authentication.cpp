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
using namespace zUtils;

// local includes
#include "Authentication.h"

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
    ManagementFrame(ManagementFrame::SUBTYPE_AUTHENTICATE), _algorithm(0), _sequenceNumber(0), _statusCode(0)
{
}

Authentication::~Authentication()
{
}

uint8_t*
Authentication::Assemble(uint8_t* p_, size_t& rem_, bool fcs_)
{

  p_ = ManagementFrame::Assemble(p_, rem_, fcs_);
  struct ieee80211_auth* f = (ieee80211_auth*) p_;

  if (f == NULL)
  {
    ZLOG_WARN("Error assembling authentication frame: " + ZLOG_INT(rem_));
    return (NULL);
  }

  p_ = this->chklen(p_, sizeof(f->algorithm), rem_);
  if (!p_)
  {
    return (NULL);
  }
  f->algorithm = htole16(this->Algorithm());

  p_ = this->chklen(p_, sizeof(f->seqNumber), rem_);
  if (!p_)
  {
    return (NULL);
  }
  f->seqNumber = htole16(this->AuthSequenceNumber());

  p_ = this->chklen(p_, sizeof(f->statusCode), rem_);
  if (!p_)
  {
    return (NULL);
  }
  f->statusCode = htole16(this->StatusCode());

  p_ = this->AssembleTags(p_, rem_);
  if (!p_)
  {
    ZLOG_ERR("Error assembling authentication frame tags: " + ZLOG_INT(rem_));
    return(NULL);
  }

  return (p_);
}

uint8_t*
Authentication::Disassemble(uint8_t* p_, size_t& rem_, bool fcs_)
{

  p_ = ManagementFrame::Disassemble(p_, rem_, fcs_);
  struct ieee80211_auth* f = (ieee80211_auth*) p_;

  if (f == NULL)
  {
    ZLOG_WARN("Error disassembling authentication frame: " + ZLOG_INT(rem_));
    return (NULL);
  }

  p_ = this->chklen(p_, sizeof(f->algorithm), rem_);
  if (!p_ || !this->Algorithm(le16toh(f->algorithm)))
  {
    return (NULL);
  }

  p_ = this->chklen(p_, sizeof(f->seqNumber), rem_);
  if (!p_ || !this->SequenceNumber(le16toh(f->seqNumber)))
  {
    return (NULL);
  }

  p_ = this->chklen(p_, sizeof(f->statusCode), rem_);
  if (!p_ || !this->StatusCode(le16toh(f->statusCode)))
  {
    return (NULL);
  }

  p_ = this->DisassembleTags((uint8_t*)&f->tags, rem_);
  if (!p_)
  {
    ZLOG_ERR("Error disassembling authentication frame tags: " + ZLOG_INT(rem_));
    return (NULL);
  }

  return (p_);
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
  cout << "Status Code: " << int(_statusCode) << " (0x" << hex << setw(2) << setfill('0') << int(_statusCode) << ")" << endl;
}


}
}
}
