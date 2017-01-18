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

//**********************************************************************
// Class: CommandOptionPath
//**********************************************************************

const std::string CommandOptionPath::DataRoot("zOption");
const std::string CommandOptionPath::NameDataPath("Name");
const std::string CommandOptionPath::ArgDataPath("Argument");

CommandOptionPath::CommandOptionPath(const std::string& root_) :
    zData::DataPath(DataRoot)
{
  if (!root_.empty())
  {
    this->_root.push_back(root_);
  }
}

CommandOptionPath::CommandOptionPath(const CommandOptionPath& other_) :
    zData::DataPath(other_)
{
}

CommandOptionPath::CommandOptionPath(const zData::DataPath& path_) :
    zData::DataPath(path_)
{
}

CommandOptionPath::~CommandOptionPath()
{
}

//**********************************************************************
// Class: CommandOption
//**********************************************************************

CommandOption::CommandOption() :
    zData::Data(CommandOptionPath::DataRoot)
{
  ZLOG_DEBUG("CommandOption::CommandOption()");
  ZLOG_DEBUG(this->Path());
  ZLOG_DEBUG(this->GetJson());
}

CommandOption::CommandOption(const zData::Data& data_) :
    zData::Data(CommandOptionPath::DataRoot)
{
  this->PutChild(data_);
  ZLOG_DEBUG("CommandOption::CommandOption(data_)");
  ZLOG_DEBUG(this->Path());
  ZLOG_DEBUG(this->GetJson());
}

CommandOption::CommandOption(CommandOption &other_) :
    zData::Data(other_)
{
  ZLOG_DEBUG("CommandOption::CommandOption(other_)");
  ZLOG_DEBUG(this->Path());
  ZLOG_DEBUG(this->GetJson());
}

CommandOption::CommandOption(const CommandOption &other_) :
    zData::Data(other_)
{
  ZLOG_DEBUG("CommandOption::CommandOption(other_)");
  ZLOG_DEBUG(this->Path());
  ZLOG_DEBUG(this->GetJson());
}

CommandOption::~CommandOption()
{
}

zData::Data&
CommandOption::GetData()
{
  return (*this);
}

std::string
CommandOption::GetName() const
{
  return (this->GetValue<std::string>(CommandOptionPath::NameDataPath));
}

bool
CommandOption::SetName(const std::string name_)
{
  return (this->PutValue(CommandOptionPath(CommandOptionPath::NameDataPath), name_));
}

std::string
CommandOption::GetArgument() const
{
  return (this->GetValue<std::string>(CommandOptionPath::ArgDataPath));
}

bool
CommandOption::SetArgument(const std::string arg_)
{
  return (this->PutValue(CommandOptionPath(CommandOptionPath::ArgDataPath), arg_));
}

}
}
