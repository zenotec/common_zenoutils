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
const std::string Message::STR_HELLO = "Hello";
const std::string Message::STR_ACK = "Ack";
const std::string Message::STR_BYE = "Bye";
const std::string Message::STR_CFG = "Cfg";
const std::string Message::STR_CMD = "Cmd";
const std::string Message::STR_ID = "Id";
const std::string Message::STR_TO = "To";
const std::string Message::STR_FROM = "From";
const std::string Message::STR_DATA = "Data";

Message::Message() :
    zData::Data(Message::STR_ROOT)
{
  zData::Data data(Message::STR_DATA);
  zNode::Node node;

  // Initialize message
  this->SetId("");
  this->SetType(Message::TYPE_NONE);
  this->SetTo(node);
  this->SetFrom(node);
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
  this->SetValue(Message::STR_ID, id_);
}

Message::TYPE
Message::GetType() const
{
  std::string type = this->GetValue(Message::STR_TYPE);
  if(type == Message::STR_HELLO)
  {
    return (Message::TYPE_HELLO);
  }
  else if(type == Message::STR_ACK)
  {
    return (Message::TYPE_ACK);
  }
  else if(type == Message::STR_BYE)
  {
    return (Message::TYPE_BYE);
  }
  else if(type == Message::STR_CFG)
  {
    return (Message::TYPE_CFG);
  }
  else if(type == Message::STR_CMD)
  {
    return (Message::TYPE_CMD);
  }
  else if(type == std::string(""))
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
  bool status = true;
  switch(type_)
  {
    case Message::TYPE_HELLO:
      this->SetValue(Message::STR_TYPE, Message::STR_HELLO);
      break;
    case Message::TYPE_ACK:
      this->SetValue(Message::STR_TYPE, Message::STR_ACK);
      break;
    case Message::TYPE_BYE:
      this->SetValue(Message::STR_TYPE, Message::STR_BYE);
      break;
    case Message::TYPE_CFG:
      this->SetValue(Message::STR_TYPE, Message::STR_CFG);
      break;
    case Message::TYPE_CMD:
      this->SetValue(Message::STR_TYPE, Message::STR_CMD);
      break;
    case Message::TYPE_NONE:
      this->SetValue(Message::STR_TYPE, std::string(""));
      break;
    default:
      status = false;
      break;
  }
  return(false);
}

zNode::Node
Message::GetTo() const
{
  zNode::Node to;
  this->GetChild(Message::STR_TO, to);
  return (to);
}

bool
Message::SetTo(const zNode::Node &to_)
{
  this->PutChild(Message::STR_TO, to_);
}

zNode::Node
Message::GetFrom() const
{
  zNode::Node from;
  this->GetChild(Message::STR_FROM, from);
  return (from);
}

bool
Message::SetFrom(const zNode::Node &from_)
{
  this->PutChild(Message::STR_FROM, from_);
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
  this->PutChild(Message::STR_DATA, data_);
}

}
}

