/*
 * Command.cpp
 *
 *  Created on: Jan 10, 2016
 *      Author: kmahoney
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

namespace zUtils
{
namespace zCommand
{

const std::string Command::ROOT = "zCommand";
const std::string Command::NAME = "Name";
const std::string Command::ARG = "Argument";
const std::string Command::OUT = "Output";

Command::Command(const std::string &cmd_, const std::string &arg_) :
    zData::Data(Command::ROOT)
{

  this->SetName(cmd_);
  this->SetArgument(arg_);
  this->SetOutput("");

}

Command::Command(const zData::Data &data_) :
    zData::Data(data_)
{
}

Command::~Command()
{
}

bool
Command::operator ==(const Command &other_)
{
  return (this->GetName() == other_.GetName());
}

bool
Command::operator !=(const Command &other_)
{
  return (this->GetName() != other_.GetName());
}

std::string
Command::GetName() const
{
  std::string str;
  this->Get(str, Command::NAME);
  return (str);
}

bool
Command::SetName(const std::string name_)
{
  return (this->Put(name_, Command::NAME));
}

std::string
Command::GetArgument() const
{
  std::string str;
  this->Get(str, Command::ARG);
  return (str);
}

bool
Command::SetArgument(const std::string arg_)
{
  return (this->Put(arg_, Command::ARG));
}

std::string
Command::GetOutput() const
{
  std::string str;
  this->Get(str, Command::OUT);
  return (str);
}

bool
Command::SetOutput(const std::string arg_)
{
  return (this->Put(arg_, Command::OUT));
}

bool
Command::Execute(const std::string &arg_)
{
  return (false);
}

bool
Command::EventHandler(const zEvent::EventNotification* notification_)
{
  bool status = false;
//  Command *cmd = static_cast<Command *>(arg_);
//  if (cmd && cmd->GetName() == this->GetName())
//  {
//    status = this->Execute(cmd->GetArgument());
//  }
  return (status);
}

}
}
