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

#include <string>
#include <list>
#include <mutex>

#include <zutils/zLog.h>
#include <zutils/zSem.h>
#include <zutils/zThread.h>
#include <zutils/zData.h>
#include <zutils/zEvent.h>
#include <zutils/zConfig.h>
#include <zutils/zSwitch.h>

#include <zutils/zGpio.h>

#include "zGpioTest.h"

int
main(int argc, const char **argv)
{
  struct stat st = { 0 };

  // Setup logging for testing
  zUtils::zLog::FileConnector conn("zGpioTest.zlog");
  zUtils::zLog::Log::Instance().RegisterConnector(zUtils::zLog::CRIT, &conn);
  zUtils::zLog::Log::Instance().RegisterConnector(zUtils::zLog::ERROR, &conn);
  zUtils::zLog::Log::Instance().RegisterConnector(zUtils::zLog::WARN, &conn);
  zUtils::zLog::Log::Instance().RegisterConnector(zUtils::zLog::INFO, &conn);
  zUtils::zLog::Log::Instance().RegisterConnector(zUtils::zLog::DBG, &conn);
  zUtils::zLog::Log::Instance().SetMaxLevel(zUtils::zLog::DBG);

  // Test all classes
  UTEST_INIT();

  if (stat(TESTDIR.c_str(), &st) == -1)
  {
    mkdir(TESTDIR.c_str(), 0777);
  }

  UTEST_TEST(zGpioTest_ConfigurationDefaults, 0);
  UTEST_TEST(zGpioTest_ConfigurationCtor, 0);
  UTEST_TEST(zGpioTest_ConfigurationGetSet, 0);

  UTEST_TEST(zGpioTest_PortDefaults, 0);
  UTEST_TEST(zGpioTest_PortGetSet, 0);

  UTEST_TEST(zGpioTest_HandlerDefaults, 0);
  UTEST_TEST(zGpioTest_HandlerAddPort, 0);
  UTEST_TEST(zGpioTest_HandlerOnOff, 0);

//  UTEST_TEST(zGpioTest_SwitchDefaults, 0);
//  UTEST_TEST(zGpioTest_SwitchOnOff, 0);

  if (stat(TESTDIR.c_str(), &st) != -1)
  {
    rmdir(TESTDIR.c_str());
  }

  UTEST_FINI();

}
