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
#include <zutils/zMessageSocket.h>

#include <zutils/zCommand.h>

namespace zUtils
{
namespace zCommand
{

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
Command::EventHandler(const zEvent::EventNotification* notification_)
{
  bool status = false;
  if (notification_ && (notification_->Type() == zEvent::Event::TYPE_COMMAND))
  {
    const zCommand::CommandNotification *n = NULL;
    n = static_cast<const zCommand::CommandNotification *>(notification_);
    CommandData data(n->GetCommandData());
    if (data == *this)
    {
      status = this->Execute(data);
    }
  }
  else if (notification_ && (notification_->Type() == zEvent::Event::TYPE_MSG))
  {
    const zMessage::MessageNotification *n = NULL;
    n = static_cast<const zMessage::MessageNotification *>(notification_);
    switch (n->Id())
    {
    case zMessage::MessageNotification::ID_MSG_RCVD:
      {
      if (n->Message() && (n->Message()->GetType() == zMessage::Message::TYPE_CMD))
      {
        CommandData data(n->Message()->GetData());
        if (data == *this)
        {
          status = this->Execute(data);
        }
      }
      break;
    }
    default:
      break;
    }
  }
  else
  {
    ZLOG_WARN("Unknown event: " + ZLOG_INT(notification_->Type()));
  }
  return (status);
}

}
}
