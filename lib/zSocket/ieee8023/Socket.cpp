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
#include <endian.h>

// libc++ includes
#include <iostream>

// libzutils includes
#include <zutils/zLog.h>
#include <zutils/zSocket.h>
using namespace zUtils;
#include <zutils/ieee8023/Frame.h>
#include <zutils/ieee8023/Socket.h>

// local includes

ZLOG_MODULE_INIT(zUtils::zLog::Log::MODULE_SOCKET);

namespace zUtils
{
namespace zSocket
{
namespace ieee8023
{

//*****************************************************************************
// Class: ieee8023::Notification
//*****************************************************************************

Notification::Notification(Socket& sock_) :
    zSocket::Notification(sock_)
{
  this->Frame(SHARED_PTR(ieee8023::Frame)(new ieee8023::Frame));
}

Notification::Notification(const zSocket::Notification& noti_) :
    zSocket::Notification(noti_)
{

}

Notification::~Notification()
{
}

SHARED_PTR(ieee8023::Frame)
Notification::Frame()
{
  return(this->_frame);
}

bool
Notification::Frame(SHARED_PTR(ieee8023::Frame)frame_)
{
  this->_frame = frame_;
  return(true);
}

//*****************************************************************************
// Class: ieee8023::Socket
//*****************************************************************************


Socket::Socket(zSocket::Socket& sock_) :
    zSocket::Adapter(sock_)
{
}

Socket::~Socket()
{
}

SHARED_PTR(zSocket::Notification)
Socket::Recv()
{
}

SHARED_PTR(zSocket::Notification)
Socket::Send(const zSocket::Address& to_, const zSocket::Buffer& sb_)
{
  return (this->socket.Send(to_, sb_));
}

SHARED_PTR(zSocket::Notification)
Socket::Send(Frame& frame_)
{
}

void
Socket::Display()
{
}

}
}
}
