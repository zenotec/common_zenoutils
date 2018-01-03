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

#include "IfIndexAttribute.h"
#include "ListStationsCommand.h"

namespace nl80211
{

static std::string
__errstr(int code)
{
  return(std::string(nl_geterror(code)));
}

//*****************************************************************************
// Class: ListStationsCommand
//*****************************************************************************

ListStationsCommand::ListStationsCommand(const unsigned int ifindex_) :
    Command(ifindex_)
{
  this->IfIndex(ifindex_);
}

ListStationsCommand::~ListStationsCommand()
{
}

std::map<int, std::string>
ListStationsCommand::operator() ()
{
  return(this->_stas);
}

void
ListStationsCommand::Display() const
{
  FOREACH(auto& sta, this->_stas)
  {
    std::cout << "Station:" << std::endl;
    std::cout << "\tIndex: \t" << sta.first << std::endl;
    std::cout << "\tName:  \t" << sta.second << std::endl;
  }
}

bool
ListStationsCommand::Exec()
{
  this->_stas.clear();
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

  GenericMessage cmdmsg(this->_sock.Family(), NLM_F_DUMP, NL80211_CMD_GET_STATION);
  this->_sock.SendMsg(cmdmsg);
  this->_sock.RecvMsg();
  this->_sock.Disconnect();

  return(true);

}

int
ListStationsCommand::valid_cb(struct nl_msg* msg_, void* arg_)
{

  uint32_t index = 0;
  std::string name;

  GenericMessage msg(msg_);
  if (!msg.Parse())
  {
    ZLOG_ERR("Error parsing generic message");
    return(NL_SKIP);
  }

  if (msg.GetAttribute(NL80211_ATTR_WIPHY, index) &&
      msg.GetAttribute(NL80211_ATTR_WIPHY_NAME, name))
  {
    this->_stas[index] = name;
  }

  return (NL_OK);
}

int
ListStationsCommand::err_cb(struct sockaddr_nl* nla, struct nlmsgerr* nlerr, void* arg)
{
  ZLOG_ERR("Error executing ListPhyCommand");
  ZLOG_ERR("Error: (" + ZLOG_INT(nlerr->error) + ") " + __errstr(nlerr->error));
  return(NL_SKIP);
}

}
