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

#include <zutils/zHelloMessage.h>
#include <zutils/zByeMessage.h>
#include <zutils/zAckMessage.h>

namespace zUtils
{
namespace zMessage
{

//**********************************************************************
// zMessage::Socket Class
//**********************************************************************

MessageSocket::MessageSocket() :
    zEvent::Event(zEvent::Event::TYPE_MSG)
{
  ZLOG_DEBUG("Creating message socket: '" + ZLOG_P(this) + "'");
  this->_sock_handler.RegisterObserver(this);
  this->_msg_handler.RegisterObserver(&this->_hello_obs);
  this->_msg_handler.RegisterObserver(&this->_bye_obs);
  this->_msg_handler.RegisterObserver(&this->_ack_obs);
  this->_msg_handler.RegisterEvent(this);
}

MessageSocket::~MessageSocket()
{
  ZLOG_DEBUG("Destroying message socket: '" + ZLOG_P(this) + "'");
  this->_sock_handler.UnregisterObserver(this);
  this->_msg_handler.UnregisterEvent(this);
  this->_msg_handler.UnregisterObserver(&this->_hello_obs);
  this->_msg_handler.UnregisterObserver(&this->_bye_obs);
  this->_msg_handler.UnregisterObserver(&this->_ack_obs);
}

bool
MessageSocket::Listen(zSocket::Socket *sock_)
{
  bool status = false;
  if (sock_)
  {
    this->_sock_handler.RegisterEvent(sock_);
    this->_sock[sock_->GetAddress().GetAddress()] = sock_;
    status = true;
  }
  return (status);
}

bool
MessageSocket::Connect(const zSocket::Address& addr_, zSocket::Socket *sock_)
{
  ZLOG_INFO("Connecting to: " + addr_.GetAddress());

  bool status = false;
  if (sock_)
  {
    this->_sock_handler.RegisterEvent(sock_);
    this->_sock[addr_.GetAddress()] = sock_;

    // Say hello and wait for response
    HelloMessage* hello = (HelloMessage*) MessageFactory::Create(Message::TYPE_HELLO);
    if (hello)
    {
      // Register for ACK
      this->_ack_obs.RegisterForAck(hello->GetId());

      // Initialize the message and send
      hello->SetDst(addr_.GetAddress());
      if (this->Send(*hello))
      {
        AckMessage ack;
        status = this->_ack_obs.WaitForAck(hello->GetId(), ack, 500);
      }
      this->_ack_obs.UnregisterForAck(hello->GetId());
      delete (hello);
    }
  }

  return (status);
}

bool
MessageSocket::Disconnect(const zSocket::Address& addr_)
{
  ZLOG_INFO("Connecting from: " + addr_.GetAddress());

  bool status = false;

  // Say goodbye and wait for a response
  ByeMessage* bye = (ByeMessage*) MessageFactory::Create(Message::TYPE_BYE);
  if (bye)
  {
    // Register for ACK
    this->_ack_obs.RegisterForAck(bye->GetId());

    // Initialize the message and sendl
    bye->SetDst(addr_.GetAddress());
    if (this->Send(*bye))
    {
      AckMessage ack;
      status = this->_ack_obs.WaitForAck(bye->GetId(), ack, 100);
    }
    this->_ack_obs.UnregisterForAck(bye->GetId());
    delete (bye);
  }
  this->_sock.erase(addr_.GetAddress());
  return (status);
}

bool
MessageSocket::RegisterForAck(const std::string& msg_id_)
{
  return(this->_ack_obs.RegisterForAck(msg_id_));
}

bool
MessageSocket::UnregisterForAck(const std::string& msg_id_)
{
  return(this->_ack_obs.UnregisterForAck(msg_id_));
}

bool
MessageSocket::WaitForAck(const std::string& msg_id_, AckMessage& ack_, uint32_t ms_)
{
  return(this->_ack_obs.WaitForAck(msg_id_, ack_, ms_));
}

bool
MessageSocket::Send(zMessage::Message &msg_)
{
  bool status = false;

  // Convert destination address
  zSocket::Address* dst = zSocket::SocketAddressFactory::Create(msg_.GetDst());
  if (dst)
  {
    std::map<std::string, zSocket::Socket*>::iterator it = this->_sock.find(dst->GetAddress());
    if (it != this->_sock.end())
    {
      // Update the message source address
      msg_.SetSrc(it->second->GetAddress().GetAddress());

      // Send message
      status = (it->second->Send(*dst, msg_.GetJson()) == msg_.GetJson().size());
      delete (dst);
    }
  }

  // Return status
  return (status);
}

bool
MessageSocket::EventHandler(zEvent::Notification* notification_)
{

  ZLOG_DEBUG("Handling socket event");

  bool status = false;
  if (notification_ && (notification_->GetType() == zEvent::Event::TYPE_SOCKET))
  {
    status = this->EventHandler((zSocket::Notification *) notification_);
  }
  return (status);
}

bool
MessageSocket::EventHandler(zSocket::Notification* notification_)
{
  bool status = false;
  switch (notification_->GetSubType())
  {
  case zSocket::Notification::SUBTYPE_PKT_RCVD:
  {
    zSocket::AddressBufferPair p = notification_->Pkt();
    // Update address / socket mapping
    this->_sock[p.first->GetAddress()] = (zSocket::Socket*) notification_->GetEvent();
    // Create message from package string
    zMessage::Message *msg = MessageFactory::Create(p.second->Str());
    if (msg)
    {
      zMessage::MessageNotification notification(this);
      notification.id(zMessage::MessageNotification::ID_MSG_RCVD);
      notification.type(msg->GetType());
      notification.message(msg);
      this->NotifyHandlers(&notification);
      delete (msg);
      status = true;
    }
    break;
  }
  case zSocket::Notification::SUBTYPE_PKT_SENT:
  {
    zSocket::AddressBufferPair p = notification_->Pkt();
    zMessage::Message *msg = MessageFactory::Create(p.second->Str());
    if (msg)
    {
      zMessage::MessageNotification notification(this);
      notification.id(zMessage::MessageNotification::ID_MSG_SENT);
      notification.type(msg->GetType());
      notification.message(msg);
      this->NotifyHandlers(&notification);
      delete (msg);
      status = true;
    }
    break;
  }
  default:
    status = false;
    break;
  }
  return (status);
}

}
}

