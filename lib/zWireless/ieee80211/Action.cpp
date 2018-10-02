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
#include <zutils/ieee80211/Action.h>
using namespace zUtils;

// local includes
#include "ieee80211.h"

ZLOG_MODULE_INIT(zUtils::zLog::Log::MODULE_WIRELESS);

namespace zUtils
{
namespace zWireless
{
namespace ieee80211
{

//*****************************************************************************
// Class: ActionRequest
//*****************************************************************************

ActionRequest::ActionRequest() :
    ManagementFrame(ManagementFrame::SUBTYPE_ACTION),
    _categoryCode(3),
    _actionCode(0),
    _dialogToken(0),
    _blockAckparms(0),
    _blockAckTimeout(0),
    _blockStartingSequence(0)

{
}

ActionRequest::~ActionRequest()
{
}

bool
ActionRequest::Assemble(zSocket::Buffer& sb_, bool fcs_)
{
  if (not ManagementFrame::Assemble(sb_, fcs_) || this->Subtype() != Frame::SUBTYPE_ACTION)
  {
    ZLOG_ERR("Error assembling ActionRequest frame header");
    return (false);
  }

  struct ieee80211_actionreq* f = (struct ieee80211_actionreq*) sb_.Data();

  return true;
}

bool
ActionRequest::Disassemble(zSocket::Buffer& sb_, bool fcs_)
{
  return true;
}

uint8_t
ActionRequest::Category() const
{
  return (this->_categoryCode);
}

bool
ActionRequest::Category(const uint8_t code_)
{
  this->_categoryCode = code_;
  return (true);
}

uint8_t
ActionRequest::Action() const
{
  return (this->_actionCode);
}

bool
ActionRequest::Action(const uint8_t code_)
{
  this->_actionCode = code_;
  return (true);
}

uint8_t
ActionRequest::Dialog() const
{
  return (this->_dialogToken);
}

bool
ActionRequest::Dialog(const uint8_t token_)
{
  this->_dialogToken = token_;
  return (true);
}

uint8_t
ActionRequest::TID() const
{
  return ((this->_blockAckparms >> 2) & 0x0f);
}

bool
ActionRequest::TID(const uint8_t id_)
{
  this->_blockAckparms &= ~0x003c;
  this->_blockAckparms |= ((id_ << 2) & 0x3c);
}

uint16_t
ActionRequest::Buffers() const
{
  return (this->_blockAckparms >> 6);
}

bool
ActionRequest::Buffers(const uint16_t num_)
{
  this->_blockAckparms &= ~0xffc0;
  this->_blockAckparms |= ((num_ << 6) & 0xffc0);
  return (true);
}

uint16_t
ActionRequest::StartingSequence() const
{
  return (this->_blockStartingSequence >> 4);
}

bool
ActionRequest::StartingSequence(const uint16_t seq_)
{
  this->_blockStartingSequence &= ~0xfff0;
  this->_blockStartingSequence |= ((seq_ << 4) & 0xfff0);
  return (true);
}

void
ActionRequest::Display() const
{
  ManagementFrame::Display();
  std::cout << "----- IEEE802.11 Action Request -----------" << std::endl;
  std::cout << "_categoryCode=" << int(_categoryCode) << std::endl;
  std::cout << "_actionCode=" << int(_actionCode) << std::endl;
}

//*****************************************************************************
// Class: ActionResponse
//*****************************************************************************

ActionResponse::ActionResponse() :
  ManagementFrame(ManagementFrame::SUBTYPE_ACTION),
  _categoryCode(3),
  _actionCode(1),
  _dialogToken(0),
  _status(0),
  _blockAckparms(0),
  _blockAckTimeout(0)
{
}

ActionResponse::~ActionResponse()
{
}

bool
ActionResponse::Assemble(zSocket::Buffer& sb_, bool fcs_)
{
  if (not ManagementFrame::Assemble(sb_, fcs_) || this->Subtype() != Frame::SUBTYPE_ACTION)
  {
    ZLOG_ERR("Error assembling ActionResponse frame header");
    return (false);
  }

  struct ieee80211_actionresp* f = (struct ieee80211_actionresp*) sb_.Data();

  return true;
}

bool
ActionResponse::Disassemble(zSocket::Buffer& sb_, bool fcs_)
{
  return true;
}

void
ActionResponse::Display() const
{
  ManagementFrame::Display();
  std::cout << "----- IEEE802.11 Action Response ----------" << std::endl;
  std::cout << "_categoryCode:\t" << int(_categoryCode) << std::endl;
  std::cout << "_actionCode:\t" << int(_actionCode) << std::endl;
  std::cout << "_dialogToken:\t" << std::hex << int(_dialogToken) << std::dec << std::endl;
  std::cout << "_status:\t" << int(_status) << std::endl;
  std::cout << "_blockAckparms:\t" << std::hex << int(_blockAckparms) << std::endl;
  std::cout << "_blockAckTimeout:\t" << int(_blockAckTimeout) << std::dec << std::endl;

}

}
}
}
