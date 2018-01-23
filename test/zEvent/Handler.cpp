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
zEventTest_EventHandlerTest(void* arg_)
{

  // Create new event and validate
  TestEvent *MyEvent = new TestEvent;
  TEST_ISNOT_NULL(MyEvent);
  TEST_EQ(zEvent::Event::TYPE_TEST, MyEvent->GetType());

  // Create new event notification and validate
  SHARED_PTR(TestNotification) MyNotification(new TestNotification(*MyEvent));
  TEST_ISNOT_NULL(MyNotification.get());
  TEST_EQ(zEvent::Event::TYPE_TEST, MyNotification->GetType());

  // Create new event handler and validate
  zEvent::Handler *MyHandler = new zEvent::Handler;
  TEST_ISNOT_NULL(MyHandler);

  // Register event with handler
  MyHandler->RegisterEvent(MyEvent);

  // Create new observer and validate
  TestObserver *MyObserver = new TestObserver;
  TEST_ISNOT_NULL(MyObserver);
  TEST_FALSE(MyObserver->TryWait());
  TEST_IS_ZERO(MyObserver->Size());
  TEST_TRUE(MyObserver->Empty());

  // Register observer with handler
  MyHandler->RegisterObserver(MyObserver);
  TEST_FALSE(MyObserver->TryWait());
  TEST_IS_ZERO(MyObserver->Size());
  TEST_TRUE(MyObserver->Empty());

  // Notify
  MyEvent->NotifyHandlers(MyNotification);
  TEST_TRUE(MyObserver->TryWait());
  TEST_EQ(MyObserver->Size(), 1);
  TEST_FALSE(MyObserver->Empty());
  TEST_TRUE(MyObserver->Front()->GetType() == zEvent::Event::TYPE_TEST);
  MyObserver->Pop();

  // Unregister observer with handler
  MyHandler->UnregisterObserver(MyObserver);
  TEST_FALSE(MyObserver->TryWait());
  TEST_IS_ZERO(MyObserver->Size());
  TEST_TRUE(MyObserver->Empty());

  // Unregister event with handler
  MyHandler->UnregisterEvent(MyEvent);

  // Cleanup
  delete (MyObserver);
  delete (MyHandler);
  delete (MyEvent);

  // Return success
  return (0);
}

