/*
 * CommandMessage.cpp
 *
 *  Created on: Jan 14, 2016
 *      Author: kmahoney
 */

#include <string>
#include <mutex>
#include <list>
#include <map>

#include <zutils/zEvent.h>
#include <zutils/zData.h>
#include <zutils/zMessage.h>

#include <zutils/zCommand.h>

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
  return(this->SetData(command_));
}

}
}

