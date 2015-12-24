/*
 * zEventTest.h
 *
 *  Created on: Jan 9, 2015
 *      Author: kmahoney
 */

#ifndef _ZEVENTTEST_H_
#define _ZEVENTTEST_H_

#include <zutils/zQueue.h>
#include <zutils/zEvent.h>

#include "UnitTest.h"

int
zEventTest_Defaults(void * arg_);
int
zEventTest_EventTest(void* arg_);
int
zEventTest_EventHandlerTest(void* arg_);
int
zEventTest_EventManagerTest(void* arg_);

using namespace zUtils;
using namespace Test;

class TestObserver : public zEvent::EventObserver, public zQueue<zEvent::Event *>
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
  EventHandler(zEvent::Event *event_, void *arg_)
  {
    ZLOG_DEBUG("Handling event");
    this->Push(event_);
    return (true);
  }
};

#endif /* _ZEVENTTEST_H_ */
