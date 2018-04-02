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
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netlink/netlink.h>

// libc++ includes
#include <iostream>

// libzutils includes
#include <zutils/zLog.h>
#include <zutils/netlink/Attribute.h>
#include <zutils/netlink/Message.h>
using namespace zUtils;

// local includes

ZLOG_MODULE_INIT(zUtils::zLog::Log::MODULE_INTERFACE);

namespace netlink
{

//*****************************************************************************
// Class: Message
//*****************************************************************************

Message::Message() :
    _type(0)
{
}

Message::~Message()
{
}

uint16_t
Message::GetType() const
{
  return(this->_type);
}

bool
Message::SetType(const uint16_t type_)
{
  this->_type = type_;
  return(true);
}

bool
Message::GetAttribute(Attribute* attr_)
{
  return (this->_attrs.Get(attr_));
}

bool
Message::PutAttribute(Attribute* attr_)
{
  return (this->_attrs.Put(attr_));
}

void
Message::Display(const std::string& prefix_) const
{
  this->_attrs.Display(std::string(prefix_ + "\t"));
}

}

