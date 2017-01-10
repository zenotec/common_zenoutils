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
// Class: CommandPath
//**********************************************************************

const std::string CommandPath::DataRoot("zCommand");
const std::string CommandPath::DataNamePath("Name");
const std::string CommandPath::DataOutputPath("Output");

CommandPath::CommandPath() :
    zData::DataPath(DataRoot)
{
}

CommandPath::~CommandPath()
{
}

zData::DataPath
CommandPath::Name()
{
  zData::DataPath path(*this);
  path.Append(DataNamePath);
  return (path);
}

zData::DataPath
CommandPath::Option()
{
  zData::DataPath path(*this);
  path.Append(CommandOptionPath::DataRoot);
  return (path);
}

zData::DataPath
CommandPath::Output()
{
  zData::DataPath path(*this);
  path.Append(DataOutputPath);
  return (path);
}

//**********************************************************************
// Class: Command
//**********************************************************************

Command::Command() :
    zData::Data(CommandPath::DataRoot), zEvent::Event(zEvent::Event::TYPE_COMMAND)
{
  ZLOG_DEBUG("Command::Command()");
  ZLOG_DEBUG(this->Path());
  ZLOG_DEBUG(this->GetJson());
}

Command::Command(const zData::Data &data_) :
    zData::Data(CommandPath::DataRoot), zEvent::Event(zEvent::Event::TYPE_COMMAND)
{
  this->Put(data_);
  ZLOG_DEBUG("Command::Command(data_)");
  ZLOG_DEBUG(this->Path());
  ZLOG_DEBUG(this->GetJson());
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
  CommandPath path;
  this->Get(path.Name(), str);
  return (str);
}

bool
Command::SetName(const std::string name_)
{
  CommandPath path;
  return (this->Put(path.Name(), name_));
}

std::map<std::string, CommandOption>
Command::GetOptions() const
{

  ZLOG_DEBUG("Command::GetOptions()");
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
Command::AddOption(CommandOption &opt_)
{
  CommandPath path;
  return (this->Add(path.Option(), opt_.GetData()));
}

std::string
Command::GetOutput() const
{
  std::string str;
  CommandPath path;
  this->Get(path.Output(), str);
  return (str);
}

bool
Command::SetOutput(const std::string arg_)
{
  CommandPath path;
  return (this->Put(path.Output(), arg_));
}

bool
Command::Execute(zCommand::Command &cmd_)
{
  return (false);
}

}
}
