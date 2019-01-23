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
#include <zutils/ieee8023/VlanFrame.h>
using namespace zSocket;

// local includes

#include <zutils/ieee8023/ieee8023.h>

ZLOG_MODULE_INIT(zUtils::zLog::Log::MODULE_WIRELESS);

namespace zUtils
{
namespace zSocket
{
namespace ieee8023
{

//*****************************************************************************
// Class: VlanFrame
//*****************************************************************************

VlanFrame::VlanFrame() :
    Frame(Frame::SUBTYPE_VLAN)
{
}

VlanFrame::~VlanFrame()
{
}

bool
VlanFrame::Assemble(zSocket::Buffer& sb_, bool fcs_)
{

  // NOTE: Assumes caller's socket buffer data and tail are set to start of frame (empty)

  // Initialize frame pointer to start of data
  struct ieee8023_hdr* f = (struct ieee8023_hdr*)sb_.Data();

  return (false); // TODO

}

bool
VlanFrame::Disassemble(zSocket::Buffer& sb_, bool fcs_)
{
  // NOTE: Assumes caller's socket buffer data is set to start of frame and
  //   tail is set to end of frame (full)

  // Initialize frame pointer to start of data
  struct ieee8023_hdr* f = (struct ieee8023_hdr*)sb_.Data();

  return (false); // TODO

}

void
VlanFrame::Display(const std::string& prefix_) const
{
  Frame::Display(prefix_);
  std::cout << prefix_ << "----- VLAN Header ----------------------" << std::endl;
}

}
}
}
