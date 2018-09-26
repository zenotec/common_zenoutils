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

#include <unistd.h>
#include <errno.h>
#include <string.h>
#include <sys/socket.h>
#include <netinet/in.h>

#include <zutils/zLog.h>
#include <zutils/zSocket.h>
#include <zutils/zLoopSocket.h>

ZLOG_MODULE_INIT(zUtils::zLog::Log::MODULE_SOCKET);

namespace zUtils
{
namespace zSocket
{

//**********************************************************************
// Class: zSocket::LoopSocket
//**********************************************************************

LoopSocket::LoopSocket() :
    Socket(SOCKET_TYPE::TYPE_LOOP)
{
  // Create a AF_INET socket
  this->_fd = socket( AF_INET, (SOCK_DGRAM | SOCK_NONBLOCK), IPPROTO_UDP);
  if (this->_fd > 0)
  {
    ZLOG_INFO("Socket created: " + ZLOG_INT(this->_fd));
  }
  else
  {
    this->_fd = 0;
    ZLOG_ERR("Cannot create socket: " + std::string(strerror(errno)));
  }
}

LoopSocket::~LoopSocket()
{
  // Make sure the socket is unregistered from all handlers
  if (!this->_handler_list.empty())
  {
    fprintf(stderr, "BUG: Socket registered with handler, not closing FD\n");
  }
  else
  {
    // Close socket
    ZLOG_INFO("Closing socket: " + ZLOG_INT(this->_fd));
    if (this->_fd)
    {
      close(this->_fd);
      this->_fd = 0;
    } // end if
  }
}

int
LoopSocket::GetId() const
{
  return (this->_fd);
}

const Address&
LoopSocket::GetAddress() const
{
  return (this->_addr);
}

bool
LoopSocket::Bind(const Address& addr_)
{
  this->_addr = LoopAddress(addr_);
  return (true);
}

SHARED_PTR(zSocket::Notification)
LoopSocket::Recv()
{
  SHARED_PTR(zSocket::Notification) n(new zSocket::Notification(*this));

  // Initialize notification
  n->SetSubType(Notification::SUBTYPE_PKT_ERR);
  n->SetSrcAddress(SHARED_PTR(Address)(new Address(this->GetAddress())));
  n->SetDstAddress(SHARED_PTR(Address)(new Address(this->GetAddress())));
  return (n);
}

SHARED_PTR(zSocket::Notification)
LoopSocket::Send(const Address& to_, const Buffer& sb_)
{

  SHARED_PTR(zSocket::Notification) n(new zSocket::Notification(*this));

  // Initialize notification
  n->SetSrcAddress(SHARED_PTR(Address)(new Address(this->GetAddress())));
  n->SetDstAddress(SHARED_PTR(Address)(new Address(this->GetAddress())));
  n->SetBuffer(SHARED_PTR(Buffer)(new Buffer(sb_)));

  if (to_ == this->GetAddress())
  {
    n->SetSubType(Notification::SUBTYPE_PKT_RCVD);
    this->notifyHandlers(n);
    n->SetSubType(Notification::SUBTYPE_PKT_SENT);
  }
  else
  {
    n->SetSubType(Notification::SUBTYPE_PKT_ERR);
    ZLOG_ERR(std::string("Cannot send packet: " + std::string(strerror(errno))));
  }

  return (n);
}

}
}
