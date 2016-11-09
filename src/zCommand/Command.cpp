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
const std::string Command::ARG = "Argument";
const std::string Command::OUT = "Output";

static std::vector<std::string>
CommandParse(const std::string &cmd_)
{

  size_t pos = 0;
  size_t npos = 0;
  std::vector<std::string> argv;

  while (!cmd_.empty() && npos != cmd_.npos)
  {
     pos = cmd_.find_first_not_of(' ', pos);
     npos = cmd_.find_first_of(' ', pos);
     std::cout << "Parsing: " << pos << "->" << npos << std::endl;
     argv.push_back(cmd_.substr(pos, npos));
     pos = npos + 1;
  }

  return (argv);
 
}

Command::Command(const std::string &cmd_) :
    zData::Data(Command::ROOT)
{
  std::string name;
  std::vector<std::string> argv = CommandParse(cmd_);
  if (!argv.empty())
  {
    name = argv[0];
  }

  this->SetName(name);
  this->SetArgv(argv);
  this->SetOutput("");

  this->DisplayJson();
}

Command::Command(const zData::Data &data_) :
    zData::Data(data_)
{
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

size_t
Command::Argc() const
{
  size_t argc = 0;
  zData::Data data;

  if (!this->GetName().empty())
  {
    argc++;
  }

  if (this->Get(data, Command::ARG))
  {
    argc += data.Size();
  }

  return (argc);
}

std::vector<std::string>
Command::GetArgv() const
{
  std::string arg;
  zData::Data data;
  std::unique_ptr<zData::Data> child;
  std::vector<std::string>argv;

  this->DisplayJson();

  if (!this->GetName().empty())
  {
    argv.push_back(this->GetName());
  }

  if (this->Get(data, Command::ARG))
  {
    std::cout << data.GetJson() << std::endl;
    for (int i = 0; i < data.Size(); i++)
    {
      child = data[i];
      if (child->Get(arg, Command::ARG))
      {
        argv.push_back(arg);
      }
    }
  }

  return (argv);
}

bool
Command::SetArgv(const std::vector<std::string> &argv_)
{
  bool status = true;
  std::vector<std::string>::const_iterator it = argv_.begin();
  std::vector<std::string>::const_iterator end = argv_.end();
  for (; it != end; ++it)
  {
    if (it == argv_.begin())
    {
      this->SetName((*it));
      continue;
    }
    if (!this->Add((*it), Command::ARG))
    {
      status = false;
      break;
    }
  }

  this->DisplayJson();

  return (status);
}

std::string
Command::GetOutput() const
{
  std::string str;
  this->Get(str, Command::OUT);
  return (str);
}

bool
Command::SetOutput(const std::string arg_)
{
  return (this->Put(arg_, Command::OUT));
}

bool
Command::Execute(int argc_, const std::vector<std::string> &argv_)
{
  return (false);
}

bool
Command::EventHandler(const zEvent::EventNotification* notification_)
{
  bool status = false;
//  Command *cmd = static_cast<Command *>(arg_);
//  if (cmd && cmd->GetName() == this->GetName())
//  {
//    status = this->Execute(cmd->GetArgument());
//  }
  return (status);
}

}
}
