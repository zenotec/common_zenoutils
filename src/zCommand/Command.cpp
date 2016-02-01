/*
 * Command.cpp
 *
 *  Created on: Jan 10, 2016
 *      Author: kmahoney
 */

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
    zData::Data(Command::ROOT)
{
  this->SetJson(data_.GetJson());
}

Command::~Command()
{
}

bool
Command::operator ==(const Command &other_) const
{
  return (this->GetName() == other_.GetName());
}

bool
Command::operator !=(const Command &other_) const
{
  return (this->GetName() != other_.GetName());
}

std::string
Command::GetName() const
{
  std::string str;
  this->GetValue(Command::NAME, str);
  return (str);
}

bool
Command::SetName(const std::string name_)
{
  return (this->SetValue(Command::NAME, name_));
}

std::string
Command::GetArgument() const
{
  std::string str;
  this->GetValue(Command::ARG, str);
  return (str);
}

bool
Command::SetArgument(const std::string arg_)
{
  return (this->SetValue(Command::ARG, arg_));
}

std::string
Command::GetOutput() const
{
  std::string str;
  this->GetValue(Command::OUT, str);
  return (str);
}

bool
Command::SetOutput(const std::string arg_)
{
  return (this->SetValue(Command::OUT, arg_));
}

bool
Command::Execute(const std::string &arg_)
{
  return (false);
}

bool
Command::EventHandler(zEvent::Event *event_, void *arg_)
{
  bool status = false;
  Command *cmd = static_cast<Command *>(arg_);
  if (cmd && cmd->GetName() == this->GetName())
  {
    status = this->Execute(cmd->GetArgument());
  }
  return (status);
}

}
}
