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
  TEST_EQ(zEvent::Event::TYPE_TEST, MyEvent->Type());

  // Cleanup
  delete (MyEvent);

  // Return success
  return (0);
}

int
zEventTest_EventNotificationDefaults(void * arg_)
{
  // Create new event and validate
  TestEvent *MyEvent = new TestEvent;
  TEST_ISNOT_NULL(MyEvent);
  TEST_EQ(zEvent::Event::TYPE_TEST, MyEvent->Type());

  // Create new event notification and validate
  TestEventNotification *MyNotification = new TestEventNotification(MyEvent);
  TEST_ISNOT_NULL(MyNotification);
  TEST_EQ(zEvent::Event::TYPE_TEST, MyNotification->Type());

  // Cleanup
  delete (MyNotification);
  delete (MyEvent);

  // Return success
  return (0);
}

int
zEventTest_EventHandlerDefaults(void * arg_)
{
  // Create new event handler and validate
  zEvent::EventHandler *MyHandler = new zEvent::EventHandler;
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
  zEvent::EventManager &manager = zEvent::EventManager::Instance();

  // Return success
  return (0);
}

