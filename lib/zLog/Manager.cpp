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

#include <stdlib.h>
#include <sys/types.h>
#include <unistd.h>

#include <iostream>
#include <iomanip>
#include <system_error>

#include <zutils/zLog.h>

namespace zUtils
{
namespace zLog
{

//*****************************************************************************
// Class: Manager
//*****************************************************************************

Manager::Manager() :
    _thread(this, this)
{
  this->_lock.Unlock();
}

Manager::~Manager()
{
  this->_conn.clear();
  this->_lock.Lock();
}

bool
Manager::RegisterModule(const std::string& module_)
{
  bool status = false;
  if (this->_lock.Lock())
  {
    this->_max_level[module_] = Log::LEVEL_DEF;
    this->_conn[module_].resize(Log::LEVEL_LAST, NULL);
    status = this->_lock.Unlock();
  }
  return (status);
}

bool
Manager::UnregisterModule(const std::string& module_)
{
  bool status = false;
  if (this->_max_level.count(module_) && this->_lock.Lock())
  {
    this->_max_level.erase(module_);
    this->_conn.erase(module_);
    status = this->_lock.Unlock();
  }
  return (status);
}

bool
Manager::RegisterConnector(const std::string& module_, Log::LEVEL level_, Connector* conn_)
{
  bool status = false;
  if (this->_conn.count(module_) && (level_ >= Log::LEVEL_ALL)
      && (level_ < Log::LEVEL_LAST) && conn_ && this->_lock.Lock())
  {
    int lfirst = level_;
    int llast = level_;

    if (level_ != Log::LEVEL_ALL)
    {
      lfirst = level_;
      llast = (lfirst + 1);
    }

    for (int l = lfirst; l < llast; l++)
    {
      this->_conn[module_][l] = conn_;
    }
    status = this->_lock.Unlock();
  }
  return (status);
}

bool
Manager::UnregisterConnector(const std::string& module_, Log::LEVEL level_)
{
  bool status = false;
  if (this->_max_level.count(module_) && (level_ >= Log::LEVEL_ALL)
      && (level_ < Log::LEVEL_LAST) && this->_lock.Lock())
  {
    int lfirst = level_;
    int llast = level_;

    if (level_ != Log::LEVEL_ALL)
    {
      lfirst = level_;
      llast = (lfirst + 1);
    }

    for (int l = lfirst; l < llast; l++)
    {
      this->_conn[module_][l] = NULL;
    }
    status = this->_lock.Unlock();
  }
  return (status);
}

void
Manager::logMessage(const SHARED_PTR(zLog::Message)& message_)
{
  if (this->_lock.Lock())
  {
    this->_msg_queue.Push(message_);
    this->_lock.Unlock();
  }
  return;
}

void
Manager::Run(zThread::ThreadArg *arg_)
{

  while (!this->Exit())
  {

    if (!this->_msg_queue.TimedWait(100))
      continue;

    SHARED_PTR(Message) msg = this->_msg_queue.Front();
    this->_msg_queue.Pop();

    if (!msg)
      continue;

    const std::string& module = msg->GetModule();
    Log::LEVEL level = msg->GetLevel();

    if (!(this->_max_level.count(module)) || !(level < this->_max_level[module]))
      continue;

    if (this->_conn[module][level])
    {
      std::stringstream ss;
      this->_conn[module][level]->Logger(ss.str());
    }

  }

  return;
}

Log::LEVEL
Manager::getMaxLevel(const std::string& module_)
{
  Log::LEVEL level = Log::LEVEL_DEF;
  if (this->_max_level.count(module_))
  {
    level = this->_max_level[module_];
  }
  return (level);
}

void
Manager::setMaxLevel(const std::string& module_, const Log::LEVEL level_)
{
  if (this->_max_level.count(module_) && (level_ > Log::LEVEL_ALL) &&
      (level_ < Log::LEVEL_LAST))
  {
    this->_max_level[module_] = level_;
  }
}

}
}
