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

#ifndef __IEEE80211_PROBE_H__
#define __IEEE80211_PROBE_H__

// libc includes

// libc++ includes

// libzutils includes

// local includes
#include "Frame.h"
#include "ManagementFrame.h"
#include "SsidTag.h"
#include "RatesTag.h"

namespace ieee80211
{

//*****************************************************************************
// Class: ProbeRequest
//*****************************************************************************

class ProbeRequest : public ManagementFrame
{

public:

  SsidTag Ssid;
  RatesTag Rates;

  ProbeRequest();

  virtual
  ~ProbeRequest();

  virtual uint8_t*
  Assemble(uint8_t* p_, size_t& len_, bool fcs_ = false);

  virtual uint8_t*
  Disassemble(uint8_t* p_, size_t& len_, bool fcs_ = false);

  virtual void
  Display() const;

protected:

private:

};

//*****************************************************************************
// Class: ProbeResponse
//*****************************************************************************

class ProbeResponse : public ManagementFrame
{

public:

  SsidTag Ssid;
  RatesTag Rates;

  ProbeResponse();

  virtual
  ~ProbeResponse();

  virtual uint8_t*
  Assemble(uint8_t* p_, size_t& len_, bool fcs_ = false);

  virtual uint8_t*
  Disassemble(uint8_t* p_, size_t& len_, bool fcs_ = false);

  uint64_t
  Timestamp() const;

  bool
  Timestamp(const uint64_t ts_);

  uint16_t
  Interval() const;

  bool
  Interval(const uint16_t int_);

  uint16_t
  Capabilities() const;

  bool
  Capabilities(const uint16_t int_);

  virtual void
  Display() const;

protected:

private:

  uint64_t _ts;
  uint16_t _interval;
  uint16_t _capabilities;

};

}

#endif /* __IEEE80211_PROBE_H__ */
