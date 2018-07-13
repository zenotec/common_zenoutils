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

#include <zutils/zLog.h>
using namespace zUtils;
ZLOG_MODULE_INIT(zLog::Log::MODULE_TEST);

#include "zDataTest.h"
#include "UnitTest.h"

int
main(int argc, const char **argv)
{

  // Setup logging for testing
  zLog::FileConnector conn("UnitTest.zlog");
  zLog::Manager::Instance().RegisterConnector(zLog::Log::LEVEL_ALL, &conn);
  zLog::Manager::Instance().SetMaxLevel(zLog::Log::MODULE_TEST, zLog::Log::LEVEL_DEBUG);

  // Test all classes
  UTEST_INIT();

  UTEST_TEST(zDataTest_PathDefaults, 0);
  UTEST_TEST(zDataTest_Defaults, 0);

  UTEST_TEST(zDataTest_PathAppend, 0);
  UTEST_TEST(zDataTest_PathPrepend, 0);
  UTEST_TEST(zDataTest_PathCompare, 0);

  UTEST_TEST(zDataTest_PutValueSingle, 0);
  UTEST_TEST(zDataTest_PutValueMultiple, 0);

  UTEST_TEST(zDataTest_GetChild, 0);
  UTEST_TEST(zDataTest_PutChild, 0);
  UTEST_TEST(zDataTest_AddChild, 0);

  UTEST_TEST(zDataTest_JsonSimple, 0);

  UTEST_TEST(zDataTest_XmlSimple, 0);

  UTEST_TEST(zDataTest_DataCopy, 0);

  UTEST_TEST(zDataTest_Array, 0);
  
  UTEST_TEST(zDataTest_PopFront, 0);
  UTEST_TEST(zDataTest_PopBack, 0);
  zLog::Manager::Instance().UnregisterConnector(zLog::Log::LEVEL_ALL);

  UTEST_FINI();

}
