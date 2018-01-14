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

#ifndef __IEEE80211_AUTHENTICATION_H__
#define __IEEE80211_AUTHENTICATION_H__

#include <string>
using namespace std;

// libc includes

// libc++ includes

// libzutils includes

// local includes
#include "Frame.h"
#include "ManagementFrame.h"

namespace zUtils
{
namespace zWireless
{
namespace ieee80211
{

//*****************************************************************************
// Class: Authentication
//*****************************************************************************

class Authentication : public ManagementFrame
{

public:

  enum ALGORITHM_TYPE
  {
    TYPE_ERR = -1,
    TYPE_OPEN_SYSTEM = 0,
    TYPE_SHARED_KEY = 1,
    TYPE_FAST_BSS_TRANSITION = 2,
    TYPE_SAE = 3,
    TYPE_VENDOR = 65535
  };

  static string
  ToString(const Authentication::ALGORITHM_TYPE type_)
  {
    switch (type_)
    {
      case TYPE_ERR: { return "TYPE_ERR"; break; }
      case TYPE_OPEN_SYSTEM: { return "TYPE_OPEN_SYSTEM"; break; }
      case TYPE_SHARED_KEY: { return "TYPE_SHARED_KEY"; break; }
      case TYPE_FAST_BSS_TRANSITION: { return "TYPE_FAST_BSS_TRANSITION"; break; }
      case TYPE_SAE: { return "TYPE_SAE"; break; }
      case TYPE_VENDOR: { return "TYPE_VENDOR"; break; }
      default: { return "TYPE_ERR"; break; }
    }

  }

  Authentication();

  virtual
  ~Authentication();

  virtual uint8_t*
  Assemble(uint8_t* p_, size_t& len_, bool fcs_ = false);

  virtual uint8_t*
  Disassemble(uint8_t* p_, size_t& len_, bool fcs_ = false);

  uint16_t
  Algorithm() const;

  bool
  Algorithm(const uint16_t authAlgorithm_);

  uint16_t
  AuthSequenceNumber() const;

  bool
  SequenceNumber(const uint16_t authSeq_);

  uint16_t
  StatusCode() const;

  bool
  StatusCode(const uint16_t statusCode_);

  virtual void
  Display() const;

protected:

private:
  uint16_t _algorithm;
  uint16_t _sequenceNumber;
  uint16_t _statusCode;

};

}
}
}

#endif /* __IEEE80211_AUTHENTICATION_H__ */
