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

#ifndef __ZWIRELESS_WIRELESSSOCKET_H__
#define __ZWIRELESS_WIRELESSSOCKET_H__

// libc includes

// libc++ includes
#include <string>

// libzutils includes
#include <zutils/zCompatibility.h>
#include <zutils/zEvent.h>
#include <zutils/zSocket.h>
using namespace zUtils;
#include <zutils/ieee80211/RadioTap.h>
#include <zutils/ieee80211/Frame.h>

// local includes

namespace zUtils
{
namespace zWireless
{

class Socket;

//*****************************************************************************
// Class: zWireless::Notification
//*****************************************************************************

class Notification :
    public zSocket::Notification
{

public:

  Notification(Socket& sock_);

  Notification(const zSocket::Notification& noti_);

  virtual
  ~Notification();

  SHARED_PTR(ieee80211::RadioTap)
  RadiotapHeader();

  bool
  RadiotapHeader(SHARED_PTR(ieee80211::RadioTap) rtaphdr_);

  SHARED_PTR(ieee80211::Frame)
  Frame();

  bool
  Frame(SHARED_PTR(ieee80211::Frame) frame_);

protected:

private:

  SHARED_PTR(ieee80211::RadioTap) _rtaphdr;
  SHARED_PTR(ieee80211::Frame) _frame;

};

//*****************************************************************************
// Class: zWireless::Socket
//*****************************************************************************

class Socket :
    public zSocket::Socket
{

public:

  Socket(zSocket::Socket& sock_);

  virtual
  ~Socket();

  virtual int
  GetId() const;

  virtual const zSocket::Address&
  GetAddress() const;

  virtual bool
  Getopt(Socket::OPTIONS opt_);

  virtual bool
  Setopt(Socket::OPTIONS opt_);

  virtual bool
  Bind(const zSocket::Address& addr_);

  virtual SHARED_PTR(zSocket::Notification)
  Recv();

  virtual SHARED_PTR(zSocket::Notification)
  Send(const zSocket::Address& to_, const zSocket::Buffer& sb_);

  virtual SHARED_PTR(zSocket::Notification)
  Send(const ieee80211::RadioTap hdr_, const ieee80211::Frame& frame_);

  void
  Display();

protected:

private:

  zSocket::Socket& _sock;

};

}
}

#endif /* __ZWIRELESS_WIRELESSSOCKET_H__ */
