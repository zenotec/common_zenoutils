/*
 * Copyright (c) 2018 Cable Television Laboratories, Inc. ("CableLabs")
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
#include <zutils/nl80211/StopApCommand.h>

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
// Class: StopApCommand
//*****************************************************************************

StopApCommand::StopApCommand(int index_) :
    Command(index_)
{
  this->IfIndex.SetValue(index_);
}

StopApCommand::StopApCommand(const std::string& ifname_) :
    Command(ifname_)
{
  this->IfIndex.SetValue(this->GetIfIndex());
  this->IfName.SetValue(ifname_);
}

StopApCommand::~StopApCommand()
{
}

bool
StopApCommand::Exec()
{
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
  cmdmsg->SetCommand(NL80211_CMD_STOP_AP);


  // Set interface index attribute
  if (!cmdmsg->PutAttribute(this->IfIndex))
  {
    ZLOG_ERR("Error setting ifindex attribute");
    return (false);
  }

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

void
StopApCommand::Display() const
{
  std::cout << "Set BSS: " << std::endl;
  std::cout << "\tName:  \t" << this->IfName() << std::endl;
  std::cout << "\tIndex: \t" << this->IfIndex() << std::endl;
  std::cout << "\tSsid:  \t" << this->Ssid.GetString() << std::endl;
}

int
StopApCommand::valid_cb(struct nl_msg* msg_, void* arg_)
{

  GenericMessage msg;
  if (!msg.Disassemble(msg_))
  {
    ZLOG_ERR("Error parsing generic message");
    return (NL_SKIP);
  }

  msg.DisplayAttributes();

  if (!msg.GetAttribute(this->IfIndex))
  {
    ZLOG_ERR("Missing attribute: " + zLog::IntStr(this->IfIndex.GetId()));
    return(NL_SKIP);
  }

  if (!msg.GetAttribute(this->IfName))
  {
    ZLOG_ERR("Missing attribute: " + zLog::IntStr(this->IfName.GetId()));
    return(NL_SKIP);
  }

  return (NL_OK);
}

int
StopApCommand::err_cb(struct sockaddr_nl* nla, struct nlmsgerr* nlerr, void* arg)
{
  ZLOG_ERR("Error executing StopApCommand");
  ZLOG_ERR("Error: (" + ZLOG_INT(nlerr->error) + ") " + __errstr(nlerr->error));
  return(NL_SKIP);
}

}
