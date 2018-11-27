/*
 * Copyright (c) 2014-2018 ZenoTec LLC (http://www.zenotec.net)
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
zEventTest_EventHandlerTestSingleEventSingleObserver(void* arg_)
{

  // Create new event and validate
  TestEvent *MyEvent = new TestEvent;
  TEST_ISNOT_NULL(MyEvent);
  TEST_EQ(zEvent::TYPE_TEST, MyEvent->GetType());

  // Create new event handler and validate
  zEvent::Handler *MyHandler = new zEvent::Handler;
  TEST_ISNOT_NULL(MyHandler);

  // Register event with handler
  MyHandler->RegisterEvent(MyEvent);

  // Create new observer and validate
  TestObserver *MyObserver = new TestObserver(false);
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
  TEST_EQ(zEvent::STATUS_OK, MyEvent->Notify(100));
  TEST_TRUE(MyObserver->TryWait());

  // Verify
  TEST_EQ(MyObserver->Size(), 1);
  TEST_FALSE(MyObserver->Empty());
  TEST_EQ(101, MyObserver->Front());
  TEST_TRUE(MyObserver->Pop());
  TEST_IS_ZERO(MyObserver->Size());

  // Unregister observer with handler
  TEST_TRUE(MyHandler->UnregisterObserver(MyObserver));
  TEST_FALSE(MyObserver->TryWait());
  TEST_IS_ZERO(MyObserver->Size());
  TEST_TRUE(MyObserver->Empty());

  // Notify
  TEST_EQ(zEvent::STATUS_NONE, MyEvent->Notify(200));
  TEST_FALSE(MyObserver->TryWait());

  // Unregister event with handler
  TEST_TRUE(MyHandler->UnregisterEvent(MyEvent));

  // Cleanup
  delete (MyObserver);
  delete (MyHandler);
  delete (MyEvent);

  // Return success
  return (UTEST_PASS);
}

int
zEventTest_EventHandlerTestSingleEventMultiObserver(void* arg_)
{

  SHARED_PTR(TestNotification) MyNotification;

  // Create new event and validate
  TestEvent *MyEvent = new TestEvent;
  TEST_ISNOT_NULL(MyEvent);
  TEST_EQ(zEvent::TYPE_TEST, MyEvent->GetType());

  // Create new event handler and validate
  zEvent::Handler *MyHandler = new zEvent::Handler;
  TEST_ISNOT_NULL(MyHandler);

  // Register event with handler
  MyHandler->RegisterEvent(MyEvent);

  // Create new observer and validate
  TestObserver *MyObserver1 = new TestObserver;
  TEST_ISNOT_NULL(MyObserver1);
  TEST_FALSE(MyObserver1->TryWait());
  TEST_IS_ZERO(MyObserver1->Size());
  TEST_TRUE(MyObserver1->Empty());

  // Create new observer and validate
  TestObserver *MyObserver2 = new TestObserver;
  TEST_ISNOT_NULL(MyObserver2);
  TEST_FALSE(MyObserver2->TryWait());
  TEST_IS_ZERO(MyObserver2->Size());
  TEST_TRUE(MyObserver2->Empty());

  // Register first observer with handler
  MyHandler->RegisterObserver(MyObserver1);
  TEST_FALSE(MyObserver1->TryWait());
  TEST_IS_ZERO(MyObserver1->Size());
  TEST_TRUE(MyObserver1->Empty());

  // Notify first observer
  TEST_EQ(zEvent::STATUS_OK_CONT, MyEvent->Notify(100));
  TEST_TRUE(MyObserver1->TryWait());
  TEST_FALSE(MyObserver1->TryWait());
  TEST_FALSE(MyObserver2->TryWait());

  // Verify first observer
  TEST_EQ(MyObserver1->Size(), 1);
  TEST_FALSE(MyObserver1->Empty());
  TEST_EQ(101, MyObserver1->Front());
  TEST_TRUE(MyObserver1->Pop());
  TEST_IS_ZERO(MyObserver1->Size());

  // Register second observer with handler
  TEST_TRUE(MyHandler->RegisterObserver(MyObserver2));
  TEST_FALSE(MyObserver2->TryWait());
  TEST_IS_ZERO(MyObserver2->Size());
  TEST_TRUE(MyObserver2->Empty());

  // Notify both observers
  TEST_EQ(zEvent::STATUS_OK_CONT, MyEvent->Notify(200));
  TEST_TRUE(MyObserver1->TryWait());
  TEST_FALSE(MyObserver1->TryWait());
  TEST_TRUE(MyObserver2->TryWait());
  TEST_FALSE(MyObserver2->TryWait());

  // Verify first observer
  TEST_EQ(MyObserver1->Size(), 1);
  TEST_FALSE(MyObserver1->Empty());
  TEST_EQ(201, MyObserver1->Front());
  TEST_TRUE(MyObserver1->Pop());
  TEST_IS_ZERO(MyObserver1->Size());

  // Verify second observer
  TEST_EQ(MyObserver2->Size(), 1);
  TEST_FALSE(MyObserver2->Empty());
  TEST_EQ(202, MyObserver2->Front());
  TEST_TRUE(MyObserver2->Pop());
  TEST_IS_ZERO(MyObserver2->Size());

  // Unregister first observer with handler
  TEST_TRUE(MyHandler->UnregisterObserver(MyObserver1));
  TEST_FALSE(MyObserver1->TryWait());
  TEST_IS_ZERO(MyObserver1->Size());
  TEST_TRUE(MyObserver1->Empty());

  // Notify second observer
  TEST_EQ(zEvent::STATUS_OK_CONT, MyEvent->Notify(100));
  TEST_FALSE(MyObserver1->TryWait());
  TEST_TRUE(MyObserver2->TryWait());
  TEST_FALSE(MyObserver2->TryWait());

  // Verify second observer
  TEST_EQ(MyObserver2->Size(), 1);
  TEST_FALSE(MyObserver2->Empty());
  TEST_EQ(101, MyObserver2->Front());
  TEST_TRUE(MyObserver2->Pop());
  TEST_IS_ZERO(MyObserver2->Size());

  // Unregister second observer with handler
  TEST_TRUE(MyHandler->UnregisterObserver(MyObserver2));
  TEST_FALSE(MyObserver2->TryWait());
  TEST_IS_ZERO(MyObserver2->Size());
  TEST_TRUE(MyObserver2->Empty());

  // Unregister event with handler
  TEST_TRUE(MyHandler->UnregisterEvent(MyEvent));

  // Cleanup
  delete (MyObserver2);
  delete (MyObserver1);
  delete (MyHandler);
  delete (MyEvent);

  // Return success
  return (UTEST_PASS);
}

// TODO
int
zEventTest_EventHandlerTestMultiEventSingleObserver(void* arg_)
{

  // Create new event and validate
  TestEvent *MyEvent1 = new TestEvent;
  TEST_ISNOT_NULL(MyEvent1);
  TEST_EQ(zEvent::TYPE_TEST, MyEvent1->GetType());

  // Create new event and validate
  TestEvent *MyEvent2 = new TestEvent;
  TEST_ISNOT_NULL(MyEvent2);
  TEST_EQ(zEvent::TYPE_TEST, MyEvent1->GetType());

  // Create new event handler and validate
  zEvent::Handler *MyHandler = new zEvent::Handler;
  TEST_ISNOT_NULL(MyHandler);

  // Register event with handler
  MyHandler->RegisterEvent(MyEvent1);
  MyHandler->RegisterEvent(MyEvent2);

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
  TEST_EQ(zEvent::STATUS_OK_CONT, MyEvent1->Notify(100));
  TEST_TRUE(MyObserver->TryWait());

  // Verify
  TEST_EQ(MyObserver->Size(), 1);
  TEST_FALSE(MyObserver->Empty());
  TEST_EQ(101, MyObserver->Front());
  TEST_TRUE(MyObserver->Pop());
  TEST_IS_ZERO(MyObserver->Size());

  // Unregister event with handler
  MyHandler->UnregisterEvent(MyEvent1);

  // Cleanup
  delete (MyObserver);
  delete (MyHandler);
  delete (MyEvent2);
  delete (MyEvent1);

  // Return success
  return (0);
}


// TODO
int
zEventTest_EventHandlerTestMultiEventMultiObserver(void* arg_)
{

  SHARED_PTR(TestNotification) MyNotification;

  // Create new event and validate
  TestEvent *MyEvent = new TestEvent;
  TEST_ISNOT_NULL(MyEvent);
  TEST_EQ(zEvent::TYPE_TEST, MyEvent->GetType());

  // Create new event handler and validate
  zEvent::Handler *MyHandler = new zEvent::Handler;
  TEST_ISNOT_NULL(MyHandler);

  // Register event with handler
  MyHandler->RegisterEvent(MyEvent);

  // Create new observer and validate
  TestObserver *MyObserver1 = new TestObserver;
  TEST_ISNOT_NULL(MyObserver1);
  TEST_FALSE(MyObserver1->TryWait());
  TEST_IS_ZERO(MyObserver1->Size());
  TEST_TRUE(MyObserver1->Empty());

  // Create new observer and validate
  TestObserver *MyObserver2 = new TestObserver;
  TEST_ISNOT_NULL(MyObserver2);
  TEST_FALSE(MyObserver2->TryWait());
  TEST_IS_ZERO(MyObserver2->Size());
  TEST_TRUE(MyObserver2->Empty());

  // Register first observer with handler
  MyHandler->RegisterObserver(MyObserver1);
  TEST_FALSE(MyObserver1->TryWait());
  TEST_IS_ZERO(MyObserver1->Size());
  TEST_TRUE(MyObserver1->Empty());

  // Notify first observer
  TEST_EQ(zEvent::STATUS_OK_CONT, MyEvent->Notify(100));
  TEST_TRUE(MyObserver1->TryWait());
  TEST_FALSE(MyObserver1->TryWait());
  TEST_FALSE(MyObserver2->TryWait());

  // Verify first observer
  TEST_EQ(MyObserver1->Size(), 1);
  TEST_FALSE(MyObserver1->Empty());
  TEST_EQ(101, MyObserver1->Front());
  TEST_TRUE(MyObserver1->Pop());
  TEST_IS_ZERO(MyObserver1->Size());

  // Register second observer with handler
  TEST_TRUE(MyHandler->RegisterObserver(MyObserver2));
  TEST_FALSE(MyObserver2->TryWait());
  TEST_IS_ZERO(MyObserver2->Size());
  TEST_TRUE(MyObserver2->Empty());

  // Notify both observers
  TEST_EQ(zEvent::STATUS_OK_CONT, MyEvent->Notify(200));
  TEST_TRUE(MyObserver1->TryWait());
  TEST_FALSE(MyObserver1->TryWait());
  TEST_TRUE(MyObserver2->TryWait());
  TEST_FALSE(MyObserver2->TryWait());

  // Verify first observer
  TEST_EQ(MyObserver1->Size(), 1);
  TEST_FALSE(MyObserver1->Empty());
  TEST_EQ(201, MyObserver1->Front());
  TEST_TRUE(MyObserver1->Pop());
  TEST_IS_ZERO(MyObserver1->Size());

  // Verify second observer
  TEST_EQ(MyObserver2->Size(), 1);
  TEST_FALSE(MyObserver2->Empty());
  TEST_EQ(202, MyObserver2->Front());
  TEST_TRUE(MyObserver2->Pop());
  TEST_IS_ZERO(MyObserver2->Size());

  // Unregister first observer with handler
  TEST_TRUE(MyHandler->UnregisterObserver(MyObserver1));
  TEST_FALSE(MyObserver1->TryWait());
  TEST_IS_ZERO(MyObserver1->Size());
  TEST_TRUE(MyObserver1->Empty());

  // Notify second observer
  TEST_EQ(zEvent::STATUS_OK_CONT, MyEvent->Notify(100));
  TEST_FALSE(MyObserver1->TryWait());
  TEST_TRUE(MyObserver2->TryWait());
  TEST_FALSE(MyObserver2->TryWait());

  // Verify second observer
  TEST_EQ(MyObserver2->Size(), 1);
  TEST_FALSE(MyObserver2->Empty());
  TEST_EQ(101, MyObserver2->Front());
  TEST_TRUE(MyObserver2->Pop());
  TEST_IS_ZERO(MyObserver2->Size());

  // Unregister second observer with handler
  TEST_TRUE(MyHandler->UnregisterObserver(MyObserver2));
  TEST_FALSE(MyObserver2->TryWait());
  TEST_IS_ZERO(MyObserver2->Size());
  TEST_TRUE(MyObserver2->Empty());

  // Unregister event with handler
  TEST_TRUE(MyHandler->UnregisterEvent(MyEvent));

  // Cleanup
  delete (MyObserver2);
  delete (MyObserver1);
  delete (MyHandler);
  delete (MyEvent);

  // Return success
  return (UTEST_PASS);
}

