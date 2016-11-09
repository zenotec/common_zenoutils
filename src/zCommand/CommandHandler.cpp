/*
 * CommandHandler.cpp
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
    status = true;
    this->_lock.Unlock();
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
CommandHandler::ProcessCommand(const zCommand::Command &cmd_)
{
  bool status = false;

  ZLOG_INFO("Processing command: '" + cmd_.GetName() + "'");

  // Lookup command name in table and execute
  std::map<std::string, zCommand::Command *>::iterator it;
  if (this->_lock.Lock())
  {
    it = this->_cmd_table.find(cmd_.GetName());
    if (it != this->_cmd_table.end() && it->second)
    {
      status = it->second->Execute(cmd_.Argc(), cmd_.GetArgv());
    }
    this->_lock.Unlock();
  }

  return (status);

}

bool
CommandHandler::ProcessCommand(const std::string &str_)
{
  bool status = false;
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
  while (!commands.empty())
  {

    // Get command string off the list
    zCommand::Command cmd(commands.front());
    commands.pop_front();

    ZLOG_INFO("Processing command: '" + cmd.GetName() + "'");

    // Lookup command name in table and execute
    if (this->_lock.Lock())
    {
      std::map<std::string, zCommand::Command *>::iterator it;
      it = this->_cmd_table.find(cmd.GetName());
      if (it != this->_cmd_table.end() && it->second)
      {
        ZLOG_INFO("Executing command: '" + it->first + "'");
        status = it->second->Execute(cmd.Argc(), cmd.GetArgv());
        ZLOG_DEBUG("Command exit status: '" + zLog::IntStr(status) + "'");
        if (!status)
        {
          break;
        }
      }
      this->_lock.Unlock();
    }  
  }  

  return (status);

}

}
}
