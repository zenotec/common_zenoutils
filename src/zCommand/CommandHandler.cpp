/*
 * Command.cpp
 *
 *  Created on: Jan 10, 2016
 *      Author: kmahoney
 */

#include <list>

#include <zutils/zCommand.h>

namespace zUtils
{
namespace zCommand
{

CommandHandler::CommandHandler()
{
  this->_lock.Unlock();
}

CommandHandler::~CommandHandler()
{

}

bool
CommandHandler::RegisterCommand(zCommand::Command *cmd_)
{
  bool status = false;
  if (cmd_ && this->_lock.Lock())
  {
    ZLOG_DEBUG("Registering command: '" + cmd_->GetName() + "'");
    this->_cmd_table[cmd_->GetName()] = cmd_;
    this->_lock.Unlock();
    status = true;
  }
  return (status);
}

bool
CommandHandler::UnregisterCommand(zCommand::Command *cmd_)
{
  bool status = false;
  if (cmd_ && this->_lock.Lock())
  {
    ZLOG_DEBUG("Unregistering command: '" + cmd_->GetName() + "'");
    std::map<std::string, zCommand::Command *>::iterator it;
    it = this->_cmd_table.find(cmd_->GetName());
    if (it != this->_cmd_table.end())
    {
      this->_cmd_table.erase(it);
      status = true;
    }
    this->_lock.Unlock();
  }
  return (status);
}

bool
CommandHandler::ProcessCommandString(const std::string &str_)
{
  bool status = true;
  size_t pos = 0;
  size_t npos = 0;
  std::list<std::string> commands;

  ZLOG_INFO("Processing command string: '" + str_ + "'");

  // Process string into vector of commands (w/ arguments); semicolon is delimiter
  while (npos != str_.npos)
  {
    pos = str_.find_first_not_of(' ', pos);
    npos = str_.find_first_of(';', pos);
    commands.push_back(str_.substr(pos, npos));
    pos = npos + 1;
  }

  // Process each command sequentially
  while (status && !commands.empty())
  {
    std::string cmd;
    std::string name;
    std::string arg;

    // Get command string off the list
    cmd = commands.front();
    commands.pop_front();

    // Parse command name from string
    pos = cmd.find_first_not_of(' ');
    npos = cmd.find_first_of(' ', pos + 1);
    name = cmd.substr(pos, npos);

    // Conditionally parse argument from string
    if (npos != cmd.npos)
    {
      pos = cmd.find_first_not_of(' ', npos);
      arg = cmd.substr(pos, cmd.npos);
    }

    // Lookup command name in table and execute
    std::map<std::string, zCommand::Command *>::iterator it;
    it = this->_cmd_table.find(name);
    if (it != this->_cmd_table.end() && it->second)
    {
      status = it->second->Execute(arg);
    }
    else
    {
      status = false;
    }

  }

  return (status);

}

}
}
