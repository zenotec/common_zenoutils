#include "zLogTest.h"

using namespace zUtils;

int
zLogTest_Defaults(void* arg_)
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
