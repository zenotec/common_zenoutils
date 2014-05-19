/*
 * zLog_utest.cpp
 *
 *  Created on: May 14, 2014
 *      Author: kmahoney
 */

#include <stdio.h>

#include <fstream>

#include "UnitTest.h"
#include "zutils/zLog.h"

using namespace Test;
using namespace zUtils;

int
GetFileSize(std::ifstream &is)
{
  is.seekg(0, is.end);
  int length = is.tellg();
  is.seekg(0, is.beg);
  return (length);
}

class TestConnector : public zLog::Connector
{
public:

  virtual void
  Logger(std::string msg_)
  {
    this->logMsg += msg_;
  }
  std::string logMsg;
};

static int
UnitTest_zLog_Defaults(int arg_)
{

  // Test log defaults
  TEST_EQ(zLog::WARN, zLog::Log::Instance().GetMaxLevel());

  // Setup test connector
  TestConnector conn;
  TEST_IS_ZERO(conn.logMsg.size());
  zLog::Log::Instance().RegisterConnector(zLog::DBG, &conn);

  // Log and validate (nothing should be log due to the max level being less than 'DEBUG'
  ZLOG_CRIT("CRIT");
  TEST_IS_ZERO(conn.logMsg.size());
  ZLOG_ERR("ERROR");
  TEST_IS_ZERO(conn.logMsg.size());
  ZLOG_WARN("WARN");
  TEST_IS_ZERO(conn.logMsg.size());
  ZLOG_INFO("INFO");
  TEST_IS_ZERO(conn.logMsg.size());
  ZLOG_DEBUG("DEBUG");
  TEST_IS_ZERO(conn.logMsg.size());

  // Update max log level
  zLog::Log::Instance().SetMaxLevel(zLog::DBG);
  TEST_EQ(zLog::DBG, zLog::Log::Instance().GetMaxLevel());

  // Log and validate (only the debug message should be logged
  ZLOG_CRIT("CRIT");
  TEST_IS_ZERO(conn.logMsg.size());
  ZLOG_ERR("ERROR");
  TEST_IS_ZERO(conn.logMsg.size());
  ZLOG_WARN("WARN");
  TEST_IS_ZERO(conn.logMsg.size());
  ZLOG_INFO("INFO");
  TEST_IS_ZERO(conn.logMsg.size());
  ZLOG_DEBUG("DEBUG");
  TEST_ISNOT_ZERO(conn.logMsg.size());

  // Cleanup
  zLog::Log::Instance().SetMaxLevel(zLog::WARN);
  TEST_EQ(zLog::WARN, zLog::Log::Instance().GetMaxLevel());
  zLog::Log::Instance().UnregisterConnector(zLog::DBG);

  // Return success
  return (0);
}

static int
UnitTest_zLog_FileConnector(int arg_)
{

  // Log file names
  const char *logName = "/tmp/fconn-utest.log";

  // Log file streams
  std::ifstream logFile;

  // Clean up from previous runs
  remove(logName);
  logFile.open(logName);
  TEST_FALSE(logFile.is_open());

  // Test log defaults
  TEST_EQ(zLog::WARN, zLog::Log::Instance().GetMaxLevel());

  // Create file connector and register
  zLog::FileConnector fileConn(logName);
  zLog::Log::Instance().RegisterConnector(zLog::CRIT, &fileConn);

  // Log message and validate
  logFile.open(logName);
  TEST_TRUE(logFile.is_open());
  int logSize = GetFileSize(logFile);
  TEST_ISNOT_ZERO(logSize);
  ZLOG_CRIT("CRIT");
  TEST_NEQ(logSize, GetFileSize(logFile));
  logFile.close();

  // Cleanup
  zLog::Log::Instance().UnregisterConnector(zLog::CRIT);

  // Clean up log files from /tmp
  remove(logName);
  logFile.open(logName);
  TEST_FALSE(logFile.is_open());

  // Return success
  return (0);
}

int
zLog_utest(void)
{
  INIT();
  UTEST( UnitTest_zLog_Defaults, 0);
  UTEST( UnitTest_zLog_FileConnector, 0);
  FINI();
}

