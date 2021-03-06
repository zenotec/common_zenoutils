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

#ifndef __NETLINK_GENERICMESSAGE_H__
#define __NETLINK_GENERICMESSAGE_H__

// libc includes
#include <stdint.h>

#include <netlink/msg.h>
#include <netlink/genl/genl.h>
#include <netlink/genl/ctrl.h>

#include <zutils/netlink/Message.h>

namespace netlink
{

//*****************************************************************************
// Class: GenericMessage
//*****************************************************************************

class GenericMessage : public Message
{
public:

  GenericMessage(const int family_ = 0);

  virtual
  ~GenericMessage();

  virtual bool
  Assemble(struct nl_msg* msg_);

  virtual bool
  Disassemble(struct nl_msg* msg_);

  int
  GetFamily() const;

  int
  GetFlags() const;

  bool
  SetFlags(const int flags_);

  int
  GetCommand() const;

  bool
  SetCommand(const int command_);

  virtual void
  Display(const std::string& prefix_ = "") const;

protected:

private:

  int _family;
  int _flags;
  int _command;

};

}

#endif /* __NETLINK_GENERICMESSAGE_H__ */
