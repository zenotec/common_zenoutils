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

#include "zEventTest.h"

int
main(int argc, const char **argv)
{

  UTEST_INIT();

    // Setup logging for testing
    zUtils::zLog::FileConnector conn("zEventTest.zlog");
    zUtils::zLog::Log::Instance().RegisterConnector(zUtils::zLog::CRIT, &conn);
    zUtils::zLog::Log::Instance().RegisterConnector(zUtils::zLog::ERROR, &conn);
    zUtils::zLog::Log::Instance().RegisterConnector(zUtils::zLog::WARN, &conn);
    zUtils::zLog::Log::Instance().RegisterConnector(zUtils::zLog::INFO, &conn);
    zUtils::zLog::Log::Instance().RegisterConnector(zUtils::zLog::DBG, &conn);
    zUtils::zLog::Log::Instance().SetMaxLevel(zUtils::zLog::DBG);

    // Test all classes
    UTEST_TEST(zEventTest_EventDefaults, 0);
    UTEST_TEST(zEventTest_EventNotificationDefaults, 0);
    UTEST_TEST(zEventTest_EventHandlerDefaults, 0);
    UTEST_TEST(zEventTest_EventManagerDefaults, 0);
    UTEST_TEST(zEventTest_EventTest, 0);
    UTEST_TEST(zEventTest_EventHandlerTest, 0);
//  UTEST_TEST(zEventTest_EventManagerTest, 0);

    zUtils::zLog::Log::Instance().UnregisterConnector(zUtils::zLog::CRIT);
    zUtils::zLog::Log::Instance().UnregisterConnector(zUtils::zLog::ERROR);
    zUtils::zLog::Log::Instance().UnregisterConnector(zUtils::zLog::WARN);
    zUtils::zLog::Log::Instance().UnregisterConnector(zUtils::zLog::INFO);
    zUtils::zLog::Log::Instance().UnregisterConnector(zUtils::zLog::DBG);

    UTEST_FINI();

  }
