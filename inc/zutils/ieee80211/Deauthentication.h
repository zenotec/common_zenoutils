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

#ifndef __IEEE80211_DEAUTHENTICATION_H__
#define __IEEE80211_DEAUTHENTICATION_H__

#include <string>
using namespace std;

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
// Class: Deauthentication
//*****************************************************************************

class Deauthentication : public ManagementFrame
{

public:

  enum REASON
  {
    // see 802.11 spec, section 9.4.1.7 for more codes
    REASON_ERR = 0,
    REASON_UNSPECIFIED = 1,
    REASON_INVALID_AUTH = 2,
    REASON_LEAVING_NETWORK_DEAUTH = 3,
    REASON_INACTIVITY = 4,
    REASON_NO_MORE_STAS = 5,
    REASON_LEAVING_NETWORK_DISASSOC = 8,
    REASON_BSS_TRANSITION_DISASSOC = 12,
    REASON_SSP_REQUESTED_DISASSOC = 27,
    REASON_STA_LEAVING = 36,
    REASON_AP_INITIATED = 47
  };

  Deauthentication();

  virtual
  ~Deauthentication();

  virtual bool
  Assemble(zSocket::Buffer& sb_);

  virtual bool
  Disassemble(zSocket::Buffer& sb_);

  virtual uint8_t*
  Assemble(uint8_t* p_, size_t& len_, bool fcs_ = false);

  virtual uint8_t*
  Disassemble(uint8_t* p_, size_t& len_, bool fcs_ = false);

  uint16_t
  ReasonCode() const;

  bool
  ReasonCode(const uint16_t reason_);

  virtual void
  Display() const;

protected:

private:
  uint16_t _reasonCode;

};

}
}
}

#endif /* __IEEE80211_DEAUTHENTICATION_H__ */
