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

static const char* levelStr[] = {
    "CRIT",
    "ERROR",
    "WARN",
    "INFO",
    "DEBUG"
};

static const char* moduleStr[] = {
    "COMMAND",
    "CONFIG",
    "DATA",
    "DISPLAY",
    "EXCEPTION",
    "GPIO",
    "INTERFACE",
    "LED",
    "MATH",
    "MESSAGE",
    "NODE",
    "PROGRAM",
    "SERIAL",
    "SOCKET",
    "SWITCH",
    "THERMO",
    "WIRELESS",
    "LAST"
};

//*****************************************************************************
// Class: Manager
//*****************************************************************************

Manager::Manager() :
    _thread(this, this)
{
  this->_conn.resize(Log::MODULE_LAST);
  for (int i = 0; i < Log::MODULE_LAST; i++)
  {
    this->_conn[i].resize(Log::LEVEL_LAST);
    for (int j = 0; j < Log::LEVEL_LAST; j++)
    {
      this->_conn[i][j] = NULL;
    }
  }
  this->_lock.Unlock();
}

Manager::~Manager()
{
  this->_lock.Lock();
}

bool
Manager::RegisterConnector(Log::MODULE module_, Log::LEVEL level_, Connector* conn_)
{
  bool status = false;
  if ((module_ >= Log::MODULE_ALL) && (module_ < Log::MODULE_LAST) && (level_ >= Log::LEVEL_ALL)
      && (level_ < Log::LEVEL_LAST) && conn_ && this->_lock.Lock())
  {
    int mfirst = 0;
    int mlast = Log::MODULE_LAST;
    int lfirst = level_;
    int llast = level_;

    if (module_ != Log::MODULE_ALL)
    {
      mfirst = module_;
      mlast = (mfirst + 1);
    }

    if (level_ != Log::LEVEL_ALL)
    {
      lfirst = level_;
      llast = (lfirst + 1);
    }

    for (int m = mfirst; m < mlast; m++)
    {
      for (int l = lfirst; l < llast; l++)
      {
        this->_conn[m][l] = conn_;
      }
    }
    status = this->_lock.Unlock();
  }
  return (status);
}

bool
Manager::UnregisterConnector(Log::MODULE module_, Log::LEVEL level_)
{
  bool status = false;
  if ((module_ >= Log::MODULE_ALL) && (module_ < Log::MODULE_LAST) && (level_ >= Log::LEVEL_ALL)
      && (level_ < Log::LEVEL_LAST) && this->_lock.Lock())
  {
    int mfirst = 0;
    int mlast = Log::MODULE_LAST;
    int lfirst = level_;
    int llast = level_;

    if (module_ != Log::MODULE_ALL)
    {
      mfirst = module_;
      mlast = (mfirst + 1);
    }

    if (level_ != Log::LEVEL_ALL)
    {
      lfirst = level_;
      llast = (lfirst + 1);
    }

    for (int m = mfirst; m < mlast; m++)
    {
      for (int l = lfirst; l < llast; l++)
      {
        this->_conn[m][l] = NULL;
      }
    }
    status = this->_lock.Unlock();
  }
  return (status);
}

void
Manager::LogMessage(const SHARED_PTR(zLog::Message)& message_)
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
    if (this->_msg_queue.TimedWait(100))
    {

    }
  }

  return;
}

}
}
