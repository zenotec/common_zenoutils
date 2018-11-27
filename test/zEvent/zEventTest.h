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

#include <zutils/zCompatibility.h>
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
zEventTest_EventHandlerTestSingleEventSingleObserver(void* arg_);
int
zEventTest_EventHandlerTestSingleEventMultiObserver(void* arg_);
int
zEventTest_EventHandlerTestMultiEventSingleObserver(void* arg_);
int
zEventTest_EventHandlerTestMultiEventMultiObserver(void* arg_);
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

  bool
  SetValue(const int value_)
  {
    this->_value = value_;
    return (this->_value == value_);
  }

private:

  int _value;

};

class TestEvent :
    public Event
{

public:

  TestEvent() :
      Event(zEvent::TYPE_TEST)
  {
  }
  ~TestEvent()
  {
  }

  zEvent::STATUS
  Notify(int val_)
  {
    SHARED_PTR(zEvent::Notification) n(new TestNotification(*this, val_));
    return (this->notifyHandlers(n));
  }

};

class TestObserver :
    public Observer,
    public zQueue::Queue<int>
{

public:

  TestObserver(bool cont_ = true) :
    _cont(cont_)
  {
  }

  virtual
  ~TestObserver()
  {
  }

  virtual zEvent::STATUS
  ObserveEvent(SHARED_PTR(zEvent::Notification) n_)
  {
    zEvent::STATUS status = zEvent::STATUS_ERR;
    TestNotification* n = (TestNotification*)n_.get();
    if (n && n->SetValue(n->GetValue() + 1) && this->Push(n->GetValue()))
    {
      status = zEvent::STATUS_OK;
      if (this->_cont)
      {
        status = zEvent::STATUS(status | zEvent::STATUS_CONT);
      }
    }
    return (status);
  }

private:

  bool _cont;
};

#endif /* _ZEVENTTEST_H_ */
