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
zEventTest_EventManagerTest(void* arg_)
{
  bool status = false;

  // Create new event and validate
  zEvent::Event *MyEvent = new zEvent::Event(zEvent::Event::TYPE_TEST);
  TEST_ISNOT_NULL(MyEvent);
  TEST_EQ(zEvent::Event::TYPE_TEST, MyEvent->GetType());

  // Register event with handler
  zEvent::Manager::Instance().RegisterEvent(MyEvent);

  // Create new observer and validate
  TestObserver *MyObserver = new TestObserver;
  TEST_ISNOT_NULL(MyObserver);

  // Register observer with handler
  zEvent::Manager::Instance().RegisterObserver(MyObserver);
  TEST_FALSE(MyObserver->TimedWait(1));

  // Notify
  MyEvent->NotifyHandlers(NULL);
  TEST_TRUE(MyObserver->TimedWait(100));

  // Unregister observer with handler
  zEvent::Manager::Instance().UnregisterObserver(MyObserver);
  TEST_FALSE(MyObserver->TimedWait(1));

  // Unregister event with handler
  zEvent::Manager::Instance().UnregisterEvent(MyEvent);

  // Cleanup
  delete (MyObserver);
  delete (MyEvent);

  // Return success
  return (0);
}

