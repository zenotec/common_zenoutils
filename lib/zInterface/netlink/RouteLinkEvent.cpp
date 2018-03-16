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
#include <net/if_arp.h>

// libc++ includes
#include <iostream>

// libzutils includes
#include <zutils/zLog.h>
#include <zutils/netlink/RouteMessage.h>
#include <zutils/netlink/RouteLinkEvent.h>
#include <zutils/netlink/GetLinkCommand.h>
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
// Class: RouteLinkEvent
//*****************************************************************************

RouteLinkEvent::RouteLinkEvent(const std::string& name_) :
    zEvent::Event(zEvent::Event::TYPE_LAST), _thread(this, this), _ifindex(0)
{
  this->SetIfName(name_);

}

RouteLinkEvent::~RouteLinkEvent()
{
  this->Stop();
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

std::string
RouteLinkEvent::GetIfName() const
{
  return(this->_ifname);
}

bool
RouteLinkEvent::SetIfName(const std::string& name_)
{
  bool status = false;
  GetLinkCommand cmd(name_);
  if (!name_.empty() && cmd.Exec())
  {
    this->_ifindex = cmd.IfIndex();
    this->_ifname = cmd.IfName();
    status = true;
  }
  return(status);
}

bool
RouteLinkEvent::Start()
{

  if (!this->_sock.Connect())
  {
    ZLOG_ERR("Error connecting Netlink socket");
    return(false);
  }

  if (!this->_sock.SetHandler(this))
  {
    ZLOG_ERR("Error setting up message handlers");
    return(false);
  }

  nl_socket_set_nonblocking(this->_sock());
  nl_socket_disable_seq_check(this->_sock());

  int ret = nl_socket_add_membership(this->_sock(), RTNLGRP_LINK);
  if (ret < 0)
  {
    ZLOG_ERR("Error joining RTNLGRP_LINK multicast group");
    ZLOG_ERR("Error: (" + ZLOG_INT(ret) + ") " + __errstr(ret));
    return(false);
  }

  return (this->_thread.Start());
}

bool
RouteLinkEvent::Stop()
{
  bool status = false;
  if (this->_thread.Stop())
  {
    nl_socket_drop_membership(this->_sock(), RTNLGRP_LINK);
    status = this->_sock.Disconnect();
  }
  return (status);
}

int
RouteLinkEvent::valid_cb(struct nl_msg* msg_, void* arg_)
{
  RouteMessage msg;
  if (!msg.Disassemble(msg_))
  {
    ZLOG_ERR("Error parsing route link message");
    return (NL_SKIP);
  }
//  msg.Display();
//  msg.DisplayAttributes();

  if (!this->_ifindex || (this->_ifindex == msg.LinkIndex()))
  {
    SHARED_PTR(RouteLinkNotification) n(new RouteLinkNotification(*this, RouteLinkEvent::EVENTID_UPDOWN));
    this->notifyHandlers(n);
  }

  return(NL_OK);
}

int
RouteLinkEvent::err_cb(struct sockaddr_nl* nla_, struct nlmsgerr* nlerr_, void* arg_)
{
  std::cout << "RouteLinkEvent::err_cb()" << std::endl;
  SHARED_PTR(RouteLinkNotification) n(new RouteLinkNotification(*this, RouteLinkEvent::EVENTID_ERR));
  this->notifyHandlers(n);

  return(NL_SKIP);
}

//*****************************************************************************
// Class: RouteLinkNotification
//*****************************************************************************

RouteLinkNotification::RouteLinkNotification(RouteLinkEvent& rlevent_, RouteLinkEvent::EVENTID id_) :
    zEvent::Notification(rlevent_), _id(id_)
{
//  GetLinkCommand cmd(index_);
//  if (cmd.Exec())
//  {
//    this->Link = cmd.Link;
//  }
}

RouteLinkNotification::~RouteLinkNotification()
{
}

RouteLinkEvent::EVENTID
RouteLinkNotification::Id() const
{
  return (this->_id);
}

}
