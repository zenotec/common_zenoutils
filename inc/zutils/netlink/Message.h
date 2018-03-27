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

#ifndef __NETLINK_MESSAGE_H__
#define __NETLINK_MESSAGE_H__

#include <stdint.h>

#include <netlink/netlink.h>
#include <netlink/msg.h>
#include <netlink/attr.h>
#include <linux/nl80211.h>

#include <string>

#include <zutils/netlink/Attribute.h>

namespace netlink
{

//*****************************************************************************
// Class: Message
//*****************************************************************************

class Message
{

public:

  Message();

  virtual
  ~Message();

  virtual bool
  Assemble(struct nl_msg* msg_) = 0;

  virtual bool
  Disassemble(struct nl_msg* msg_) = 0;

  uint16_t
  GetType() const;

  bool
  SetType(const uint16_t type_);

  bool
  GetAttribute(Attribute* attr_);

  bool
  PutAttribute(Attribute* attr_);

  void
  Display() const;

protected:

  AttributeTable _attrs;

private:

  uint16_t _type;

};

}

#endif /* __NETLINK_MESSAGE_H__ */
