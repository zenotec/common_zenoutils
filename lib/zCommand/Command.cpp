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

const std::string Command::ROOT = "zCommand";
const std::string Command::NAME = "Name";
const std::string Command::OUTPUT = "Output";

Command::Command(const std::string &name_) :
    zData::Data(Command::ROOT), zEvent::Event(zEvent::Event::TYPE_NONE)
{
  this->SetName(name_);
  this->SetOutput(this->GetOutput());
}

Command::Command(const zData::Data &data_) :
    zData::Data(data_), zEvent::Event(zEvent::Event::TYPE_NONE)
{
  this->SetName(this->GetName());
  this->SetOutput(this->GetOutput());
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

std::vector<CommandOption>
Command::GetOptions() const
{

  zData: Data data;
  std::unique_ptr<zData::Data> child;
  std::vector<CommandOption> options;

  if (this->Get(data, CommandOption::ROOT))
  {
    for (int i = 0; i < data.Size(); i++)
    {
      CommandOption opt(*data[i]);
      options.push_back(opt);
    }
  }

  return (options);
}

bool
Command::AddOption(CommandOption &opt_)
{
  return (this->Add(static_cast<zData::Data &>(opt_), CommandOption::ROOT));
}

std::string
Command::GetOutput() const
{
  std::string str;
  this->Get(str, Command::OUTPUT);
  return (str);
}

bool
Command::SetOutput(const std::string arg_)
{
  return (this->Put(arg_, Command::OUTPUT));
}

bool
Command::Execute(zCommand::Command &cmd_)
{
  return (false);
}

bool
Command::EventHandler(const zEvent::EventNotification* notification_)
{
  bool status = false;
  return (status);
}

}
}