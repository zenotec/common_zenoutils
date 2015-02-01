/*
 * zMessage.h
 *
 *  Created on: Feb 4, 2014
 *      Author: kmahoney
 */

#ifndef ZMESSAGE_H_
#define ZMESSAGE_H_

#include <zutils/zData.h>
#include <zutils/zNode.h>
#include <zutils/zSocket.h>

namespace zUtils
{
namespace zMessage
{

class Handler;

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
  static const std::string STR_TYPE_CFG;
  static const std::string STR_TYPE_CMD;
  static const std::string STR_TYPE_DATA;
  static const std::string STR_ID;
  static const std::string STR_TO;
  static const std::string STR_FROM;
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
    TYPE_CFG = 5,
    TYPE_CMD = 6,
    TYPE_DATA = 7,
    TYPE_LAST
  };

  Message();

  Message(const zData::Data &msg_);

  virtual
  ~Message();

  Message::TYPE
  GetType() const;
  bool
  SetType(const Message::TYPE &type_);

  std::string
  GetId() const;
  bool
  SetId(const std::string &id_);

  zNode::Node
  GetTo() const;
  bool
  SetTo(const zNode::Node &to_);

  zNode::Node
  GetFrom() const;
  bool
  SetFrom(const zNode::Node &from_);

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
// zMessage::Observer Class
//**********************************************************************

class Observer
{
public:

  virtual bool
  RecvMsg(zMessage::Handler &handler_, zMessage::Message &msg_) = 0;

};

//**********************************************************************
// zMessage::Handler Class
//**********************************************************************

class Handler : public zSocket::Handler, public zSocket::Observer
{

public:
  Handler();
    ~Handler();

  bool
  Register(zMessage::Message::TYPE type_, zMessage::Observer *obs_);
  bool
  Unregister(zMessage::Message::TYPE type_, zMessage::Observer *obs_);

  bool
  Send(zMessage::Message &msg_);

protected:

  virtual bool
  SocketRecv(zSocket::Socket *sock_, const zSocket::Address &addr_, zSocket::Buffer &buf_);

private:

  void
  _notify(zMessage::Message::TYPE type, zMessage::Message &msg_);

  std::map<zMessage::Message::TYPE, std::list<zMessage::Observer *> > _obsMap;
  std::map<std::string, std::pair<zSocket::Socket *, zSocket::Address> > _addrMap;

};

}
}

#endif /* ZMESSAGE_H_ */

