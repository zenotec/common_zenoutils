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
    _valid(false)
{
}

Capabilities::~Capabilities()
{
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
Capabilities::GetRates() const
{
  return (this->_rates);
}

bool
Capabilities::SetRates(const std::vector<uint8_t>& rates_)
{
  this->_rates = rates_;
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

