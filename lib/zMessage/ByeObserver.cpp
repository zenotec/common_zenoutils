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
#include <zutils/zByeMessage.h>
#include <zutils/zAckMessage.h>

namespace zUtils
{
namespace zMessage
{

//**********************************************************************
// Class: ByeObserver
//**********************************************************************

ByeObserver::ByeObserver()
{
}

ByeObserver::~ByeObserver()
{
}

bool
ByeObserver::EventHandler(zEvent::Notification* notification_)
{

  bool status = false;
  if (notification_ && (notification_->GetType() == zEvent::Event::TYPE_MSG))
  {
    status = this->EventHandler(static_cast<zMessage::MessageNotification *>(notification_));
  }

  return (status);
}

bool
ByeObserver::EventHandler(zMessage::MessageNotification* notification_)
{

  bool status = false;
  switch (notification_->Id())
  {
  case zMessage::MessageNotification::ID_MSG_RCVD:
  {
    if (notification_->MessageType() == zMessage::Message::TYPE_BYE)
    {
      ByeMessage *bye = static_cast<ByeMessage*>(notification_->GetMessage());
      if (bye)
      {
        ZLOG_INFO("Received bye from: " + bye->GetSrc());
        AckMessage *ack = (AckMessage *) MessageFactory::Create(Message::TYPE_ACK);
        if (ack)
        {
          ack->SetId(bye->GetId());
          ack->SetSrc(bye->GetDst());
          ack->SetDst(bye->GetSrc());
          status = notification_->Sock()->Send(*ack);
          delete(ack);
        }
      }
    }
    break;
  }
  case zMessage::MessageNotification::ID_MSG_SENT:
  {
    if (notification_->MessageType() == zMessage::Message::TYPE_BYE)
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

