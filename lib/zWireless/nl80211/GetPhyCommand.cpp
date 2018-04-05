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
#include <zutils/nl80211/GetPhyCommand.h>

ZLOG_MODULE_INIT(zUtils::zLog::Log::MODULE_WIRELESS);

namespace nl80211
{

static std::string
__errstr(int code)
{
  return(std::string(nl_geterror(code)));
}

//*****************************************************************************
// Class: GetPhyCommand
//*****************************************************************************

GetPhyCommand::GetPhyCommand(int ifindex_) :
    Command(ifindex_)
{
  this->IfIndex(this->GetIfIndex());
}

GetPhyCommand::GetPhyCommand(const std::string& ifname_) :
    Command(ifname_)
{
  this->IfIndex(this->GetIfIndex());
  this->IfName(ifname_);
}

GetPhyCommand::~GetPhyCommand()
{
}

void
GetPhyCommand::Display() const
{
  std::cout << "Phy:" << std::endl;
  std::cout << "\tIndex: \t" << this->PhyIndex() << std::endl;
  std::cout << "\tName:  \t" << this->PhyName() << std::endl;
  this->PhyBands.Display("\t");
}

bool
GetPhyCommand::Exec()
{

  this->_status = false;
  this->_count.Reset();

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
  cmdmsg->SetCommand(NL80211_CMD_GET_WIPHY);

  // Set PHY index attribute
  if (!cmdmsg->PutAttribute(&this->PhyIndex))
  {
    ZLOG_ERR("Error setting phyindex attribute");
    return (false);
  }

  // Send message
  if (!this->_sock.SendMsg(cmdmsg))
  {
    ZLOG_ERR("Error sending get_phy netlink message");
    return(false);
  }

  // Wait for the response
  if (!this->_sock.RecvMsg())
  {
    ZLOG_ERR("Error receiving response for get_phy netlink message");
    return(false);
  }

  if (!this->_count.TimedWait(1000))
  {
    ZLOG_ERR("Error receiving response for get_phy netlink message");
    return(false);
  }

  // Clean up
  this->_sock.Disconnect();

  return(this->_status);

}

int
GetPhyCommand::valid_cb(struct nl_msg* msg_, void* arg_)
{

  GenericMessage msg;
  if (!msg.Disassemble(msg_))
  {
    ZLOG_ERR("Error parsing generic message");
    return(NL_SKIP);
  }

  std::cout << "GetPhyCommand::valid_cb()" << std::endl;
  msg.Display();

  if (!msg.GetAttribute(&this->PhyIndex))
  {
    ZLOG_ERR("Missing attribute: " + zLog::IntStr(this->PhyIndex.GetId()));
    return(NL_SKIP);
  }

  if (!msg.GetAttribute(&this->PhyName))
  {
    ZLOG_ERR("Missing attribute: " + zLog::IntStr(this->PhyName.GetId()));
    return(NL_SKIP);
  }

  msg.GetAttribute(&this->PhyBands);

  this->_status = true;
  this->_count.Post();

  return (NL_OK);
}

int
GetPhyCommand::err_cb(struct sockaddr_nl* nla_, struct nlmsgerr* nlerr_, void* arg_)
{
  ZLOG_ERR("Error executing GetPhyCommand");
  ZLOG_ERR("Error: (" + ZLOG_INT(nlerr_->error) + ") " + __errstr(nlerr_->error));
  this->_status = false;
  this->_count.Post();
  return(NL_SKIP);
}


}
