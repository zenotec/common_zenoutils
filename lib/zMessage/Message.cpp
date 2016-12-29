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

const std::string Message::DataIdDefault("");
const std::string Message::DataDstDefault("");
const std::string Message::DataSrcDefault("");

Message::Message() :
    zData::Data(MessagePath::DataRoot)
{
  // Initialize message
  this->SetId(this->GetId());
  this->SetType(this->GetType());
  this->SetDst(this->GetDst());
  this->SetSrc(this->GetSrc());
  this->SetData(this->GetData());
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

Message::Message(Message &other_) :
    zData::Data(other_)
{

}

Message::Message(const Message &other_) :
    zData::Data(other_)
{
}

Message::~Message()
{
}

std::string
Message::GetId() const
{
  std::string id;
  MessagePath path;
  if (!this->Get(path.Id(), id))
  {
    id = DataIdDefault;
  }
  return (id);
}

bool
Message::SetId(const std::string &id_)
{
  MessagePath path;
  return (this->Put(path.Id(), id_));
}

Message::TYPE
Message::GetType() const
{
  std::string type;
  MessagePath path;

  if (this->Get(path.Type(), type))
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
  bool status = false;
  MessagePath path;
  switch (type_)
  {
  case Message::TYPE_AUTH:
    status = this->Put(path.Type(), Message::DataTypeAuth);
    break;
  case Message::TYPE_HELLO:
    status = this->Put(path.Type(), Message::DataTypeHello);
    break;
  case Message::TYPE_ACK:
    status = this->Put(path.Type(), Message::DataTypeAck);
    break;
  case Message::TYPE_BYE:
    status = this->Put(path.Type(), Message::DataTypeBye);
    break;
  case Message::TYPE_NODE:
    status = this->Put(path.Type(), Message::DataTypeNode);
    break;
  case Message::TYPE_CFG:
    status = this->Put(path.Type(), Message::DataTypeConfig);
    break;
  case Message::TYPE_CMD:
    status = this->Put(path.Type(), Message::DataTypeCommand);
    break;
  case Message::TYPE_DATA:
    status = this->Put(path.Type(), Message::DataTypeData);
    break;
  case Message::TYPE_NONE:
    status = this->Put(path.Type(), Message::DataTypeNone);
    break;
  default:
    this->Put(path.Type(), std::string(""));
    status = false;
    break;
  }
  return (status);
}

std::string
Message::GetDst() const
{
  std::string dst;
  MessagePath path;
  if (!this->Get(path.Dst(), dst))
  {
    dst = DataDstDefault;
  }
  return (dst);
}

bool
Message::SetDst(const std::string &to_)
{
  MessagePath path;
  return (this->Put(path.Dst(), to_));
}

std::string
Message::GetSrc() const
{
  std::string src;
  MessagePath path;
  if (!this->Get(path.Src(), src))
  {
    src = DataSrcDefault;
  }
  return (src);
}

bool
Message::SetSrc(const std::string &from_)
{
  MessagePath path;
  return (this->Put(path.Src(), from_));
}

zData::Data
Message::GetData() const
{
  zData::Data data;
  MessagePath path;
  this->Get(path.Data(), data);
  return (data);
}

bool
Message::SetData(const zData::Data &data_)
{
  MessagePath path;
  return (this->Put(path.Data(), data_));
}

}
}
