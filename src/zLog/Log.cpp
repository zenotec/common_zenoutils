//*****************************************************************************
//    Copyright (C) 2016 ZenoTec LLC (http://www.zenotec.net)
//
//    File:
//    Description:
//
//*****************************************************************************

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

static const char *levelStr[] = { "CRIT", "ERROR", "WARN", "INFO", "DEBUG" };

//*****************************************************************************
// Connector Class
//*****************************************************************************
Connector::Connector()
{
}

Connector::~Connector()
{
}

//*****************************************************************************
// FileConnector Class
//*****************************************************************************
FileConnector::FileConnector(const char *logfile_) :
    _file(logfile_, std::ios_base::app)
{
  if (this->_file.is_open())
  {
    this->_file << "************************************************************\n";
    this->_file << "* Logging started\n";
    this->_file << "************************************************************\n";
    this->_file.flush();
  }
}

FileConnector::~FileConnector()
{
  if (this->_file.is_open())
  {
    this->_file << "************************************************************\n";
    this->_file << "* Logging stopped\n";
    this->_file << "************************************************************\n";
    this->_file.flush();
    this->_file.close();
  }
}

void
FileConnector::Logger(std::string msg_)
{
  this->_file << msg_;
  this->_file.flush();
}

//*****************************************************************************
// ConsoleConnector Class
//*****************************************************************************
ConsoleConnector::ConsoleConnector()
{
}

ConsoleConnector::~ConsoleConnector()
{
}

void
ConsoleConnector::Logger(std::string msg_)
{
  std::cout << msg_;
  std::cout.flush();
}

//*****************************************************************************
// Message Class
//*****************************************************************************
Message::Message(zLog::LogLevel level_, const char *name_, int line_) :
    _level(level_)
{
  std::string name(name_);
  name = name.substr(name.find_last_of("/") + 1);
  this->_file = name;
  this->_line = ZLOG_INT(line_);
}

Message::~Message()
{
}

zLog::LogLevel
Message::GetLevel() const
{
  return (this->_level);
}

std::string
Message::GetStr() const
{
  std::string str;
  str += this->_getTimestamp() + "\t";
  str += this->_getProcId() + "\t";
  str += this->_getThreadId() + "\t";
  str += std::string(levelStr[this->_level]) + "\t";
  str += this->_file + "[" + this->_line + "]\t";
  str += this->_msg + "\n";
  return (str);
}

void
Message::AddStr(const std::string &str_)
{
  this->_msg += str_;
}

std::string
Message::_getProcId() const
{
  return (ZLOG_INT(getpid()));
}

std::string
Message::_getThreadId() const
{
  pthread_t tid = pthread_self();
  return (ZLOG_HEX((uint32_t ) tid));
}

std::string
Message::_getTimestamp() const
{
  struct timespec ts;
  std::stringstream timestamp;
  clock_gettime(CLOCK_REALTIME, &ts);
  timestamp << std::setprecision(20)
      << ((double) ts.tv_sec + (double) ts.tv_nsec / (double) (1000000000));
  return (timestamp.str());
}

//*****************************************************************************
// Log Class
//*****************************************************************************
Log::Log() :
    _maxLevel(zLog::WARN), _connTable(zLog::LAST)
{

  // Initialize connector table to defaults
  for (int i = 0; i < zLog::LAST; i++)
  {
    this->_connTable[i] = NULL;
  } // end for

  // Unlock
  this->_log_lock.unlock();
}

Log::~Log()
{
  // Grab lock
  this->_log_lock.lock();
  // Initialize connector table to defaults
  for (int i = 0; i < zLog::LAST; i++)
  {
    this->_connTable[i] = NULL;
  } // end for
}

zLog::LogLevel
Log::GetMaxLevel()
{
  zLog::LogLevel level;
  this->_log_lock.lock();
  level = this->_maxLevel;
  this->_log_lock.unlock();
  return (level);
}

void
Log::SetMaxLevel(zLog::LogLevel level_)
{
  if (level_ < zLog::LAST)
  {
    this->_log_lock.lock();
    this->_maxLevel = level_;
    this->_log_lock.unlock();
  } // end if
  return;
}

void
Log::RegisterConnector(zLog::LogLevel level_, Connector *conn_)
{
  this->_log_lock.lock();
  this->_connTable[level_] = conn_;
  this->_log_lock.unlock();
  ZLOG_LOGGER(level_, "Connector registered");
  return;
}

void
Log::UnregisterConnector(zLog::LogLevel level_)
{
  ZLOG_LOGGER(level_, "Connector unregistered");
  this->_log_lock.lock();
  this->_connTable[level_] = NULL;
  this->_log_lock.unlock();
  return;
}

void
Log::LogMsg(const Message &msg_)
{
//  std::cout << "+" << std::endl;
//  std::cout.flush();
  try
  {
    this->_log_lock.lock();
//    std::cout << ZLOG_HEX((uint32_t )pthread_self()) << ": log lock(" << &this->_log_lock << ")"
//        << std::endl;
//    std::cout.flush();
    if (msg_.GetLevel() <= this->_maxLevel)
    {
      if (this->_connTable[msg_.GetLevel()] != NULL)
      {
//        std::cout << ZLOG_HEX((uint32_t )pthread_self()) << ": " << msg_.GetStr();
        this->_connTable[msg_.GetLevel()]->Logger(msg_.GetStr());
      }
    }
//    std::cout << ZLOG_HEX((uint32_t )pthread_self()) << ": log unlock(" << &this->_log_lock << ")"
//        << std::endl;
//    std::cout.flush();
    this->_log_lock.unlock();
  }
  catch (const std::system_error& e)
  {
//    std::cout << ZLOG_HEX((uint32_t )pthread_self()) << ": " << "Caught system error: " << e.what() << std::endl;
//    std::cout << ZLOG_HEX((uint32_t )pthread_self()) << ": " << msg_.GetStr();
//    std::cout.flush();
  }
//  std::cout << "-" << std::endl;
//  std::cout.flush();
}

}
}

