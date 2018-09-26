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
#include <zutils/netlink/Callback.h>

// libc++ includes
#include <iostream>

// libzutils includes
#include <zutils/zLog.h>
using namespace zUtils;

// local includes

ZLOG_MODULE_INIT(zUtils::zLog::Log::MODULE_INTERFACE);

namespace netlink
{

//*****************************************************************************
// Class: Handler
//*****************************************************************************

Callback::Callback(void* arg_) :
    _arg(arg_)
{
}

Callback::~Callback()
{
}

int
Callback::SequenceCheckCallback(struct nl_msg* msg_, void* arg_)
{
  int ret = NL_SKIP;
  Callback* self = (Callback*)arg_;
  if (self)
  {
    ret = self->seqchk_cb(msg_, self->_arg);
  }
  return(ret);
}

int
Callback::ValidCallback(struct nl_msg* msg_, void* arg_)
{
  int ret = NL_SKIP;
  Callback* self = (Callback*)arg_;
  if (self)
  {
    ret = self->valid_cb(msg_, self->_arg);
  }
  else
  {
    ZLOG_ERR("Invalid \"this\" pointer");
  }
  return(ret);
}

int
Callback::FinishCallback(struct nl_msg* msg_, void* arg_)
{
  int ret = NL_SKIP;
  Callback* self = (Callback*)arg_;
  if (self)
  {
    ret = self->finish_cb(msg_, self->_arg);
  }
  return(ret);
}

int
Callback::AckCallback(struct nl_msg* msg_, void* arg_)
{
  int ret = NL_SKIP;
  Callback* self = (Callback*)arg_;
  if (self)
  {
    ret = self->ack_cb(msg_, self->_arg);
  }
  return(ret);
}

int
Callback::ErrorCallback(struct sockaddr_nl* nla_, struct nlmsgerr* nlerr_, void* arg_)
{
  int ret = NL_SKIP;
  Callback* self = (Callback*)arg_;
  if (self)
  {
    ret = self->err_cb(nla_, nlerr_, self->_arg);
  }
  return(ret);
}

int
Callback::seqchk_cb(struct nl_msg* msg_, void* arg_)
{
  return(NL_OK);
}

int
Callback::valid_cb(struct nl_msg* msg, void* arg)
{
  ZLOG_WARN("Unhandled valid callback");
  return(NL_SKIP);
}

int
Callback::finish_cb(struct nl_msg* msg, void* arg)
{
  ZLOG_WARN("Unhandled finish callback");
  return(NL_SKIP);
}

int
Callback::ack_cb(struct nl_msg* msg, void* arg)
{
  ZLOG_WARN("Unhandled ack callback");
  return(NL_SKIP);
}

}
