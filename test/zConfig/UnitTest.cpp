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

#include "zConfTest.h"
#include "UnitTest.h"

int
main(int argc, const char **argv)
{

  // Setup logging for testing
  zUtils::zLog::FileConnector conn("zConfTest.zlog");
  zUtils::zLog::Log::Instance().RegisterConnector(zUtils::zLog::CRIT, &conn);
  zUtils::zLog::Log::Instance().RegisterConnector(zUtils::zLog::ERROR, &conn);
  zUtils::zLog::Log::Instance().RegisterConnector(zUtils::zLog::WARN, &conn);
  zUtils::zLog::Log::Instance().RegisterConnector(zUtils::zLog::INFO, &conn);
  zUtils::zLog::Log::Instance().RegisterConnector(zUtils::zLog::DBG, &conn);
  zUtils::zLog::Log::Instance().SetMaxLevel(zUtils::zLog::DBG);

  // Test all classes
  UTEST_INIT();

  UTEST_TEST(zConfigTest_DataDefaults, 0);
  UTEST_TEST(zConfigTest_ConfigurationDefaults, 0);

  UTEST_TEST(zConfigTest_DataGetPutValue, 0);
  UTEST_TEST(zConfigTest_DataGetPutChild, 0);

  UTEST_TEST(zConfigTest_FileLoadStore, 0);

  UTEST_TEST(zConfigTest_ConnectorDefaults, 0);
  UTEST_TEST(zConfigTest_ConfigurationCtor, 0);
  UTEST_TEST(zConfigTest_ConfigurationGetSetData, 0);
  UTEST_TEST(zConfigTest_ConfigurationCompare, 0);
  UTEST_TEST(zConfigTest_ConfigurationDataArray, 0);
  UTEST_TEST(zConfigTest_ConfigurationLoadStore, 0);

  UTEST_FINI();

  // Exit
  exit(0);

}
