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
using namespace zEvent;

class TestNotification :
    public Notification
{

public:

  TestNotification(Event& event_, int value_) :
      Notification(event_), _value(value_)
  {
  }

  ~TestNotification()
  {
  }

  int
  GetValue() const
  {
    return (this->_value);
  }

private:

  int _value;

};

class TestEvent :
    public Event
{

public:

  TestEvent() :
      Event(zEvent::Event::TYPE_TEST)
  {
  }
  ~TestEvent()
  {
  }

  bool
  Notify(int val_)
  {
    SHARED_PTR(TestNotification) n(new TestNotification(*this, val_));
    this->notifyHandlers(n);
    return (true);
  }

};

class TestObserver :
    public Observer,
    public zQueue::Queue<SHARED_PTR(zEvent::Notification)>
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
  ObserveEvent(SHARED_PTR(zEvent::Notification) noti_)
  {
    return (this->Push(noti_));
  }
};

#endif /* _ZEVENTTEST_H_ */
