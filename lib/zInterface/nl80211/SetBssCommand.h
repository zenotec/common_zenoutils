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

#ifndef __NL80211_SETBSSCOMMAND_H__
#define __NL80211_SETBSSCOMMAND_H__

// libc includes
#include <linux/netlink.h>
#include <netlink/netlink.h>
#include <netlink/msg.h>

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
#include "SsidAttribute.h"
#include "BeaconIntervalAttribute.h"
#include "DtimPeriodAttribute.h"
#include "BeaconHeadAttribute.h"
#include "BeaconTailAttribute.h"
using namespace nl80211;

namespace nl80211
{

//*****************************************************************************
// Class: SetBssCommand
//*****************************************************************************

class SetBssCommand : public Command, public netlink::Handler
{

public:

  IfIndexAttribute IfIndex;
  IfNameAttribute IfName;
  SsidAttribute Ssid;
  BeaconIntervalAttribute BeaconInterval;
  DtimPeriodAttribute DtimPeriod;
  BeaconHeadAttribute BeaconHead;
  BeaconTailAttribute BeaconTail;

  SetBssCommand(int index_ = 0);

  SetBssCommand(const std::string& name_);

  virtual
  ~SetBssCommand();

  virtual bool
  Exec();

  void
  Display() const;

protected:

  virtual int
  valid_cb(struct nl_msg* msg, void* arg);

  virtual int
  err_cb(struct sockaddr_nl* nla, struct nlmsgerr* nlerr, void* arg);

private:

  netlink::GenericSocket _sock;

};

}

#endif /* __NL80211_SETBSSCOMMAND_H__ */
