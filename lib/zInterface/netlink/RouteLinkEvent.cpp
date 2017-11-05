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
#include <unistd.h>
#include <poll.h>
#include <sys/socket.h>
#include <linux/if_arp.h>

// libc++ includes
#include <iostream>

// libzutils includes
#include <zutils/zLog.h>
using namespace zUtils;

// local includes
#include "GetLinkCommand.h"
#include "RouteMessage.h"
#include "RouteLinkEvent.h"

namespace netlink
{

static std::string
__errstr(int code)
{
  return(std::string(nl_geterror(code)));
}

//*****************************************************************************
// Class: RouteLinkEvent
//*****************************************************************************

RouteLinkEvent::RouteLinkEvent() :
    zEvent::Event(zEvent::Event::TYPE_LAST), _thread(this, this)
{
  int ret = 0;

  if (!this->_sock.Connect())
  {
    ZLOG_ERR("Error connecting Netlink socket");
    return;
  }

  if (!this->_sock.SetHandler(this))
  {
    ZLOG_ERR("Error setting up message handlers");
    return;
  }

  nl_socket_set_nonblocking(this->_sock());
  nl_socket_disable_seq_check(this->_sock());

  ret = nl_socket_add_membership(this->_sock(), RTNLGRP_LINK);
  if (ret < 0)
  {
    ZLOG_ERR("Error joining RTNLGRP_LINK multicast group");
    ZLOG_ERR("Error: (" + ZLOG_INT(ret) + ") " + __errstr(ret));
    return;
  }

  if (!this->_thread.Start())
  {
    ZLOG_ERR("Error starting receiving thread");
  }

}

RouteLinkEvent::~RouteLinkEvent()
{
  this->_thread.Stop();
  nl_socket_drop_membership(this->_sock(), RTNLGRP_LINK);
  this->_sock.Disconnect();
}

void
RouteLinkEvent::Run(zThread::ThreadArg *arg_)
{
  // Setup for poll loop
  struct pollfd fds[1];
  fds[0].fd = nl_socket_get_fd(this->_sock());
  fds[0].events = (POLLIN | POLLERR);

  while (!this->Exit())
  {
    // Poll for received data
    int ret = poll(fds, 1, 100);
    if (ret > 0 && (fds[0].revents == POLLIN))
    {
      nl_recvmsgs_default(this->_sock());
    }
  }
}

int
RouteLinkEvent::valid_cb(struct nl_msg* msg_, void* arg_)
{
  std::cout << "RouteLinkEvent::valid_cb()" << std::endl;

  RouteMessage msg(msg_);
  if (!msg.Parse())
  {
    ZLOG_ERR("Error parsing route link message");
    return (NL_SKIP);
  }
//  msg.Display();
//  msg.DisplayAttributes();

  RouteLinkNotification* n = new RouteLinkNotification(RouteLinkEvent::EVENT_UPDOWN, msg.LinkIndex());
  n->Link.Display();
  this->Notify(n);
  delete(n);

  return(NL_OK);
}

int
RouteLinkEvent::err_cb(struct sockaddr_nl* nla_, struct nlmsgerr* nlerr_, void* arg_)
{
  std::cout << "RouteLinkEvent::err_cb()" << std::endl;
  RouteLinkNotification* n = new RouteLinkNotification(RouteLinkEvent::EVENT_ERR, 0);
  this->Notify(n);
  delete(n);

  return(NL_SKIP);
}

//*****************************************************************************
// Class: RouteLinkNotification
//*****************************************************************************

RouteLinkNotification::RouteLinkNotification(RouteLinkEvent::EVENT id_, uint32_t index_) :
  _id(id_)
{
  GetLinkCommand cmd(index_);
  if (cmd.Exec())
  {
    this->Link = cmd.Link;
  }
}

RouteLinkNotification::~RouteLinkNotification()
{
}

RouteLinkEvent::EVENT
RouteLinkNotification::Id() const
{
  return(this->_id);
}


}

