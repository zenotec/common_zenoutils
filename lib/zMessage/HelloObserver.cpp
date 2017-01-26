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
#include <zutils/zMessageSocket.h>
#include <zutils/zHelloMessage.h>
#include <zutils/zAckMessage.h>

namespace zUtils
{
namespace zMessage
{

//**********************************************************************
// Class: HelloObserver
//**********************************************************************

HelloObserver::HelloObserver()
{
}

HelloObserver::~HelloObserver()
{
}

bool
HelloObserver::EventHandler(zEvent::EventNotification* notification_)
{

  bool status = false;
  if (notification_ && (notification_->Type() == zEvent::Event::TYPE_MSG))
  {
    status = this->EventHandler(static_cast<zMessage::MessageNotification *>(notification_));
  }

  return (status);
}

bool
HelloObserver::EventHandler(zMessage::MessageNotification* notification_)
{

  bool status = false;
  switch (notification_->Id())
  {
  case zMessage::MessageNotification::ID_MSG_RCVD:
  {
    if (notification_->MessageType() == zMessage::Message::TYPE_HELLO)
    {
      HelloMessage *hello = static_cast<HelloMessage*>(notification_->GetMessage());
      if (hello)
      {
        ZLOG_INFO("Received hello from: " + hello->GetSrc());
        AckMessage *ack = (AckMessage *) MessageFactory::Create(Message::TYPE_ACK);
        if (ack)
        {
          ack->SetId(hello->GetId());
          ack->SetSrc(hello->GetDst());
          ack->SetDst(hello->GetSrc());
          status = notification_->Sock()->Send(*ack);
          delete(ack);
        }
      }
    }
    break;
  }
  case zMessage::MessageNotification::ID_MSG_SENT:
  {
    if (notification_->MessageType() == zMessage::Message::TYPE_HELLO)
    {
      status = true;
    }
    break;
  }
  default:
    break;
  }

  return (status);
}

}
}

