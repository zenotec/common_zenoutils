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
// Class: Log
//*****************************************************************************

Log::Log(const Log::MODULE module_) :
    _module(Log::ToString(module_))
{
  zLog::Manager::Instance().RegisterModule(this->_module);
}

Log::Log(const std::string& module_) :
    _module(module_)
{
  zLog::Manager::Instance().RegisterModule(this->_module);
}

Log::~Log()
{
  zLog::Manager::Instance().UnregisterModule(this->_module);
}

Log::LEVEL
Log::GetMaxLevel()
{
  return (zLog::Manager::Instance().GetMaxLevel(this->_module));
}

void
Log::SetMaxLevel(Log::LEVEL level_)
{
  zLog::Manager::Instance().SetMaxLevel(this->_module, level_);
  return;
}

SHPTR(Message)
Log::CreateMessage(Log::LEVEL level_)
{
  SHPTR(Message) msg;
  if (level_ <= this->GetMaxLevel())
  {
    msg = SHPTR(Message)(new Message(this->_module, level_));
  }
  return (msg);
}

void
Log::LogMessage(const SHPTR(Message)& message_)
{
  zLog::Manager::Instance().logMessage(message_);
}

std::string
Log::ToString(const Log::MODULE module_)
{
  std::string str;
  switch (module_)
  {
    case  MODULE_ALL: str = "ALL"; break;
    case  MODULE_COMMAND: str = "COMMAND"; break;
    case  MODULE_CONFIG: str = "CONFIG"; break;
    case  MODULE_DATA: str = "DATA"; break;
    case  MODULE_DISPLAY: str = "DISPLAY"; break;
    case  MODULE_EVENT: str = "EVENT"; break;
    case  MODULE_GPIO: str = "GPIO"; break;
    case  MODULE_INTERFACE: str = "INTERFACE"; break;
    case  MODULE_LED: str = "LED"; break;
    case  MODULE_MATH: str = "MATH"; break;
    case  MODULE_MESSAGE: str = "MESSAGE"; break;
    case  MODULE_NODE: str = "NODE"; break;
    case  MODULE_PROGRAM: str = "PROGRAM"; break;
    case  MODULE_SERIAL: str = "SERIAL"; break;
    case  MODULE_SOCKET: str = "SOCKET"; break;
    case  MODULE_STATE: str = "STATE"; break;
    case  MODULE_SWITCH: str = "SWITCH"; break;
    case  MODULE_THERMO: str = "THERMO"; break;
    case  MODULE_WIRELESS: str = "WIRELESS"; break;
    case  MODULE_TEST: str = "TEST"; break;
    default: str = "UNKNOWN"; break;
  }

  return (str);
}

std::string
Log::ToString(const Log::LEVEL level_)
{
  std::string str;
  switch (level_)
  {
    case LEVEL_ALL: str = "ALL"; break;
    case LEVEL_CRIT: str = "CRIT"; break;
    case LEVEL_ERROR: str = "ERROR"; break;
    case LEVEL_WARN: str = "WARN"; break;
    case LEVEL_INFO: str = "INFO"; break;
    case LEVEL_DEBUG: str = "DEBUG"; break;
    case LEVEL_DEBUG1: str = "DEBUG1"; break;
    case LEVEL_DEBUG2: str = "DEBUG2"; break;
    case LEVEL_DEBUG3: str = "DEBUG3"; break;
    default: str = "UNKNOWN"; break;
  }
  return (str);
}

}
}
