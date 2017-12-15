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


SetLinkCommand::SetLinkCommand(const unsigned int ifindex_) :
    Command(ifindex_)
{
  this->_orig.IfIndex(this->GetIfIndex());
  this->IfIndex(this->GetIfIndex());
}

SetLinkCommand::SetLinkCommand(const std::string& ifname_) :
    Command(ifname_)
{
  this->_orig.IfIndex(this->GetIfIndex());
  this->_orig.IfName(ifname_);
  this->IfIndex(this->GetIfIndex());
  this->IfName(ifname_);
}

SetLinkCommand::~SetLinkCommand()
{
}

bool
SetLinkCommand::Exec()
{

  bool status = false;
  int ret = 0;

  if (!this->GetIfIndex())
  {
    ZLOG_ERR("Error executing SetLinkCommand: " + ZLOG_INT(this->GetIfIndex()));
    ZLOG_ERR("Valid interface index must be specified");
    return(false);
  }

  if (!this->_orig.IfIndex(this->GetIfIndex()) || !this->IfIndex(this->GetIfIndex()))
  {
    ZLOG_ERR("Error executing SetLinkCommand: " + ZLOG_INT(this->GetIfIndex()));
    ZLOG_ERR("Error setting interface index");
    return(false);
  }

  if (!this->_sock.Connect())
  {
    ZLOG_ERR("Error executing SetLinkCommand: " + ZLOG_INT(this->GetIfIndex()));
    ZLOG_ERR("Error connecting Netlink socket");
    return (false);
  }

  ret = rtnl_link_change(this->_sock(), this->_orig(), this->operator ()(), 0);
  if (ret < 0)
  {
    ZLOG_ERR("Error executing SetLinkCommand: " + zLog::IntStr(this->GetIfIndex()));
    ZLOG_ERR("Error: (" + ZLOG_INT(ret) + ") " + __errstr(ret));
  }
  else
  {
    status = true;
  }

  this->_sock.Disconnect();

  return (status);
}

void
SetLinkCommand::Display() const
{
  std::cout << "##################################################" << std::endl;
  std::cout << "SetLinkCommand: " << std::endl;
  std::cout << "Orig: " << std::endl;
  this->_orig.Display();
  std::cout << "New:  " << std::endl;
  RouteLink::Display();
  std::cout << "##################################################" << std::endl;
}

}