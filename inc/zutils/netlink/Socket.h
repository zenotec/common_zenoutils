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

#ifndef __NETLINK_SOCKET_H__
#define __NETLINK_SOCKET_H__

#include <netlink/netlink.h>
#include <zutils/netlink/Callback.h>

#include <zutils/zCompatibility.h>

#include "Message.h"

namespace netlink
{

//*****************************************************************************
// Class: Socket
//*****************************************************************************

class Socket
{

public:

  Socket();

  virtual
  ~Socket();

  virtual struct nl_sock*
  operator()() const;

  bool
  SetCallback(Callback* cb_);

  virtual bool
  Connect(const int proto_);

  bool
  Disconnect();

  bool
  IsConnected() const;

  bool
  SendMsg(SHARED_PTR(Message) msg_);

  bool
  RecvMsg(); // Receives the message and invokes the installed handler

protected:

private:

  struct nl_sock* _sock;
  int _proto;

};

}

#endif /* __NETLINK_SOCKET_H__ */
