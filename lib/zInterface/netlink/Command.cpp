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

// libc includes
#include <net/if.h>

// libc++ includes

// libzutils includes
#include <zutils/zLog.h>
#include <zutils/netlink/Command.h>
using namespace zUtils;

// local includes

ZLOG_MODULE_INIT(zUtils::zLog::Log::MODULE_INTERFACE);

namespace netlink
{

//*****************************************************************************
// Class: Command
//*****************************************************************************

Command::Command(const unsigned int ifindex_) :
  _status(false), _ifindex(ifindex_)
{
}


Command::Command(const std::string& ifname_) :
  _status(false), _ifindex(0), _ifname(ifname_)
{
  if (!ifname_.empty())
  {
    this->_ifindex = if_nametoindex(ifname_.c_str());
  }
  else
  {
    ZLOG_WARN("Name is empty!");
  }
}


Command::~Command()
{
}

unsigned int
Command::GetIfIndex()
{
  unsigned int index = this->_ifindex;
  if (!index && !this->_ifname.empty())
  {
    index = this->_ifindex = if_nametoindex(this->_ifname.c_str());
  }
  return (index);
}

bool
Command::SetIfIndex(const unsigned int ifindex_)
{
  bool status = false;
  if (ifindex_)
  {
    this->_ifindex = ifindex_;
    status = true;
  }
  return (status);
}

}
