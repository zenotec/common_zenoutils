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

#include <errno.h>
#include <netlink/route/link.h>

// libc++ includes

#include <string>
#include <iostream>

// libzutils includes
#include <zutils/zLog.h>
#include <zutils/netlink/Attribute.h>
#include <zutils/netlink/Command.h>
#include <zutils/netlink/Message.h>
#include <zutils/netlink/Handler.h>
#include <zutils/netlink/Socket.h>
#include <zutils/netlink/RouteSocket.h>
#include <zutils/netlink/GetLinkCommand.h>
using namespace zUtils;

ZLOG_MODULE_INIT(zUtils::zLog::Log::MODULE_INTERFACE);

// local includes

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
    Command(ifindex_)
{
}

GetLinkCommand::GetLinkCommand(const std::string& ifname_) :
    Command(ifname_)
{
  this->IfName(ifname_);
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

  if (!this->GetIfIndex())
  {
    ZLOG_ERR("Error executing GetLinkCommand: " + ZLOG_INT(this->GetIfIndex()));
    ZLOG_ERR("Valid interface index must be specified");
    return(false);
  }

  if (!this->IfIndex(this->GetIfIndex()))
  {
    ZLOG_ERR("Error executing GetLinkCommand: " + ZLOG_INT(this->GetIfIndex()));
    ZLOG_ERR("Error setting interface index");
    return(false);
  }

  if (!this->_sock.Connect())
  {
    ZLOG_ERR("Error executing GetLinkCommand: " + ZLOG_INT(this->GetIfIndex()));
    ZLOG_ERR("Error connecting Netlink socket");
    return (false);
  }

  ret = rtnl_link_get_kernel(this->_sock(), this->GetIfIndex(), NULL, &link);
  if (ret < 0)
  {
    ZLOG_ERR("Error executing GetLinkCommand: " + this->IfName());
    ZLOG_ERR("Error: (" + ZLOG_INT(ret) + ") " + __errstr(ret));
  }
  else
  {
    this->operator()(link); // Transfers ownership of link pointer
    status = true;
  }

  this->_sock.Disconnect();

  return(status);
}

void
GetLinkCommand::Display() const
{
  std::cout << "##################################################" << std::endl;
  std::cout << "GetLinkCommand: " << std::endl;
  RouteLink::Display();
  std::cout << "##################################################" << std::endl;
}

}
