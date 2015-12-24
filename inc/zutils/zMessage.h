/*
 * zMessage.h
 *
 *  Created on: Feb 4, 2014
 *      Author: kmahoney
 */

#ifndef ZMESSAGE_H_
#define ZMESSAGE_H_

#include <map>

#include <zutils/zData.h>
#include <zutils/zSocket.h>
#include <zutils/zEvent.h>

namespace zUtils
{
namespace zMessage
{

class MessageHandler;

//**********************************************************************
// zMessage::Message Class
//**********************************************************************

class Message : public zData::Data
{

  static const std::string STR_ROOT;
  static const std::string STR_TYPE;
  static const std::string STR_TYPE_AUTH;
  static const std::string STR_TYPE_HELLO;
  static const std::string STR_TYPE_ACK;
  static const std::string STR_TYPE_BYE;
  static const std::string STR_TYPE_NODE;
  static const std::string STR_TYPE_CFG;
  static const std::string STR_TYPE_CMD;
  static const std::string STR_TYPE_DATA;
  static const std::string STR_ID;
  static const std::string STR_DST;
  static const std::string STR_SRC;
  static const std::string STR_DATA;

public:

  enum TYPE
  {
    TYPE_ERR = -1,
    TYPE_NONE = 0,
    TYPE_AUTH = 1,
    TYPE_HELLO = 2,
    TYPE_ACK = 3,
    TYPE_BYE = 4,
    TYPE_NODE = 5,
    TYPE_CFG = 6,
    TYPE_CMD = 7,
    TYPE_DATA = 8,
    TYPE_LAST
  };

  Message();

  Message(const zData::Data &msg_);

  Message(const std::string &json_);

  Message(Message &other_) :
      zData::Data(other_)
  {
  }

  Message(const Message &other_) :
      zData::Data(other_)
  {
  }

  virtual
  ~Message();

  Message &
  operator=(Message &other_)
  {
    zData::Data::operator =(other_);
    return (*this);
  }

  Message &
  operator=(const Message &other_)
  {
    zData::Data::operator =(other_);
    return (*this);
  }

  Message::TYPE
  GetType() const;

  bool
  SetType(const Message::TYPE &type_);

  std::string
  GetId() const;

  bool
  SetId(const std::string &id_);

  std::string
  GetDst() const;

  bool
  SetDst(const std::string &dst_);

  std::string
  GetSrc() const;

  bool
  SetSrc(const std::string &src_);

  zData::Data
  GetData() const;

  bool
  SetData(const zData::Data &data_);

protected:

private:

};

//**********************************************************************
// zMessage::Factory Class
//**********************************************************************

class Factory
{
public:
  static zMessage::Message *
  Create(const zMessage::Message::TYPE &type_);
};

//**********************************************************************
// zMessage::MessageEvent Class
//**********************************************************************

class MessageEvent : public zEvent::Event
{
public:

  enum EVENTID
  {
    EVENTID_ERR = -1,
    EVENTID_NONE = 0,
    EVENTID_MSG_RCVD = 1,
    EVENTID_MSG_SENT = 2,
    EVENTID_MSG_ERR = 3,
    EVENTID_LAST
  };

  MessageEvent(const MessageEvent::EVENTID id_);

  virtual
  ~MessageEvent();

protected:

private:
  MessageEvent(MessageEvent &other_);

  MessageEvent(const MessageEvent &other_);

  MessageEvent &
  operator=(MessageEvent &other_);

  MessageEvent &
  operator=(const MessageEvent &other_);

};

//**********************************************************************
// zMessage::MessageSocket Class
//**********************************************************************

class MessageSocket : public zEvent::EventHandler, private zEvent::EventObserver
{

public:
  MessageSocket(zSocket::Socket *sock_);

  virtual
  ~MessageSocket();

  bool
  Send(zMessage::Message &msg_);

protected:

  virtual bool
  EventHandler(zEvent::Event *event_, void *arg_);

private:

  zSocket::Socket *_sock;

  zEvent::Event _rx_event;
  zEvent::Event _tx_event;
  zEvent::Event _err_event;

  MessageSocket(MessageSocket &other_);

  MessageSocket(const MessageSocket &other_);

  MessageSocket &
  operator=(MessageSocket &other_);

  MessageSocket &
  operator=(const MessageSocket &other_);

};

//**********************************************************************
// zMessage::MessageHandler Class
//**********************************************************************

class MessageHandler : public zEvent::EventHandler, private zEvent::EventObserver
{

public:
  MessageHandler(zMessage::Message::TYPE type_, zMessage::MessageSocket *sock_);

  virtual
  ~MessageHandler();

  bool
  Send(zMessage::Message &msg_);

protected:

  zMessage::MessageSocket *_sock;

  virtual bool
  EventHandler(zEvent::Event *event_, void *arg_);

private:

  zMessage::Message::TYPE _type;
  zSem::Mutex _lock;

  zEvent::Event _rx_event;
  zEvent::Event _tx_event;
  zEvent::Event _err_event;

  MessageHandler(MessageHandler &other_);

  MessageHandler(const MessageHandler &other_);

  MessageHandler &
  operator=(MessageHandler &other_);

  MessageHandler &
  operator=(const MessageHandler &other_);

};

}
}

#endif /* ZMESSAGE_H_ */

