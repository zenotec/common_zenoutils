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
#include <zutils/zAckMessage.h>

namespace zUtils
{
namespace zMessage
{

//**********************************************************************
// Class: AckObserver
//**********************************************************************

AckObserver::AckObserver()
{
}

AckObserver::~AckObserver()
{
}

bool
AckObserver::RegisterForAck(const std::string& msg_id_)
{
  return(!this->_ack_table[msg_id_].TryWait());
}

bool
AckObserver::UnregisterForAck(const std::string& msg_id_)
{
  size_t count = this->_ack_table.erase(msg_id_);
  return ((count == 1) ? true : false);
}

bool
AckObserver::WaitForAck(const std::string& msg_id_, AckMessage& ack_, uint32_t ms_)
{
  bool status = false;
  AckMessageTable::iterator it = this->_ack_table.find(msg_id_);
  if (it != this->_ack_table.end())
  {
    if (it->second.TimedWait(ms_))
    {
      ack_ = it->second.Front();
      it->second.Pop();
      status = true;
    }
  }
  return(status);
}

bool
AckObserver::EventHandler(zEvent::Notification* notification_)
{

  bool status = false;
  if (notification_ && (notification_->GetType() == zEvent::Event::TYPE_MSG))
  {
    status = this->EventHandler(static_cast<zMessage::MessageNotification *>(notification_));
  }

  return (status);
}

bool
AckObserver::EventHandler(zMessage::MessageNotification* notification_)
{

  bool status = false;
  switch (notification_->Id())
  {
  case zMessage::MessageNotification::ID_MSG_RCVD:
    {
    if (notification_->MessageType() == zMessage::Message::TYPE_ACK)
    {
      AckMessage *ack = static_cast<AckMessage*>(notification_->GetMessage());
      if (ack)
      {
        ZLOG_INFO("Received ack from: " + ack->GetSrc());
        AckMessageTable::iterator it = this->_ack_table.find(ack->GetId());
        if (it != this->_ack_table.end())
        {
          it->second.Push(*ack);
        }
      }
    }
    break;
  }
  case zMessage::MessageNotification::ID_MSG_SENT:
    {
    if (notification_->MessageType() == zMessage::Message::TYPE_ACK)
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

