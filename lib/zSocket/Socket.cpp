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
Socket::Socket(SocketType type_) :
    zEvent::Event(zEvent::Event::TYPE_SOCKET), _type(type_), _addr(NULL)
{
  ZLOG_DEBUG("Creating socket: '" + ZLOG_P(this) + "'");
}

Socket::~Socket()
{
  ZLOG_DEBUG("Destroying socket: '" + ZLOG_P(this) + "'");
  if (this->_addr)
  {
    delete (this->_addr);
    this->_addr = NULL;
  }
}

const SocketType
Socket::Type() const
{
  return (this->_type);
}

const zSocket::SocketAddress&
Socket::Address() const
{
  return (*this->_addr);
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
Socket::Bind(const SocketAddress& addr_)
{
  bool status = false;
  this->_addr = zSocket::SocketAddressFactory::Create(addr_);
  if (this->_addr)
  {
    status = this->_bind();
  }
  return (status);
}

ssize_t
Socket::Send(SocketAddressBufferPair& pair_)
{
  ZLOG_DEBUG("Sending packet: " + pair_.first->Address() +
      "(" + zLog::IntStr(pair_.second->Size()) + ")");
  this->_txq.Push(pair_);
  return (pair_.second->Size());
}

ssize_t
Socket::Send(const SocketAddress& addr_, SocketBuffer& sb_)
{
  SHARED_PTR(SocketAddress)addr(zSocket::SocketAddressFactory::Create(addr_));
  SHARED_PTR(SocketBuffer) sb(new SocketBuffer(sb_));
  SocketAddressBufferPair p(addr, sb);
  return (this->Send(p));
}

ssize_t
Socket::Send(const SocketAddress &addr_, const std::string &str_)
{
  SocketBuffer sb(str_);
  return (this->Send(addr_, sb));
}

bool
Socket::rxbuf(SocketAddressBufferPair &pair_)
{
  SHARED_PTR(SocketNotification) n(new SocketNotification(*this));
  n->id(SocketNotification::ID_PKT_RCVD);
  n->pkt(pair_);
  this->NotifyHandlers(n);
  return (true);
}

bool
Socket::txbuf(SocketAddressBufferPair &pair_, size_t timeout_)
{
  bool status = false;
  if (this->_txq.TimedWait(timeout_))
  {
    pair_ = this->_txq.Front();
    this->_txq.Pop();
    SHARED_PTR(SocketNotification) n(new SocketNotification(*this));
    n->id(SocketNotification::ID_PKT_SENT);
    n->pkt(pair_);
    this->NotifyHandlers(n);
    status = true;
  }
  return (status);
}

}
}
