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
#include <linux/nl80211.h>
#include <netlink/netlink.h>
#include <netlink/msg.h>
#include <netlink/attr.h>
#include <netlink/genl/genl.h>
#include <netlink/genl/ctrl.h>

// libc++ includes
#include <iostream>
#include <map>

// libzutils includes
#include <zutils/zLog.h>
using namespace zUtils;

// local includes
#include "Command.h"
#include "Attribute.h"

#include "Message.h"
#include "Handler.h"
#include "Socket.h"
#include "GenericMessage.h"
#include "GenericSocket.h"
using namespace netlink;

#include "PhyIndexAttribute.h"
#include "PhyNameAttribute.h"
#include "SetPhyCommand.h"

namespace nl80211
{

static std::string
__errstr(int code)
{
  return(std::string(nl_geterror(code)));
}

//*****************************************************************************
// Class: SetPhyCommand
//*****************************************************************************

SetPhyCommand::SetPhyCommand(int ifindex_) :
    Command(ifindex_)
{
  this->IfIndex(this->GetIfIndex());
}

SetPhyCommand::SetPhyCommand(const std::string& ifname_) :
    Command(ifname_)
{
  this->IfIndex(this->GetIfIndex());
  this->IfName(ifname_);
}

SetPhyCommand::~SetPhyCommand()
{
}

void
SetPhyCommand::Display() const
{
  std::cout << "##################################################" << std::endl;
  std::cout << "SetPhyCommand:" << std::endl;
  std::cout << "\tIndex:    \t" << this->IfIndex.GetValue() << std::endl;
  std::cout << "\tName:     \t" << this->IfName.GetValue() << std::endl;
  std::cout << "\tPhyIndex: \t" << this->PhyIndex.GetValue() << std::endl;
  if (this->PhyName.IsValid())
  {
    std::cout << "\tPhyName:  \t" << this->PhyName.GetValue() << std::endl;
  }
  if (this->Frequency.IsValid())
  {
    std::cout << "\tChannel:  \t" << this->Frequency.GetChannel() << std::endl;
    std::cout << "\tFreq:     \t" << this->Frequency.GetValue() << std::endl;
  }
  std::cout << "##################################################" << std::endl;
}

bool
SetPhyCommand::Exec()
{

  this->_status = false;
  this->_count.Reset();

  if (!this->IfIndex())
  {
    ZLOG_ERR("Error executing SetPhyCommand: " + this->IfName());
    ZLOG_ERR("Valid interface index must be specified");
    return (false);
  }

  if (!this->_sock.Connect())
  {
    ZLOG_ERR("Error connecting NL80211 socket");
    return (false);
  }

  if (!this->_sock.SetHandler(this))
  {
    ZLOG_ERR("Error setting up message handlers");
    return (false);
  }

  GenericMessage cmdmsg(this->_sock.Family(), 0, NL80211_CMD_SET_WIPHY);

  // Set interface index attribute
  if (!cmdmsg.PutAttribute(&this->IfIndex))
  {
    ZLOG_ERR("Error setting ifindex attribute");
    return (false);
  }

  // Set phy index attribute
  if (!cmdmsg.PutAttribute(&this->PhyIndex))
  {
    ZLOG_ERR("Error setting phyindex attribute");
    return (false);
  }

  // Set optional phy name attribute
  cmdmsg.PutAttribute(&this->PhyName);

  // Set optional phy channel attribute
  cmdmsg.PutAttribute(&this->Frequency);

  // Send message
  if (!this->_sock.SendMsg(cmdmsg))
  {
    ZLOG_ERR("Error sending set_phy netlink message");
    return (false);
  }

  // Wait for the response
  if (!this->_sock.RecvMsg())
  {
    ZLOG_ERR("Error receiving response for set_phy netlink message");
    return (false);
  }

  if (!this->_count.TimedWait(1000))
  {
    ZLOG_ERR("Error receiving response for set_phy netlink message");
    return (false);
  }

  // Clean up
  this->_sock.Disconnect();

  return (this->_status);

}

int
SetPhyCommand::ack_cb(struct nl_msg* msg_, void* arg_)
{

  GenericMessage msg(msg_);
  if (!msg.Parse())
  {
    ZLOG_ERR("Error parsing generic message");
    return (NL_SKIP);
  }

  std::cout << "SetPhyCommand::ack_cb()" << std::endl;
  //msg.Display();
  msg.DisplayAttributes();

  this->_status = true;
  this->_count.Post();

  return (NL_OK);
}

int
SetPhyCommand::err_cb(struct sockaddr_nl* nla_, struct nlmsgerr* nlerr_, void* arg_)
{
  ZLOG_ERR("Error executing SetPhyCommand");
  ZLOG_ERR("Error: (" + ZLOG_INT(nlerr_->error) + ") " + __errstr(nlerr_->error));
  this->_status = false;
  this->_count.Post();
  return (NL_SKIP);
}


}
