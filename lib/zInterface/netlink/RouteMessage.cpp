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
#include <string.h>
#include <unistd.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netlink/netlink.h>
#include <netlink/attr.h>
#include <netlink/msg.h>
#include <netlink/route/rtnl.h>
#include <linux/rtnetlink.h>
#include <net/if.h>
#include <linux/if_link.h>

// libc++ includes
#include <iostream>

// libzutils includes
#include <zutils/zLog.h>
#include <zutils/netlink/Attribute.h>
#include <zutils/netlink/Message.h>
#include <zutils/netlink/RouteMessage.h>
using namespace zUtils;

// local includes

ZLOG_MODULE_INIT(zUtils::zLog::Log::MODULE_INTERFACE);

namespace netlink
{

static std::string
__errstr(int code)
{
  return(std::string(nl_geterror(code)));
}

static const std::string
_attr2str(const int attr_)
{
  std::string str = "[" + zLog::IntStr(attr_) + "] ";
  switch(attr_)
  {
  case IFLA_UNSPEC:
    str += "Unspecified";
    break;
  case IFLA_ADDRESS:
    str += "Address";
    break;
  case IFLA_BROADCAST:
    str += "Broadcast";
    break;
  case IFLA_IFNAME:
    str += "Interface Name";
    break;
  case IFLA_MTU:
    str += "MTU";
    break;
//  IFLA_LINK,
  case IFLA_QDISC:
    str += "QDisc";
    break;
  case IFLA_STATS:
    str += "Statistics";
    break;
//  IFLA_COST,
//#define IFLA_COST IFLA_COST
//  IFLA_PRIORITY,
//#define IFLA_PRIORITY IFLA_PRIORITY
//  IFLA_MASTER,
//#define IFLA_MASTER IFLA_MASTER
//  IFLA_WIRELESS,      /* Wireless Extension event - see wireless.h */
//#define IFLA_WIRELESS IFLA_WIRELESS
//  IFLA_PROTINFO,      /* Protocol specific information for a link */
//#define IFLA_PROTINFO IFLA_PROTINFO
  case IFLA_TXQLEN:
    str += "TX Queue Length";
    break;
//#define IFLA_TXQLEN IFLA_TXQLEN
  case IFLA_MAP:
    str += "Map";
    break;
//#define IFLA_MAP IFLA_MAP
//  IFLA_WEIGHT,
//#define IFLA_WEIGHT IFLA_WEIGHT
  case IFLA_OPERSTATE:
    str += "Operational State";
    break;
  case IFLA_LINKMODE:
    str += "Link Mode";
    break;
//  IFLA_LINKINFO,
//#define IFLA_LINKINFO IFLA_LINKINFO
//  IFLA_NET_NS_PID,
//  IFLA_IFALIAS,
//  IFLA_NUM_VF,        /* Number of VFs if device is SR-IOV PF */
//  IFLA_VFINFO_LIST,
//  IFLA_STATS64,
//  IFLA_VF_PORTS,
//  IFLA_PORT_SELF,
//  IFLA_AF_SPEC,
//  IFLA_GROUP,     /* Group the device belongs to */
//  IFLA_NET_NS_FD,
//  IFLA_EXT_MASK,      /* Extended info mask, VFs, etc */
//  IFLA_PROMISCUITY,   /* Promiscuity count: > 0 means acts PROMISC */
//#define IFLA_PROMISCUITY IFLA_PROMISCUITY
//  IFLA_NUM_TX_QUEUES,
//  IFLA_NUM_RX_QUEUES,
//  IFLA_CARRIER,
//  IFLA_PHYS_PORT_ID,
//  IFLA_CARRIER_CHANGES,
//  IFLA_PHYS_SWITCH_ID,
//  IFLA_LINK_NETNSID,
//  IFLA_PHYS_PORT_NAME,
//  IFLA_PROTO_DOWN,

    default:
    str += "Unknown";
    break;
  }
  return(str);
};

//*****************************************************************************
// Class: RouteMessage
//*****************************************************************************

RouteMessage::RouteMessage(const int family_) :
    _family(family_), _flags(0), _command(0)
{
}

RouteMessage::~RouteMessage()
{
}

bool
RouteMessage::Assemble(struct nl_msg* msg_)
{
  return (false);
}

bool
RouteMessage::Disassemble(struct nl_msg* msg_)
{
  struct ifinfomsg* rtnlhdr = (struct ifinfomsg *) nlmsg_data(nlmsg_hdr(msg_));
  struct nlattr* attr = nlmsg_attrdata(nlmsg_hdr(msg_), sizeof(struct ifinfomsg));
  int len = nlmsg_attrlen(nlmsg_hdr(msg_), sizeof(struct ifinfomsg));

  //    family = rtnlhdr->ifi_family;
  //    type = rtnlhdr->ifi_type;
  //    index = rtnlhdr->ifi_index;
  //    flags = rtnlhdr->ifi_flags;
  //    flags = rtnlhdr->ifi_change;

  return(this->_attrs.Disassemble(attr, len));
}

int
RouteMessage::GetFamily() const
{
  return (this->_family);
}

int
RouteMessage::GetFlags() const
{
  return (this->_flags);
}

bool
RouteMessage::SetFlags(const int flags_)
{
  this->_flags = flags_;
  return (true);
}

int
RouteMessage::GetCommand() const
{
  return (this->_command);
}

bool
RouteMessage::SetCommand(const int command_)
{
  this->_command = command_;
  return (true);
}

uint8_t
RouteMessage::AddressFamily() const
{
  uint8_t family = 0;
//  struct ifinfomsg* rtnlhdr = (struct ifinfomsg *) this->Data();
//  if (rtnlhdr)
//  {
//    family = rtnlhdr->ifi_family;
//  }
  return(family);
}

uint16_t
RouteMessage::LinkLayerType() const
{
  uint16_t type = 0;
//  struct ifinfomsg* rtnlhdr = (struct ifinfomsg *) this->Data();
//  if (rtnlhdr)
//  {
//    type = rtnlhdr->ifi_type;
//  }
  return(type);
}

uint32_t
RouteMessage::LinkIndex() const
{
  uint32_t index = 0;
//  struct ifinfomsg* rtnlhdr = (struct ifinfomsg *) this->Data();
//  if (rtnlhdr)
//  {
//    index = rtnlhdr->ifi_index;
//  }
  return(index);
}

uint32_t
RouteMessage::Flags() const
{
  uint32_t flags = 0;
//  struct ifinfomsg* rtnlhdr = (struct ifinfomsg *) this->Data();
//  if (rtnlhdr)
//  {
//    flags = rtnlhdr->ifi_flags;
//  }
  return(flags);
}

uint32_t
RouteMessage::FlagsMask() const
{
  uint32_t flags = 0;
//  struct ifinfomsg* rtnlhdr = (struct ifinfomsg *) this->Data();
//  if (rtnlhdr)
//  {
//    flags = rtnlhdr->ifi_change;
//  }
  return(flags);
}

//bool
//RouteMessage::Parse()
//{
//
//  int ret = 0;
//
//  switch (this->Type())
//  {
//  case RTM_NEWLINK:
//    // no break
//  case RTM_DELLINK:
//    // no break
//  case RTM_GETLINK:
//    // no break
//  case RTM_SETLINK:
//    // no break
//  {
//    struct ifinfomsg* rtnlhdr = (struct ifinfomsg *) this->Data();
//    struct nlattr* attr = nlmsg_attrdata(this->Header(), sizeof(struct ifinfomsg));
//    int len = nlmsg_attrlen(this->Header(), sizeof(struct ifinfomsg));
//
//    ret = nla_parse(this->_attrs, RTA_MAX, attr, len, NULL);
//    if (ret < 0)
//    {
//      ZLOG_ERR("Error parsing route link message attributes");
//      ZLOG_ERR("Error: (" + ZLOG_INT(ret) + ") " + __errstr(ret));
//      return (false);
//    }
//    return (true);
//  }
//  default:
//  {
//    std::cout << "RouteMessage::Parse(): UNKNOWN" << std::endl;
//    return (false);
//  }
//  }
//
//}

void
RouteMessage::DisplayAttributes() const
{
//  for(int i = 0; i < RTA_MAX; i++)
//  {
//    if (this->_attrs[i])
//    {
//      std::cout << _attr2str(i) << std::endl;
//    }
//  }
  std::cout << std::endl;
}

}

