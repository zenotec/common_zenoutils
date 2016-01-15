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

Command::Command(const std::string &command_) :
    zData::Data(Command::ROOT)
{

  size_t pos = 0;
  size_t npos = 0;

  // Parse command name from string
  pos = command_.find_first_not_of(' ');
  if (pos != command_.npos)
  {
    npos = command_.find_first_of(' ', pos + 1);
    this->SetName(command_.substr(pos, npos));
    // Conditionally parse argument from string
    if (npos != command_.npos)
    {
      pos = command_.find_first_not_of(' ', npos);
      this->SetArgument(command_.substr(pos, command_.npos));
    }
  }

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
