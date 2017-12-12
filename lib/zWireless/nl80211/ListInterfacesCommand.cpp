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
#include <net/if.h>
#include <linux/nl80211.h>
#include <netlink/netlink.h>
#include <netlink/msg.h>
#include <netlink/attr.h>
#include <netlink/genl/genl.h>
#include <netlink/genl/ctrl.h>

// libc++ includes
#include <iostream>
#include <map>

// libzutils includes
#include <zutils/zLog.h>
using namespace zUtils;

// local includes
#include "Command.h"
#include "Attribute.h"

#include "Message.h"
#include "Handler.h"
#include "Socket.h"
#include "GenericMessage.h"
#include "GenericSocket.h"
using namespace netlink;

#include "PhyIndexAttribute.h"
#include "IfIndexAttribute.h"
#include "IfNameAttribute.h"
#include "IfTypeAttribute.h"
#include "ListInterfacesCommand.h"

namespace nl80211
{

static std::string
__errstr(int code)
{
  return(std::string(nl_geterror(code)));
}

//*****************************************************************************
// Class: ListInterfacesCommand
//*****************************************************************************

ListInterfacesCommand::ListInterfacesCommand() :
    Command(0)
{
}

ListInterfacesCommand::~ListInterfacesCommand()
{
}

std::map<uint32_t, std::string>
ListInterfacesCommand::operator() ()
{
  return(this->_ifs);
}

void
ListInterfacesCommand::Display() const
{
  FOREACH(auto& iface, this->_ifs)
  {
    std::cout << "Interface:" << std::endl;
    std::cout << "\tIndex: \t" << iface.first << std::endl;
    std::cout << "\tName:  \t" << iface.second << std::endl;
  }
}

bool
ListInterfacesCommand::Exec()
{
  this->_ifs.clear();
  if (!this->_sock.Connect())
  {
    ZLOG_ERR("Error connecting NL80211 socket");
    return(false);
  }

  if (!this->_sock.SetHandler(this))
  {
    ZLOG_ERR("Error setting up message handlers");
    return(false);
  }

  GenericMessage cmdmsg(this->_sock.Family(), NLM_F_DUMP, NL80211_CMD_GET_INTERFACE);
  this->_sock.SendMsg(cmdmsg);
  this->_sock.RecvMsg();
  this->_sock.Disconnect();

  return(true);

}

int
ListInterfacesCommand::valid_cb(struct nl_msg* msg_, void* arg_)
{

  uint32_t index = 0;
  std::string name;

  GenericMessage msg(msg_);
  if (!msg.Parse())
  {
    ZLOG_ERR("Error parsing generic message");
    return(NL_SKIP);
  }

  if (msg.GetAttribute(NL80211_ATTR_IFINDEX, index) &&
      msg.GetAttribute(NL80211_ATTR_IFNAME, name))
  {
    this->_ifs[index] = name;
  }

  return (NL_OK);

}

int
ListInterfacesCommand::err_cb(struct sockaddr_nl* nla, struct nlmsgerr* nlerr, void* arg)
{
  ZLOG_ERR("Error executing ListInterfaceCommand");
  ZLOG_ERR("Error: (" + ZLOG_INT(nlerr->error) + ") " + __errstr(nlerr->error));
  return(NL_SKIP);
}

}
