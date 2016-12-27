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

#include <zutils/zEvent.h>
#include <zutils/zSocket.h>
#include <zutils/zMessage.h>

namespace zUtils
{
namespace zMessage
{

//**********************************************************************
// Class: MessageSocket
//**********************************************************************

class MessageSocket : public zEvent::Event, public zEvent::EventObserver
{

public:
  MessageSocket(zSocket::Socket *sock_);

  virtual
  ~MessageSocket();

  virtual bool
  Open();

  virtual void
  Close();

  virtual bool
  Bind();

  virtual bool
  Connect(const zSocket::SocketAddress* addr_);

  bool
  Send(const zSocket::SocketAddress& addr_, zMessage::Message &msg_);

protected:

  virtual bool
  EventHandler(const zEvent::EventNotification* notification_);

private:

  zSocket::Socket* _sock;
  zEvent::EventHandler _sock_handler;

};

//**********************************************************************
// Class: MessageNotification
//**********************************************************************

class MessageNotification : public zEvent::EventNotification
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

  const zSocket::SocketAddress&
  SrcAddr() const;

  zMessage::MessageSocket*
  Sock() const;

  zMessage::Message*
  Message() const;

protected:

  bool
  id(MessageNotification::ID id_);

  bool
  srcaddr(zSocket::SocketAddress& addr_);

  bool
  message(zMessage::Message* msg_);

private:

  MessageNotification::ID _id;
  zSocket::SocketAddress _addr;
  zMessage::Message* _msg;

};

}
}

#endif /* __ZMESSAGESOCKET_H__ */

