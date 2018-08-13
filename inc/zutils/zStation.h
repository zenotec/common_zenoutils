/*
 * Copyright (c) 2017 Cable Television Laboratories, Inc. ("CableLabs")
 *                    and others.  All rights reserved.
 *
 * Copyright (c) 2014-2017 ZenoTec LLC (http://www.zenotec.net)
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

#ifndef __ZSTATION_H__
#define __ZSTATION_H__

#include <string>
#include <vector>
using namespace std;

namespace zUtils
{

namespace zWireless
{


// ****************************************************************************
// Class: Station
// ****************************************************************************

class Station
{

public:

  Station(const std::string& addr_);

  virtual
  ~Station();

  uint16_t
  AssociationId() const;

  bool
  AssociationId(const uint16_t id_);

  uint8_t
  Flags() const;

  bool
  Flags(const uint8_t flags_);

  bool
  MacAddress(const std::string& macAddress_);

  std::string
  MacAddress() const;

  uint16_t
  Capabilities() const;

  bool
  Capabilities(const uint16_t caps_); // as defined in 802.11 9.4.1.4

  uint16_t
  ListenInterval() const;

  bool
  ListenInterval(const uint16_t interval_);

  vector<uint8_t>
  SupportedRates() const;

  bool
  SupportedRates(const vector<uint8_t> rates_);

//  virtual void
//  Display(const std::string &prefix_ = std::string(""));

protected:

private:

  uint16_t _associationId;
  uint8_t _flags;
  std::string _macAddress;
  uint16_t _capabilities;
  uint16_t _listenInterval;
  vector<uint8_t> _supportedRates;

};

}
}
#endif /* __ZSTATION_H__ */
