/*
 * Message.cpp
 *
 *  Created on: Jan 19, 2014
 *      Author: kmahoney
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

namespace zUtils
{
namespace zMessage
{

//**********************************************************************
// Message
//**********************************************************************

const std::string Message::STR_ROOT = "zMessage";
const std::string Message::STR_TYPE = "Type";
const std::string Message::STR_TYPE_AUTH = "Auth";
const std::string Message::STR_TYPE_HELLO = "Hello";
const std::string Message::STR_TYPE_ACK = "Ack";
const std::string Message::STR_TYPE_BYE = "Bye";
const std::string Message::STR_TYPE_NODE = "Node";
const std::string Message::STR_TYPE_CFG = "Cfg";
const std::string Message::STR_TYPE_CMD = "Cmd";
const std::string Message::STR_TYPE_DATA = "Data";
const std::string Message::STR_ID = "Id";
const std::string Message::STR_DST = "Dst";
const std::string Message::STR_SRC = "Src";
const std::string Message::STR_DATA = "zData";

Message::Message() :
    zData::Data(Message::STR_ROOT)
{
  zData::Data data(Message::STR_DATA);

  // Initialize message
  this->SetId("");
  this->SetType(Message::TYPE_NONE);
  this->SetDst("");
  this->SetSrc("");
  this->SetData(data);

}

Message::Message(const zData::Data &msg_) :
    zData::Data(msg_)
{
  // Initialize message
  this->SetId(this->GetId());
  this->SetType(this->GetType());
  this->SetDst(this->GetDst());
  this->SetSrc(this->GetSrc());
  this->SetData(this->GetData());
}

Message::Message(const std::string &json_)
{
  // Initialize message
  this->SetJson(json_);
  this->SetId(this->GetId());
  this->SetType(this->GetType());
  this->SetDst(this->GetDst());
  this->SetSrc(this->GetSrc());
  this->SetData(this->GetData());
}

Message::~Message()
{
}

std::string
Message::GetId() const
{
  std::string id;
  this->Get(id, Message::STR_ID);
  return (id);
}

bool
Message::SetId(const std::string &id_)
{
  return (this->Put(id_, Message::STR_ID));
}

Message::TYPE
Message::GetType() const
{
  std::string type;
  this->Get(type, Message::STR_TYPE);
  if (type == Message::STR_TYPE_AUTH)
  {
    return (Message::TYPE_AUTH);
  }
  else if (type == Message::STR_TYPE_HELLO)
  {
    return (Message::TYPE_HELLO);
  }
  else if (type == Message::STR_TYPE_ACK)
  {
    return (Message::TYPE_ACK);
  }
  else if (type == Message::STR_TYPE_BYE)
  {
    return (Message::TYPE_BYE);
  }
  else if (type == Message::STR_TYPE_NODE)
  {
    return (Message::TYPE_NODE);
  }
  else if (type == Message::STR_TYPE_CFG)
  {
    return (Message::TYPE_CFG);
  }
  else if (type == Message::STR_TYPE_CMD)
  {
    return (Message::TYPE_CMD);
  }
  else if (type == Message::STR_TYPE_DATA)
  {
    return (Message::TYPE_DATA);
  }
  else if (type == std::string(""))
  {
    return (Message::TYPE_NONE);
  }
  else
  {
    return (Message::TYPE_ERR);
  }
}

bool
Message::SetType(const Message::TYPE &type_)
{
  bool status = false;
  switch (type_)
  {
  case Message::TYPE_AUTH:
    status = this->Put(Message::STR_TYPE_AUTH, Message::STR_TYPE);
    break;
  case Message::TYPE_HELLO:
    status = this->Put(Message::STR_TYPE_HELLO, Message::STR_TYPE);
    break;
  case Message::TYPE_ACK:
    status = this->Put(Message::STR_TYPE_ACK, Message::STR_TYPE);
    break;
  case Message::TYPE_BYE:
    status = this->Put(Message::STR_TYPE_BYE, Message::STR_TYPE);
    break;
  case Message::TYPE_NODE:
    status = this->Put(Message::STR_TYPE_NODE, Message::STR_TYPE);
    break;
  case Message::TYPE_CFG:
    status = this->Put(Message::STR_TYPE_CFG, Message::STR_TYPE);
    break;
  case Message::TYPE_CMD:
    status = this->Put(Message::STR_TYPE_CMD, Message::STR_TYPE);
    break;
  case Message::TYPE_DATA:
    status = this->Put(Message::STR_TYPE_DATA, Message::STR_TYPE);
    break;
  case Message::TYPE_NONE:
    status = this->Put(std::string(""), Message::STR_TYPE);
    break;
  default:
    status = false;
    break;
  }
  return (status);
}

std::string
Message::GetDst() const
{
  std::string dst;
  this->Get(dst, Message::STR_DST);
  return (dst);
}

bool
Message::SetDst(const std::string &to_)
{
  return (this->Put(to_, Message::STR_DST));
}

std::string
Message::GetSrc() const
{
  std::string src;
  this->Get(src, Message::STR_SRC);
  return (src);
}

bool
Message::SetSrc(const std::string &from_)
{
  return (this->Put(from_, Message::STR_SRC));
}

zData::Data
Message::GetData() const
{
  zData::Data data;
  this->Get(data, Message::STR_DATA);
  return (data);
}

bool
Message::SetData(const zData::Data &data_)
{
  //return (this->Put(data_, Message::STR_DATA));
}

}
}
