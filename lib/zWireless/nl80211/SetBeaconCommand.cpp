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
#include <zutils/nl80211/SetBeaconCommand.h>

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
// Class: SetBeaconCommand
//*****************************************************************************

SetBeaconCommand::SetBeaconCommand(int index_) :
    Command(index_)
{
  this->IfIndex.Set(index_);
}

SetBeaconCommand::SetBeaconCommand(const std::string& name_) :
    Command(name_)
{
  this->IfName.Set(name_);
}

SetBeaconCommand::~SetBeaconCommand()
{
}

bool
SetBeaconCommand::Exec()
{

  this->_count.Reset();

  if (!this->IfIndex())
  {
    ZLOG_ERR("Error getting interface index for: " + this->IfName());
    return(false);
  }

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
  cmdmsg->SetCommand(NL80211_CMD_SET_BEACON);

  cmdmsg->PutAttribute(&this->IfIndex);
  cmdmsg->PutAttribute(&this->Ssid);
  cmdmsg->PutAttribute(&this->BeaconInterval);
  cmdmsg->PutAttribute(&this->DtimPeriod);
  cmdmsg->PutAttribute(&this->BeaconHead);
  cmdmsg->PutAttribute(&this->BeaconTail);

  // Send message
  if (!this->_sock.SendMsg(cmdmsg))
  {
    ZLOG_ERR("Error sending get_interface netlink message");
    return(false);
  }

  // Wait for the response
  if (!this->_sock.RecvMsg())
  {
    ZLOG_ERR("Error receiving response for get_interface netlink message");
    return(false);
  }

  if (!this->_count.TimedWait(100))
  {
    ZLOG_ERR("Error receiving response for get_interface netlink message");
    return(false);
  }

  // Clean up
  this->_sock.Disconnect();

  return(this->_status);

}

void
SetBeaconCommand::Display() const
{
  std::cout << "##################################################" << std::endl;
  std::cout << "SetBeaconCommand: " << std::endl;
  std::cout << "\tName:  \t" << this->IfName() << std::endl;
  std::cout << "\tIndex: \t" << this->IfIndex() << std::endl;
  std::cout << "\tSsid:  \t" << this->Ssid() << std::endl;
  std::cout << "\tBINT:  \t" << this->BeaconInterval() << std::endl;
  std::cout << "\tDTIM:  \t" << this->DtimPeriod() << std::endl;
//  std::cout << "\tBHEAD: \t" << this->BeaconHead()<< std::endl;
//  std::cout << "\tBTAIL: \t" << this->BeaconTail() << std::endl;
  std::cout << "##################################################" << std::endl;
}

int
SetBeaconCommand::valid_cb(struct nl_msg* msg_, void* arg_)
{

  GenericMessage msg;
  if (!msg.Disassemble(msg_))
  {
    ZLOG_ERR("Error parsing generic message");
    return (NL_SKIP);
  }
//  msg.DisplayAttributes();

  if (!msg.GetAttribute(&this->IfIndex))
  {
    ZLOG_ERR("Missing attribute: " + zLog::IntStr(this->IfIndex.GetId()));
    return(NL_SKIP);
  }

  if (!msg.GetAttribute(&this->IfName))
  {
    ZLOG_ERR("Missing attribute: " + zLog::IntStr(this->IfName.GetId()));
    return(NL_SKIP);
  }

  if (!msg.GetAttribute(&this->Ssid))
  {
    ZLOG_ERR("Missing attribute: " + zLog::IntStr(this->Ssid.GetId()));
    return(NL_SKIP);
  }

  this->_status = true;
  this->_count.Post();

  return (NL_OK);
}

int
SetBeaconCommand::err_cb(struct sockaddr_nl* nla, struct nlmsgerr* nlerr, void* arg)
{
  ZLOG_ERR("Error executing SetBeaconCommand");
  ZLOG_ERR("Error: (" + ZLOG_INT(nlerr->error) + ") " + __errstr(nlerr->error));
  this->_status = false;
  this->_count.Post();
  return(NL_SKIP);
}

}
