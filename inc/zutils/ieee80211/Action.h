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

#ifndef __IEEE80211_ACTION_H__
#define __IEEE80211_ACTION_H__

// libc includes

// libc++ includes

// libzutils includes
#include <zutils/ieee80211/Frame.h>
#include <zutils/ieee80211/ManagementFrame.h>

// local includes

namespace zUtils
{
namespace zWireless
{
namespace ieee80211
{

//*****************************************************************************
// Class: ActionRequest
//*****************************************************************************

class ActionRequest : public ManagementFrame
{

public:

  ActionRequest();

  virtual
  ~ActionRequest();

  virtual uint8_t*
  Assemble(uint8_t* p_, size_t& len_, bool fcs_ = false);

  virtual uint8_t*
  Disassemble(uint8_t* p_, size_t& len_, bool fcs_ = false);

  virtual void
  Display() const;

protected:

public:
  uint8_t _categoryCode;
  uint8_t _actionCode;
  uint8_t _dialogToken;
  uint16_t _blockAckparms;
  uint16_t _blockAckTimeout;
  uint16_t _blockStartingSequence;

};

//*****************************************************************************
// Class: ActionResponse
//*****************************************************************************

class ActionResponse : public ManagementFrame
{

public:

  ActionResponse();

  virtual
  ~ActionResponse();

  virtual uint8_t*
  Assemble(uint8_t* p_, size_t& len_, bool fcs_ = false);

  virtual uint8_t*
  Disassemble(uint8_t* p_, size_t& len_, bool fcs_ = false);

  virtual void
  Display() const;

protected:

public:
  uint8_t _categoryCode;
  uint8_t _actionCode;
  uint8_t _dialogToken;
  uint16_t _status;
  uint16_t _blockAckparms;
  uint16_t _blockAckTimeout;

};

}
}
}

#endif /* __IEEE80211_ACTION_H__ */
