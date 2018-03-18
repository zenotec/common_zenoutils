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

#ifndef __NL80211_LISTINTERFACESCOMMAND_H__
#define __NL80211_LISTINTERFACESCOMMAND_H__

#include <zutils/netlink/Attribute.h>
#include <zutils/netlink/Command.h>
#include <zutils/netlink/Message.h>
#include <zutils/netlink/Handler.h>
#include <zutils/netlink/Socket.h>
#include <zutils/netlink/GenericMessage.h>
#include <zutils/netlink/GenericSocket.h>
using namespace netlink;
#include <zutils/nl80211/Socket.h>

// local includes

namespace nl80211
{

//*****************************************************************************
// Class: ListInterfacesCommand
//*****************************************************************************

class ListInterfacesCommand :
    public netlink::Command,
    public netlink::Handler
{

public:

  ListInterfacesCommand();

  virtual
  ~ListInterfacesCommand();

  virtual bool
  Exec();

  virtual std::map<uint32_t, std::string>
  operator() ();

  void
  Display() const;

protected:

  virtual int
  valid_cb(struct nl_msg* msg, void* arg);

  virtual int
  finish_cb(struct nl_msg* msg_, void* arg_);

  virtual int
  err_cb(struct sockaddr_nl* nla, struct nlmsgerr* nlerr, void* arg);

private:

  nl80211::Socket _sock;

  std::map<uint32_t, std::string> _ifs;

};

}

#endif /* __NL80211_LISTINTERFACESCOMMAND_H__ */
