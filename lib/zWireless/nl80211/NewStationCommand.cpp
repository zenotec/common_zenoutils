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
#include <zutils/nl80211/NewStationCommand.h>

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
// Class: NewStationCommand
//*****************************************************************************


NewStationCommand::NewStationCommand(const unsigned int ifindex_) :
    Command(ifindex_)
{
  this->IfIndex(ifindex_);
}

NewStationCommand::~NewStationCommand()
{
}

bool
NewStationCommand::Exec()
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
  cmdmsg->SetCommand(NL80211_CMD_NEW_STATION);
  cmdmsg->PutAttribute(&this->IfIndex);
  cmdmsg->PutAttribute(&this->IfName);

  if (!cmdmsg->PutAttribute(&this->Mac))
  {
    ZLOG_ERR("Error putting Mac attribute: ");
    std::cout << "sam..................errro putting Mac attribute" <<std::endl;
    return (false);
  }

  if (!cmdmsg->PutAttribute(&this->ListenInterval))
  {
    ZLOG_ERR("Error putting Listen Interval attribute: ");
    std::cout << "sam..................errro putting listen interval" <<std::endl;
    return (false);
  }

  if (!cmdmsg->PutAttribute(&this->StaSupportedRates))
  {
    ZLOG_ERR("Error putting StaSupportedRates attribute: ");
    return (false);
  }

  if (!cmdmsg->PutAttribute(&this->StaAid))
  {
    ZLOG_ERR("Error putting StaAid attribute: ");
    std::cout << "sam..................errro putting staaid" <<std::endl;
    return (false);
  }

  if (!cmdmsg->PutAttribute(&this->StaFlags))
  {
	ZLOG_ERR("Error putting Station Flags attribute: ");
	std::cout << "sam..................errro putting station flags" <<std::endl;
	return (false);
  }
  std::cout << "8" << std::endl;
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

  if (!this->_count.TimedWait(100))
  {
    ZLOG_ERR("Error receiving response for netlink message");
    return(false);
  }

  // Clean up
  this->_sock.Disconnect();

  return(this->_status);

}

void
NewStationCommand::Display(const std::string& prefix_) const
{
  std::cout << "##################################################" << std::endl;
  std::cout << "NewStationCommand: " << std::endl;
  std::cout << "\tName:  \t" << this->IfName() << std::endl;
  std::cout << "\tIndex: \t" << this->IfIndex() << std::endl;
  std::cout << "\tMac:   \t" << this->Mac() << std::endl;
  std::cout << "##################################################" << std::endl;
}

int
NewStationCommand::ack_cb(struct nl_msg* msg_, void* arg_)
{
  this->_status = true;
  this->_count.Post();
  return (NL_OK);
}

int
NewStationCommand::valid_cb(struct nl_msg* msg_, void* arg_)
{

  GenericMessage msg;
  if (!msg.Disassemble(msg_))
  {
    ZLOG_ERR("Error parsing generic message");
    return (NL_SKIP);
  }
  msg.Display();

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

  this->_status = true;
  this->_count.Post();

  return (NL_OK);
}

int
NewStationCommand::err_cb(struct sockaddr_nl* nla, struct nlmsgerr* nlerr, void* arg)
{
  ZLOG_ERR("Error executing NewStationCommand");
  ZLOG_ERR("Error: (" + ZLOG_INT(nlerr->error) + ") " + __errstr(nlerr->error));
  this->_status = false;
  this->_count.Post();
  return(NL_SKIP);
}

}
