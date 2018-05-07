/*
 * Copyright (c) 2018 Cable Television Laboratories, Inc. ("CableLabs")
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

// libc++ includes

// libzutils includes

#include <zutils/zCompatibility.h>
#include <zutils/zLog.h>
#include <zutils/zInterface.h>
#include <zutils/zWireless.h>
#include <zutils/nl80211/GetPhyCommand.h>

using namespace nl80211;

ZLOG_MODULE_INIT(zUtils::zLog::Log::MODULE_WIRELESS);

namespace zUtils
{
namespace zWireless
{


// ****************************************************************************
// Class: zWireless::Capabilities
// ****************************************************************************

Capabilities::Capabilities() :
    _valid(false), _power_caps { 0 }, _ht_caps { 0 }, _ht_info { 0 }
{
}

Capabilities::~Capabilities()
{
}

bool
Capabilities::IsValid() const
{
  return (this->_valid);
}

bool
Capabilities::SetValid()
{
  this->_valid = true;
  return (this->_valid);
}

bool
Capabilities::ClrValid()
{
  this->_valid = false;
  return (!this->_valid);
}

ieee80211::PowerCapsTag::power_caps_tag
Capabilities::GetPowerCaps() const
{
  return (this->_power_caps);
}

bool
Capabilities::SetPowerCaps(const ieee80211::PowerCapsTag::power_caps_tag& caps_)
{
  this->_power_caps = caps_;
  return (true);
}

ieee80211::HtCapsTag::ht_caps
Capabilities::GetHtCaps() const
{
  return (this->_ht_caps);
}

bool
Capabilities::SetHtCaps(const ieee80211::HtCapsTag::ht_caps& caps_)
{
  this->_ht_caps = caps_;
  return (true);
}

ieee80211::HtInfoTag::ht_info
Capabilities::GetHtInfo() const
{
  return (this->_ht_info);
}

bool
Capabilities::SetHtInfo(const ieee80211::HtInfoTag::ht_info& info_)
{
  this->_ht_info = info_;
  return (true);
}

std::vector<uint8_t>
Capabilities::GetChannels() const
{
  return (this->_channels);
}

bool
Capabilities::SetChannels(const std::vector<uint8_t>& channels_)
{
  this->_channels = channels_;
  return (true);
}

std::vector<uint8_t>
Capabilities::GetBitRates() const
{
  return (this->_rates);
}

bool
Capabilities::SetBitRates(const std::vector<uint8_t>& rates_)
{
  // Copy rates
  this->_rates = rates_;
  // Update basic rates (set MSB)
  for (int i = 0; i < rates_.size(); i++)
  {
    switch (this->_rates[i])
    {
    case 2:
      // no break
    case 4:
      // no break
    case 11:
      // no break
    case 22:
      this->_rates[i] |= 0x80;
      break;
    default:
      break;
    }
  }
  return (true);
}

std::vector<uint8_t>
Capabilities::GetExtBitRates() const
{
  return (this->_ext_rates);
}

bool
Capabilities::SetExtBitRates(const std::vector<uint8_t>& rates_)
{
  this->_ext_rates = rates_;
  return (true);
}

std::vector<uint8_t>
Capabilities::GetMcsRates() const
{
  return (this->_mcs_rates);
}

bool
Capabilities::SetMcsRates(const std::vector<uint8_t>& rates_)
{
  this->_mcs_rates = rates_;
  return (true);
}

void
Capabilities::Display() const
{
  std::cout << "Capabilities: " << std::endl;
  if (this->IsValid())
  {

  }
  else
  {
    std::cout << "\tNot valid" << std::endl;
  }
}

}
}

