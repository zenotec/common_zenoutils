/*
 * Copyright (c) 2014-2018 ZenoTec LLC (http://www.zenotec.net)
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

#include <zutils/zLog.h>
#include <zutils/zSocket.h>

ZLOG_MODULE_INIT(zUtils::zLog::Log::MODULE_SOCKET);

namespace zUtils
{
namespace zSocket
{

//*****************************************************************************
// Class: zSocket::Adapter
//*****************************************************************************

Adapter::Adapter(Socket& socket_) :
    zSocket::Socket(socket_.GetType()), socket(socket_)
{
}

Adapter::~Adapter()
{
}

int
Adapter::GetFd() const
{
  return (this->socket.GetFd());
}

const zSocket::Address&
Adapter::GetAddress() const
{
  return (this->socket.GetAddress());
}

bool
Adapter::Getopt(Socket::OPTIONS opt_)
{
  return (this->socket.Getopt(opt_));
}

bool
Adapter::Setopt(Socket::OPTIONS opt_)
{
  return (this->socket.Setopt(opt_));
}

bool
Adapter::Bind(const zSocket::Address& addr_)
{
  return (this->socket.Bind(addr_));
}

SHARED_PTR(zSocket::Notification)
Adapter::Recv()
{
  return (this->socket.Recv());
}

bool
Adapter::Send(const zSocket::Address& to_, const zSocket::Buffer& sb_)
{
  return (this->socket.Send(to_, sb_));
}

bool
Adapter::Send(Frame& frame_)
{
  return (this->socket.Send(frame_));
}

bool
Adapter::Send(const Address& to_, const std::string& str_)
{
  return (this->socket.Send(to_, str_));
}

}
}
