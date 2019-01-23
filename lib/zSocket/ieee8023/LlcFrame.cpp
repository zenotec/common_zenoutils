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
#include <zutils/ieee8023/LlcFrame.h>
using namespace zSocket;

// local includes

#include <zutils/ieee8023/ieee8023.h>

ZLOG_MODULE_INIT(zUtils::zLog::Log::MODULE_SOCKET);

namespace zUtils
{
namespace zSocket
{
namespace ieee8023
{

//*****************************************************************************
// Class: LlcFrame
//*****************************************************************************

LlcFrame::LlcFrame() :
    Frame(Frame::SUBTYPE_LLC)
{
}

LlcFrame::~LlcFrame()
{
}

bool
LlcFrame::Assemble(zSocket::Buffer& sb_, bool fcs_)
{

  // NOTE: Assumes caller's socket buffer data and tail are set to start of frame (empty)

  // Initialize frame pointer to start of data
  struct ieee8023_hdr* f = (struct ieee8023_hdr*)sb_.Data();

  return (false); // TODO

//  // Assemble lower level frame and validate
//  p_ = Frame::Assemble(p_, rem_, fcs_);
//  if (!p_ || (this->GetType() != Frame::TYPE_ETHER))
//  {
//    ZLOG_WARN("Error assembling frame: " + ZLOG_INT(rem_));
//    return (NULL);
//  }
//
//  // Write protocol identifier
//  p_ = this->chklen(p_, sizeof(f->u.llc), rem_);
//  if (!p_)
//  {
//    ZLOG_WARN("Error assembling frame: " + ZLOG_INT(rem_));
//    return(NULL);
//  }
//  f->u.llc.dst_sap = 0xaa;
//  f->u.llc.src_sap = 0xaa;
//  f->u.llc.cntl = 0x03;
//  f->u.llc.oui[0] = 0x00;
//  f->u.llc.oui[1] = 0x00;
//  f->u.llc.oui[2] = 0x00;
//  f->u.llc.proto = htobe16(uint16_t(this->GetProto()));
//
//  // Write payload
//  uint8_t* pay = p_;
//  size_t len = this->GetPayloadLength();
//
//  if (!(p_ = this->chklen(pay, len, rem_)))
//  {
//    ZLOG_ERR("Buffer overrun");
//    return (NULL);
//  }
//  if (this->GetPayload(pay, len) != len)
//  {
//    ZLOG_WARN("Missing or invalid payload");
//  }

}

bool
LlcFrame::Disassemble(zSocket::Buffer& sb_, bool fcs_)
{
  // NOTE: Assumes caller's socket buffer data is set to start of frame and
  //   tail is set to end of frame (full)

  // Initialize frame pointer to start of data
  struct ieee8023_hdr* f = (struct ieee8023_hdr*)sb_.Data();

  return (false); // TODO

//  // Disassemble lower level frame and validate
//  p_ = Frame::Disassemble(p_, rem_, fcs_);
//  if (!p_ || (this->GetType() != Frame::TYPE_ETHER))
//  {
//    ZLOG_WARN("Error disassembling frame: " + ZLOG_INT(rem_));
//    return (NULL);
//  }
//
////  // Read and save protocol identifier
////  p_ = this->chklen(p_, sizeof(f->u.ether2.proto), rem_);
////  if (!p_ || !this->SetProto(Frame::PROTO(be16toh(f->u.ether2.proto))))
////  {
////    ZLOG_WARN("Error disassembling frame: " + ZLOG_INT(rem_));
////    return(NULL);
////  }
//
//  // Copy out the frame payload
//  uint8_t* pay = p_;
//  size_t len = rem_;
//  p_ = this->chklen(pay, len, rem_);
//  if (!p_ || !this->PutPayload(pay, len))
//  {
//    ZLOG_WARN("Error disassembling frame: " + ZLOG_INT(rem_));
//    return(NULL);
//  }

}

void
LlcFrame::Display(const std::string& prefix_) const
{
  Frame::Display(prefix_);
  std::cout << prefix_ << "----- LLC Header ----------------------" << std::endl;
}

}
}
}
