#include <fstream>

#include "zLogTest.h"


using namespace zUtils;

int
GetFileSize(std::ifstream &is)
{
  is.seekg(0, is.end);
  int length = is.tellg();
  is.seekg(0, is.beg);
  return (length);
}

int
zLogTest_FileConnector(void* arg_)
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

