/*
 * Copyright (c) 2018 Cable Television Laboratories, Inc. ("CableLabs")
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
//#include <stdlib.h>
//#include <string.h>
//#include <unistd.h>
//#include <sys/socket.h>
//#include <netinet/in.h>
//#include <netlink/netlink.h>
//#include <netlink/genl/genl.h>
//#include <netlink/genl/ctrl.h>
//#include <linux/nl80211.h>

// libc++ includes

//#include <string>
//#include <iostream>

// libzutils includes

#include <zutils/zCompatibility.h>
#include <zutils/zLog.h>
using namespace zUtils;
#include <zutils/netlink/Attribute.h>
#include <zutils/netlink/Message.h>
#include <zutils/netlink/Handler.h>
#include <zutils/netlink/GenericSocket.h>
#include <zutils/netlink/GenericMessage.h>
#include <zutils/nl80211/Socket.h>
using namespace netlink;

// local includes

ZLOG_MODULE_INIT(zUtils::zLog::Log::MODULE_INTERFACE);

namespace nl80211
{

//*****************************************************************************
// Class: Socket
//*****************************************************************************

Socket::Socket()
{
}

Socket::~Socket()
{
}

SHARED_PTR(netlink::GenericMessage)
Socket::CreateMsg()
{
  return (netlink::GenericSocket::CreateMsg(std::string(NL80211_GENL_NAME)));
}

}

