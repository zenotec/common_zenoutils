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

#ifndef __NETLINK_ROUTELINKEVENT_H__
#define __NETLINK_ROUTELINKEVENT_H__

// libc includes
#include <netlink/route/link.h>

// libc++ includes
#include <iostream>

// libzutils includes
#include <zutils/zThread.h>
#include <zutils/zEvent.h>
using namespace zUtils;

// local includes
#include "RouteLink.h"
#include "RouteSocket.h"

namespace netlink
{


//*****************************************************************************
// Class: RouteLinkEvent
//*****************************************************************************

class RouteLinkEvent : public Handler,
    public zEvent::Event,
    public zThread::ThreadFunction,
    public zThread::ThreadArg
{

public:

  enum EVENT
  {
    EVENT_ERR = -1,
    EVENT_NONE = 0,
    EVENT_UPDOWN = 1,
    EVENT_LAST
  };

  RouteLinkEvent(const int index_ = 0);

  RouteLinkEvent(const std::string& name_);

  virtual
  ~RouteLinkEvent();

  int
  GetIfIndex() const;

  bool
  SetIfIndex(const int index_);

  std::string
  GetIfName() const;

  bool
  SetIfName(const std::string& name_);

  bool
  Start();

  bool
  Stop();

protected:

  virtual void
  Run(zThread::ThreadArg *arg_);

  virtual int
  valid_cb(struct nl_msg* msg_, void* arg_);

  virtual int
  err_cb(struct sockaddr_nl* nla_, struct nlmsgerr* nlerr_, void* arg_);

private:

  RouteSocket _sock;
  zThread::Thread _thread;

  int _ifindex;
  std::string _ifname;

};

//*****************************************************************************
// Class: RouteLinkNotification
//*****************************************************************************

class RouteLinkNotification : public zEvent::EventNotification
{

public:

  RouteLink Link;

  RouteLinkNotification(RouteLinkEvent::EVENT id_, uint32_t index_);

  virtual
  ~RouteLinkNotification();

  RouteLinkEvent::EVENT
  Id() const;

protected:

private:

  RouteLinkEvent::EVENT _id;

};

}

#endif /* __NETLINK_ROUTELINKEVENT_H__ */
