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
const std::string Command::ARG = "Arg";

Command::Command() :
    zData::Data(Command::ROOT)
{
  this->SetName("");
  this->SetArgument("");
}

Command::Command(zData::Data &data_) :
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
  return (this->_pt == other_._pt);
}

bool
Command::operator !=(const Command &other_) const
    {
  return (this->_pt != other_._pt);
}

std::string
Command::GetName()
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
Command::GetArgument()
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
  return (this->Execute(""));
}

}
}
