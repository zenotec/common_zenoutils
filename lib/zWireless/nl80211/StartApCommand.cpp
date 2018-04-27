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
#include <zutils/nl80211/StartApCommand.h>

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
// Class: StartApCommand
//*****************************************************************************

StartApCommand::StartApCommand(const unsigned int ifindex_) :
    Command(ifindex_)
{
  this->IfIndex(ifindex_);
}

StartApCommand::~StartApCommand()
{
}

bool
StartApCommand::Exec()
{

  this->_status = false;
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
  cmdmsg->SetCommand(NL80211_CMD_START_AP);

  if (!cmdmsg->PutAttribute(&this->IfIndex))
  {
    ZLOG_ERR("Error putting interface index attribute");
    return (false);
  }

  if (!cmdmsg->PutAttribute(&this->Ssid))
  {
    ZLOG_ERR("Error putting SSID attribute: " + this->Ssid());
    return (false);
  }

  if (!cmdmsg->PutAttribute(&this->ChannelWidth))
  {
    ZLOG_ERR("Error putting channel width attribute: " + zToStr(this->ChannelWidth()));
    return (false);
  }

  if (!cmdmsg->PutAttribute(&this->Channel))
  {
    ZLOG_ERR("Error putting channel attribute: " + zToStr(this->Channel.GetChannel()));
    return (false);
  }

  if (!cmdmsg->PutAttribute(&this->CenterFrequency1))
  {
    ZLOG_ERR("Error putting center frequency 1 attribute: " + zToStr(this->CenterFrequency1()));
    return (false);
  }

  if ((this->ChannelWidth() == NL80211_CHAN_WIDTH_80P80) && !cmdmsg->PutAttribute(&this->CenterFrequency2))
  {
    ZLOG_ERR("Error putting center frequency 2 attribute: " + zToStr(this->CenterFrequency2()));
    return (false);
  }

  if (!cmdmsg->PutAttribute(&this->BeaconInterval))
  {
    ZLOG_ERR("Error putting beacon interval attribute: " + zToStr(this->BeaconInterval()));
    return (false);
  }

  if (!cmdmsg->PutAttribute(&this->DtimPeriod))
  {
    ZLOG_ERR("Error putting DTIM period attribute");
    return (false);
  }

  if (!cmdmsg->PutAttribute(&this->BeaconHead))
  {
    ZLOG_ERR("Error putting beacon head attribute");
    return (false);
  }

  if (!cmdmsg->PutAttribute(&this->BeaconTail))
  {
    ZLOG_ERR("Error putting beacon tail attribute");
    return (false);
  }

  if (!cmdmsg->PutAttribute(&this->ProbeResp))
  {
    ZLOG_ERR("Error putting probe response attribute");
    return (false);
  }

  cmdmsg->PutAttribute(&this->Channel);

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

void
StartApCommand::Display(const std::string& prefix_) const
{
  std::cout << "##################################################" << std::endl;
  std::cout << "StartApCommand: " << std::endl;
  if (this->IfName.IsValid())
    std::cout << "\tName:   \t" << this->IfName() << std::endl;
  if (this->IfIndex.IsValid())
    std::cout << "\tIndex:  \t" << this->IfIndex() << std::endl;
  if (this->Ssid.IsValid())
    std::cout << "\tSsid:   \t" << this->Ssid() << std::endl;
  if (this->ChannelWidth.IsValid())
    std::cout << "\tChannelWidth:\t" << this->ChannelWidth() << std::endl;
  if (this->Channel.IsValid())
    std::cout << "\tChannel:\t" << this->Channel.GetChannel() << " [" << this->Channel() << "]" << std::endl;
  if (this->CenterFrequency1.IsValid())
    std::cout << "\tCenter Freq 1:\t" << this->CenterFrequency1() << std::endl;
  if (this->CenterFrequency2.IsValid())
    std::cout << "\tCenter Freq 2:\t" << this->CenterFrequency2() << std::endl;
  if (this->BeaconInterval.IsValid())
    std::cout << "\tBINT:   \t" << this->BeaconInterval() << std::endl;
  if (this->DtimPeriod.IsValid())
    std::cout << "\tDTIM:   \t" << this->DtimPeriod() << std::endl;
  if (this->BeaconHead.IsValid())
    printf("\tBHEAD:  \t%p: %zd\n", this->BeaconHead.GetData(), this->BeaconHead.GetLength());
  if (this->BeaconTail.IsValid())
    printf("\tBTAIL:  \t%p: %zd\n", this->BeaconTail.GetData(), this->BeaconTail.GetLength());
  if (this->ProbeResp.IsValid())
    printf("\tPROBE:  \t%p: %zd\n", this->ProbeResp.GetData(), this->ProbeResp.GetLength());
  std::cout << "##################################################" << std::endl;
}

int
StartApCommand::ack_cb(struct nl_msg* msg_, void* arg_)
{
  this->Display();
  this->_status = true;
  this->_count.Post();
  return (NL_OK);
}

int
StartApCommand::err_cb(struct sockaddr_nl* nla, struct nlmsgerr* nlerr, void* arg)
{
  ZLOG_ERR("Error executing StartApCommand");
  ZLOG_ERR("Error: (" + ZLOG_INT(nlerr->error) + ") " + __errstr(nlerr->error));
  this->_status = false;
  this->_count.Post();
  return(NL_SKIP);
}

}
