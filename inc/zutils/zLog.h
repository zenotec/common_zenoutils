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

#ifndef __ZLOG_H__
#define __ZLOG_H__

//*****************************************************************************
//  Includes
//*****************************************************************************

#include <stdio.h>
#include <stdint.h>
#include <time.h>

#include <string>
#include <istream>
#include <ostream>
#include <fstream>
#include <iostream>
#include <sstream>
#include <vector>
#include <map>

#include <zutils/zCompatibility.h>
#include <zutils/zSem.h>
#include <zutils/zThread.h>
#include <zutils/zQueue.h>

namespace zUtils
{
namespace zLog
{

#define ZLOG_MODULE_INIT(x) static zUtils::zLog::Log _zlogger(x)

#define ZLOG_BOOL(b_)   ((b_) ? std::string("true") : std::string("false"))
#define ZLOG_CHAR(n_)   zUtils::zLog::CharStr((n_))
#define ZLOG_UCHAR(n_)  zUtils::zLog::UcharStr((n_))
#define ZLOG_INT(n_)    zUtils::zLog::IntStr((n_))
#define ZLOG_UINT(n_)   zUtils::zLog::UintStr((n_))
#define ZLOG_LONG(n_)   zUtils::zLog::LongStr((n_))
#define ZLOG_ULONG(n_)  zUtils::zLog::UlongStr((n_))
#define ZLOG_HEX(x_)    zUtils::zLog::HexStr<typeof(x_)>((x_))
#define ZLOG_P(p_)      zUtils::zLog::PointerStr((void*)p_)

#define ZLOG_LOGGER(l_,m_) \
  do { \
	  SHARED_PTR(zUtils::zLog::Message) msg = _zlogger.CreateMessage(l_); \
	  if (msg) { \
	    msg->SetFile(__FILE__); \
	    msg->SetLine(__LINE__); \
	    msg->AddMessage(m_); \
	    _zlogger.LogMessage(msg); \
	  } \
  } while(false);

#define ZLOG_CRIT(x)    ZLOG_LOGGER(zUtils::zLog::Log::LEVEL_CRIT,(x))
#define ZLOG_ERR(x)     ZLOG_LOGGER(zUtils::zLog::Log::LEVEL_ERROR,(x))
#define ZLOG_WARN(x)    ZLOG_LOGGER(zUtils::zLog::Log::LEVEL_WARN,(x))
#define ZLOG_INFO(x)    ZLOG_LOGGER(zUtils::zLog::Log::LEVEL_INFO,(x))
#define ZLOG_DEBUG(x)   ZLOG_LOGGER(zUtils::zLog::Log::LEVEL_DEBUG,(x))
#define ZLOG_DEBUG1(x)   ZLOG_LOGGER(zUtils::zLog::Log::LEVEL_DEBUG1,(x))
#define ZLOG_DEBUG2(x)   ZLOG_LOGGER(zUtils::zLog::Log::LEVEL_DEBUG2,(x))
#define ZLOG_DEBUG3(x)   ZLOG_LOGGER(zUtils::zLog::Log::LEVEL_DEBUG3,(x))

inline std::string
CharStr(char n_)
{
  char str[256];
  snprintf(str, 256, "%hhd", n_);
  return (std::string(str));
}

inline std::string
UcharStr(unsigned char n_)
{
  char str[256];
  snprintf(str, 256, "%hhu", n_);
  return (std::string(str));
}

inline std::string
IntStr(int n_)
{
  char str[256];
  snprintf(str, 256, "%d", n_);
  return (std::string(str));
}

inline std::string
UintStr(unsigned int n_)
{
  char str[256];
  snprintf(str, 256, "%u", n_);
  return (std::string(str));
}

inline std::string
LongStr(long n_)
{
  char str[256];
  snprintf(str, 256, "%ld", n_);
  return (std::string(str));
}

inline std::string
UlongStr(unsigned long n_)
{
  char str[256];
  snprintf(str, 256, "%lu", n_);
  return (std::string(str));
}

inline std::string
PointerStr(void *p_)
{
  char str[256];
  snprintf(str, 256, "%p", p_);
  return (std::string(str));
}

template<typename T>
  inline std::string
  HexStr(T n_)
  {
    char str[256] = { 0x30, 0x78, 0x30, 0x30, 0 };
    if (n_)
    {
      snprintf(str, 256, "%#*x", (int) sizeof(T), (T) n_);
    }
    return (std::string(str));
  }

//*****************************************************************************
// Class: Connector
//*****************************************************************************

class Connector
{

public:

  virtual void
  Logger(std::string msg_) = 0;

protected:

private:

};

//*****************************************************************************
// Class: FileConnector
//*****************************************************************************
class FileConnector : public Connector
{

public:

  FileConnector(const std::string& logfile_);

  virtual
  ~FileConnector();

  virtual void
  Logger(std::string msg_);

protected:

private:

  std::ofstream _file;

};

//*****************************************************************************
// Class: ConsoleConnector
//*****************************************************************************

class ConsoleConnector : public Connector
{
public:

  ConsoleConnector();

  virtual
  ~ConsoleConnector();

  virtual void
  Logger(std::string msg_);

protected:

private:

};

//*****************************************************************************
// Class: Log
//*****************************************************************************

class Message;

class Log
{

public:

  enum MODULE
  {
    MODULE_ALL = -1,
    MODULE_COMMAND,
    MODULE_CONFIG,
    MODULE_DATA,
    MODULE_DISPLAY,
    MODULE_GPIO,
    MODULE_INTERFACE,
    MODULE_LED,
    MODULE_MATH,
    MODULE_MESSAGE,
    MODULE_NODE,
    MODULE_PROGRAM,
    MODULE_SERIAL,
    MODULE_SOCKET,
    MODULE_SWITCH,
    MODULE_THERMO,
    MODULE_WIRELESS,
    MODULE_TEST,
    MODULE_LAST
  };

  enum LEVEL
  {
    LEVEL_ALL = -1,
    LEVEL_CRIT = 0,
    LEVEL_ERROR = 1,
    LEVEL_WARN = 2,
    LEVEL_DEF = LEVEL_WARN,
    LEVEL_INFO = 3,
    LEVEL_DEBUG = 4,
    LEVEL_DEBUG1 = 5,
    LEVEL_DEBUG2 = 6,
    LEVEL_DEBUG3 = 7,
    LEVEL_LAST
  };

  Log(const Log::MODULE module_);

  Log(const std::string& module_);

  virtual
  ~Log();

  Log::LEVEL
  GetMaxLevel();

  void
  SetMaxLevel(Log::LEVEL level_);

  SHARED_PTR(Message)
  CreateMessage(Log::LEVEL level_);

  void
  LogMessage(const SHARED_PTR(Message)& message_);

  static std::string
  ToString(const Log::MODULE module_);

  static std::string
  ToString(const Log::LEVEL level_);

protected:

private:

  std::string _module;

};

//*****************************************************************************
// Message Class
//*****************************************************************************

class Message
{

public:

  Message(const std::string& module_, const Log::LEVEL level_);

  ~Message();

  const std::string&
  GetModule() const;

  const Log::LEVEL
  GetLevel() const;

  const std::string&
  GetProcessId() const;

  const std::string&
  GetThreadId() const;

  const std::string&
  GetTimestamp() const;

  const std::string&
  GetFile() const;

  void
  SetFile(const std::string& file_);

  const std::string&
  GetLine() const;

  void
  SetLine(const unsigned int line_);

  void
  SetLine(const std::string& line_);

  const std::string&
  GetMessage() const;

  void
  AddMessage(const std::string &str);

protected:

private:

  std::string _module;
  Log::LEVEL _level;
  std::string _proc;
  std::string _thread;
  std::string _time;
  std::string _file;
  std::string _line;
  std::string _message;

};

//*****************************************************************************
// Class::Manager
//*****************************************************************************

class Manager : public zThread::ThreadArg, public zThread::ThreadFunction
{

  friend Log;

public:

  virtual
  ~Manager();

  static Manager&
  Instance()
  {
    static Manager instance;
    return (instance);
  }

  bool
  RegisterModule(const std::string& module_);

  bool
  RegisterModule(const Log::MODULE module_);

  bool
  UnregisterModule(const std::string& module_);

  bool
  UnregisterModule(const Log::MODULE module_);

  bool
  RegisterConnector(const Log::LEVEL level_, Connector* conn_);

  bool
  UnregisterConnector(const Log::LEVEL level_);

  Log::LEVEL
  GetMaxLevel(const std::string& module_);

  Log::LEVEL
  GetMaxLevel(const Log::MODULE module_);

  void
  SetMaxLevel(const std::string& module_, const Log::LEVEL level_);

  void
  SetMaxLevel(const Log::MODULE module_, const Log::LEVEL level_);

protected:

  void
  logMessage(const SHARED_PTR(zLog::Message)& message_);

  virtual void
  Run(zThread::ThreadArg *arg_);

private:

  zSem::Mutex _log_lock;
  zThread::Thread _thread;
  zQueue<SHARED_PTR(zLog::Message)> _msg_queue;
  std::map<std::string, Log::LEVEL> _max_level;
  std::map<std::string, int> _mod_refcnt;
  Connector* _conn[Log::LEVEL_LAST];

  Manager();

};

}
}

#endif /* __ZLOG_H__ */
