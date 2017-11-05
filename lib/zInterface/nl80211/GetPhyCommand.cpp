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
#include <stdlib.h>
#include <net/if.h>
#include <linux/nl80211.h>
#include <netlink/netlink.h>
#include <netlink/msg.h>
#include <netlink/attr.h>
#include <netlink/genl/genl.h>
#include <netlink/genl/ctrl.h>

// libc++ includes
#include <iostream>
#include <map>

// libzutils includes
#include <zutils/zLog.h>
using namespace zUtils;

// local includes
#include "Command.h"
#include "Attribute.h"

#include "Message.h"
#include "Handler.h"
#include "Socket.h"
#include "GenericMessage.h"
#include "GenericSocket.h"
using namespace netlink;

#include "PhyIndexAttribute.h"
#include "PhyNameAttribute.h"
#include "GetPhyCommand.h"

namespace nl80211
{

static std::string
__errstr(int code)
{
  return(std::string(nl_geterror(code)));
}

//*****************************************************************************
// Class: GetPhyCommand
//*****************************************************************************

GetPhyCommand::GetPhyCommand(int index_)
{
  this->PhyIndex.SetValue(index_);
}

GetPhyCommand::GetPhyCommand(const std::string& name_)
{
  this->PhyName.SetValue(name_);
}

GetPhyCommand::~GetPhyCommand()
{
}

void
GetPhyCommand::Display() const
{
  std::cout << "Phy:" << std::endl;
  std::cout << "\tIndex: \t" << this->PhyIndex.GetValue() << std::endl;
  std::cout << "\tName:  \t" << this->PhyName.GetValue() << std::endl;
}

bool
GetPhyCommand::Exec()
{
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

  GenericMessage cmdmsg(this->_sock.Family(), 0, NL80211_CMD_GET_WIPHY);
  cmdmsg.PutAttribute(&this->PhyIndex);
  this->_sock.SendMsg(cmdmsg);
  this->_sock.RecvMsg();
  this->_sock.Disconnect();

  return(true);

}

int
GetPhyCommand::valid_cb(struct nl_msg* msg_, void* arg_)
{

  GenericMessage msg(msg_);
  if (!msg.Parse())
  {
    ZLOG_ERR("Error parsing generic message");
    return(NL_SKIP);
  }

  if (!msg.GetAttribute(&this->PhyIndex))
  {
    ZLOG_ERR("Missing attribute: " + zLog::IntStr(this->PhyIndex.Id()));
    return(NL_SKIP);
  }

  if (!msg.GetAttribute(&this->PhyName))
  {
    ZLOG_ERR("Missing attribute: " + zLog::IntStr(this->PhyName.Id()));
    return(NL_SKIP);
  }

//  for (int i = 0; i < NL80211_ATTR_MAX; i++)
//  {
//    if (!tb[i])
//      continue;
//    switch (i)
//    {
//    case NL80211_ATTR_WIPHY:
//      this->_phyindex = nla_get_u32(tb[i]);
//      break;
//    case NL80211_ATTR_WIPHY_NAME:
//      this->_phyname = std::string(nla_get_string(tb[i]));
//      break;
//    case NL80211_ATTR_WIPHY_BANDS:
//      // TODO
//      break;
//    case NL80211_ATTR_SUPPORTED_IFTYPES:
//      // TODO
//      break;
//    case NL80211_ATTR_MAX_NUM_SCAN_SSIDS:
//      // TODO
//      break;
//    case NL80211_ATTR_GENERATION:
//      // TODO
//      break;
//    case NL80211_ATTR_SUPPORTED_COMMANDS:
//      // TODO
//      break;
//    case NL80211_ATTR_MAX_SCAN_IE_LEN:
//      // TODO
//      break;
//    case NL80211_ATTR_CIPHER_SUITES:
//      // TODO
//      break;
//    case NL80211_ATTR_WIPHY_RETRY_SHORT:
//      // TODO
//      break;
//    case NL80211_ATTR_WIPHY_RETRY_LONG:
//      // TODO
//      break;
//    case NL80211_ATTR_WIPHY_FRAG_THRESHOLD:
//      // TODO
//      break;
//    case NL80211_ATTR_WIPHY_RTS_THRESHOLD:
//      // TODO
//      break;
//    case NL80211_ATTR_MAX_NUM_PMKIDS:
//      // TODO
//      break;
//    case NL80211_ATTR_WIPHY_COVERAGE_CLASS:
//      // TODO
//      break;
//    case NL80211_ATTR_TX_FRAME_TYPES:
//      // TODO
//      break;
//    case NL80211_ATTR_RX_FRAME_TYPES:
//      // TODO
//      break;
//    case NL80211_ATTR_CONTROL_PORT_ETHERTYPE:
//      // TODO
//      break;
//    case NL80211_ATTR_SUPPORT_IBSS_RSN:
//      // TODO
//      break;
//    case NL80211_ATTR_WIPHY_ANTENNA_TX:
//      // TODO
//      break;
//    case NL80211_ATTR_WIPHY_ANTENNA_RX:
//      // TODO
//      break;
//    case NL80211_ATTR_OFFCHANNEL_TX_OK:
//      // TODO
//      break;
//    case NL80211_ATTR_MAX_REMAIN_ON_CHANNEL_DURATION:
//      // TODO
//      break;
//    case NL80211_ATTR_WIPHY_ANTENNA_AVAIL_TX:
//      // TODO
//      break;
//    case NL80211_ATTR_WIPHY_ANTENNA_AVAIL_RX:
//      // TODO
//      break;
//    case NL80211_ATTR_SUPPORT_MESH_AUTH:
//      // TODO
//      break;
//    case NL80211_ATTR_INTERFACE_COMBINATIONS:
//      // TODO
//      break;
//    case NL80211_ATTR_SOFTWARE_IFTYPES:
//      // TODO
//      break;
//    case NL80211_ATTR_MAX_NUM_SCHED_SCAN_SSIDS:
//      // TODO
//      break;
//    case NL80211_ATTR_MAX_SCHED_SCAN_IE_LEN:
//      // TODO
//      break;
//    case NL80211_ATTR_SUPPORT_AP_UAPSD:
//      // TODO
//      break;
//    case NL80211_ATTR_MAX_MATCH_SETS:
//      // TODO
//      break;
//    case NL80211_ATTR_TDLS_SUPPORT:
//      // TODO
//      break;
//    case NL80211_ATTR_TDLS_EXTERNAL_SETUP:
//      // TODO
//      break;
//    case NL80211_ATTR_FEATURE_FLAGS:
//      // TODO
//      break;
//    case NL80211_ATTR_HT_CAPABILITY_MASK:
//      // TODO
//      break;
//    case NL80211_ATTR_MAX_NUM_SCHED_SCAN_PLANS:
//      // TODO
//      break;
//    case NL80211_ATTR_MAX_SCAN_PLAN_INTERVAL:
//      // TODO
//      break;
//    case NL80211_ATTR_MAX_SCAN_PLAN_ITERATIONS:
//      // TODO
//      break;
//    default:
//      ZLOG_INFO("Unhandled attribute: " + zLog::IntStr(i));
//      break;
//    }
//  }

  return (NL_OK);
}

int
GetPhyCommand::err_cb(struct sockaddr_nl* nla_, struct nlmsgerr* nlerr_, void* arg_)
{
  ZLOG_ERR("Error executing GetPhyCommand");
  ZLOG_ERR("Error: (" + ZLOG_INT(nlerr_->error) + ") " + __errstr(nlerr_->error));
  return(NL_SKIP);
}


}
