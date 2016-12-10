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
