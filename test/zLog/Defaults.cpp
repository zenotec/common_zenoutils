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
