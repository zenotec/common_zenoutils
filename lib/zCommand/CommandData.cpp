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
// Class: CommandData
//**********************************************************************

CommandData::CommandData() :
    zData::Data(CommandPath::DataRoot)
{
  ZLOG_DEBUG("CommandData::CommandData()");
  ZLOG_DEBUG(this->Path());
  ZLOG_DEBUG(this->GetJson());
}

CommandData::CommandData(const zData::Data &data_) :
    zData::Data(CommandPath::DataRoot)
{
  this->PutChild(data_);
  ZLOG_DEBUG("CommandData::CommandData(data_)");
  ZLOG_DEBUG(this->Path());
  ZLOG_DEBUG(this->GetJson());
}

CommandData::CommandData(CommandData &other_) :
    zData::Data(other_)
{
  ZLOG_DEBUG("CommandData::CommandData(other_)");
  ZLOG_DEBUG(this->Path());
  ZLOG_DEBUG(this->GetJson());
}

CommandData::CommandData(const CommandData &other_) :
    zData::Data(other_)
{
  ZLOG_DEBUG("CommandData::CommandData(other_)");
  ZLOG_DEBUG(this->Path());
  ZLOG_DEBUG(this->GetJson());
}

CommandData::~CommandData()
{
}

CommandData &
CommandData::operator=(const CommandData &other_)
{
  zData::Data::operator =(other_);
  return (*this);
}

bool
CommandData::operator ==(const CommandData &other_) const
{
  return (this->GetName() == other_.GetName());
}

bool
CommandData::operator !=(const CommandData &other_) const
{
  return (this->GetName() != other_.GetName());
}

zData::Data&
CommandData::GetData()
{
  return(*this);
}

std::string
CommandData::GetName() const
{
  return (this->GetValue<std::string>(CommandPath::NameDataPath));
}

bool
CommandData::SetName(const std::string name_)
{
  return (this->PutValue(CommandPath(CommandPath::NameDataPath), name_));
}

std::map<std::string, CommandOption>
CommandData::GetOptions() const
{

  ZLOG_DEBUG("CommandData::GetOptions()");
  ZLOG_DEBUG(this->Path());
  ZLOG_DEBUG(this->GetJson());

  std::map<std::string, CommandOption> options;

  for (int i = 0; i < this->operator ()(CommandPath::OptionDataPath).Size(); i++)
  {
    CommandOption opt(this->operator ()(CommandPath::OptionDataPath)[i]);
    options[opt.GetName()] = opt;
  }

  return (options);
}

bool
CommandData::AddOption(CommandOption &opt_)
{
  return (this->AddChild(CommandPath(CommandPath::OptionDataPath), opt_.GetData()));
}

std::string
CommandData::GetOutput() const
{
  return (this->GetValue<std::string>(CommandPath::OutputDataPath));
}

bool
CommandData::SetOutput(const std::string arg_)
{
  return (this->PutValue(CommandPath(CommandPath::OutputDataPath), arg_));
}

}
}
