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

#include <zutils/zLog.h>
#include <zutils/zSem.h>
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

const std::string CommandOption::ROOT = "zOption";
const std::string CommandOption::OPT = "Option";
const std::string CommandOption::ARG = "Argument";

CommandOption::CommandOption(const std::string &opt_, const std::string &arg_) :
    zData::Data(CommandOption::ROOT)
{
  this->SetOption(opt_);
  this->SetArgument(arg_);
}

CommandOption::CommandOption(const zData::Data &data_) :
        zData::Data(data_)
{
  this->SetOption(this->GetOption());
  this->SetArgument(this->GetArgument());
}

CommandOption::~CommandOption()
{
}

std::string
CommandOption::GetOption() const
{
  std::string str;
  this->Get(str, CommandOption::OPT);
  return (str);
}

bool
CommandOption::SetOption(const std::string opt_)
{
  return (this->Put(opt_, CommandOption::OPT));
}

std::string
CommandOption::GetArgument() const
{
  std::string str;
  this->Get(str, CommandOption::ARG);
  return (str);
}

bool
CommandOption::SetArgument(const std::string arg_)
{
  return (this->Put(arg_, CommandOption::ARG));
}

}
}
