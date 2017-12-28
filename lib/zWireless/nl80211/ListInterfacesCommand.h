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

// local includes
#include "Command.h"
#include "Attribute.h"

#include "Message.h"
#include "Handler.h"
#include "Socket.h"
#include "GenericMessage.h"
#include "GenericSocket.h"
using namespace netlink;

namespace nl80211
{

//*****************************************************************************
// Class: ListInterfacesCommand
//*****************************************************************************

class ListInterfacesCommand : public netlink::Command, public netlink::Handler
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
  err_cb(struct sockaddr_nl* nla, struct nlmsgerr* nlerr, void* arg);

private:

  netlink::GenericSocket _sock;

  std::map<uint32_t, std::string> _ifs;

};

}

#endif /* __NL80211_LISTINTERFACESCOMMAND_H__ */
