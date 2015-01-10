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

const std::string Message::ROOT = "zMessage";
const std::string Message::ID = "Id";
const std::string Message::TYPE = "Type";
const std::string Message::TO = "To";
const std::string Message::FROM = "From";
const std::string Message::DATA = "Data";

Message::Message() :
    zData::Data(Message::ROOT)
{
  zData::Data data(Message::DATA);
  zNode::Node node;

//    std::cout << std::endl << "Message::Message(): " << std::endl << this->GetJson() << std::endl;

// Initialize message
  this->SetId("");
  this->SetType("");
  this->SetTo(node);
  this->SetFrom(node);
  this->SetData(data);

//    std::cout << "Message::Message(): " << std::endl << this->GetJson() << std::endl;
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
  return (this->GetValue(Message::ID));
}

void
Message::SetId(const std::string &id_)
{
  this->SetValue(Message::ID, id_);
}

std::string
Message::GetType() const
{
  return (this->GetValue(Message::TYPE));
}

void
Message::SetType(const std::string &type_)
{
  this->SetValue(Message::TYPE, type_);
}

zNode::Node
Message::GetTo() const
{
  zNode::Node to;
  this->GetChild(Message::TO, to);
  return (to);
}

void
Message::SetTo(const zNode::Node &to_)
{
//    std::cout << std::endl << "Message::SetTo(): " << std::endl << this->GetJson() << std::endl;
  this->PutChild(Message::TO, to_);
//    std::cout << "Message::SetTo(): " << std::endl << this->GetJson() << std::endl;
}

zNode::Node
Message::GetFrom() const
{
  zNode::Node from;
  this->GetChild(Message::FROM, from);
  return (from);
}

void
Message::SetFrom(const zNode::Node &from_)
{
//    std::cout << std::endl << "Message::SetFrom(): " << std::endl << this->GetJson() << std::endl;
  this->PutChild(Message::FROM, from_);
//    std::cout << "Message::SetFrom(): " << std::endl << this->GetJson() << std::endl;
}

zData::Data
Message::GetData() const
{
  zData::Data data(Message::DATA);
  this->GetChild(Message::DATA, data);
  return (data);
}

void
Message::SetData(const zData::Data &data_)
{
//    std::cout << std::endl << "Message::SetData(): " << std::endl << this->GetJson() << std::endl;
  this->PutChild(Message::DATA, data_);
//    std::cout << "Message::SetData(): " << std::endl << this->GetJson() << std::endl;
}

//**********************************************************************
// MessageFactory
//**********************************************************************

MessageFactory::MessageFactory()
{
}

MessageFactory::~MessageFactory()
{
}

Message *
MessageFactory::Create(const std::string &type_)
{
  if (type_ == HelloMessage::TYPE)
  {
    return (new HelloMessage());
  } // end if
  else if (type_ == AckMessage::TYPE)
  {
    return (new AckMessage());
  } // end else if
  else if (type_ == ByeMessage::TYPE)
  {
    return (new ByeMessage());
  } // end else if
  else
  {
    return ( NULL);
  } // end else
}

}
}

