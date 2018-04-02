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
#include <string.h>
#include <stdint.h>
#include <netlink/genl/genl.h>
#include <netlink/genl/ctrl.h>

// libc++ includes

#include <iostream>
#include <string>

// libzutils includes

#include <zutils/zUtils.h>
#include <zutils/zLog.h>
#include <zutils/netlink/Attribute.h>
#include <zutils/netlink/Message.h>
#include <zutils/netlink/GenericMessage.h>
using namespace zUtils;

// local includes

ZLOG_MODULE_INIT(zUtils::zLog::Log::MODULE_INTERFACE);

namespace netlink
{

//*****************************************************************************
// Class: GenericMessage
//*****************************************************************************

GenericMessage::GenericMessage(const int family_) :
    _family(family_), _flags(0), _command(0)
{
}

GenericMessage::~GenericMessage()
{
}

bool
GenericMessage::Assemble(struct nl_msg* msg_)
{
  bool status = false;
  if (!genlmsg_put(msg_, NL_AUTO_PORT, NL_AUTO_SEQ, this->_family, 0, this->_flags,
      this->_command, 0))
  {
    ZLOG_ERR("Error initializing netlink generic message header");
  }
  else
  {
    status = this->_attrs.Assemble(msg_);
  }
  return (status);
}

bool
GenericMessage::Disassemble(struct nl_msg* msg_)
{
  bool status = true;
  struct genlmsghdr *gnlhdr = (struct genlmsghdr *) nlmsg_data(nlmsg_hdr(msg_));
  struct nlattr* attr = genlmsg_attrdata(gnlhdr, 0);
  int len = genlmsg_attrlen(gnlhdr, 0);

  status = this->_attrs.Disassemble(attr, len);

  return(status);
}

int
GenericMessage::GetFamily() const
{
  return (this->_family);
}

int
GenericMessage::GetFlags() const
{
  return (this->_flags);
}

bool
GenericMessage::SetFlags(const int flags_)
{
  this->_flags = flags_;
  return (true);
}

int
GenericMessage::GetCommand() const
{
  return (this->_command);
}

bool
GenericMessage::SetCommand(const int command_)
{
  this->_command = command_;
  return (true);
}

void
GenericMessage::Display(const std::string& prefix_) const
{
  printf("%sGeneric Message[%d]: %d (0x%x)\n",
      prefix_.c_str(), this->_family, this->_command, this->_flags);
  Message::Display();
}

}

