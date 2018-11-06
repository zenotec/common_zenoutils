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
#include <zutils/nl80211/NewInterfaceCommand.h>

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
// Class: NewInterfaceCommand
//*****************************************************************************

NewInterfaceCommand::NewInterfaceCommand(const std::string& ifname_, const int phyindex_) :
    Command(ifname_)
{
  this->IfName(ifname_);
  this->PhyIndex(phyindex_);
}

NewInterfaceCommand::~NewInterfaceCommand()
{
}

bool
NewInterfaceCommand::Exec()
{

  this->_status = false;
  this->_count.Reset();

  if (this->IfName().empty())
  {
    ZLOG_ERR("Error executing NewInterfaceCommand: name is empty");
    return(false);
  }

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

  SHARED_PTR(GenericMessage) cmdmsg = this->_sock.CreateMsg();
  if (!cmdmsg)
  {
    ZLOG_ERR("Error creating the Netlink message");
    return(false);
  }
  cmdmsg->SetCommand(NL80211_CMD_NEW_INTERFACE);

  // Set phy index attribute
  if (!cmdmsg->PutAttribute(&this->PhyIndex))
  {
    ZLOG_ERR("Error setting phyindex attribute");
    return (false);
  }

  // Set interface name attribute
  if (!cmdmsg->PutAttribute(&this->IfName))
  {
    ZLOG_ERR("Error setting interface name attribute");
    return (false);
  }

  // Set interface type attribute
  if (!cmdmsg->PutAttribute(&this->IfType))
  {
    ZLOG_ERR("Error setting iftype attribute");
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
NewInterfaceCommand::Display(const std::string& prefix_) const
{
  std::cout << "New Interface: " << std::endl;
  if (this->PhyIndex.IsValid())
    std::cout << "\tPhyIndex:\t" << this->PhyIndex() << std::endl;
  if (this->IfName.IsValid())
    std::cout << "\tIfName:  \t" << this->IfName() << std::endl;
  if (this->IfIndex.IsValid())
    std::cout << "\tIfIndex: \t" << this->IfIndex() << std::endl;
  if (this->IfType.IsValid())
    std::cout << "\tIfType:  \t" << this->IfType.ToString() << std::endl;
}

int
NewInterfaceCommand::valid_cb(struct nl_msg* msg_, void* arg)
{

  this->_count.Reset();

  GenericMessage msg;
  if (!msg.Disassemble(msg_))
  {
    ZLOG_ERR("Error parsing generic message");
    return (NL_SKIP);
  }

//  std::cout << "NewInterfaceCommand::valid_cb()" << std::endl;
//  msg.Display();

  if (!msg.GetAttribute(&this->PhyIndex))
  {
    ZLOG_ERR("Missing attribute: " + zLog::IntStr(this->PhyIndex.GetId()));
    return(NL_SKIP);
  }

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

  if (!msg.GetAttribute(&this->IfType))
  {
    ZLOG_ERR("Missing attribute: " + zLog::IntStr(this->IfType.GetId()));
    return(NL_SKIP);
  }

  this->SetIfIndex(this->IfIndex());
  this->_status = true;
  this->_count.Post();

  return(NL_OK);
}

int
NewInterfaceCommand::err_cb(struct sockaddr_nl* nla, struct nlmsgerr* nlerr, void* arg)
{
  ZLOG_ERR("Error executing NewInterfaceCommand");
  ZLOG_ERR("Error: (" + ZLOG_INT(nlerr->error) + ") " + __errstr(nlerr->error));
  this->_status = false;
  this->_count.Post();
  return(NL_SKIP);
}

}
