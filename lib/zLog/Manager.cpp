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

#include <string.h>

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
  this->_conn.clear();
  this->_log_lock.Unlock();
}

Manager::~Manager()
{
  this->_thread.Stop();
  this->_log_lock.Lock();
  this->_conn.clear();
  this->_mod_refcnt.clear();
  this->_max_level.clear();
}

bool
Manager::RegisterModule(const std::string& module_)
{
  bool status = false;
  if (this->_log_lock.Lock())
  {
    this->_mod_refcnt[module_]++;
    this->_max_level[module_] = Log::LEVEL_DEF;
    status = this->_log_lock.Unlock();
  }
  return (status);
}

bool
Manager::RegisterModule(const Log::MODULE module_)
{
  return (this->RegisterModule(Log::ToString(module_)));
}

bool
Manager::UnregisterModule(const std::string& module_)
{
  bool status = false;
  if (this->_mod_refcnt.count(module_) && this->_log_lock.Lock())
  {
    if (this->_mod_refcnt[module_]-- == 1)
    {
      this->_mod_refcnt.erase(module_);
      this->_max_level.erase(module_);
    }
    status = this->_log_lock.Unlock();
  }
  return (status);
}

bool
Manager::UnregisterModule(const Log::MODULE module_)
{
  return (this->UnregisterModule(Log::ToString(module_)));
}

bool
Manager::RegisterConnector(Log::LEVEL level_, Connector* conn_)
{
  bool status = false;
  if ((level_ >= Log::LEVEL_ALL)
      && (level_ < Log::LEVEL_LAST) && conn_ && this->_log_lock.Lock())
  {
    int lfirst = 0;
    int llast = Log::LEVEL_LAST;

    if (level_ != Log::LEVEL_ALL)
    {
      lfirst = level_;
      llast = (lfirst + 1);
    }

    for (int l = lfirst; l < llast; l++)
    {
      this->_conn[Log::LEVEL(l)] = conn_;
    }

    if (!this->_conn.empty())
    {
      this->_thread.Start();
    }

    status = this->_log_lock.Unlock();
  }
  return (status);
}

bool
Manager::UnregisterConnector(Log::LEVEL level_)
{
  bool status = false;
  if ((level_ >= Log::LEVEL_ALL)
      && (level_ < Log::LEVEL_LAST) && this->_log_lock.Lock())
  {
    int lfirst = 0;
    int llast = Log::LEVEL_LAST;

    if (level_ != Log::LEVEL_ALL)
    {
      lfirst = level_;
      llast = (lfirst + 1);
    }

    for (int l = lfirst; l < llast; l++)
    {
      if (this->_conn.count(Log::LEVEL(l)))
      {
        this->_conn.erase(Log::LEVEL(l));
      }
    }

    if (this->_conn.empty())
    {
      this->_thread.Stop();
    }

    status = this->_log_lock.Unlock();
  }
  return (status);
}

void
Manager::logMessage(const SHPTR(zLog::Message)& message_)
{
  this->_msg_queue.Push(message_);
}

void
Manager::Run(zThread::ThreadArg *arg_)
{

  bool exit = false;

  this->RegisterFd(this->_msg_queue.GetFd(), (POLLIN | POLLERR));

  while (!exit)
  {

    std::vector<struct pollfd> fds;

    // Wait on file descriptor set
    int ret = this->Poll(fds);

    FOREACH (auto& fd, fds)
    {
      if (this->IsExit(fd.fd) && (fd.revents == POLLIN))
      {
        exit = true;
        continue;
      }
      else if (this->IsReload(fd.fd) && (fd.revents == POLLIN))
      {
        continue;
      }
      else if ((this->_msg_queue.GetFd() == fd.fd) && (fd.revents == POLLIN))
      {
        if (this->_msg_queue.TryWait())
        {

          SHPTR(Message) msg = this->_msg_queue.Front();
          this->_msg_queue.Pop();

          if (msg && this->_log_lock.TimedLock(100))
          {
            const std::string& module = msg->GetModule();
            Log::LEVEL level = msg->GetLevel();

            if ((this->_max_level.count(module)) && (level <= this->_max_level[module]))
            {
              if (this->_conn.count(Log::LEVEL(level)) && this->_conn[Log::LEVEL(level)])
              {
                std::stringstream ss;
                ss << msg->GetTimestamp() << "\t";
                ss << msg->GetProcessId() << "\t";
                ss << msg->GetThreadId() << "\t";
                ss << msg->GetModule() << "\t";
                ss << Log::ToString(msg->GetLevel()) << "\t";
                ss << msg->GetFile() << "[" << msg->GetLine() << "]\t";
                ss << msg->GetMessage() << "\t";
                this->_conn[level]->Logger(ss.str());
              }
            }
            this->_log_lock.Unlock();
          }
        }
      }
    }

  }

  this->UnregisterFd(this->_msg_queue.GetFd());

  return;
}

Log::LEVEL
Manager::GetMaxLevel(const std::string& module_)
{
  Log::LEVEL level = Log::LEVEL_ALL;
  if (this->_log_lock.Lock())
  {
    if (this->_max_level.count(module_))
    {
      for (int i = 0; i <= int(this->_max_level[module_]); i++)
      {
        if (this->_conn.count(Log::LEVEL(i)) && this->_conn[Log::LEVEL(i)])
        {
          level = Log::LEVEL(i);
        }
      }
    }
    this->_log_lock.Unlock();
  }
  return (level);
}

Log::LEVEL
Manager::GetMaxLevel(const Log::MODULE module_)
{
  return (this->GetMaxLevel(Log::ToString(module_)));
}

void
Manager::SetMaxLevel(const std::string& module_, const Log::LEVEL level_)
{
  if (this->_max_level.count(module_) && (level_ > Log::LEVEL_ALL) &&
      (level_ < Log::LEVEL_LAST))
  {
    this->_max_level[module_] = level_;
  }
}

void
Manager::SetMaxLevel(const Log::MODULE module_, const Log::LEVEL level_)
{
  this->SetMaxLevel(Log::ToString(module_), level_);
}

}
}
