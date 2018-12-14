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
#include <string.h>
#include <errno.h>

// libc++ includes
#include <iostream>

// libzutils includes
#include <zutils/zLog.h>
using namespace zUtils;
#include <zutils/nl80211/IfIndexAttribute.h>
#include <zutils/nl80211/IfNameAttribute.h>
#include <zutils/nl80211/ListInterfacesCommand.h>

// local includes

ZLOG_MODULE_INIT(zUtils::zLog::Log::MODULE_WIRELESS);

namespace nl80211
{

static std::string
__errstr(int code)
{
  return(std::string(strerror(-code)));
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
ListInterfacesCommand::Display(const std::string& prefix_) const
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

  this->_status = false;
  this->_count.Reset();
  this->_ifs.clear();

  if (!this->_sock.Connect())
  {
    ZLOG_ERR("Error connecting NL80211 socket");
    return(false);
  }

  if (!this->_sock.SetCallback(this))
  {
    ZLOG_ERR("Error setting up message handlers");
    return(false);
  }

  SHPTR(GenericMessage) cmdmsg = this->_sock.CreateMsg();
  if (!cmdmsg)
  {
    ZLOG_ERR("Error creating the Netlink message");
    return(false);
  }
  cmdmsg->SetCommand(NL80211_CMD_GET_INTERFACE);
  cmdmsg->SetFlags(NLM_F_DUMP);

  // Send message
  if (!this->_sock.SendMsg(cmdmsg))
  {
    ZLOG_ERR("Error sending netlink message");
    return(false);
  }

  // Wait for the response
  if (!this->_sock.RecvMsg())
  {
    ZLOG_ERR("Error receiving netlink message");
    return(false);
  }

  if (!this->_count.TimedWait(100))
  {
    ZLOG_ERR("Error waiting for response for netlink message");
    return(false);
  }

  // Clean up
  this->_sock.Disconnect();

  return(this->_status);

}

int
ListInterfacesCommand::valid_cb(struct nl_msg* msg_, void* arg_)
{

  IfIndexAttribute ifindex;
  IfNameAttribute ifname;

  GenericMessage msg;
  if (!msg.Disassemble(msg_))
  {
    ZLOG_ERR("Error parsing generic message");
    return(NL_SKIP);
  }

//  std::cout << "ListInterfacesCommand::valid_cb()" << std::endl;
//  msg.Display();

  if (!msg.GetAttribute(&ifindex))
  {
    ZLOG_ERR("Missing attribute: " + zLog::IntStr(ifindex.GetId()));
    return(NL_SKIP);
  }

  if (!msg.GetAttribute(&ifname))
  {
    ZLOG_ERR("Missing attribute: " + zLog::IntStr(ifname.GetId()));
    return(NL_SKIP);
  }

  // Add interface to map
  this->_ifs[ifindex()] = ifname();

  return (NL_OK);

}

int
ListInterfacesCommand::finish_cb(struct nl_msg* msg_, void* arg_)
{
  this->_status = true;
  this->_count.Post();
  return (NL_OK);
}

int
ListInterfacesCommand::err_cb(struct sockaddr_nl* nla, struct nlmsgerr* nlerr, void* arg)
{
  ZLOG_ERR("Error executing ListInterfaceCommand");
  ZLOG_ERR("Error: (" + ZLOG_INT(nlerr->error) + ") " + __errstr(nlerr->error));
  this->_status = false;
  this->_count.Post();
  return(NL_SKIP);
}

}
