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

#include <errno.h>
#include <netlink/route/link.h>

// libc++ includes

#include <string>
#include <iostream>
#include <map>

// libzutils includes

#include <zutils/zLog.h>
#include <zutils/netlink/Attribute.h>
#include <zutils/netlink/Callback.h>
#include <zutils/netlink/Command.h>
#include <zutils/netlink/Message.h>
#include <zutils/netlink/Socket.h>
#include <zutils/netlink/RouteSocket.h>
#include <zutils/netlink/ListLinksCommand.h>
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
// Class: ListLinksCommand
//*****************************************************************************

ListLinksCommand::ListLinksCommand() :
    Command(0)
{
}

ListLinksCommand::~ListLinksCommand()
{
}

bool
ListLinksCommand::Exec()
{
  int ret = 0;
  struct nl_cache* cache = NULL;
  struct rtnl_link* link = NULL;

  if (!this->_sock.Connect())
  {
    ZLOG_ERR("Error connecting Netlink socket");
    return(false);
  }

  ret = rtnl_link_alloc_cache(this->_sock(), AF_UNSPEC, &cache);
  if (ret < 0)
  {
    ZLOG_ERR("Error allocating link cache");
    ZLOG_ERR("Error: (" + ZLOG_INT(ret) + ") " + __errstr(ret));
    return(false);
  }

  for (int i = 1; i < 50; i++)
  {
    link = rtnl_link_get(cache, i);
    if (link != NULL)
    {
      RouteLink rlink(link);
      this->LinkMap[i] = rlink;
    }
  }

  nl_cache_put(cache);
  this->_sock.Disconnect();

  return(true);
}

void
ListLinksCommand::Display(const std::string& prefix_) const
{
  FOREACH(auto& link, this->LinkMap)
  {
    link.second.Display();
  }
}
}
