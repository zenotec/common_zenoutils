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
#include <string.h>
#include <errno.h>
#include <unistd.h>
#include <poll.h>
#include <sys/socket.h>
#include <net/if_arp.h>

// libc++ includes
#include <iostream>

// libzutils includes
#include <zutils/zLog.h>
using namespace zUtils;
#include <zutils/nl80211/FrameEvent.h>

// local includes

ZLOG_MODULE_INIT(zUtils::zLog::Log::MODULE_WIRELESS);

namespace nl80211
{

static std::string
__errstr(int code)
{
  return(std::string(strerror(-code)));
}

//*****************************************************************************
// Class: FrameEvent
//*****************************************************************************

FrameEvent::FrameEvent(const int ifindex_, netlink::Callback& cb_) :
    _thread(this, this),
    netlink::Callback(&cb_),  // need to do this to daisy chain callbacks
    netlink::Command(ifindex_),
    _cb(cb_)
{

  this->IfIndex.Set(this->GetIfIndex());

  if (!this->_sock.Connect())
  {
    ZLOG_ERR("Error connecting Netlink socket");
    return;
  }

  if (!this->_sock.SetCallback(this))
  {
    ZLOG_ERR("Error setting up message handlers");
    return;
  }

}

FrameEvent::~FrameEvent()
{
  if (this->_thread.Stop())
  {
    this->_sock.Disconnect();
  }
}

int
FrameEvent::GetSockFd() const
{
  return (nl_socket_get_fd(this->_sock()));
}

bool
FrameEvent::RecvMsg()
{
  return (nl_recvmsgs_default(this->_sock()) == 0);
}

bool
FrameEvent::Listen()
{
  return (this->_thread.Start());
}

void
FrameEvent::Display(const std::string& prefix_) const
{
}

void
FrameEvent::Run(zThread::ThreadArg *arg_)
{

  bool exit = false;

  // Setup for poll loop
  this->RegisterFd(this->GetSockFd(), (POLLIN | POLLERR));

  while (!exit)
  {

    std::vector<struct pollfd> fds;

    // Wait on file descriptor set
    int ret = this->Poll(fds);

    FOREACH (auto& fd, fds)
    {
      if (this->IsExitFd(fd))
      {
        exit = true;
        continue;
      }
      else if (this->IsReloadFd(fd))
      {
        continue;
      }
      else if ((fd.fd == this->GetSockFd()) && (fd.revents == POLLIN))
      {
        if (!this->RecvMsg())
        {
          ZLOG_ERR("Error receiving netlink message");
          ZLOG_ERR("Error: (" + ZLOG_INT(ret) + ") " + __errstr(ret));
        }
      }
    }

  }

  this->UnregisterFd(this->GetSockFd());

  return;
}

bool
FrameEvent::Exec()
{

  this->_status = false;
  this->_count.Reset();

  if (!this->IfIndex())
  {
    ZLOG_ERR("Error executing GetInterfaceCommand: " + this->IfName());
    ZLOG_ERR("Valid interface index must be specified");
    return(false);
  }

  SHPTR(GenericMessage) cmdmsg = this->_sock.CreateMsg();
  if (!cmdmsg)
  {
    ZLOG_ERR("Error creating the Netlink message");
    return(false);
  }
  cmdmsg->SetCommand(NL80211_CMD_REGISTER_FRAME);

  // Set interface index attribute
  if (!cmdmsg->PutAttribute(&this->IfIndex))
  {
    ZLOG_ERR("Error setting ifindex attribute");
    return (false);
  }

  // Set frame type/subtype
  if (!cmdmsg->PutAttribute(&this->FrameType))
  {
    ZLOG_ERR("Error setting frame type attribute");
    return (false);
  }

  // Set frame matching bytes
  if (!cmdmsg->PutAttribute(&this->FrameMatch))
  {
    ZLOG_ERR("Error setting frame match attribute");
    return (false);
  }

//  cmdmsg->Display();

  // Send message
  if (!this->_sock.SendMsg(cmdmsg))
  {
    ZLOG_ERR("Error sending netlink message");
    return(false);
  }

  // Wait for the response
  if (!this->_sock.RecvMsg())
  {
    ZLOG_ERR("Error receiving response for netlink message");
    return(false);
  }

  if (!this->_count.TimedWait(1000))
  {
    ZLOG_ERR("Error receiving response for netlink message");
    return(false);
  }

  return(this->_status);

}

int
FrameEvent::valid_cb(struct nl_msg* msg_, void* arg_)
{
  // Called when a management frame is received
  int ret = this->_cb.ValidCallback(msg_, arg_);
  this->_status = true;
  this->_count.Post();
  return (ret);
}

int
FrameEvent::ack_cb(struct nl_msg* msg_, void* arg_)
{
  // Only called after sending command to register for a frame
  this->_status = true;
  this->_count.Post();
  return (NL_OK);
}

int
FrameEvent::err_cb(struct sockaddr_nl* nla_, struct nlmsgerr* nlerr_, void* arg_)
{
  ZLOG_ERR("Error executing FrameEvent: (" + zLog::IntStr(this->IfIndex()) +
      std::string("): ") + this->IfName());
  ZLOG_ERR("Error: (" + ZLOG_INT(nlerr_->error) + ") " + __errstr(nlerr_->error));
  this->_status = false;
  this->_count.Post();
  return(NL_SKIP);
}

}
