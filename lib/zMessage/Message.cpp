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

const std::string Message::DataIdDefault("");

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

const std::string Message::DataDstDefault("");
const std::string Message::DataSrcDefault("");

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

Message::Message(Message &other_) :
    zData::Data(other_)
{
  ZLOG_DEBUG("Message::Message(other_)");
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

std::string
Message::GetId() const
{
  std::string id;
  if (!this->GetValue(MessagePath(MessagePath::IdDataPath), id))
  {
    id = DataIdDefault;
  }
  return (id);
}

bool
Message::SetId(const std::string &id_)
{
  return (this->PutValue(MessagePath(MessagePath::IdDataPath), id_));
}

Message::TYPE
Message::GetType() const
{
  std::string type;
  if (this->GetValue(MessagePath(MessagePath::TypeDataPath), type))
  {
    if (type == Message::DataTypeAuth)
    {
      return (Message::TYPE_AUTH);
    }
    else if (type == Message::DataTypeHello)
    {
      return (Message::TYPE_HELLO);
    }
    else if (type == Message::DataTypeAck)
    {
      return (Message::TYPE_ACK);
    }
    else if (type == Message::DataTypeBye)
    {
      return (Message::TYPE_BYE);
    }
    else if (type == Message::DataTypeNode)
    {
      return (Message::TYPE_NODE);
    }
    else if (type == Message::DataTypeConfig)
    {
      return (Message::TYPE_CFG);
    }
    else if (type == Message::DataTypeCommand)
    {
      return (Message::TYPE_CMD);
    }
    else if (type == Message::DataTypeData)
    {
      return (Message::TYPE_DATA);
    }
    else if (type == Message::DataTypeNone)
    {
      return (Message::TYPE_NONE);
    }
    else
    {
      return (Message::TYPE_ERR);
    }
  }
  return (Message::TYPE_DEFAULT);
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
  std::string dst;
  if (!this->GetValue(MessagePath(MessagePath::DstDataPath), dst))
  {
    dst = DataDstDefault;
  }
  return (dst);
}

bool
Message::SetDst(const std::string &to_)
{
  return (this->PutValue(MessagePath(MessagePath::DstDataPath), to_));
}

std::string
Message::GetSrc() const
{
  std::string src;
  if (!this->GetValue(MessagePath(MessagePath::SrcDataPath), src))
  {
    src = DataSrcDefault;
  }
  return (src);
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
