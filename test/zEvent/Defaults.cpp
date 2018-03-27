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
zEventTest_EventDefaults(void * arg_)
{
  // Create new event and validate
  TestEvent *MyEvent = new TestEvent;
  TEST_ISNOT_NULL(MyEvent);
  TEST_EQ(Event::TYPE_TEST, MyEvent->GetType());

  // Cleanup
  delete (MyEvent);

  // Return success
  UTEST_RETURN;
}

int
zEventTest_EventAdapterDefaults(void * arg_)
{
  // Create new event and validate
  TestEvent *MyEvent = new TestEvent;
  TEST_ISNOT_NULL(MyEvent);
  TEST_EQ(Event::TYPE_TEST, MyEvent->GetType());

  // Create new event adapter and validate
  TestAdapter *MyAdapter = new TestAdapter(*MyEvent);
  TEST_ISNOT_NULL(MyAdapter);
  TEST_EQ(Event::TYPE_TEST, MyAdapter->GetType());

  // Cleanup
  delete (MyAdapter);
  delete (MyEvent);

  // Return success
  UTEST_RETURN;
}

int
zEventTest_EventNotificationDefaults(void * arg_)
{
  // Create new event and validate
  TestEvent *MyEvent = new TestEvent;
  TEST_ISNOT_NULL(MyEvent);
  TEST_EQ(Event::TYPE_TEST, MyEvent->GetType());

  // Create new event notification and validate
  TestNotification* MyNotification = new TestNotification(*MyEvent, 88);
  TEST_ISNOT_NULL(MyNotification);
  TEST_EQ(Event::TYPE_TEST, MyNotification->GetType());
  TEST_EQ(88, MyNotification->GetValue());

  // Cleanup
  delete (MyNotification);
  delete (MyEvent);

  // Return success
  UTEST_RETURN;
}

int
zEventTest_EventHandlerDefaults(void * arg_)
{
  // Create new event handler and validate
  zEvent::Handler *MyHandler = new zEvent::Handler;
  TEST_ISNOT_NULL(MyHandler);

  // Cleanup
  delete (MyHandler);

  // Return success
  return (0);
}

int
zEventTest_EventManagerDefaults(void * arg_)
{

  // Get instance of event manager
  zEvent::Manager &manager = zEvent::Manager::Instance();

  // Return success
  return (0);
}

