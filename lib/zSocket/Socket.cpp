/*
 * Copyright (c) 2014-2016 ZenoTec LLC (http://www.zenotec.net)
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

#include <stdint.h>
#include <string.h>

#include <string>
#include <list>
#include <mutex>
#include <memory>

#include <zutils/zLog.h>
#include <zutils/zSem.h>
#include <zutils/zQueue.h>
#include <zutils/zThread.h>
#include <zutils/zEvent.h>

#include <zutils/zSocket.h>

ZLOG_MODULE_INIT(zUtils::zLog::Log::MODULE_SOCKET);

namespace zUtils
{
namespace zSocket
{

//*****************************************************************************
// zSocket::Socket Class
//*****************************************************************************
Socket::Socket(const Socket::SOCKET_TYPE type_) :
    zEvent::Event(zEvent::Event::TYPE_SOCKET), _type(type_), _addr(Address::TYPE_NONE)
{
  ZLOG_DEBUG("Creating socket: '" + ZLOG_P(this) + "'");
}

Socket::~Socket()
{
  ZLOG_DEBUG("Destroying socket: '" + ZLOG_P(this) + "'");
}

int
Socket::GetId() const
{
  return (0);
}

const Socket::SOCKET_TYPE
Socket::GetType() const
{
  return (this->_type);
}

const Address&
Socket::GetAddress() const
{
  return (this->_addr);
}

bool
Socket::SetAddress(const Address& addr_)
{
  this->_addr = addr_;
  return (true);
}

bool
Socket::Getopt(Socket::OPTIONS opt_)
{
  return (false);
}

bool
Socket::Setopt(Socket::OPTIONS opt_)
{
  return (false);
}

bool
Socket::Bind(const Address& addr_)
{
  return (false);
}

SHARED_PTR(zSocket::Notification)
Socket::Recv()
{
  SHARED_PTR(zSocket::Notification) n(new zSocket::Notification(*this));
  n->SetSubType(Notification::SUBTYPE_ERR);
  return (n);
}

SHARED_PTR(zSocket::Notification)
Socket::Send(const Address& addr_, const Buffer& sb_)
{
  SHARED_PTR(zSocket::Notification) n(new zSocket::Notification(*this));
  SHARED_PTR(zSocket::Address) src(new zSocket::Address(this->GetAddress()));
  n->SetSrcAddress(src);
  SHARED_PTR(zSocket::Address) dst(new zSocket::Address(addr_));
  n->SetDstAddress(dst);
  SHARED_PTR(zSocket::Buffer) sb(new zSocket::Buffer(sb_));
  n->SetBuffer(sb);
  n->SetSubType(Notification::SUBTYPE_PKT_ERR);
  return (n);
}

SHARED_PTR(zSocket::Notification)
Socket::Send(const Address& to_, const std::string& str_)
{
  Buffer sb(str_);
  return (this->Send(to_, sb));
}

}
}

