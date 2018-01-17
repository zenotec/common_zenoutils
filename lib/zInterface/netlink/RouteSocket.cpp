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

// libc++ includes
#include <iostream>

// libzutils includes
#include <zutils/zLog.h>
using namespace zUtils;

// local includes
#include "Attribute.h"

#include "Message.h"
#include "Handler.h"
#include "Socket.h"
#include "RouteSocket.h"

ZLOG_MODULE_INIT(zUtils::zLog::Log::MODULE_INTERFACE);

namespace netlink
{

//*****************************************************************************
// Class: RouteSocket
//*****************************************************************************

RouteSocket::RouteSocket()
{
}

RouteSocket::~RouteSocket()
{
}

bool
RouteSocket::Connect()
{

  if (!Socket::Connect(NETLINK_ROUTE))
  {
    ZLOG_ERR("Error connecting route netlink socket");
    this->Disconnect();
    return (false);
  }

  return (true);
}


}

