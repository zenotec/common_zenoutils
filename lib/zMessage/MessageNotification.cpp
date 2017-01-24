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
// Class: MessageNotification
//**********************************************************************

MessageNotification::MessageNotification(zMessage::MessageSocket *sock_) :
    _id(MessageNotification::ID_NONE), zEvent::EventNotification(sock_)
{
}

MessageNotification::~MessageNotification()
{
}

MessageNotification::ID
MessageNotification::Id() const
{
  return (this->_id);
}

bool
MessageNotification::id(MessageNotification::ID id_)
{
  this->_id = id_;
  return (true);
}

Message::TYPE
MessageNotification::MessageType() const
{
  return (this->_type);
}

bool
MessageNotification::type(Message::TYPE type_)
{
  this->_type = type_;
  return (true);
}

zMessage::MessageSocket*
MessageNotification::Sock() const
{
  return (static_cast<zMessage::MessageSocket*>(this->GetEvent()));
}

zMessage::Message*
MessageNotification::GetMessage() const
{
  return (this->_msg);
}

bool
MessageNotification::message(zMessage::Message* msg_)
{
  this->_msg = msg_;
  return (true);
}

}
}

