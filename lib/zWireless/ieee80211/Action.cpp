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

uint8_t*
ActionRequest::Assemble(uint8_t* p_, size_t& rem_, bool fcs_)
{

  p_ = ManagementFrame::Assemble(p_, rem_, fcs_);
  struct ieee80211_actionreq* f = (ieee80211_actionreq*) p_;

  if (f == NULL)
  {
    ZLOG_WARN("Error assembling action request frame: " + ZLOG_INT(rem_));
    return (NULL);
  }

  p_ = this->chklen(p_, sizeof(f->_categoryCode), rem_);
  if (!p_)
  {
    return (NULL);
  }
  f->_categoryCode = this->_categoryCode;

  p_ = this->chklen(p_, sizeof(f->_actionCode), rem_);
  if (!p_)
  {
    return (NULL);
  }
  f->_actionCode = this->_actionCode;

  p_ = this->chklen(p_, sizeof(f->_dialogToken), rem_);
  if (!p_)
  {
    return (NULL);
  }
  f->_dialogToken = this->_dialogToken;

  p_ = this->chklen(p_, sizeof(f->_blockAckparms), rem_);
  if (!p_)
  {
    return (NULL);
  }
  f->_blockAckparms = htole16(this->_blockAckparms);

  p_ = this->chklen(p_, sizeof(f->_blockAckTimeout), rem_);
  if (!p_)
  {
    return (NULL);
  }
  f->_blockAckTimeout = htole16(this->_blockAckTimeout);

  p_ = this->chklen(p_, sizeof(f->_blockStartingSequence), rem_);
  if (!p_)
  {
    return (NULL);
  }
  f->_blockStartingSequence = htole16(this->_blockStartingSequence);

  return (p_);
}

uint8_t*
ActionRequest::Disassemble(uint8_t* p_, size_t& rem_, bool fcs_)
{

  p_ = ManagementFrame::Disassemble(p_, rem_, fcs_);
  struct ieee80211_actionreq* f = (ieee80211_actionreq*) p_;

  if (f == NULL)
  {
    ZLOG_WARN("Error disassembling action request frame: " + ZLOG_INT(rem_));
    return (NULL);
  }

  p_ = this->chklen(p_, sizeof(f->_categoryCode), rem_);
  if (!p_)
  {
    return (NULL);
  }
  _categoryCode = f->_categoryCode;

  p_ = this->chklen(p_, sizeof(f->_actionCode), rem_);
  if (!p_)
  {
    return (NULL);
  }
  _actionCode = f->_actionCode;

  p_ = this->chklen(p_, sizeof(f->_dialogToken), rem_);
  if (!p_)
  {
    return (NULL);
  }
  _dialogToken = f->_dialogToken;

  p_ = this->chklen(p_, sizeof(f->_blockAckparms), rem_);
  if (!p_)
  {
    return (NULL);
  }
  _blockAckparms = le16toh(f->_blockAckparms);

  p_ = this->chklen(p_, sizeof(f->_blockAckTimeout), rem_);
  if (!p_)
  {
    return (NULL);
  }
  _blockAckTimeout = le16toh(f->_blockAckTimeout);

  p_ = this->chklen(p_, sizeof(f->_blockStartingSequence), rem_);
  if (!p_)
  {
    return (NULL);
  }
  _blockStartingSequence = le16toh(f->_blockStartingSequence);
  return (p_);
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

uint8_t*
ActionResponse::Assemble(uint8_t* p_, size_t& rem_, bool fcs_)
{
  p_ = ManagementFrame::Assemble(p_, rem_, fcs_);
  struct ieee80211_actionresp* f = (ieee80211_actionresp*) p_;

  if (f == NULL)
  {
    ZLOG_WARN("Error assembling action response frame: " + ZLOG_INT(rem_));
    return (NULL);
  }

  p_ = this->chklen(p_, sizeof(f->_categoryCode), rem_);
  if (!p_)
  {
    return (NULL);
  }
  f->_categoryCode = this->_categoryCode;

  p_ = this->chklen(p_, sizeof(f->_actionCode), rem_);
  if (!p_)
  {
    return (NULL);
  }
  f->_actionCode = this->_actionCode;

  p_ = this->chklen(p_, sizeof(f->_dialogToken), rem_);
  if (!p_)
  {
    return (NULL);
  }
  f->_dialogToken = this->_dialogToken;

  p_ = this->chklen(p_, sizeof(f->_status), rem_);
  if (!p_)
  {
    return (NULL);
  }
  f->_status = htole16(this->_status);

  p_ = this->chklen(p_, sizeof(f->_blockAckparms), rem_);
  if (!p_)
  {
    return (NULL);
  }
  f->_blockAckparms = htole16(this->_blockAckparms);

  p_ = this->chklen(p_, sizeof(f->_blockAckTimeout), rem_);
  if (!p_)
  {
    return (NULL);
  }
  f->_blockAckTimeout = htole16(this->_blockAckTimeout);

  return (p_);
}

uint8_t*
ActionResponse::Disassemble(uint8_t* p_, size_t& rem_, bool fcs_)
{
  p_ = ManagementFrame::Disassemble(p_, rem_, fcs_);
  struct ieee80211_actionresp* f = (ieee80211_actionresp*) p_;

  if (f == NULL)
  {
    ZLOG_WARN("Error disassembling action response frame: " + ZLOG_INT(rem_));
    return (NULL);
  }

  p_ = this->chklen(p_, sizeof(f->_categoryCode), rem_);
  if (!p_)
  {
    return (NULL);
  }
  _categoryCode = f->_categoryCode;

  p_ = this->chklen(p_, sizeof(f->_actionCode), rem_);
  if (!p_)
  {
    return (NULL);
  }
  _actionCode = f->_actionCode;

  p_ = this->chklen(p_, sizeof(f->_dialogToken), rem_);
  if (!p_)
  {
    return (NULL);
  }
  _dialogToken = f->_dialogToken;

  p_ = this->chklen(p_, sizeof(f->_status), rem_);
  if (!p_)
  {
    return (NULL);
  }
  _status = le16toh(f->_status);

  p_ = this->chklen(p_, sizeof(f->_blockAckparms), rem_);
  if (!p_)
  {
    return (NULL);
  }
  _blockAckparms = le16toh(f->_blockAckparms);

  p_ = this->chklen(p_, sizeof(f->_blockAckTimeout), rem_);
  if (!p_)
  {
    return (NULL);
  }
  _blockAckTimeout = le16toh(f->_blockAckTimeout);

  return (p_);
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
