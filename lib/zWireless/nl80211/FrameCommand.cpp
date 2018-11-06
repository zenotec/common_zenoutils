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
#include <zutils/nl80211/FrameCommand.h>

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
// Class: FrameCommand
//*****************************************************************************

FrameCommand::FrameCommand(int ifindex_) :
    Command(ifindex_)
{
  this->IfIndex.Set(this->GetIfIndex());
}

FrameCommand::FrameCommand(const std::string& ifname_) :
    Command(ifname_)
{
  this->IfIndex.Set(this->GetIfIndex());
  this->IfName.Set(ifname_);
}

FrameCommand::~FrameCommand()
{
}

void
FrameCommand::Display(const std::string& prefix_) const
{
  std::cout << "##################################################" << std::endl;
  std::cout << "FrameCommand: " << std::endl;
  std::cout << "\tIndex: \t" << int(this->IfIndex()) << std::endl;
  std::cout << "\tName:  \t" << this->IfName() << std::endl;
  if (this->Frequency.IsValid())
  {
    std::cout << "\tChannel:  \t" << this->Frequency.GetChannel();
    std::cout << " [" << this->Frequency() << "]" << std::endl;
  }
  std::cout << "##################################################" << std::endl;
}

bool
FrameCommand::Exec()
{

  this->_status = false;
  this->_count.Reset();

  if (!this->IfIndex())
  {
    ZLOG_ERR("Error executing FrameCommand: " + this->IfName());
    ZLOG_ERR("Valid interface index must be specified");
    return(false);
  }

  if (!this->_sock.Connect())
  {
    ZLOG_ERR("Error connecting NL80211 socket");
    return (false);
  }

  if (!this->_sock.SetCallback(this))
  {
    ZLOG_ERR("Error setting up message handlers");
    return (false);
  }

  SHARED_PTR(GenericMessage) cmdmsg = this->_sock.CreateMsg();
  if (!cmdmsg)
  {
    ZLOG_ERR("Error creating the Netlink message");
    return(false);
  }
  cmdmsg->SetCommand(NL80211_CMD_FRAME);

  // Set interface index attribute
  if (!cmdmsg->PutAttribute(&this->IfIndex))
  {
    ZLOG_ERR("Error setting ifindex attribute");
    return (false);
  }

  // Copy frame contents
  if (!cmdmsg->PutAttribute(&this->Frame))
  {
    ZLOG_ERR("Error setting frame attribute");
    return (false);
  }

  // Optional attribute
  cmdmsg->PutAttribute(&this->Frequency);

  // Send message
  if (!this->_sock.SendMsg(cmdmsg))
  {
    ZLOG_ERR("Error sending netlink message");
    return(false);
  }

  // Wait for the response
  if (!this->_sock.RecvMsg())
  {
    ZLOG_ERR("Error receiving response for netlink message");
    return(false);
  }

  if (!this->_count.TimedWait(1000))
  {
    ZLOG_ERR("Error receiving response for netlink message");
    return(false);
  }

  // Clean up
  this->_sock.Disconnect();

  return(this->_status);
}

int
FrameCommand::valid_cb(struct nl_msg* msg_, void* arg_)
{

  GenericMessage msg;
  if (!msg.Disassemble(msg_))
  {
    ZLOG_ERR("Error parsing generic message");
    return (NL_SKIP);
  }
//  msg.Display();

  // Optional attributes
  msg.GetAttribute(&this->IfIndex);
  msg.GetAttribute(&this->IfName);
  msg.GetAttribute(&this->Frequency);
  msg.GetAttribute(&this->Cookie);
  msg.GetAttribute(&this->Frame);

  this->_status = true;
  this->_count.Post();

  return (NL_OK);
}

int
FrameCommand::err_cb(struct sockaddr_nl* nla, struct nlmsgerr* nlerr, void* arg)
{
  ZLOG_ERR("Error executing FrameCommand: (" + zLog::IntStr(this->IfIndex()) +
      std::string("): ") + this->IfName());
  ZLOG_ERR("Error: (" + ZLOG_INT(nlerr->error) + ") " + __errstr(nlerr->error));
  this->_status = false;
  this->_count.Post();
  return(NL_SKIP);
}

}
