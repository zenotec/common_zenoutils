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

#include "zutils/zLog.h"
#include "zDataTest.h"
#include "UnitTest.h"

int
main(int argc, const char **argv)
{

  // Setup logging for testing
  zUtils::zLog::FileConnector conn("zDataTest.zlog");
  zUtils::zLog::Log::Instance().RegisterConnector(zUtils::zLog::CRIT, &conn);
  zUtils::zLog::Log::Instance().RegisterConnector(zUtils::zLog::ERROR, &conn);
  zUtils::zLog::Log::Instance().RegisterConnector(zUtils::zLog::WARN, &conn);
  zUtils::zLog::Log::Instance().RegisterConnector(zUtils::zLog::INFO, &conn);
  zUtils::zLog::Log::Instance().RegisterConnector(zUtils::zLog::DBG, &conn);
  zUtils::zLog::Log::Instance().SetMaxLevel(zUtils::zLog::DBG);

  // Test all classes
  UTEST_INIT();

  UTEST_TEST(zDataTest_Defaults, 0);

  UTEST_TEST(zDataTest_PutValueSingle, 0);
  UTEST_TEST(zDataTest_PutValueMultiple, 0);

  UTEST_TEST(zDataTest_GetChildSingle, 0);
  UTEST_TEST(zDataTest_GetChildMultiple, 0);
  UTEST_TEST(zDataTest_PutChildSingle, 0);
  UTEST_TEST(zDataTest_PutChildMultiple, 0);

  UTEST_TEST(zDataTest_JsonSimple, 0);

  UTEST_TEST(zDataTest_XmlSimple, 0);

  UTEST_TEST(zDataTest_DataCopy, 0);

  UTEST_TEST(zDataTest_Array, 0);

  UTEST_FINI();

}
