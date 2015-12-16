/*
 * zEventTest.h
 *
 *  Created on: Jan 9, 2015
 *      Author: kmahoney
 */

#ifndef _ZEVENTTEST_H_
#define _ZEVENTTEST_H_

#include <zutils/zEvent.h>
#include <zutils/zSem.h>

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

class TestObserver : public zEvent::EventObserver, public zSem::Semaphore
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
    return (this->Post());
  }
};

#endif /* _ZEVENTTEST_H_ */
