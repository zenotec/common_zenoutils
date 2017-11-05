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
#include "Attribute.h"

#include "Message.h"
#include "Handler.h"
#include "Socket.h"

namespace netlink
{

static std::string
__errstr(int code)
{
  return(std::string(nl_geterror(code)));
}

//*****************************************************************************
// Class: Socket
//*****************************************************************************

Socket::Socket() :
    _sock(NULL), _family(-1)
{
}

Socket::~Socket()
{
  this->Disconnect();
}

struct nl_sock*
Socket::operator ()()
{
  return(this->_sock);
}

bool
Socket::SetHandler(Handler* handler_)
{
  int ret = 0;

  if (!this->_sock)
  {
    ZLOG_ERR("Error setting handler, socket does not exists");
    return(false);
  }

  ret = nl_socket_modify_cb(this->_sock, NL_CB_VALID, NL_CB_CUSTOM, handler_->ValidCallback, handler_);
  if (ret < 0)
  {
    ZLOG_ERR("Error setting up netlink valid message callback");
    ZLOG_ERR("Error: (" + ZLOG_INT(ret) + ") " + __errstr(ret));
    this->Disconnect();
    return(false);
  }

  ret = nl_socket_modify_cb(this->_sock, NL_CB_FINISH, NL_CB_CUSTOM, handler_->FinishCallback, handler_);
  if (ret < 0)
  {
    ZLOG_ERR("Error setting up netlink finish callback");
    ZLOG_ERR("Error: (" + ZLOG_INT(ret) + ") " + __errstr(ret));
    this->Disconnect();
    return(false);
  }

  ret = nl_socket_modify_cb(this->_sock, NL_CB_ACK, NL_CB_CUSTOM, handler_->AckCallback, handler_);
  if (ret < 0)
  {
    ZLOG_ERR("Error setting up netlink ACK callback");
    ZLOG_ERR("Error: (" + ZLOG_INT(ret) + ") " + __errstr(ret));
    this->Disconnect();
    return(false);
  }

  ret = nl_socket_modify_err_cb(this->_sock, NL_CB_CUSTOM, handler_->ErrorCallback, handler_);
  if (ret < 0)
  {
    ZLOG_ERR("Error setting up netlink error callback");
    ZLOG_ERR("Error: (" + ZLOG_INT(ret) + ") " + __errstr(ret));
    this->Disconnect();
    return(false);
  }

  return(true);
}

int
Socket::Family() const
{
  if (this->_family == -1)
  {
    ZLOG_WARN("Requesting socket family identifier before connecting socket");
  }
  return(this->_family);
}

bool
Socket::Connect(const int family_)
{
  int ret = 0;

  this->_sock = nl_socket_alloc();
  if (!this->_sock)
  {
    ZLOG_ERR("Error allocating netlink socket");
    return (false);
  }

  ret = nl_connect(this->_sock, family_);
  if (ret < 0)
  {
    ZLOG_ERR("Error connecting netlink socket");
    ZLOG_ERR("Error: (" + ZLOG_INT(ret) + ") " + __errstr(ret));
    return (false);
  }

  return(true);
}

bool
Socket::Disconnect()
{
  bool status = false;
  // There is no disconnect API so free and reallocate socket
  if (this->_sock)
  {
    nl_socket_free(this->_sock);
    this->_sock = NULL;
    this->_family = -1;
    status = true;
  }
  return(status);
}

bool
Socket::SendMsg(Message& msg_)
{
//  std::cout << "Sending message: " << std::endl;
//  msg_.Display();

  int ret = nl_send_auto(this->_sock, msg_());
  if (ret < 0)
  {
    ZLOG_ERR("Error sending netlink message");
    ZLOG_ERR("Error: (" + ZLOG_INT(ret) + ") " + __errstr(ret));
    return (false);
  }

  return(true);
}

bool
Socket::RecvMsg()
{
  int ret = nl_recvmsgs_default(this->_sock);
  if (ret < 0)
  {
    ZLOG_ERR("Error receiving netlink message");
    ZLOG_ERR("Error: (" + ZLOG_INT(ret) + ") " + __errstr(ret));
    return (false);
  }

  return(true);
}

}

