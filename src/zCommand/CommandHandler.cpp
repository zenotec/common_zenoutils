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
CommandHandler::ProcessCommand(zCommand::Command &cmd_)
{
  bool status = false;

  ZLOG_INFO("Processing command: '" + cmd_.GetName() + "'");

  // Lookup command name in table and execute
  std::map<std::string, zCommand::Command *>::iterator it;
  if (this->_lock.Lock())
  {
    std::cout << "CommandHandler::ProcessCommand(): " << cmd_.GetName() << std::endl;
    it = this->_cmd_table.find(cmd_.GetName());
    if (it != this->_cmd_table.end() && it->second)
    {
      std::cout << "Executing command: '" << it->first << "'" << std::endl;
      status = it->second->Execute(cmd_);
    }
    else
    {
        ZLOG_WARN("Command not found: '" + cmd_.GetName() + "'");
    }
    this->_lock.Unlock();
  }

  return (status);

}

}
}
