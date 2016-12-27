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
// libc++ includes
// libzutils includes
#include <zutils/zCompatibility.h>
#include <zutils/zInterface.h>
#include <zutils/zWirelessInterface.h>

namespace zUtils
{
namespace zInterface
{

// ****************************************************************************
// Class: InterfaceFactory
// ****************************************************************************

SHARED_PTR(Interface)
InterfaceFactory::Create(const InterfaceConfigData& config_)
{
  SHARED_PTR(Interface) iface;
  if (config_.GetType() == InterfaceConfigData::ConfigTypeWireless)
  {
    iface = SHARED_PTR(Interface)(new WirelessInterface(config_));
  }
  else
  {
    iface = SHARED_PTR(Interface)(new Interface(config_));
  }
  return (iface);
}

}
}
