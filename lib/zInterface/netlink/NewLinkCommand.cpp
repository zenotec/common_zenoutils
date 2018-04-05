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
#include <netlink/netlink.h>
#include <netlink/errno.h>
#include <netlink/route/link.h>
#include <netlink/route/link/vlan.h>
#include <netlink/route/link/macvlan.h>

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
#include <zutils/netlink/NewLinkCommand.h>
using namespace zUtils;


// local includes

ZLOG_MODULE_INIT(zUtils::zLog::Log::MODULE_INTERFACE);

namespace netlink
{

static std::string
__errstr(int code)
{
  return(std::string(nl_geterror(code)));
}

//*****************************************************************************
// Class: NewLinkCommand
//*****************************************************************************

NewLinkCommand::NewLinkCommand(const TYPE type_, const std::string& ifname_) :
    Command(ifname_), _type(type_)
{
  switch(type_)
  {
  case TYPE_VLAN:
    RouteLink::operator ()(rtnl_link_vlan_alloc());
    break;
  case TYPE_MACVLAN:
    RouteLink::operator ()(rtnl_link_macvlan_alloc());
    break;
  default:
    break;
  }
  this->IfName(ifname_);
}

NewLinkCommand::~NewLinkCommand()
{
}

bool
NewLinkCommand::Exec()
{

  bool status = false;
  int ret = 0;

  if (!this->MasterIfIndex())
  {
    ZLOG_ERR("Error executing NewLinkCommand: " + ZLOG_INT(this->MasterIfIndex()));
    ZLOG_ERR("Valid interface index must be specified for the master interface");
    return(false);
  }

  if (this->IfName().empty())
  {
    ZLOG_ERR("Error executing NewLinkCommand: name is empty");
    return(false);
  }

  if (!this->_sock.Connect())
  {
    ZLOG_ERR("Error executing NewLinkCommand: " + ZLOG_INT(this->GetIfIndex()));
    ZLOG_ERR("Error connecting Netlink socket");
    return (false);
  }

  switch(this->_type)
  {
  case TYPE_VLAN:
    rtnl_link_vlan_set_id(this->operator ()(), 10); // TODO: Add vlan id to config
    break;
  case TYPE_MACVLAN:
    rtnl_link_macvlan_set_mode(this->operator ()(), rtnl_link_macvlan_str2mode("bridge"));
    break;
  default:
    break;
  }

  ret = rtnl_link_add(this->_sock(), this->operator ()(), NLM_F_CREATE);
  if (ret < 0)
  {
    ZLOG_ERR("Error executing NewLinkCommand: " + this->IfName());
    ZLOG_ERR("Error: (" + ZLOG_INT(ret) + ") " + __errstr(ret));
  }
  else
  {
    status = true;
  }

  this->_sock.Disconnect();

  return(status);
}

void
NewLinkCommand::Display(const std::string& prefix_) const
{
  std::cout << "##################################################" << std::endl;
  std::cout << "NewLinkCommand: " << std::endl;
  RouteLink::Display();
  std::cout << "##################################################" << std::endl;
}

}
