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

// libc++ includes
#include <iostream>

// libzutils includes
#include <zutils/zLog.h>
using namespace zUtils;
#include <zutils/nl80211/PhyIndexAttribute.h>
#include <zutils/nl80211/PhyNameAttribute.h>
#include <zutils/nl80211/ListPhysCommand.h>

// local includes

ZLOG_MODULE_INIT(zUtils::zLog::Log::MODULE_WIRELESS);

namespace nl80211
{

static std::string
__errstr(int code)
{
  return(std::string(nl_geterror(code)));
}

//*****************************************************************************
// Class: ListPhysCommand
//*****************************************************************************

ListPhysCommand::ListPhysCommand() :
    Command(0)
{
}

ListPhysCommand::~ListPhysCommand()
{
}

std::map<int, std::string>
ListPhysCommand::operator() ()
{
  return(this->_phys);
}

void
ListPhysCommand::Display() const
{
  FOREACH(auto& phy, this->_phys)
  {
    std::cout << "Phy:" << std::endl;
    std::cout << "\tIndex: \t" << phy.first << std::endl;
    std::cout << "\tName:  \t" << phy.second << std::endl;
  }
}

bool
ListPhysCommand::Exec()
{
  this->_status = false;
  this->_count.Reset();
  this->_phys.clear();

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

  SHARED_PTR(GenericMessage) cmdmsg = this->_sock.CreateMsg();
  cmdmsg->SetFlags(NLM_F_DUMP);
  cmdmsg->SetCommand(NL80211_CMD_GET_WIPHY);

  // Send message
  if (!this->_sock.SendMsg(cmdmsg))
  {
    ZLOG_ERR("Error sending get_interface netlink message");
    return(false);
  }

  // Wait for the response
  if (!this->_sock.RecvMsg())
  {
    ZLOG_ERR("Error receiving response for del_interface netlink message");
    return(false);
  }

  if (!this->_count.TimedWait(100))
  {
    ZLOG_ERR("Error receiving response for del_interface netlink message");
    return(false);
  }

  // Clean up
  this->_sock.Disconnect();

  return(this->_status);

}

int
ListPhysCommand::valid_cb(struct nl_msg* msg_, void* arg_)
{

  PhyIndexAttribute phyindex;
  PhyNameAttribute phyname;

  GenericMessage msg;
  if (!msg.Disassemble(msg_))
  {
    ZLOG_ERR("Error parsing generic message");
    return(NL_SKIP);
  }

  if (!msg.GetAttribute(phyindex))
  {
    ZLOG_ERR("Missing attribute: " + zLog::IntStr(phyindex.GetId()));
    return(NL_SKIP);
  }

  if (!msg.GetAttribute(phyname))
  {
    ZLOG_ERR("Missing attribute: " + zLog::IntStr(phyname.GetId()));
    return(NL_SKIP);
  }

  this->_phys[phyindex()] = phyname();

  return (NL_OK);
}

int
ListPhysCommand::finish_cb(struct nl_msg* msg_, void* arg_)
{
  this->_status = true;
  this->_count.Post();
  return (NL_OK);
}

int
ListPhysCommand::err_cb(struct sockaddr_nl* nla, struct nlmsgerr* nlerr, void* arg)
{
  ZLOG_ERR("Error executing ListPhyCommand");
  ZLOG_ERR("Error: (" + ZLOG_INT(nlerr->error) + ") " + __errstr(nlerr->error));
  this->_status = false;
  this->_count.Post();
  return(NL_SKIP);
}

}
