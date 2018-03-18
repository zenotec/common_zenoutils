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
#include <zutils/nl80211/GetInterfaceCommand.h>

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
// Class: GetInterfaceCommand
//*****************************************************************************

GetInterfaceCommand::GetInterfaceCommand(int ifindex_) :
    Command(ifindex_)
{
  this->IfIndex.SetValue(this->GetIfIndex());
}

GetInterfaceCommand::GetInterfaceCommand(const std::string& ifname_) :
    Command(ifname_)
{
  this->IfIndex.SetValue(this->GetIfIndex());
  this->IfName.SetValue(ifname_);
}

GetInterfaceCommand::~GetInterfaceCommand()
{
}

void
GetInterfaceCommand::Display() const
{
  std::cout << "##################################################" << std::endl;
  std::cout << "GetInterfaceCommand: " << std::endl;
  if (this->IfIndex.IsValid())
    std::cout << "\tIndex: \t" << int(this->IfIndex()) << std::endl;
  if (this->IfName.IsValid())
    std::cout << "\tName:  \t" << this->IfName() << std::endl;
  std::cout << "\tPhy:   \t" << this->PhyIndex() << std::endl;
  std::cout << "\tType:  \t" << this->IfType.ToString() << std::endl;
  std::cout << "\tMAC:   \t" << this->Mac.GetString() << std::endl;
  std::cout << "\tFreq:  \t" << this->Frequency.GetValue<uint32_t>() << std::endl;
  std::cout << "\tSSID:  \t" << this->Ssid.GetString() << std::endl;
  std::cout << "\tPower: \t" << this->TxPowerLevel.GetValue<uint32_t>() << std::endl;
  std::cout << "##################################################" << std::endl;
}

bool
GetInterfaceCommand::Exec()
{

  this->_status = false;
  this->_count.Reset();

  if (!this->IfIndex.IsValid())
  {
    ZLOG_ERR("Error executing GetInterfaceCommand: " + this->IfName());
    ZLOG_ERR("Valid interface index must be specified");
    return(false);
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

  SHARED_PTR(GenericMessage) cmdmsg = this->_sock.CreateMsg();
  cmdmsg->SetCommand(NL80211_CMD_GET_INTERFACE);

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
    ZLOG_ERR("Error receiving response for get_interface netlink message");
    return(false);
  }

  if (!this->_count.TimedWait(1000))
  {
    ZLOG_ERR("Error receiving response for get_interface netlink message");
    return(false);
  }

  // Clean up
  this->_sock.Disconnect();

  return(this->_status);
}

int
GetInterfaceCommand::valid_cb(struct nl_msg* msg_, void* arg_)
{

  GenericMessage msg;
  if (!msg.Disassemble(msg_))
  {
    ZLOG_ERR("Error parsing generic message");
    return (NL_SKIP);
  }

  std::cout << "GetInterfaceCommand::valid_cb()" << std::endl;
  msg.Display();
  msg.DisplayAttributes();

  if (!msg.GetAttribute(this->PhyIndex))
  {
    ZLOG_ERR("Missing attribute: " + zLog::IntStr(this->PhyIndex.GetId()));
    return(NL_SKIP);
  }

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

  if (!msg.GetAttribute(this->IfType))
  {
    ZLOG_ERR("Missing attribute: " + zLog::IntStr(this->IfType.GetId()));
    return(NL_SKIP);
  }

  if (!msg.GetAttribute(this->Mac))
  {
    ZLOG_ERR("Missing attribute: " + zLog::IntStr(this->Mac.GetId()));
    return (NL_SKIP);
  }

  // Optional attributes
  msg.GetAttribute(this->Ssid);
  msg.GetAttribute(this->Frequency);
  msg.GetAttribute(this->ChannelType);
  msg.GetAttribute(this->ChannelWidth);
  msg.GetAttribute(this->TxPowerMode);
  msg.GetAttribute(this->TxPowerLevel);

  this->_status = true;
  this->_count.Post();

  return (NL_OK);
}

int
GetInterfaceCommand::err_cb(struct sockaddr_nl* nla, struct nlmsgerr* nlerr, void* arg)
{
  ZLOG_ERR("Error executing GetInterfaceCommand: (" + zLog::IntStr(this->IfIndex()) +
      std::string("): ") + this->IfName());
  ZLOG_ERR("Error: (" + ZLOG_INT(nlerr->error) + ") " + __errstr(nlerr->error));
  this->_status = false;
  this->_count.Post();
  return(NL_SKIP);
}

}
