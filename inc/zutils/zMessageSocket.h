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

#ifndef __ZMESSAGESOCKET_H__
#define __ZMESSAGESOCKET_H__

#include <string>
#include <map>

#include <zutils/zEvent.h>
#include <zutils/zSocket.h>
#include <zutils/zMessage.h>
#include <zutils/zHelloMessage.h>
#include <zutils/zByeMessage.h>
#include <zutils/zAckMessage.h>

namespace zUtils
{
namespace zMessage
{

//**********************************************************************
// Class: MessageSocket
//**********************************************************************

class MessageSocket : public zEvent::Event, public zEvent::Observer
{

public:

  MessageSocket();

  virtual
  ~MessageSocket();

  bool
  Listen(zSocket::Socket *sock_);

  bool
  Connect(const zSocket::Address& addr_, zSocket::Socket *sock_);

  bool
  Disconnect(const zSocket::Address& addr_);

  bool
  RegisterForAck(const std::string& msg_id_);

  bool
  UnregisterForAck(const std::string& msg_id_);

  bool
  WaitForAck(const std::string& msg_id_, AckMessage& ack_, uint32_t ms_);

  bool
  Send(zMessage::Message &msg_);

protected:

  virtual bool
  EventHandler(zEvent::Notification* notification_);

private:

  bool
  EventHandler(zSocket::Notification* notification_);

  std::map<std::string, zSocket::Socket*> _sock;
  zEvent::Handler _sock_handler;
  zEvent::Handler _msg_handler;
  HelloObserver _hello_obs;
  ByeObserver _bye_obs;
  AckObserver _ack_obs;

};

//**********************************************************************
// Class: MessageNotification
//**********************************************************************

class MessageNotification : public zEvent::Notification
{

  friend MessageSocket;

public:

  enum ID
  {
    ID_ERR = -1,
    ID_NONE = 0,
    ID_MSG_RCVD = 1,
    ID_MSG_SENT = 2,
    ID_MSG_ERR = 3,
    ID_LAST
  };

  MessageNotification(zMessage::MessageSocket *sock_);

  virtual
  ~MessageNotification();

  MessageNotification::ID
  Id() const;

  Message::TYPE
  MessageType() const;

  zMessage::MessageSocket*
  Sock() const;

  zMessage::Message*
  GetMessage() const;

protected:

  bool
  id(MessageNotification::ID id_);

  bool
  type(Message::TYPE type_);

  bool
  message(zMessage::Message* msg_);

private:

  MessageNotification::ID _id;
  Message::TYPE _type;
  zMessage::Message* _msg;

};

}
}

#endif /* __ZMESSAGESOCKET_H__ */

