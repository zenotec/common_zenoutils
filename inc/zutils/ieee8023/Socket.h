/*
 * Copyright (c) 2017 - 2018 Cable Television Laboratories, Inc. ("CableLabs")
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

#ifndef __IEEE8023_SOCKET_H__
#define __IEEE8023_SOCKET_H__

// libc includes
#include <stdio.h>

// libc++ includes
#include <map>

// libzutils includes
#include <zutils/zCompatibility.h>
#include <zutils/zEvent.h>
#include <zutils/zSocket.h>
using namespace zUtils;
#include <zutils/ieee8023/Frame.h>

// local includes

namespace zUtils
{
namespace zSocket
{
namespace ieee8023
{

class Socket;

//*****************************************************************************
// Class: ieee8023::Notification
//*****************************************************************************

class Notification :
    public zSocket::Notification
{

public:

  Notification(Socket& sock_);

  Notification(const zSocket::Notification& noti_);

  virtual
  ~Notification();

protected:

private:

};

//*****************************************************************************
// Class: ieee8023::Socket
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

  void
  Display();

protected:

private:

};

}
}
}

#endif /* __IEEE8023_SOCKET_H__ */
