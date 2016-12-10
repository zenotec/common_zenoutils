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

#include <zutils/zCommand.h>
#include <zutils/zCommandMessage.h>

namespace zUtils
{
namespace zCommand
{

CommandMessage::CommandMessage()
{
  this->SetType(zMessage::Message::TYPE_CMD);
}

CommandMessage::CommandMessage(const zCommand::Command &command_)
{
  this->SetType(zMessage::Message::TYPE_CMD);
  this->SetCommand(command_);
}

CommandMessage::~CommandMessage()
{
}

bool
CommandMessage::SetCommand(const zCommand::Command &command_)
{
  return (this->SetData(command_));
}

}
}

