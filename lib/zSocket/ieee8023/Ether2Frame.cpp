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
#include <string.h>
#include <endian.h>

// libc++ includes
#include <iostream>

// libzutils includes
#include <zutils/zLog.h>
using namespace zUtils;
#include <zutils/ieee8023/Frame.h>
#include <zutils/ieee8023/Ether2Frame.h>
using namespace zSocket;

// local includes

#include "ieee8023.h"

ZLOG_MODULE_INIT(zUtils::zLog::Log::MODULE_WIRELESS);

namespace zUtils
{
namespace zSocket
{
namespace ieee8023
{

//*****************************************************************************
// Class: Frame
//*****************************************************************************

Ether2Frame::Ether2Frame() :
    Frame(Frame::TYPE_ETHER2)
{
}

Ether2Frame::~Ether2Frame()
{
}

uint8_t*
Ether2Frame::Assemble(uint8_t* p_, size_t& rem_, bool fcs_)
{


  struct ieee8023_hdr* f = (struct ieee8023_hdr*) p_;

  // Assemble lower level frame and validate
  p_ = Frame::Assemble(p_, rem_, fcs_);
  if (!p_ || (this->GetType() != Frame::TYPE_ETHER2))
  {
    ZLOG_WARN("Error assembling frame: " + ZLOG_INT(rem_));
    return (NULL);
  }

  // Write protocol identifier
  p_ = this->chklen(p_, sizeof(f->u.ether2.proto), rem_);
  if (!p_)
  {
    ZLOG_WARN("Error assembling frame: " + ZLOG_INT(rem_));
    return(NULL);
  }
  f->u.ether2.proto = htobe16(uint16_t(this->GetProto()));

  // Write payload
  uint8_t* pay = p_;
  size_t len = this->GetPayloadLength();

  if (!(p_ = this->chklen(pay, len, rem_)))
  {
    ZLOG_ERR("Buffer overrun");
    return (NULL);
  }
  if (this->GetPayload(pay, len) != len)
  {
    ZLOG_WARN("Missing or invalid payload");
  }

  return (p_);
}

uint8_t*
Ether2Frame::Disassemble(uint8_t* p_, size_t& rem_, bool fcs_)
{

  struct ieee8023_hdr* f = (struct ieee8023_hdr*) p_;

  // Disassemble lower level frame and validate
  p_ = Frame::Disassemble(p_, rem_, fcs_);
  if (!p_ || (this->GetType() != Frame::TYPE_ETHER2))
  {
    ZLOG_WARN("Error disassembling frame: " + ZLOG_INT(rem_));
    return (NULL);
  }

  // Read and save protocol identifier
  p_ = this->chklen(p_, sizeof(f->u.ether2.proto), rem_);
  if (!p_ || !this->SetProto(Frame::PROTO(be16toh(f->u.ether2.proto))))
  {
    ZLOG_WARN("Error disassembling frame: " + ZLOG_INT(rem_));
    return(NULL);
  }

  // Copy out the frame payload
  p_ = this->chklen(p_, rem_, rem_);
  if (!p_ || !this->PutPayload(f->u.ether2.data, rem_))
  {
    p_ = NULL;
  }

  return (p_);
}

void
Ether2Frame::Display() const
{
  std::cout << "----- Ether2 Header ----------------------" << std::endl;
}

}
}
}
