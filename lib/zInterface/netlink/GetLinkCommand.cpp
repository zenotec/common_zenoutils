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

// libc++ includes
#include <iostream>
#include <map>

// libzutils includes
#include <zutils/zLog.h>
using namespace zUtils;

// local includes
#include "Attribute.h"
#include "Command.h"

#include "Message.h"
#include "Handler.h"
#include "Socket.h"
#include "RouteSocket.h"
using namespace netlink;

#include "GetLinkCommand.h"


namespace netlink
{

static std::string
__errstr(int code)
{
  return(std::string(nl_geterror(code)));
}

//*****************************************************************************
// Class: GetLinkCommand
//*****************************************************************************

GetLinkCommand::GetLinkCommand(const unsigned int ifindex_) :
    _ifindex(ifindex_)
{
  if (!ifindex_)
  {
    ZLOG_WARN("Interface index is NULL");
  }
}

GetLinkCommand::GetLinkCommand(const std::string& ifname_) :
    _ifindex(0)
{
  if (!ifname_.empty())
  {
    this->_ifindex = if_nametoindex(ifname_.c_str());
    if (!this->_ifindex)
    {
      ZLOG_ERR("Error retrieving interface index for: " + ifname_);
    }
  }
  else
  {
    ZLOG_WARN("Name is empty!");
  }
}

GetLinkCommand::~GetLinkCommand()
{
}

bool
GetLinkCommand::Exec()
{

  bool status = false;
  int ret = 0;
  struct rtnl_link* link = NULL;

  if (!this->_ifindex)
  {
    ZLOG_ERR("Error executing GetLinkCommand: " + this->Link.IfName());
    ZLOG_ERR("Valid interface index must be specified");
    return(false);
  }

  if (!this->_sock.Connect())
  {
    ZLOG_ERR("Error executing GetLinkCommand: " + this->Link.IfName());
    ZLOG_ERR("Error connecting Netlink socket");
    return(false);
  }

  ret = rtnl_link_get_kernel(this->_sock(), this->_ifindex, NULL, &link);
  if (ret < 0)
  {
    ZLOG_ERR("Error executing GetLinkCommand: " + this->Link.IfName());
    ZLOG_ERR("Error: (" + ZLOG_INT(ret) + ") " + __errstr(ret));
  }
  else
  {
    this->Link(link); // Transfers ownership of link pointer
    status = true;
  }

  this->_sock.Disconnect();

  return(status);
}

void
GetLinkCommand::Display() const
{
  this->Link.Display();
}

}
