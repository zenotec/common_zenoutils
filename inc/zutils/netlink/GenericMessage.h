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


namespace netlink
{

//*****************************************************************************
// Class: GenericMessage
//*****************************************************************************

class GenericMessage : public Message
{
public:

  GenericMessage(const int family_, const int flags_, const uint8_t id_);

  GenericMessage(struct nl_msg* msg_);

  virtual
  ~GenericMessage();

  virtual bool
  Parse();

  void
  DisplayAttributes() const;

protected:

private:

};

}

#endif /* __NETLINK_GENERICMESSAGE_H__ */
