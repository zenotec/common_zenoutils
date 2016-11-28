/*
 * zMessage.h
 *
 *  Created on: Feb 4, 2014
 *      Author: kmahoney
 */

#ifndef _ZMESSAGESOCKET_H_
#define _ZMESSAGESOCKET_H_

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

  zMessage::MessageSocket*
  Sock() const;

  zMessage::Message*
  Message() const;

protected:

  bool
  id(MessageNotification::ID id_);

  bool
  message(zMessage::Message* msg_);

private:

  MessageNotification::ID _id;
  zMessage::Message* _msg;

};

}
}

#endif /* _ZMESSAGESOCKET_H_ */

