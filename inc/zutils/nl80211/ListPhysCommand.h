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

#ifndef __NL80211_LISTPHYSCOMMAND_H__
#define __NL80211_LISTPHYSCOMMAND_H__

// libc includes

// libc++ includes
#include <map>
#include <string>

// libzutils includes
#include <zutils/netlink/Attribute.h>
#include <zutils/netlink/Command.h>
#include <zutils/netlink/Message.h>
#include <zutils/netlink/Handler.h>
#include <zutils/netlink/Socket.h>
#include <zutils/netlink/GenericMessage.h>
#include <zutils/netlink/GenericSocket.h>
using namespace netlink;
#include <zutils/nl80211/PhyIndexAttribute.h>
#include <zutils/nl80211/PhyNameAttribute.h>
#include <zutils/nl80211/IfIndexAttribute.h>
#include <zutils/nl80211/IfNameAttribute.h>
#include <zutils/nl80211/MacAttribute.h>
#include <zutils/nl80211/SsidAttribute.h>
#include <zutils/nl80211/FrequencyAttribute.h>
#include <zutils/nl80211/ChannelTypeAttribute.h>
#include <zutils/nl80211/ChannelWidthAttribute.h>
#include <zutils/nl80211/TxPowerModeAttribute.h>
#include <zutils/nl80211/TxPowerLevelAttribute.h>

// local includes

namespace nl80211
{

//*****************************************************************************
// Class: ListPhysCommand
//*****************************************************************************

class ListPhysCommand : public netlink::Command, public netlink::Handler
{

public:

  ListPhysCommand();

  virtual
  ~ListPhysCommand();

  virtual bool
  Exec();

  virtual std::map<int, std::string>
  operator() ();

  void
  Display() const;

protected:

  virtual int
  valid_cb(struct nl_msg* msg, void* arg);

  virtual int
  err_cb(struct sockaddr_nl* nla, struct nlmsgerr* nlerr, void* arg);

private:

  netlink::GenericSocket _sock;
  std::map<int, std::string> _phys;

};

}

#endif /* __NL80211_LISTPHYSCOMMAND_H__ */
