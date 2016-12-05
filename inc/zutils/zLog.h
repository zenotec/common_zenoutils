//*****************************************************************************
//    Copyright (C) 2016 ZenoTec LLC (http://www.zenotec.net)
//
//    File:
//    Description:
//
//*****************************************************************************

#ifndef __ZLOG_H__
#define __ZLOG_H__

//*****************************************************************************
//  Includes
//*****************************************************************************

#include <stdio.h>
#include <stdint.h>
#include <time.h>

#include <string>
#include <fstream>
#include <iostream>
#include <sstream>
#include <vector>

#include <zutils/zCompatibility.h>

namespace zUtils
{
namespace zLog
{

#define ZLOG_BOOL(b_)   ((b_) ? std::string("true") : std::string("false"))
#define ZLOG_INT(n_)    zUtils::zLog::IntStr((n_))
#define ZLOG_UINT(n_)   zUtils::zLog::UintStr((n_))
#define ZLOG_LONG(n_)   zUtils::zLog::LongStr((n_))
#define ZLOG_ULONG(n_)  zUtils::zLog::UlongStr((n_))
#define ZLOG_HEX(x_)    zUtils::zLog::HexStr<typeof(x_)>((x_))
#define ZLOG_P(p_)      zUtils::zLog::PointerStr((void*)p_)

#define ZLOG_LOGGER(l_,m_)  \
  do { \
    zUtils::zLog::Message msg((l_), __FILE__, __LINE__); \
    msg.AddStr(std::string(__func__)); \
    msg.AddStr(std::string(": ")); \
    msg.AddStr((m_)); \
    zUtils::zLog::Log::Instance().LogMsg(msg); \
  } while(false);

#define ZLOG_CRIT(x)    ZLOG_LOGGER(zUtils::zLog::CRIT,(x))
#define ZLOG_ERR(x)     ZLOG_LOGGER(zUtils::zLog::ERROR,(x))
#define ZLOG_WARN(x)    ZLOG_LOGGER(zUtils::zLog::WARN,(x))
#define ZLOG_INFO(x)    ZLOG_LOGGER(zUtils::zLog::INFO,(x))
#define ZLOG_DEBUG(x)   ZLOG_LOGGER(zUtils::zLog::DBG,(x))

enum LogLevel
{
  CRIT = 0, ERROR, WARN, INFO, DBG, LAST
};

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
    char str[256];
    snprintf(str, 256, "%#*x", (int) sizeof(T), (T) n_);
    return (std::string(str));
  }

//*****************************************************************************
// Connector Class
//*****************************************************************************
class Connector
{
public:
  Connector();
  virtual
  ~Connector();

  virtual void
  Logger(std::string msg_) = 0;

protected:

private:

};

//*****************************************************************************
// FileConnector Class
//*****************************************************************************
class FileConnector : public Connector
{
public:
  FileConnector(const char *logfile_);
  virtual
  ~FileConnector();

  virtual void
  Logger(std::string msg_);

protected:

private:
  std::ofstream _file;

};

//*****************************************************************************
// ConsoleConnector Class
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
// Message Class
//*****************************************************************************
class Message
{
public:
  Message(zLog::LogLevel level_, const char *name_, int line_);
  virtual
  ~Message();

  zLog::LogLevel
  GetLevel() const;

  std::string
  GetStr() const;

  void
  AddStr(const std::string &str);

protected:

private:
  std::string
  _getProcId() const;
  std::string
  _getThreadId() const;
  std::string
  _getTimestamp() const;

  zLog::LogLevel _level;
  std::string _file;
  std::string _line;
  std::string _msg;

};

//*****************************************************************************
// Log Class
//*****************************************************************************
class Log
{
public:

  virtual
  ~Log();

  static Log &
  Instance()
  {
    static Log instance;
    return (instance);
  }

  zLog::LogLevel
  GetMaxLevel();

  void
  SetMaxLevel(zLog::LogLevel level_);

  void
  RegisterConnector(zLog::LogLevel level_, Connector *conn_);

  void
  UnregisterConnector(zLog::LogLevel level_);

  void
  LogMsg(const Message &msg_);

protected:

private:

  Log();

  Log(Log const&);

  void
  operator=(Log const&);

  MUTEX _log_lock;

  zLog::LogLevel _maxLevel;
  ConsoleConnector _defConn;
  std::vector<Connector *> _connTable;

};

}
}

#endif /* __ZLOG_H__ */
