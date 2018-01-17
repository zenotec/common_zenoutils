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

// libc++ includes
#include <iostream>
#include <map>

// libzutils includes
#include <zutils/zLog.h>
using namespace zUtils;

// local includes
#include "Attribute.h"
#include "Command.h"

#include "Message.h"
#include "Handler.h"
#include "Socket.h"
#include "RouteSocket.h"
using namespace netlink;

#include "ListLinksCommand.h"

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
      this->Link[i] = rlink;
    }
  }

  nl_cache_put(cache);
  this->_sock.Disconnect();

  return(true);
}

void
ListLinksCommand::Display() const
{
  FOREACH(auto& link, this->Link)
  {
    link.second.Display();
  }
}
}
