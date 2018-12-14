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

#ifndef __IEEE80211_SOCKET_H__
#define __IEEE80211_SOCKET_H__

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
namespace ieee80211
{

class Socket;

//*****************************************************************************
// Class: ieee80211::Notification
//*****************************************************************************

class Notification :
    public zSocket::Notification
{

public:

  Notification(Socket& sock_);

  Notification(const zSocket::Notification& noti_);

  virtual
  ~Notification();

  SHPTR(ieee80211::RadioTap)
  RadiotapHeader();

  bool
  RadiotapHeader(SHPTR(ieee80211::RadioTap) rtaphdr_);

protected:

private:

  SHPTR(ieee80211::RadioTap) _rtaphdr;

};

//*****************************************************************************
// Class: ieee80211::Socket
//*****************************************************************************

class Socket :
    public zSocket::Adapter
{

public:

  Socket(zSocket::Socket& sock_);

  virtual
  ~Socket();

  virtual SHPTR(zSocket::Notification)
  Recv();

  virtual bool
  Send(const zSocket::Address& to_, const zSocket::Buffer& sb_);

  virtual bool
  Send(ieee80211::RadioTap hdr_, ieee80211::Frame& frame_);

  void
  Display();

protected:

private:

};

}
}
}

#endif /* __IEEE80211_SOCKET_H__ */
