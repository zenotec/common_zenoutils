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

#ifndef __IEEE80211_CONTROLFRAME_H__
#define __IEEE80211_CONTROLFRAME_H__

// libc includes

// libc++ includes

// libzutils includes
#include <zutils/ieee80211/Frame.h>

// local includes

namespace zUtils
{
namespace zWireless
{
namespace ieee80211
{

//*****************************************************************************
// Class: ControlFrame
//*****************************************************************************

class ControlFrame : public Frame
{

public:

  ControlFrame(const Frame::SUBTYPE subtype_ = Frame::SUBTYPE_ERR);

  virtual
  ~ControlFrame();

  virtual uint8_t*
  Assemble(uint8_t* p_, size_t& len_, bool fcs_ = false);

  virtual uint8_t*
  Disassemble(uint8_t* p_, size_t& len_, bool fcs_ = false);

  std::string
  ReceiverAddress() const;

  bool
  ReceiverAddress(const std::string& address_);

  std::string
  TransmitterAddress() const;

  bool
  TransmitterAddress(const std::string& address_);

  virtual void
  Display() const;

protected:

private:

};

}
}
}

#endif /* __IEEE80211_CONTROLFRAME_H__ */
