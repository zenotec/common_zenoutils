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

#ifndef __NL80211_SETPHYCOMMAND_H__
#define __NL80211_SETPHYCOMMAND_H__

// libc includes
#include <linux/nl80211.h>
#include <linux/netlink.h>
#include <netlink/netlink.h>
#include <netlink/msg.h>
#include <netlink/attr.h>

// libc++ includes
#include <string>

// libzutils includes
#include <zutils/netlink/Attribute.h>
#include <zutils/netlink/Command.h>
#include <zutils/netlink/Message.h>
#include <zutils/netlink/Handler.h>
#include <zutils/netlink/Socket.h>
#include <zutils/netlink/GenericMessage.h>
#include <zutils/netlink/GenericSocket.h>
using namespace netlink;
#include <zutils/nl80211/Socket.h>
#include <zutils/nl80211/PhyIndexAttribute.h>
#include <zutils/nl80211/PhyNameAttribute.h>
#include <zutils/nl80211/IfIndexAttribute.h>
#include <zutils/nl80211/IfNameAttribute.h>
#include <zutils/nl80211/FrequencyAttribute.h>
#include <zutils/nl80211/ChannelTypeAttribute.h>
#include <zutils/nl80211/ChannelWidthAttribute.h>
#include <zutils/nl80211/TxPowerModeAttribute.h>
#include <zutils/nl80211/TxPowerLevelAttribute.h>

#include <zutils/nl80211/CenterFrequency1Attribute.h>
#include <zutils/nl80211/CenterFrequency2Attribute.h>

namespace nl80211
{

//*****************************************************************************
// Class: SetPhyCommand
//*****************************************************************************

class SetPhyCommand :
    public netlink::Command,
    public netlink::Handler
{

public:

  IfIndexAttribute IfIndex;
  IfNameAttribute IfName;
  PhyIndexAttribute PhyIndex;
  PhyNameAttribute PhyName;
  TxPowerModeAttribute TxPowerMode;
  TxPowerLevelAttribute TxPowerLevel; // only set for non-auto power mode
  FrequencyAttribute Frequency;
  ChannelTypeAttribute ChannelType; // only set for non-HT and HT
  ChannelWidthAttribute ChannelWidth; // only set for VHT
  CenterFrequency1Attribute CenterFrequency1; // only set for VHT 20/40/80/160 MHz
  CenterFrequency1Attribute CenterFrequency2; // only set for VHT 80 plus 80 Mhz


  SetPhyCommand(int ifindex_);

  SetPhyCommand(const std::string& ifname_);

  virtual
  ~SetPhyCommand();

  virtual bool
  Exec();

  virtual void
  Display(const std::string& prefix_ = "") const;

protected:

  virtual int
  ack_cb(struct nl_msg* msg_, void* arg_);

  virtual int
  err_cb(struct sockaddr_nl* nla_, struct nlmsgerr* nlerr_, void* arg_);

private:

  nl80211::Socket _sock;

};

}

#endif /* __NL80211_SETPHYCOMMAND_H__ */
