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

  SHARED_PTR(TestNotification) tn;

  // Create new event handler and validate
  zEvent::Handler *MyHandler = new zEvent::Handler;
  TEST_ISNOT_NULL(MyHandler);

  // Create new event and validate
  TestEvent *MyEvent = new TestEvent;
  TEST_ISNOT_NULL(MyEvent);
  TEST_EQ(zEvent::Event::TYPE_TEST, MyEvent->GetType());

  // Register event with handler
  TEST_TRUE(MyHandler->RegisterEvent(MyEvent));

  // Create new event adapter and validate
  TestAdapter *MyAdapter = new TestAdapter(*MyEvent);
  TEST_ISNOT_NULL(MyAdapter);
  TEST_EQ(Event::TYPE_TEST, MyAdapter->GetType());

  // Register event adapter with handler
  TEST_TRUE(MyHandler->RegisterEvent(MyAdapter));

  // Create new observer and validate
  TestObserver *MyObserver = new TestObserver;
  TEST_ISNOT_NULL(MyObserver);
  TEST_FALSE(MyObserver->TryWait());
  TEST_IS_ZERO(MyObserver->Size());
  TEST_TRUE(MyObserver->Empty());

  // Register observer with handler
  TEST_TRUE(MyHandler->RegisterObserver(MyObserver));
  TEST_FALSE(MyObserver->TryWait());
  TEST_IS_ZERO(MyObserver->Size());
  TEST_TRUE(MyObserver->Empty());

  // Notify
  TEST_TRUE(MyEvent->Notify(8));
  TEST_TRUE(MyObserver->TryWait());
  TEST_EQ(MyObserver->Size(), 1);
  TEST_FALSE(MyObserver->Empty());
  tn = STATIC_CAST(TestNotification)(MyObserver->Front());
  MyObserver->Pop();
  TEST_ISNOT_NULL(tn.get());
  TEST_TRUE(tn->GetType() == zEvent::Event::TYPE_TEST);
  TEST_TRUE(tn->GetEvent().GetType() == zEvent::Event::TYPE_TEST);
  TEST_EQ(8, tn->GetValue());

  // Notify
  TEST_TRUE(MyAdapter->Notify(8));
  TEST_TRUE(MyObserver->TryWait());
  TEST_EQ(MyObserver->Size(), 1);
  TEST_FALSE(MyObserver->Empty());
  tn = STATIC_CAST(TestNotification)(MyObserver->Front());
  MyObserver->Pop();
  TEST_ISNOT_NULL(tn.get());
  TEST_TRUE(tn->GetType() == zEvent::Event::TYPE_TEST);
  TEST_TRUE(tn->GetEvent().GetType() == zEvent::Event::TYPE_TEST);
  TEST_EQ(16, tn->GetValue()); // Adapter doubles the value

  // Unregister observer with handler
  TEST_TRUE(MyHandler->UnregisterObserver(MyObserver));
  TEST_FALSE(MyObserver->TryWait());
  TEST_IS_ZERO(MyObserver->Size());
  TEST_TRUE(MyObserver->Empty());

  // Unregister event adapter with handler
  TEST_TRUE(MyHandler->UnregisterEvent(MyAdapter));

  // Unregister event with handler
  TEST_TRUE(MyHandler->UnregisterEvent(MyEvent));

  // Cleanup
  delete (MyObserver);
  delete (MyHandler);
  delete (MyAdapter);
  delete (MyEvent);

  // Return success
  return (0);
}

