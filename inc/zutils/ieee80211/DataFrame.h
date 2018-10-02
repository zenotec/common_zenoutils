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

#ifndef __IEEE80211_DATAFRAME_H__
#define __IEEE80211_DATAFRAME_H__

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

struct data_llc
{
  uint8_t dsap;
  uint8_t ssap;
  uint8_t cntl;
  uint8_t oui[3];
  uint16_t proto;
} __attribute__ ((packed));

//*****************************************************************************
// Class: DataFrame
//*****************************************************************************

class DataFrame : public Frame
{

public:

  DataFrame(const Frame::SUBTYPE subtype_ = Frame::SUBTYPE_NONE);

  virtual
  ~DataFrame();

  virtual bool
  Assemble(zSocket::Buffer& sb_, bool fcs_);

  virtual bool
  Disassemble(zSocket::Buffer& sb_, bool fcs_);

  std::string
  ReceiverAddress() const;

  bool
  ReceiverAddress(const std::string& address_);

  std::string
  TransmitterAddress() const;

  bool
  TransmitterAddress(const std::string& address_);

  std::string
  DestinationAddress() const;

  bool
  DestinationAddress(const std::string& address_);

  std::string
  SourceAddress() const;

  bool
  SourceAddress(const std::string& address_);

  std::string
  Bssid() const;

  bool
  Bssid(const std::string& address_);

  const data_llc&
  Llc() const;

  bool
  Llc(const data_llc& llc_);

  virtual void
  Display() const;

protected:

private:

  data_llc _llc;

};

}
}
}

#endif /* __IEEE80211_DATAFRAME_H__ */
