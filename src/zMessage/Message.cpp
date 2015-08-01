/*
 * Message.cpp
 *
 *  Created on: Jan 19, 2014
 *      Author: kmahoney
 */

#include "zutils/zMessage.h"

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
const std::string Message::STR_TYPE_CFG = "Cfg";
const std::string Message::STR_TYPE_CMD = "Cmd";
const std::string Message::STR_TYPE_DATA = "Data";
const std::string Message::STR_ID = "Id";
const std::string Message::STR_TO = "To";
const std::string Message::STR_FROM = "From";
const std::string Message::STR_DATA = "zData";

Message::Message() :
    zData::Data(Message::STR_ROOT)
{
  zData::Data data(Message::STR_DATA);

  // Initialize message
  this->SetId("");
  this->SetType(Message::TYPE_NONE);
  this->SetTo("");
  this->SetFrom("");
  this->SetData(data);

}

Message::Message(const zData::Data &msg_) :
    zData::Data(msg_)
{
}

Message::~Message()
{
}

std::string
Message::GetId() const
{
  return (this->GetValue(Message::STR_ID));
}

bool
Message::SetId(const std::string &id_)
{
  return (this->SetValue(Message::STR_ID, id_));
}

Message::TYPE
Message::GetType() const
{
  std::string type = this->GetValue(Message::STR_TYPE);
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
    status = this->SetValue(Message::STR_TYPE, Message::STR_TYPE_AUTH);
    break;
  case Message::TYPE_HELLO:
    status = this->SetValue(Message::STR_TYPE, Message::STR_TYPE_HELLO);
    break;
  case Message::TYPE_ACK:
    status = this->SetValue(Message::STR_TYPE, Message::STR_TYPE_ACK);
    break;
  case Message::TYPE_BYE:
    status = this->SetValue(Message::STR_TYPE, Message::STR_TYPE_BYE);
    break;
  case Message::TYPE_CFG:
    status = this->SetValue(Message::STR_TYPE, Message::STR_TYPE_CFG);
    break;
  case Message::TYPE_CMD:
    status = this->SetValue(Message::STR_TYPE, Message::STR_TYPE_CMD);
    break;
  case Message::TYPE_DATA:
    status = this->SetValue(Message::STR_TYPE, Message::STR_TYPE_DATA);
    break;
  case Message::TYPE_NONE:
    status = this->SetValue(Message::STR_TYPE, std::string(""));
    break;
  default:
    status = false;
    break;
  }
  return (status);
}

std::string
Message::GetTo() const
{
  return (this->GetValue(Message::STR_TO));
}

bool
Message::SetTo(const std::string &to_)
{
  return (this->SetValue(Message::STR_TO, to_));
}

std::string
Message::GetFrom() const
{
  return (this->GetValue(Message::STR_FROM));
}

bool
Message::SetFrom(const std::string &from_)
{
  return (this->SetValue(Message::STR_FROM, from_));
}

zData::Data
Message::GetData() const
{
  zData::Data data(Message::STR_DATA);
  this->GetChild(Message::STR_DATA, data);
  return (data);
}

bool
Message::SetData(const zData::Data &data_)
{
  return (this->PutChild(Message::STR_DATA, data_));
}

}
}
