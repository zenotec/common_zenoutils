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
	_macAddress(addr_),
	_associationId(0),
	_flags(0),
	_capabilities(0),
    _listenInterval(0),
    _htCapable(false),
    _htCapabilities{0},
    _vhtCapable(false),
    _vhtCapabilities{0}
{
}

Station::~Station()
{
}

uint16_t
Station::GetAssociationId() const
{
  return (this->_associationId);
}

bool
Station::SetAssociationId(const uint16_t id_)
{
  bool status = true;
  this->_associationId = id_;
  return (status);
}

uint8_t
Station::GetFlags() const
{
  return (this->_flags);
}

bool
Station::SetFlags(const uint8_t flags_)
{
  bool status = true;
  this->_flags = flags_;
  return (status);
}

std::string
Station::GetAddress() const
{
  return (this->_macAddress);
}

bool
Station::SetAddress(const std::string& macAddress_)
{
  bool status = true;
  this->_macAddress = macAddress_;
  return (status);
}

uint16_t
Station::GetCapabilities() const
{
  return (this->_capabilities);
}

bool
Station::SetCapabilities(const uint16_t caps_)
{
  bool status = true;
  this->_capabilities = caps_;
  return (status);
}

bool
Station::IsHtCapable() const
{
  return (this->_htCapable);
}

bool
Station::SetHtCapable()
{
  this->_htCapable = true;
  return (this->_htCapable);
}

bool
Station::ClrHtCapable()
{
  this->_htCapable = false;
  return (!this->_htCapable);
}

struct ht_caps
Station::GetHtCapabilities() const
{
  return (this->_htCapabilities);
}

bool
Station::SetHtCapabilities(const struct ht_caps& caps_)
{
  bool status = true;
  this->_htCapabilities = caps_;
  return (status);
}

bool
Station::IsVhtCapable() const
{
  return (this->_vhtCapable);
}

bool
Station::SetVhtCapable()
{
  this->_vhtCapable = true;
  return (this->_vhtCapable);
}

bool
Station::ClrVhtCapable()
{
  this->_vhtCapable = false;
  return (!this->_vhtCapable);
}

struct vht_caps
Station::GetVhtCapabilities() const
{
  return (this->_vhtCapabilities);
}

bool
Station::SetVhtCapabilities(const struct vht_caps& caps_)
{
  bool status = true;
  this->_vhtCapabilities = caps_;
  return (status);
}

uint16_t
Station::GetListenInterval() const
{
  return (this->_listenInterval);
}

bool
Station::SetListenInterval(const uint16_t interval_)
{
  bool status = true;
  this->_listenInterval = interval_;
  return (status);
}

vector<uint8_t>
Station::GetSupportedRates() const
{
  return (this->_supportedRates);
}

bool
Station::SetSupportedRates(const vector<uint8_t>& rates_)
{
  bool status = true;
  this->_supportedRates = rates_;
  return (status);
}

void
Station::Display(const std::string &prefix_) const
{
  std::cout << prefix_ << "" << std::endl;
  std::cout << prefix_ << "Station: " << this->_macAddress << std::endl;
  std::cout << prefix_ << "Flags:           0x" << std::hex << int(this->_flags) << std::dec << std::endl;
  std::cout << prefix_ << "AID:             " << int(this->_associationId) << std::endl;
  std::cout << prefix_ << "Capabilities:    0x" << std::hex << int(this->_capabilities) << std::dec << std::endl;
  std::cout << prefix_ << "Listen Interval: " << int(this->_listenInterval) << std::endl;
  fprintf(stderr, "%sRates:           %zd\n", prefix_.c_str(), this->_supportedRates.size());
  for (int i = 0; i < this->_supportedRates.size(); i++)
  {
    fprintf(stderr, "%sRate[% 2d]:      0x%02x\n", prefix_.c_str(), i, this->_supportedRates[i]);
  }
  std::cout << prefix_ << "HT Capable:      " << (this->IsHtCapable() ? "true" : "false") << std::endl;
  std::cout << prefix_ << "VHT Capable:     " << (this->IsVhtCapable() ? "true" : "false") << std::endl;
  std::cout << prefix_ << "" << std::endl;
}


}
}
