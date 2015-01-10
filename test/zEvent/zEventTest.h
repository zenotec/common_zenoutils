/*
 * zEventTest.h
 *
 *  Created on: Jan 9, 2015
 *      Author: kmahoney
 */

#ifndef _ZEVENTTEST_H_
#define _ZEVENTTEST_H_

#include <zutils/zEvent.h>

#include "UnitTest.h"

int
zEventTest_Defaults(void * arg_);
int
zEventTest_EventList(void* arg_);

using namespace zUtils;
using namespace Test;

class TestEvent : public zEvent::Event
{
public:
  void
  Notify()
  {
    this->_notify();
  }

protected:

private:

};

#endif /* _ZEVENTTEST_H_ */
