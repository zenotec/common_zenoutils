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

#include <signal.h>

#include <mutex>

#include <zutils/zLog.h>
using namespace zUtils;
ZLOG_MODULE_INIT(zLog::Log::MODULE_TEST);

#include "zTimerTest.h"

int
main(int argc, const char **argv)
{

  // Setup logging for testing
  zLog::FileConnector conn("UnitTest.zlog");
  zLog::Manager::Instance().RegisterConnector(zLog::Log::LEVEL_ALL, &conn);
  zLog::Manager::Instance().SetMaxLevel(zLog::Log::MODULE_TEST, zLog::Log::LEVEL_DEBUG);

  // Test all classes
  UTEST_INIT();
  UTEST_TEST(zTimerTest_Defaults, 0);
  UTEST_TEST(zTimerTest_StartStop, 0);

  zLog::Manager::Instance().UnregisterConnector(zLog::Log::LEVEL_ALL);

  UTEST_FINI();

}
