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

#ifndef __NL80211_FRAMEEVENT_H__
#define __NL80211_FRAMEEVENT_H__

// libc includes
#include <linux/nl80211.h>
#include <linux/netlink.h>
#include <netlink/netlink.h>
#include <netlink/msg.h>
#include <netlink/attr.h>

// libc++ includes
#include <list>

// libzutils includes
#include <zutils/zThread.h>
using namespace zUtils;
#include <zutils/netlink/Attribute.h>
#include <zutils/netlink/Callback.h>
#include <zutils/netlink/Command.h>
#include <zutils/netlink/Message.h>
using namespace netlink;
#include <zutils/nl80211/Socket.h>
#include <zutils/nl80211/IfIndexAttribute.h>
#include <zutils/nl80211/IfNameAttribute.h>
#include <zutils/nl80211/FrameTypeAttribute.h>
#include <zutils/nl80211/FrameMatchAttribute.h>

// local includes

namespace nl80211
{

//*****************************************************************************
// Class: FrameEvent
//*****************************************************************************

class FrameEvent :
    public zThread::ThreadFunction,
    public zThread::ThreadArg,
    public netlink::Command,
    public netlink::Callback
{

public:

  IfIndexAttribute IfIndex;
  IfNameAttribute IfName;
  FrameTypeAttribute FrameType;
  FrameMatchAttribute FrameMatch;

  FrameEvent(const int ifindex_, Callback& cb_);

  virtual
  ~FrameEvent();

  int
  GetSockFd() const;

  bool
  RecvMsg();

  bool
  Listen();

  virtual bool
  Exec();

  virtual void
  Display(const std::string& prefix_ = "") const;

protected:

  virtual void
  Run(zThread::ThreadArg *arg_);

  virtual int
  valid_cb(struct nl_msg* msg_, void* arg_);

  virtual int
  ack_cb(struct nl_msg* msg_, void* arg_);

  virtual int
  err_cb(struct sockaddr_nl* nla_, struct nlmsgerr* nlerr_, void* arg_);

private:

  nl80211::Socket _sock;
  zThread::Thread _thread;
  netlink::Callback& _cb;

};

}

#endif /* __NL80211_FRAMEEVENT_H__ */
