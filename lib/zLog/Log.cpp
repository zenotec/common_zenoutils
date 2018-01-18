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
    "TEST",
    "LAST"
};

//*****************************************************************************
// Class: Log
//*****************************************************************************

Log::Log(const Log::MODULE module_) :
    _module(std::string(moduleStr[module_]))
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
}

Log::LEVEL
Log::GetMaxLevel()
{
  return (zLog::Manager::Instance().getMaxLevel(this->_module));
}

void
Log::SetMaxLevel(Log::LEVEL level_)
{
  zLog::Manager::Instance().setMaxLevel(this->_module, level_);
  return;
}

SHARED_PTR(Message)
Log::CreateMessage(Log::LEVEL level_)
{
  SHARED_PTR(Message) msg;
  if (level_ <= this->GetMaxLevel())
  {
    msg = SHARED_PTR(Message)(new Message(this->_module, level_));
  }
  return (msg);
}

void
Log::LogMessage(const SHARED_PTR(Message)& message_)
{
  zLog::Manager::Instance().logMessage(message_);
}

//void
//LogMsg(const Message& msg_)
//{
////  std::cout << "+" << std::endl;
////  std::cout.flush();
//  try
//  {
//    this->_log_lock.lock();
////    std::cout << ZLOG_HEX((uint32_t )pthread_self()) << ": log lock(" << &this->_log_lock << ")"
////        << std::endl;
////    std::cout.flush();
//    if (msg_.GetLevel() <= this->_maxLevel)
//    {
//      if (this->_connTable[msg_.GetLevel()] != NULL)
//      {
////        std::cout << ZLOG_HEX((uint32_t )pthread_self()) << ": " << msg_.GetStr();
//        this->_connTable[msg_.GetLevel()]->Logger(msg_.GetMessage());
//      }
//    }
////    std::cout << ZLOG_HEX((uint32_t )pthread_self()) << ": log unlock(" << &this->_log_lock << ")"
////        << std::endl;
////    std::cout.flush();
//    this->_log_lock.unlock();
//  }
//  catch (const std::system_error& e)
//  {
////    std::cout << ZLOG_HEX((uint32_t )pthread_self()) << ": " << "Caught system error: " << e.what() << std::endl;
////    std::cout << ZLOG_HEX((uint32_t )pthread_self()) << ": " << msg_.GetStr();
////    std::cout.flush();
//  }
////  std::cout << "-" << std::endl;
////  std::cout.flush();
//}

}
}
