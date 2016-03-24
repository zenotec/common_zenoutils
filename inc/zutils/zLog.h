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

#include <semaphore.h>
#include <time.h>

#include <string>
#include <fstream>
#include <iostream>
#include <sstream>
#include <vector>

namespace zUtils
{
namespace zLog
{

#define ZLOG_INT(n_)    zUtils::zLog::IntStr((n_))
#define ZLOG_HEX(x_)    zUtils::zLog::HexStr((x_))

#define ZLOG_LOGGER(l_,m_)  \
  { \
    zUtils::zLog::Message msg((l_), __FILE__, __LINE__); \
    msg.AddStr(std::string(__func__)); \
    msg.AddStr(std::string(": ")); \
    msg.AddStr((m_)); \
    zUtils::zLog::Log::Instance().LogMsg(msg); \
  }

#define ZLOG_CRIT(x)    ZLOG_LOGGER(zUtils::zLog::CRIT,(x))
#define ZLOG_ERR(x)     ZLOG_LOGGER(zUtils::zLog::ERROR,(x))
#define ZLOG_WARN(x)    ZLOG_LOGGER(zUtils::zLog::WARN,(x))
#define ZLOG_INFO(x)    ZLOG_LOGGER(zUtils::zLog::INFO,(x))
#define ZLOG_DEBUG(x)   ZLOG_LOGGER(zUtils::zLog::DBG,(x))

enum LogLevel
{
  CRIT = 0, ERROR, WARN, INFO, DBG, LAST
};

std::string
IntStr(int n_);

std::string
PointerStr(void *p_);

template<typename T>
  std::string
  HexStr(T n_)
  {
    char str[256];
    snprintf(str, 256, "%#*x", (int) sizeof(T), n_);
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

  bool
  RegisterConnector(zLog::LogLevel level_, Connector *conn_);
  bool
  UnregisterConnector(zLog::LogLevel level_);

  void
  LogMsg(const Message &msg_);

protected:

private:
  Log();
    Log(Log const&);
  void
  operator=(Log const&);

  zLog::LogLevel _maxLevel;
  ConsoleConnector _defConn;
  std::vector<Connector *> _connTable;

  void
  _lock();
  void
  _unlock();
  sem_t _lockSem;

};

}
}

#endif /* __ZLOG_H__ */
