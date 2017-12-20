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
#include <iostream>

// libzutils includes
#include <zutils/zLog.h>
using namespace zUtils;

// local includes
#include "Handler.h"

namespace netlink
{

static std::string
__errstr(int code)
{
  return(std::string(nl_geterror(code)));
}

//*****************************************************************************
// Class: Handler
//*****************************************************************************

Handler::Handler(void* arg_) :
    _arg(arg_), _msgcnt(0)
{
}

Handler::~Handler()
{
}

int
Handler::Count() const
{
  return(this->_msgcnt);
}

int
Handler::ValidCallback(struct nl_msg* msg, void* arg)
{
  Handler* self = (Handler*)arg;
  self->_msgcnt++;
  return(self->valid_cb(msg, self->_arg));
}

int
Handler::FinishCallback(struct nl_msg* msg, void* arg)
{
  Handler* self = (Handler*)arg;
  return(self->finish_cb(msg, self->_arg));
}

int
Handler::AckCallback(struct nl_msg* msg, void* arg)
{
  Handler* self = (Handler*)arg;
  return(self->ack_cb(msg, self->_arg));
}

int
Handler::ErrorCallback(struct sockaddr_nl* nla, struct nlmsgerr* nlerr, void* arg)
{
  Handler* self = (Handler*)arg;
  return(self->err_cb(nla, nlerr, self->_arg));
}

int
Handler::valid_cb(struct nl_msg* msg, void* arg)
{
  this->_msgcnt = 0;
  ZLOG_WARN("Unhandled valid callback");
  return(NL_SKIP);
}

int
Handler::finish_cb(struct nl_msg* msg, void* arg)
{
  this->_msgcnt = 0;
  ZLOG_WARN("Unhandled finish callback");
  return(NL_SKIP);
}

int
Handler::ack_cb(struct nl_msg* msg, void* arg)
{
  this->_msgcnt = 0;
  ZLOG_WARN("Unhandled ack callback");
  return(NL_SKIP);
}

}

