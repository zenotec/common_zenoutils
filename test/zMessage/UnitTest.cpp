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

#include "zMessageTest.h"

int
main(int argc, const char **argv)
{

  // Setup logging for testing
  zUtils::zLog::FileConnector conn("zMessageTest.zlog");
  zUtils::zLog::Log::Instance().RegisterConnector(zUtils::zLog::CRIT, &conn);
  zUtils::zLog::Log::Instance().RegisterConnector(zUtils::zLog::ERROR, &conn);
  zUtils::zLog::Log::Instance().RegisterConnector(zUtils::zLog::WARN, &conn);
  zUtils::zLog::Log::Instance().RegisterConnector(zUtils::zLog::INFO, &conn);
  zUtils::zLog::Log::Instance().RegisterConnector(zUtils::zLog::DBG, &conn);
  zUtils::zLog::Log::Instance().SetMaxLevel(zUtils::zLog::DBG);

  // Test all classes
  UTEST_INIT();

  UTEST_TEST(zMessageTest_MessageDefaults, 0);

  UTEST_TEST(zMessageTest_FactoryError, 0);
  UTEST_TEST(zMessageTest_FactoryAuth, 0);
  UTEST_TEST(zMessageTest_FactoryHello, 0);
  UTEST_TEST(zMessageTest_FactoryAck, 0);
  UTEST_TEST(zMessageTest_FactoryBye, 0);
  UTEST_TEST(zMessageTest_FactoryCfg, 0);
  UTEST_TEST(zMessageTest_FactoryCmd, 0);
  UTEST_TEST(zMessageTest_FactoryData, 0);

  UTEST_TEST(zMessageTest_MessageGetSet, 0);
  UTEST_TEST(zMessageTest_MessageCopy, 0);

  UTEST_TEST(zMessageTest_MessageLoopSocket, 0);
  UTEST_TEST(zMessageTest_MessageUnixSocket, 0);

//  UTEST_TEST(zMessageTest_MessageHandler, 0);

  UTEST_FINI();

}
