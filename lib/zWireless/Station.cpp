/*
 * Copyright (c) 2016 Cable Television Laboratories, Inc. ("CableLabs")
 *                    and others.  All rights reserved.
 *
 * Copyright (c) 2014-2016 ZenoTec LLC (http://www.zenotec.net)
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
#include <vector>

// libzutils includes
#include <zutils/zLog.h>
#include <zutils/zStation.h>

ZLOG_MODULE_INIT(zUtils::zLog::Log::MODULE_WIRELESS);

namespace zUtils
{
namespace zWireless
{

// ****************************************************************************
// Class: Station
// ****************************************************************************

Station::Station(const std::string& addr_) :
	_macAddress(addr_), _associationId(0), _flags(0),  _capabilities(0),
    _listenInterval(0)
{
	_supportedRates = vector<uint8_t>(0);
}

Station::~Station()
{
}

uint16_t
Station::AssociationId() const
{
  return _associationId;
}

bool
Station::AssociationId(const uint16_t id_)
{
  bool status = true;
  _associationId = id_;
  return status;
}

uint8_t
Station::Flags() const
{
  return _flags;
}

bool
Station::Flags(const uint8_t flags_)
{
  bool status = true;
  _flags = flags_;
  return status;
}


bool
Station::MacAddress(const std::string& macAddress_)
{
  bool status = true;
  _macAddress = macAddress_;
  return status;
}

std::string
Station::MacAddress() const
{
  return _macAddress;
}

uint16_t
Station::Capabilities() const
{
  return _capabilities;
}

bool
Station::Capabilities(const uint16_t caps_)
{
  bool status = true;
  _capabilities = caps_;
  return status;
}

uint16_t
Station::ListenInterval() const
{
  return _listenInterval;
}

bool
Station::ListenInterval(const uint16_t interval_)
{
  bool status = true;
  _listenInterval = interval_;
  return status;
}

vector<uint8_t>
Station::SupportedRates() const
{
  return _supportedRates;
}

bool
Station::SupportedRates(const vector<uint8_t> rates_)
{
  bool status = true;
  _supportedRates = rates_;
  return status;
}


}
}
