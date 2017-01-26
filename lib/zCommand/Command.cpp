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

#include <zutils/zLog.h>
#include <zutils/zSem.h>
#include <zutils/zEvent.h>
#include <zutils/zData.h>
#include <zutils/zQueue.h>
#include <zutils/zSocket.h>
#include <zutils/zMessage.h>
#include <zutils/zCommandMessage.h>
#include <zutils/zMessageSocket.h>

#include <zutils/zCommand.h>

namespace zUtils
{
namespace zCommand
{

using namespace zMessage;

//**********************************************************************
// Class: Command
//**********************************************************************

Command::Command()
{
}

Command::Command(const CommandData& data_) :
    CommandData(data_)
{
}

Command::~Command()
{
}

bool
Command::EventHandler(zEvent::EventNotification* notification_)
{

  bool status = false;
  if (notification_ && (notification_->Type() == zEvent::Event::TYPE_MSG))
  {
    status = this->EventHandler((MessageNotification*) notification_);
  }
  else if (notification_ && (notification_->Type() == zEvent::Event::TYPE_COMMAND))
  {
    status = this->EventHandler((zCommand::CommandNotification*) notification_);
  }
  else
  {
    ZLOG_WARN("Unknown event: " + ZLOG_INT(notification_->Type()));
  }

  return (status);
}

bool
Command::EventHandler(MessageNotification* notification_)
{
  bool status = false;

  if (notification_->MessageType() == Message::TYPE_CMD)
  {
    CommandMessage* cmdmsg = (CommandMessage*) notification_->GetMessage();
    if (cmdmsg)
    {
      switch (notification_->Id())
      {
      case MessageNotification::ID_MSG_RCVD:
      {
        ZLOG_INFO("Received command from: " + cmdmsg->GetSrc());
        zCommand::CommandData data(cmdmsg->GetCommandData());
        if (data == *this)
        {
          status = this->Execute(data);
          AckMessage* ack = (AckMessage *) MessageFactory::Create(Message::TYPE_ACK);
          if (ack)
          {
            ack->SetId(cmdmsg->GetId());
            ack->SetSrc(cmdmsg->GetDst());
            ack->SetDst(cmdmsg->GetSrc());
            ack->SetStatus((status) ? AckMessage::STATUS_PASS : AckMessage::STATUS_FAIL);
            ack->SetInfo(data.GetOutput());
            status = notification_->Sock()->Send(*ack);
            delete (ack);
          }
        }
        break;
      }
      case MessageNotification::ID_MSG_SENT:
        {
        status = true;
        break;
      }
      default:
        break;
      }
    }
  }

  return (status);
}

bool
Command::EventHandler(zCommand::CommandNotification* notification_)
{
  bool status = false;
  if (notification_->GetCommandData() == *this)
  {
    status = this->Execute(notification_->GetCommandData());
  }
  return (status);
}

}
}
