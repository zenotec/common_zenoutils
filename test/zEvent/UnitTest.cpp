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

#include "zEventTest.h"

int
main(int argc, const char **argv)
{

  // Test all classes
  UTEST_INIT();
  UTEST_TEST(zEventTest_EventDefaults, 0);
  UTEST_TEST(zEventTest_EventNotificationDefaults, 0);
  UTEST_TEST(zEventTest_EventHandlerDefaults, 0);
  UTEST_TEST(zEventTest_EventManagerDefaults, 0);
  UTEST_TEST(zEventTest_EventTest, 0);
  UTEST_TEST(zEventTest_EventHandlerTestSingleEventSingleObserver, 0);
  UTEST_TEST(zEventTest_EventHandlerTestSingleEventMultiObserver, 0);
  UTEST_TEST(zEventTest_EventHandlerTestMultiEventSingleObserver, 0);
  UTEST_TEST(zEventTest_EventHandlerTestMultiEventMultiObserver, 0);
  UTEST_TEST(zEventTest_EventManagerTest, 0);
  UTEST_FINI();

}
