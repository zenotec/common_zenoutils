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

ZLOG_MODULE_INIT(zLog::Log::MODULE_TEST);

int
zLogTest_Defaults(void* arg_)
{

  pthread_t tid = pthread_self();

  // Test log defaults
  TEST_EQ(zLog::Log::LEVEL_DEF, _zlogger.GetMaxLevel());

  // Setup test connector
  TestConnector conn;
  TEST_IS_ZERO(conn.MsgQueue.Size());

  // Register test connector
  zLog::Manager::Instance().RegisterConnector(zLog::Log::LEVEL_DEBUG, &conn);

  // Log and validate (nothing should be log due to the max level being less than 'DEBUG'
  ZLOG_CRIT("CRIT");
  TEST_FALSE(conn.MsgQueue.TimedWait(10));
  TEST_IS_ZERO(conn.MsgQueue.Size());
  ZLOG_ERR("ERROR");
  TEST_FALSE(conn.MsgQueue.TimedWait(10));
  TEST_IS_ZERO(conn.MsgQueue.Size());
  ZLOG_WARN("WARN");
  TEST_FALSE(conn.MsgQueue.TimedWait(10));
  TEST_IS_ZERO(conn.MsgQueue.Size());
  ZLOG_INFO("INFO");
  TEST_FALSE(conn.MsgQueue.TimedWait(10));
  TEST_IS_ZERO(conn.MsgQueue.Size());
  ZLOG_DEBUG("DEBUG");
  TEST_FALSE(conn.MsgQueue.TimedWait(10));
  TEST_IS_ZERO(conn.MsgQueue.Size());

  // Update max log level
  zLog::Manager::Instance().SetMaxLevel(zLog::Log::MODULE_TEST, zLog::Log::LEVEL_DEBUG);
  TEST_EQ(zLog::Log::LEVEL_DEBUG, zLog::Manager::Instance().GetMaxLevel(zLog::Log::MODULE_TEST));

  // Log and validate (only the debug message should be logged
  ZLOG_CRIT("CRIT");
  TEST_FALSE(conn.MsgQueue.TimedWait(10));
  TEST_IS_ZERO(conn.MsgQueue.Size());
  ZLOG_ERR("ERROR");
  TEST_FALSE(conn.MsgQueue.TimedWait(10));
  TEST_IS_ZERO(conn.MsgQueue.Size());
  ZLOG_WARN("WARN");
  TEST_FALSE(conn.MsgQueue.TimedWait(10));
  TEST_IS_ZERO(conn.MsgQueue.Size());
  ZLOG_INFO("INFO");
  TEST_FALSE(conn.MsgQueue.TimedWait(10));
  TEST_IS_ZERO(conn.MsgQueue.Size());
  ZLOG_DEBUG("DEBUG");
  TEST_TRUE(conn.MsgQueue.TimedWait(10));
  TEST_ISNOT_ZERO(conn.MsgQueue.Size());
  TEST_TRUE(conn.MsgQueue.Pop());
  TEST_IS_ZERO(conn.MsgQueue.Size());

  // Cleanup
  zLog::Manager::Instance().SetMaxLevel(zLog::Log::MODULE_TEST, zLog::Log::LEVEL_WARN);
  TEST_EQ(zLog::Log::LEVEL_WARN, zLog::Manager::Instance().GetMaxLevel(zLog::Log::MODULE_TEST));
  zLog::Manager::Instance().UnregisterConnector(zLog::Log::LEVEL_DEBUG);

  // Return success
  return (0);
}
