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
#include <string.h>
#include <stdio.h>
#include <unistd.h>
#include <math.h>
#include <sys/types.h>
#include <sys/ioctl.h>
#include <sys/socket.h>
#include <linux/wireless.h>

#include <arpa/inet.h>
#include <netinet/in.h>
#include <ifaddrs.h>

// libc++ includes

#include <string>
#include <iostream>
#include <fstream>
#include <list>
#include <map>

// libzutils includes

#include <zutils/zMonitorInterface.h>

// local includes

namespace zUtils
{
namespace zInterface
{

// ****************************************************************************
// Class: MonitorInterface
// ****************************************************************************

MonitorInterface::MonitorInterface(const std::string& name_) :
    WirelessInterface(name_)
{
  this->WiConfig.SetOpMode(WirelessInterfaceConfigData::OPMODE_MONITOR);
}

MonitorInterface::MonitorInterface(const zInterface::ConfigData& config_) :
    WirelessInterface(config_)
{
  this->WiConfig.SetOpMode(WirelessInterfaceConfigData::OPMODE_MONITOR);
}

MonitorInterface::~MonitorInterface()
{
}

}
}
