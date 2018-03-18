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

#ifndef __NL80211_GETPHYCOMMAND_H__
#define __NL80211_GETPHYCOMMAND_H__

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
#include <zutils/nl80211/MacAttribute.h>
#include <zutils/nl80211/SsidAttribute.h>
#include <zutils/nl80211/FrequencyAttribute.h>
#include <zutils/nl80211/ChannelTypeAttribute.h>
#include <zutils/nl80211/ChannelWidthAttribute.h>
#include <zutils/nl80211/TxPowerModeAttribute.h>
#include <zutils/nl80211/TxPowerLevelAttribute.h>

namespace nl80211
{

//*****************************************************************************
// Class: GetPhyCommand
//*****************************************************************************

class GetPhyCommand :
    public netlink::Command,
    public netlink::Handler
{

public:

  IfIndexAttribute IfIndex;
  IfNameAttribute IfName;
  PhyIndexAttribute PhyIndex;
  PhyNameAttribute PhyName;
  FrequencyAttribute Frequency;
  ChannelTypeAttribute ChannelType;
  ChannelWidthAttribute ChannelWidth;
  TxPowerModeAttribute TxPowerMode;
  TxPowerLevelAttribute TxPowerLevel;

  GetPhyCommand(int ifindex_);

  GetPhyCommand(const std::string& ifname_);

  virtual
  ~GetPhyCommand();

  virtual bool
  Exec();

  void
  Display() const;

protected:

  virtual int
  valid_cb(struct nl_msg* msg_, void* arg_);

  virtual int
  err_cb(struct sockaddr_nl* nla_, struct nlmsgerr* nlerr_, void* arg_);

private:

  nl80211::Socket _sock;

};

}

#endif /* __NL80211_GETPHYCOMMAND_H__ */
