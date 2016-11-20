/*
 * zEventTest.h
 *
 *  Created on: Jan 9, 2015
 *      Author: kmahoney
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
