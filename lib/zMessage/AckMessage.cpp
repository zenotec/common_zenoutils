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

#include <string>
#include <mutex>
#include <list>
#include <map>

#include <zutils/zEvent.h>
#include <zutils/zData.h>
#include <zutils/zQueue.h>
#include <zutils/zSocket.h>
#include <zutils/zMessage.h>

#include <zutils/zAckMessage.h>

namespace zUtils
{
namespace zMessage
{

//**********************************************************************
// Class: AckMessage
//**********************************************************************

const std::string AckMessage::RespDataPath("Id");

AckMessage::AckMessage()
{
  this->SetType(zMessage::Message::TYPE_ACK);
}

AckMessage::AckMessage(const zData::Data &data_) :
    zMessage::Message(data_)
{
}

AckMessage::AckMessage(const zMessage::Message& msg_) :
    zMessage::Message(msg_)
{
}

AckMessage::~AckMessage()
{
}

const zMessage::Message&
AckMessage::GetMessage() const
{
  return (*this);
}

std::string
AckMessage::GetResponse() const
{
  return (this->GetValue<std::string>(AckMessage::RespDataPath));
}

bool
AckMessage::SetResponse(const std::string& resp_)
{
  return (this->PutValue(MessagePath(AckMessage::RespDataPath), resp_));
}

}
}

