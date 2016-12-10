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

#ifndef _ZEVENTTEST_H_
#define _ZEVENTTEST_H_

#include <stdint.h>

#include <mutex>
#include <list>
#include <queue>
#include <vector>

#include <zutils/zQueue.h>
#include <zutils/zEvent.h>

#include "UnitTest.h"

int
zEventTest_EventDefaults(void * arg_);
int
zEventTest_EventNotificationDefaults(void * arg_);
int
zEventTest_EventHandlerDefaults(void * arg_);
int
zEventTest_EventManagerDefaults(void * arg_);

int
zEventTest_EventTest(void* arg_);
int
zEventTest_EventHandlerTest(void* arg_);
int
zEventTest_EventManagerTest(void* arg_);

using namespace zUtils;
using namespace Test;

class TestEventNotification : public zEvent::EventNotification
{
public:
  TestEventNotification(zEvent::Event* event_) :
      zEvent::EventNotification(event_)
  {

  }
  ~TestEventNotification()
  {

  }
};

class TestEvent : public zEvent::Event
{
public:
  TestEvent() :
      zEvent::Event(zEvent::Event::TYPE_TEST)
  {

  }
  ~TestEvent()
  {

  }
};

class TestObserver : public zEvent::EventObserver, public zQueue<const zEvent::EventNotification*>
{
public:
  TestObserver()
  {
  }

  virtual
  ~TestObserver()
  {
  }

  virtual bool
  EventHandler(const zEvent::EventNotification* notification_)
  {
    ZLOG_DEBUG("Handling event");
    this->Push(notification_);
    return (true);
  }
};

#endif /* _ZEVENTTEST_H_ */
