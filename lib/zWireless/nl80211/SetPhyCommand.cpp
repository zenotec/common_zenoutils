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
#include <zutils/nl80211/SetPhyCommand.h>

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
SetPhyCommand::Display(const std::string& prefix_) const
{
  std::cout << "##################################################" << std::endl;
  std::cout << "SetPhyCommand:" << std::endl;
  std::cout << "\tIndex:    \t" << this->IfIndex() << std::endl;
  if (this->IfName.IsValid())
  {
    std::cout << "\tName:     \t" << this->IfName() << std::endl;
  }
  if (this->PhyIndex.IsValid())
  {
    std::cout << "\tPhyIndex: \t" << this->PhyIndex() << std::endl;
  }
  if (this->PhyName.IsValid())
  {
    std::cout << "\tPhyName:  \t" << this->PhyName() << std::endl;
  }
  if (this->Frequency.IsValid())
  {
    std::cout << "\tChannel:  \t" << this->Frequency.GetChannel();
    std::cout << " [" << this->Frequency() << "]" << std::endl;
  }
  if (this->ChannelWidth.IsValid())
  {
    std::cout << "\tChannelWidth:  \t" << this->ChannelWidth() << std::endl;
  }
  if (this->CenterFrequency1.IsValid())
  {
    std::cout << "\tCenter 1:  \t" << this->CenterFrequency1() << std::endl;
  }
  if (this->CenterFrequency2.IsValid())
  {
    std::cout << "\tCenter 2:  \t" << this->CenterFrequency2() << std::endl;
  }
  if (this->TxPowerMode.IsValid())
  {
    std::cout << "\tPowerMode:\t" << this->TxPowerMode() << std::endl;
  }
  if (this->TxPowerLevel.IsValid())
  {
    std::cout << "\tPowerLevel:\t" << this->TxPowerLevel() << std::endl;
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
  cmdmsg->SetCommand(NL80211_CMD_SET_WIPHY);

  // Set interface index attribute
  if (!cmdmsg->PutAttribute(&this->IfIndex))
  {
    ZLOG_ERR("Error setting ifindex attribute");
    return (false);
  }

  // Set phy index attribute
  if (!cmdmsg->PutAttribute(&this->PhyIndex))
  {
    ZLOG_ERR("Error setting phyindex attribute");
    return (false);
  }

  // Set optional phy name attribute
  cmdmsg->PutAttribute(&this->PhyName);
  cmdmsg->PutAttribute(&this->TxPowerMode);
  cmdmsg->PutAttribute(&this->TxPowerLevel);
  cmdmsg->PutAttribute(&this->Frequency);
  cmdmsg->PutAttribute(&this->ChannelWidth);
  cmdmsg->PutAttribute(&this->CenterFrequency1);
  cmdmsg->PutAttribute(&this->CenterFrequency2);

//  std::cout << "SetPhyCommand::Exec()" << std::endl;
//  cmdmsg->Display();

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
//  std::cout << "SetPhyCommand::ack_cb()" << std::endl;
//  this->Display();
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
