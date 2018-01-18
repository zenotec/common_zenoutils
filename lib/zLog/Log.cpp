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

const std::string Log::LogModuleAll("ALL");
const std::string Log::LogModuleCommand("COMMAND");
const std::string Log::LogModuleConfig("CONFIG");
const std::string Log::LogModuleData("DATA");
const std::string Log::LogModuleDisplay("DISPLAY");
const std::string Log::LogModuleGpio("GPIO");
const std::string Log::LogModuleInterface("INTERFACE");
const std::string Log::LogModuleLed("LED");
const std::string Log::LogModuleMath("MATH");
const std::string Log::LogModuleMessage("MESSAGE");
const std::string Log::LogModuleNode("NODE");
const std::string Log::LogModuleProgram("PROGRAM");
const std::string Log::LogModuleSerial("SERIAL");
const std::string Log::LogModuleSocket("SOCKET");
const std::string Log::LogModuleSwitch("SWITCH");
const std::string Log::LogModuleThermo("THERMO");
const std::string Log::LogModuleWireless("WIRELESS");
const std::string Log::LogModuleTest("TEST");

const std::string Log::LogLevelAll("ALL");
const std::string Log::LogLevelCritical("CRIT");
const std::string Log::LogLevelError("ERROR");
const std::string Log::LogLevelWarning("WARN");
const std::string Log::LogLevelInfo("INFO");
const std::string Log::LogLevelDebug("DEBUG");
const std::string Log::LogLevelDebug1("DEBUG1");
const std::string Log::LogLevelDebug2("DEBUG2");
const std::string Log::LogLevelDebug3("DEBUG3");
const std::string Log::LogLevelDefault(Log::LogLevelWarning);

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

std::string
Log::ToString(const Log::MODULE module_)
{
  std::string str;
  switch (module_)
  {
  case  MODULE_ALL:
    str = Log::LogModuleAll;
    break;
  case  MODULE_COMMAND:
    str = Log::LogModuleCommand;
    break;
  case  MODULE_CONFIG:
    str = Log::LogModuleConfig;
    break;
  case  MODULE_DATA:
    str = Log::LogModuleData;
    break;
  case  MODULE_DISPLAY:
    str = Log::LogModuleDisplay;
    break;
  case  MODULE_GPIO:
    str = Log::LogModuleGpio;
    break;
  case  MODULE_INTERFACE:
    str = Log::LogModuleInterface;
    break;
  case  MODULE_LED:
    str = Log::LogModuleLed;
    break;
  case  MODULE_MATH:
    str = Log::LogModuleMath;
    break;
  case  MODULE_MESSAGE:
    str = Log::LogModuleMessage;
    break;
  case  MODULE_NODE:
    str = Log::LogModuleNode;
    break;
  case  MODULE_PROGRAM:
    str = Log::LogModuleProgram;
    break;
  case  MODULE_SERIAL:
    str = Log::LogModuleSerial;
    break;
  case  MODULE_SOCKET:
    str = Log::LogModuleSocket;
    break;
  case  MODULE_SWITCH:
    str = Log::LogModuleSwitch;
    break;
  case  MODULE_THERMO:
    str = Log::LogModuleThermo;
    break;
  case  MODULE_WIRELESS:
    str = Log::LogModuleWireless;
    break;
  case  MODULE_TEST:
    str = Log::LogModuleTest;
    break;
  default:
    str = "UNKNOWN";
    break;
  }

  return (str);
}

std::string
Log::ToString(const Log::LEVEL level_)
{
  std::string str;
  switch (level_)
  {
  case  LEVEL_ALL:
    str = Log::LogLevelAll;
    break;
  case  LEVEL_CRIT:
    str = Log::LogLevelCritical;
    break;
  case LEVEL_ERROR:
    str = Log::LogLevelError;
    break;
  case LEVEL_WARN:
    str = Log::LogLevelWarning;
    break;
  case LEVEL_INFO:
    str = Log::LogLevelInfo;
    break;
  case LEVEL_DEBUG:
    str = Log::LogLevelDebug;
    break;
  case LEVEL_DEBUG1:
    str = Log::LogLevelDebug1;
    break;
  case LEVEL_DEBUG2:
    str = Log::LogLevelDebug2;
    break;
  case LEVEL_DEBUG3:
    str = Log::LogLevelDebug3;
    break;
  default:
    str = "UNKNOWN";
    break;
  }
  return (str);
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
