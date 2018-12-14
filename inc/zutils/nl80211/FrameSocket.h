/*
 * Copyright (c) 2018 Cable Television Laboratories, Inc. ("CableLabs")
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

#ifndef __NL80211_FRAMESOCKET_H__
#define __NL80211_FRAMESOCKET_H__

// libc includes

// libc++ includes

// libzutils includes
#include <zutils/zSocket.h>
#include <zutils/zQueue.h>
using namespace zUtils;
#include <zutils/netlink/Attribute.h>
#include <zutils/netlink/Callback.h>
#include <zutils/netlink/Command.h>
#include <zutils/netlink/Message.h>
using namespace netlink;
#include <zutils/nl80211/FrameEvent.h>
#include <zutils/nl80211/FrameCommand.h>
using namespace nl80211;

// local includes

namespace nl80211
{

//**********************************************************************
// Class: FrameSocketTx
//**********************************************************************

class FrameSocketTx :
    public zThread::ThreadFunction
{

public:

protected:

  virtual void
  Run(zThread::ThreadArg *arg_);

private:

};

//*****************************************************************************
// Class: FrameSocket
//*****************************************************************************

class FrameSocket :
    public zSocket::Socket,
    public zThread::ThreadArg,
    public netlink::Callback
{

public:

  friend FrameSocketTx;

  FrameSocket();

  virtual
  ~FrameSocket();

  bool
  RegisterFrame(const uint16_t type_, const uint8_t* match_, const size_t len_);

  // Address is name of interface
  virtual bool
  Bind(const zSocket::Address& addr_);

  bool
  Listen();

protected:

  // Sends from transmit queue and returns notification
  virtual SHPTR(zSocket::Notification)
  send();

  virtual int
  valid_cb(struct nl_msg* msg_, void* arg_);

  virtual int
  ack_cb(struct nl_msg* msg_, void* arg_);

  virtual int
  err_cb(struct sockaddr_nl* nla_, struct nlmsgerr* nlerr_, void* arg_);

private:

  nl80211::FrameCommand* _fcmd;
  nl80211::FrameEvent* _fevent;

  zThread::Thread _txthread;
  FrameSocketTx _txfunc;

};

}

#endif /* __NL80211_FRAMESOCKET_H__ */
