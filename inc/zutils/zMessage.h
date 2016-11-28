/*
 * zMessage.h
 *
 *  Created on: Feb 4, 2014
 *      Author: kmahoney
 */

#ifndef ZMESSAGE_H_
#define ZMESSAGE_H_

namespace zUtils
{
namespace zMessage
{

//**********************************************************************
// Class: Message
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
// Class: MessageFactory
//**********************************************************************

class MessageFactory
{
public:
  static zMessage::Message *
  Create(const zMessage::Message::TYPE &type_);
};
//
////**********************************************************************
//// Class: MessageObserver
////**********************************************************************
//
//class MessageObserver : public zEvent::EventObserver
//{
//
//public:
//  MessageObserver(zMessage::Message::TYPE type_);
//
//  virtual
//  ~MessageObserver();
//
//  virtual bool
//  MessageHandler(const zMessage::MessageNotification* notification_) = 0;
//
//protected:
//
//  virtual bool
//  EventHandler(const zEvent::EventNotification* notification_);
//
//private:
//
//  zMessage::Message::TYPE _type;
//
//};

}
}

#endif /* ZMESSAGE_H_ */

