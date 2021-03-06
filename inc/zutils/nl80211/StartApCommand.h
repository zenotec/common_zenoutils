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

#ifndef __NL80211_STARTAPCOMMAND_H__
#define __NL80211_STARTAPCOMMAND_H__

// libc includes

// libc++ includes
#include <string>

// libzutils includes

// local includes
#include <zutils/netlink/Attribute.h>
#include <zutils/netlink/Callback.h>
#include <zutils/netlink/Command.h>
#include <zutils/netlink/Message.h>
#include <zutils/netlink/Socket.h>
#include <zutils/netlink/GenericMessage.h>
#include <zutils/netlink/GenericSocket.h>
using namespace netlink;
#include <zutils/nl80211/Socket.h>
#include <zutils/nl80211/IfIndexAttribute.h>
#include <zutils/nl80211/IfNameAttribute.h>
#include <zutils/nl80211/SsidAttribute.h>
#include <zutils/nl80211/FrequencyAttribute.h>
#include <zutils/nl80211/CenterFrequency1Attribute.h>
#include <zutils/nl80211/CenterFrequency2Attribute.h>
#include <zutils/nl80211/ChannelWidthAttribute.h>
#include <zutils/nl80211/BeaconIntervalAttribute.h>
#include <zutils/nl80211/DtimPeriodAttribute.h>
#include <zutils/nl80211/BeaconHeadAttribute.h>
#include <zutils/nl80211/BeaconTailAttribute.h>
#include <zutils/nl80211/ProbeResponseAttribute.h>

namespace nl80211
{

//*****************************************************************************
// Class: StartApCommand
//*****************************************************************************

class StartApCommand :
    public netlink::Command,
    public netlink::Callback
{

public:

  IfIndexAttribute IfIndex;
  IfNameAttribute IfName;
  SsidAttribute Ssid;
  ChannelWidthAttribute ChannelWidth;
  FrequencyAttribute Channel;
  CenterFrequency1Attribute CenterFrequency1;
  CenterFrequency2Attribute CenterFrequency2;
  BeaconIntervalAttribute BeaconInterval;
  DtimPeriodAttribute DtimPeriod;
  BeaconHeadAttribute BeaconHead;
  BeaconTailAttribute BeaconTail;
  ProbeResponseAttribute ProbeResp;

  StartApCommand(const unsigned int ifindex_);

  virtual
  ~StartApCommand();

  virtual bool
  Exec();

  virtual void
  Display(const std::string& prefix_ = "") const;

protected:

  virtual int
  ack_cb(struct nl_msg* msg_, void* arg_);

  virtual int
  err_cb(struct sockaddr_nl* nla, struct nlmsgerr* nlerr, void* arg);

private:

  nl80211::Socket _sock;

};

}

#endif /* __NL80211_STARTAPCOMMAND_H__ */
