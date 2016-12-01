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
  this->_line = zLog::IntStr(line_);
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
  return (zUtils::zLog::IntStr(getpid()));
}

std::string
Message::_getThreadId() const
{
  pthread_t tid = pthread_self();
  return (zUtils::zLog::HexStr((uint32_t) tid));
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
  // Initialize lock to 'locked'
  int ret = sem_init(&this->_lockSem, 0, 0);
  if (ret != 0)
  {
    throw;
  } // end if

  // Initialize connector table to defaults
  for (int i = 0; i < zLog::LAST; i++)
  {
    this->_connTable[i] = NULL;
  } // end for

  // Unlock
  this->_unlock();
}

Log::~Log()
{
  // Grab lock
  this->_lock();

  // Destroy lock
  int ret = sem_destroy(&this->_lockSem);
  if (ret != 0)
  {
    throw;
  } // end if
}

zLog::LogLevel
Log::GetMaxLevel()
{
  zLog::LogLevel level;
  this->_lock();
  level = this->_maxLevel;
  this->_unlock();
  return (level);
}

void
Log::SetMaxLevel(zLog::LogLevel level_)
{
  if (level_ < zLog::LAST)
  {
    this->_lock();
    this->_maxLevel = level_;
    this->_unlock();
  } // end if
}

bool
Log::RegisterConnector(zLog::LogLevel level_, Connector *conn_)
{
  this->_lock();
  this->_connTable[level_] = conn_;
  this->_unlock();
  return (true);
}

bool
Log::UnregisterConnector(zLog::LogLevel level_)
{
  this->_lock();
  this->_connTable[level_] = NULL;
  this->_unlock();
  return (true);
}

void
Log::LogMsg(const Message &msg_)
{
  if (msg_.GetLevel() <= this->_maxLevel)
  {
    this->_lock();
    std::string str = msg_.GetStr();
    Connector *conn = this->_connTable[msg_.GetLevel()];
    if (conn != NULL)
    {
      conn->Logger(str);
    }
    this->_unlock();
  }
}

void
Log::_lock()
{
  sem_wait(&this->_lockSem);
}

void
Log::_unlock()
{
  sem_post(&this->_lockSem);
}

}
}

