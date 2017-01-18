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

const std::string Message::DataTypeNone("None");
const std::string Message::DataTypeAuth("Auth");
const std::string Message::DataTypeHello("Hello");
const std::string Message::DataTypeAck("Ack");
const std::string Message::DataTypeBye("Bye");
const std::string Message::DataTypeNode("Node");
const std::string Message::DataTypeConfig("Config");
const std::string Message::DataTypeCommand("Cmd");
const std::string Message::DataTypeData("Data");
const std::string Message::DataTypeDefault(DataTypeNone);

Message::Message() :
    zData::Data(MessagePath::DataRoot)
{
  ZLOG_DEBUG("Message::Message()");
  ZLOG_DEBUG(this->Path());
  ZLOG_DEBUG(this->GetJson());
}

Message::Message(const zData::Data &data_) :
    zData::Data(MessagePath::DataRoot)
{
  this->PutChild(data_);
  ZLOG_DEBUG("Message::Message(data_)");
  ZLOG_DEBUG(this->Path());
  ZLOG_DEBUG(this->GetJson());
}

Message::Message(const Message &other_) :
    zData::Data(other_)
{
  ZLOG_DEBUG("Message::Message(other_)");
  ZLOG_DEBUG(this->Path());
  ZLOG_DEBUG(this->GetJson());
}

Message::~Message()
{
}

Message &
Message::operator=(const Message &other_)
{
  zData::Data::operator =(other_);
  return (*this);
}

bool
Message::operator ==(const Message& other_) const
    {
  return (zData::Data::operator ==(other_));
}

bool
Message::operator !=(const Message& other_) const
    {
  return (zData::Data::operator !=(other_));
}

std::string
Message::GetId() const
{
  return (this->GetValue<std::string>(MessagePath::IdDataPath));
}

bool
Message::SetId(const std::string &id_)
{
  return (this->PutValue(MessagePath(MessagePath::IdDataPath), id_));
}

Message::TYPE
Message::GetType() const
{
  Message::TYPE type = Message::TYPE_DEFAULT;
  std::string value = this->GetValue<std::string>(MessagePath::TypeDataPath);

  if (value == Message::DataTypeAuth)
  {
    type = Message::TYPE_AUTH;
  }
  else if (value == Message::DataTypeHello)
  {
    type = Message::TYPE_HELLO;
  }
  else if (value == Message::DataTypeAck)
  {
    type = Message::TYPE_ACK;
  }
  else if (value == Message::DataTypeBye)
  {
    type = Message::TYPE_BYE;
  }
  else if (value == Message::DataTypeNode)
  {
    type = Message::TYPE_NODE;
  }
  else if (value == Message::DataTypeConfig)
  {
    type = Message::TYPE_CFG;
  }
  else if (value == Message::DataTypeCommand)
  {
    type = Message::TYPE_CMD;
  }
  else if (value == Message::DataTypeData)
  {
    type = Message::TYPE_DATA;
  }
  else if (value == Message::DataTypeNone)
  {
    type = Message::TYPE_NONE;
  }
  else
  {
    type = Message::TYPE_DEFAULT;
  }
  return(type);
}

bool
Message::SetType(const Message::TYPE &type_)
{
  std::string value;
  switch (type_)
  {
  case Message::TYPE_AUTH:
    value = Message::DataTypeAuth;
    break;
  case Message::TYPE_HELLO:
    value = Message::DataTypeHello;
    break;
  case Message::TYPE_ACK:
    value = Message::DataTypeAck;
    break;
  case Message::TYPE_BYE:
    value = Message::DataTypeBye;
    break;
  case Message::TYPE_NODE:
    value = Message::DataTypeNode;
    break;
  case Message::TYPE_CFG:
    value = Message::DataTypeConfig;
    break;
  case Message::TYPE_CMD:
    value = Message::DataTypeCommand;
    break;
  case Message::TYPE_DATA:
    value = Message::DataTypeData;
    break;
  case Message::TYPE_NONE:
    value = Message::DataTypeNone;
    break;
  default:
    return (false);
    break;
  }
  return (this->PutValue(MessagePath(MessagePath::TypeDataPath), value));
}

std::string
Message::GetDst() const
{
  return (this->GetValue<std::string>(MessagePath::DstDataPath));
}

bool
Message::SetDst(const std::string &to_)
{
  return (this->PutValue(MessagePath(MessagePath::DstDataPath), to_));
}

std::string
Message::GetSrc() const
{
  return (this->GetValue<std::string>(MessagePath::SrcDataPath));
}

bool
Message::SetSrc(const std::string &from_)
{
  return (this->PutValue(MessagePath(MessagePath::SrcDataPath), from_));
}

zData::Data
Message::GetData() const
{
  return (this->operator ()(MessagePath::DataDataPath));
}

bool
Message::SetData(const zData::Data &data_)
{
  return (this->PutChild(MessagePath(MessagePath::DataDataPath), data_));
}

}
}
