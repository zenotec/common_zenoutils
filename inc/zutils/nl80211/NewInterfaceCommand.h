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

#ifndef __NL80211_NEWINTERFACECOMMAND_H__
#define __NL80211_NEWINTERFACECOMMAND_H__

#include <linux/nl80211.h>
#include <netlink/netlink.h>
#include <netlink/msg.h>
#include <netlink/attr.h>

#include <zutils/netlink/Attribute.h>
#include <zutils/netlink/Callback.h>
#include <zutils/netlink/Command.h>
#include <zutils/netlink/Message.h>
#include <zutils/netlink/Socket.h>
#include <zutils/netlink/GenericMessage.h>
#include <zutils/netlink/GenericSocket.h>
using namespace netlink;

#include <zutils/nl80211/Socket.h>
#include <zutils/nl80211/PhyIndexAttribute.h>
#include <zutils/nl80211/IfIndexAttribute.h>
#include <zutils/nl80211/IfNameAttribute.h>
#include <zutils/nl80211/IfTypeAttribute.h>

namespace nl80211
{

//*****************************************************************************
// Class: NewInterfaceCommand
//*****************************************************************************

class NewInterfaceCommand :
    public netlink::Command,
    public netlink::Callback
{

public:

  PhyIndexAttribute PhyIndex;
  IfIndexAttribute IfIndex;
  IfNameAttribute IfName;
  IfTypeAttribute IfType;

  NewInterfaceCommand(const std::string& name_, const int phyindex_ = 0);

  virtual
  ~NewInterfaceCommand();

  virtual bool
  Exec();

  virtual void
  Display(const std::string& prefix_ = "") const;

protected:

  virtual int
  valid_cb(struct nl_msg* msg, void* arg);

  virtual int
  err_cb(struct sockaddr_nl* nla, struct nlmsgerr* nlerr, void* arg);

private:

  nl80211::Socket _sock;

};

}

#endif /* __NL80211_NEWINTERFACECOMMAND_H__ */
