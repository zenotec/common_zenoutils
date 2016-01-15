/*
 * CommandMessage.cpp
 *
 *  Created on: Jan 14, 2016
 *      Author: kmahoney
 */


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
  this->AddCommand(command_);
}

CommandMessage::~CommandMessage()
{
}

bool
CommandMessage::AddCommand(const zCommand::Command &command_)
{
  zData::Data data = this->GetData();
  data.AddChild("zCommand", command_);
  this->SetData(data);
}

}
}

