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
    zEvent::Event(zEvent::TYPE_SOCKET),
    _type(type_),
    _addr(Address::TYPE_NONE)
{
  ZLOG_DEBUG("Creating socket: '" + ZLOG_INT(this->GetFd()) + "'");
}

Socket::~Socket()
{
  ZLOG_DEBUG("Destroying socket: '" + ZLOG_INT(this->GetFd()) + "'");
}

int
Socket::GetFd() const
{
  return (this->rxq.GetFd());
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
Socket::Open()
{
  return (false);
}

bool
Socket::Bind(const Address& addr_)
{
  return (false);
}

bool
Socket::Close()
{
  return (false);
}

SHPTR(zSocket::Notification)
Socket::Recv()
{

  SHPTR(zSocket::Notification) n(new zSocket::Notification(*this));
  n->SetSubType(Notification::SUBTYPE_ERR);

  if (this->rxq.TryWait())
  {
    n = this->rxq.Front();
    this->rxq.Pop();
  }

  return (n);
}

bool
Socket::Send(const Address& addr_, const Buffer& sb_)
{

  if (sb_.Length() == 0)
  {
    ZLOG_WARN("Empty socket buffer");
    return (false);
  }

  // Create shared notification
  SHPTR(zSocket::Notification) n(new zSocket::Notification(*this));
  if (!n.get())
  {
    ZLOG_ERR("NULL notification");
    return (false);
  }
  n->SetSubType(Notification::SUBTYPE_PKT_SENT);

  // Create shared source address
  SHPTR(zSocket::Address) src(new zSocket::Address(this->GetAddress()));
  if (!src.get())
  {
    ZLOG_ERR("NULL source address");
    return (false);
  }
  n->SetSrcAddress(src);

  // Create shared destination address
  SHPTR(zSocket::Address) dst(new zSocket::Address(addr_));
  if (!dst.get())
  {
    ZLOG_ERR("NULL destination address");
    return (false);
  }
  n->SetDstAddress(dst);

  // Create shared socket buffer
  SHPTR(zSocket::Buffer) sb(new zSocket::Buffer(sb_));
  if (!sb.get())
  {
    ZLOG_ERR("NULL socket buffer");
    return (false);
  }
  n->SetBuffer(sb);

  // Push notification onto transmit queue for socket to send
  return (this->txq.Push(n));

}

bool
Socket::Send(Frame& frame_)
{
  bool status = false;
  zSocket::Address to(Address::TYPE_NONE, frame_.GetDestination());
  zSocket::Buffer sb;
  if (frame_.Assemble(sb, false))
  {
    sb.Push(sb.Size()); // restore data pointer to start of frame
    status = this->Send(to, sb);
  }
  return (status);
}

bool
Socket::Send(const Address& to_, const std::string& str_)
{
  return (this->Send(to_, Buffer(str_)));
}

}
}

