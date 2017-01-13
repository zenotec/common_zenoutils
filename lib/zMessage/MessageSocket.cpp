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
#include <netinet/in.h>
#include <string.h>

#include <string>
#include <list>
#include <mutex>
#include <memory>

#include <zutils/zLog.h>
#include <zutils/zSem.h>
#include <zutils/zThread.h>
#include <zutils/zQueue.h>
#include <zutils/zEvent.h>
#include <zutils/zData.h>
#include <zutils/zSocket.h>

#include <zutils/zMessage.h>
#include <zutils/zMessageSocket.h>

namespace zUtils
{
namespace zMessage
{

//**********************************************************************
// zMessage::Socket Class
//**********************************************************************

MessageSocket::MessageSocket(zSocket::Socket *sock_) :
    _sock(sock_), zEvent::Event(zEvent::Event::TYPE_MSG)
{
  ZLOG_DEBUG("Creating message socket: '" + ZLOG_P(this) + "'");
  this->_sock_handler.RegisterEvent(this->_sock);
  this->_sock_handler.RegisterObserver(this);
}

MessageSocket::~MessageSocket()
{
  ZLOG_DEBUG("Destroying message socket: '" + ZLOG_P(this) + "'");
  this->_sock_handler.UnregisterObserver(this);
  this->_sock_handler.UnregisterEvent(this->_sock);
}

bool
MessageSocket::Open()
{
  return (this->_sock->Open());
}

void
MessageSocket::Close()
{
  this->_sock->Close();
  return;
}

bool
MessageSocket::Bind()
{
  return (this->_sock->Bind());
}

bool
MessageSocket::Connect(const zSocket::SocketAddress* addr_)
{
  return (this->_sock->Connect(addr_));
}

bool
MessageSocket::Send(const zSocket::SocketAddress& addr_, const zMessage::Message &msg_)
{
  bool status = false;

  // Send message
  if (this->_sock->Send(addr_, msg_.GetJson()) == msg_.GetJson().size())
  {
    status = true;
  }

  // Return status
  return (status);
}

bool
MessageSocket::EventHandler(const zEvent::EventNotification* notification_)
{
  const zSocket::SocketNotification *n = NULL;

  ZLOG_DEBUG("Handling socket event");

  bool status = false;
  if (notification_ && (notification_->Type() == zEvent::Event::TYPE_SOCKET))
  {
    n = static_cast<const zSocket::SocketNotification *>(notification_);
    switch (n->Id())
    {
    case zSocket::SocketNotification::ID_PKT_RCVD:
      {
      zSocket::SocketAddressBufferPair p = n->Pkt();
      zMessage::Message msg;
      msg.SetJson(p.second->Str());
      zMessage::MessageNotification notification(this);
      notification.id(zMessage::MessageNotification::ID_MSG_RCVD);
      notification.message(&msg);
      this->Notify(&notification);
      status = true;
      break;
    }
    case zSocket::SocketNotification::ID_PKT_SENT:
      {
      zSocket::SocketAddressBufferPair p = n->Pkt();
      zMessage::MessageNotification notification(this);
      notification.id(zMessage::MessageNotification::ID_MSG_SENT);
      notification.message(NULL);
      this->Notify(&notification);
      status = true;
      break;
    }
    default:
      status = false;
      break;
    }
  }
  return (status);
}

}
}

