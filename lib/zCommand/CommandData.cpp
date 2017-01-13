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

CommandData::CommandData(zData::Data &data_) :
    zData::Data(CommandPath::DataRoot)
{
  this->Put(data_);
  ZLOG_DEBUG("CommandData::CommandData(data_)");
  ZLOG_DEBUG(this->Path());
  ZLOG_DEBUG(this->GetJson());
}

CommandData::CommandData(const zData::Data &data_) :
    zData::Data(CommandPath::DataRoot)
{
  this->Put(data_);
  ZLOG_DEBUG("CommandData::CommandData(data_)");
  ZLOG_DEBUG(this->Path());
  ZLOG_DEBUG(this->GetJson());
}

CommandData::CommandData(CommandData &other_) :
    zData::Data(CommandPath::DataRoot)
{
  this->Put(other_);
  ZLOG_DEBUG("CommandData::CommandData(other_)");
  ZLOG_DEBUG(this->Path());
  ZLOG_DEBUG(this->GetJson());
}

CommandData::CommandData(const CommandData &other_) :
    zData::Data(CommandPath::DataRoot)
{
  this->Put(other_);
  ZLOG_DEBUG("CommandData::CommandData(other_)");
  ZLOG_DEBUG(this->Path());
  ZLOG_DEBUG(this->GetJson());
}

CommandData::~CommandData()
{
}

bool
CommandData::operator ==(const CommandData &other_)
{
  return (this->GetName() == other_.GetName());
}

bool
CommandData::operator !=(const CommandData &other_)
{
  return (this->GetName() != other_.GetName());
}

std::string
CommandData::GetName() const
{
  std::string str;
  CommandPath path;
  this->Get(path.Name(), str);
  return (str);
}

bool
CommandData::SetName(const std::string name_)
{
  CommandPath path;
  return (this->Put(path.Name(), name_));
}

std::map<std::string, CommandOption>
CommandData::GetOptions() const
{

  ZLOG_DEBUG("CommandData::GetOptions()");
  ZLOG_DEBUG(this->Path());
  ZLOG_DEBUG(this->GetJson());

  std::map<std::string, CommandOption> options;
  CommandPath path;
  zData::Data data;

  if (this->Get(path.Option(), data))
  {
    for (int i = 0; i < data.Size(); i++)
    {
      CommandOption opt(*data[i]);
      options[opt.GetName()] = opt;
    }
  }

  return (options);
}

bool
CommandData::AddOption(CommandOption &opt_)
{
  CommandPath path;
  return (this->Add(path.Option(), opt_.GetData()));
}

std::string
CommandData::GetOutput() const
{
  std::string str;
  CommandPath path;
  this->Get(path.Output(), str);
  return (str);
}

bool
CommandData::SetOutput(const std::string arg_)
{
  CommandPath path;
  return (this->Put(path.Output(), arg_));
}

}
}
