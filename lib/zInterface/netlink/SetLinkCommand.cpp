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

#include "SetLinkCommand.h"


namespace netlink
{

static std::string
__errstr(int code)
{
  return(std::string(nl_geterror(code)));
}

//*****************************************************************************
// Class: SetLinkCommand
//*****************************************************************************

SetLinkCommand::SetLinkCommand(int index_)
{
  this->_orig.IfIndex(index_);
  this->Link.IfIndex(index_);
}

SetLinkCommand::SetLinkCommand(const std::string& name_)
{
  this->_orig.IfName(name_);
  this->Link.IfName(name_);
}

SetLinkCommand::~SetLinkCommand()
{
}

bool
SetLinkCommand::Exec()
{
  int ret = 0;
  struct rtnl_link* link = NULL;

  if (!this->_sock.Connect())
  {
    ZLOG_ERR("Error connecting Netlink socket");
    return(false);
  }

  ret = rtnl_link_change(this->_sock(), this->_orig(), this->Link(), 0);
  if (ret < 0)
  {
    ZLOG_ERR(
        "Error executing GetLinkCommand: (" + zLog::IntStr(this->_orig.IfIndex())
            + std::string("): ") + this->_orig.IfName());
    ZLOG_ERR("Error: (" + ZLOG_INT(ret) + ") " + __errstr(ret));
    return (false);
  }

  this->Link(link);
  this->_sock.Disconnect();

  return(true);
}

void
SetLinkCommand::Display() const
{
  this->Link.Display();
}

}
