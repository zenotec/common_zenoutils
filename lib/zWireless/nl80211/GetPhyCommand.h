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

// local includes
#include "Command.h"
#include "Attribute.h"

#include "Message.h"
#include "Handler.h"
#include "Socket.h"
#include "GenericMessage.h"
#include "GenericSocket.h"
using namespace netlink;

#include "IfIndexAttribute.h"
#include "IfNameAttribute.h"
#include "PhyIndexAttribute.h"
#include "PhyNameAttribute.h"
#include "FrequencyAttribute.h"
#include "ChannelTypeAttribute.h"
#include "ChannelWidthAttribute.h"
#include "TxPowerModeAttribute.h"
#include "TxPowerLevelAttribute.h"

namespace nl80211
{

//*****************************************************************************
// Class: GetPhyCommand
//*****************************************************************************

class GetPhyCommand : public netlink::Command, public netlink::Handler
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

  netlink::GenericSocket _sock;

};

}

#endif /* __NL80211_GETPHYCOMMAND_H__ */
