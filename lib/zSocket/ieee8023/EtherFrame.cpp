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
#include <zutils/ieee8023/EtherFrame.h>
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
// Class: EtherFrame
//*****************************************************************************

EtherFrame::EtherFrame() :
    Frame(Frame::SUBTYPE_ETHER), _length(0)
{
}

EtherFrame::~EtherFrame()
{
}

bool
EtherFrame::Assemble(zSocket::Buffer& sb_, bool fcs_)
{

  // NOTE: Assumes caller's socket buffer data and tail are set to start of frame (empty)

  // Initialize frame pointer to start of data
  struct ieee8023_hdr* f = (struct ieee8023_hdr*)sb_.Data();

  // Assemble lower level frame and validate
  if (!Frame::Assemble(sb_, fcs_) || (this->GetSubtype() != Frame::SUBTYPE_ETHER))
  {
    ZLOG_WARN("Error assembling frame");
    return (false);
  }

  // Write length
  if (sb_.Put(sizeof(f->u.ether.length)))
  {
    f->u.ether.length = htobe16(this->GetLength());
    sb_.Pull(sizeof(f->u.ether.length));
  }
  else
  {
    ZLOG_WARN("Error assembling frame");
    return (false);
  }

  // Write payload (use actual payload length, not length field)
  uint8_t* pay = f->u.ether.data;
  size_t len = this->GetPayloadLength();
  if (sb_.Put(len) && this->GetPayload(pay, len))
  {
    sb_.Pull(len);
  }
  else
  {
    ZLOG_WARN("Error assembling frame");
    return (false);
  }

  return (true);
}

bool
EtherFrame::Disassemble(zSocket::Buffer& sb_, bool fcs_)
{

  // NOTE: Assumes caller's socket buffer data is set to start of frame and
  //   tail is set to end of frame (full)

  // Initialize frame pointer to start of data
  struct ieee8023_hdr* f = (struct ieee8023_hdr*)sb_.Data();

  // Disassemble lower level frame and validate
  if (!Frame::Disassemble(sb_, fcs_) || (this->GetSubtype() != Frame::SUBTYPE_ETHER))
  {
    ZLOG_WARN("Error assembling frame");
    return (false);
  }

  // Read out length field
  if (this->SetLength(be16toh(f->u.ether.length)))
  {
    sb_.Pull(sizeof(f->u.ether.length));
  }
  else
  {
    ZLOG_WARN("Error disassembling frame");
    return(false);
  }

  // Copy out the frame payload (use length of socket buffer, not length field)
  uint8_t* pay = f->u.ether.data;
  size_t len = sb_.Length();
  if (this->PutPayload(pay, len))
  {
    sb_.Pull(len);
  }
  else
  {
    ZLOG_WARN("Error disassembling frame");
    return(false);
  }

  return (true);
}

uint16_t
EtherFrame::GetLength() const
{
  return (this->_length);
}

bool
EtherFrame::SetLength(const uint16_t len_)
{
  this->_length = len_;
  return (true);
}

void
EtherFrame::Display(const std::string& prefix_) const
{
  Frame::Display(prefix_);
  std::cout << prefix_ << "----- Ether Header ----------------------" << std::endl;
}

}
}
}
