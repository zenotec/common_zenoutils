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
#include <netlink/genl/genl.h>
#include <netlink/genl/ctrl.h>
#include <linux/nl80211.h>

// libc++ includes

#include <string>
#include <iostream>

// libzutils includes

#include <zutils/zCompatibility.h>
#include <zutils/zLog.h>
#include <zutils/netlink/Attribute.h>
#include <zutils/netlink/Callback.h>
#include <zutils/netlink/Message.h>
#include <zutils/netlink/Socket.h>
#include <zutils/netlink/GenericMessage.h>
#include <zutils/netlink/GenericSocket.h>
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

//*****************************************************************************
// Class: GenericSocket
//*****************************************************************************

GenericSocket::GenericSocket() :
    _fid(0)
{
}

GenericSocket::~GenericSocket()
{
}

bool
GenericSocket::Connect(const std::string& family_)
{

  if (!Socket::Connect(NETLINK_GENERIC))
  {
    ZLOG_ERR("Error connecting generic netlink socket");
    this->Disconnect();
    return (false);
  }

  this->_fid = genl_ctrl_resolve((struct nl_sock*)this->operator ()(), family_.c_str());
  if (this->_fid < 0)
  {
    ZLOG_ERR("Error resolving generic netlink family name: " + family_);
    ZLOG_ERR("Error: (" + ZLOG_INT(this->_fid) + ") " + __errstr(this->_fid));
  }

  return (this->_fid > 0);
}

SHPTR(GenericMessage)
GenericSocket::CreateMsg()
{
  SHPTR(GenericMessage) msg;

  if (this->_fid > 0)
  {
    msg = SHPTR(GenericMessage)(new GenericMessage(this->_fid));
  }

  return (msg);
}

}

