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

#include <zutils/zUuid.h>
#include <zutils/zMessage.h>
#include <zutils/zHelloMessage.h>
#include <zutils/zByeMessage.h>
#include <zutils/zAckMessage.h>
#include <zutils/zCommandMessage.h>

namespace zUtils
{
namespace zMessage
{

//**********************************************************************
// MessageFactory
//**********************************************************************

Message *
MessageFactory::Create(const Message::TYPE &type_)
{
  Message *msg = NULL;

  // Yes, this might look questionable, but it serves two purposes. The
  //   first is to validate the message type being passed in. The second
  //   is that it is a placeholder for future enhancements

  switch (type_)
  {
  case Message::TYPE_HELLO:
    msg = new HelloMessage;
    break;
  case Message::TYPE_BYE:
    msg = new ByeMessage;
    break;
  case Message::TYPE_ACK:
    msg = new AckMessage;
    break;
  case Message::TYPE_CMD:
    msg = new CommandMessage;
    break;
  case Message::TYPE_AUTH:
    // No break
  case Message::TYPE_CFG:
    // No break
  case Message::TYPE_DATA:
    msg = new Message;
    msg->SetType(type_);
    break;
  default:
    break;
  }

  if (msg && msg->GetId().empty())
  {
    msg->SetId(zUuid::Uuid::Create());
  }
  return (msg);
}

zMessage::Message *
MessageFactory::Create(const std::string& json_)
{
  zData::Data data(MessagePath::DataRoot);
  data.SetJson(json_);
  return (MessageFactory::Create(data));
}

zMessage::Message *
MessageFactory::Create(const zData::Data& data_)
{
  zMessage::Message msg(data_);
  return (MessageFactory::Create(msg));
}

zMessage::Message *
MessageFactory::Create(const zMessage::Message& msg_)
{
  zMessage::Message *msg = MessageFactory::Create(msg_.GetType());
  if (msg)
  {
    *msg = msg_;
  }
  return (msg);
}

}
}

