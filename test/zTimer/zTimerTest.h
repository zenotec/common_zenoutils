/*
 * zTimerTest.h
 *
 *  Created on: Jan 9, 2015
 *      Author: kmahoney
 */

#ifndef _ZTIMERTEST_H_
#define _ZTIMERTEST_H_

#include <zutils/zTimer.h>

#include "UnitTest.h"

int
zTimerTest_Defaults(void* arg_);
int
zTimerTest_StartStop(void* arg_);

using namespace zUtils;
using namespace Test;

class TimerTestObserver : public zEvent::EventObserver
{
public:
  TimerTestObserver() :
      _cnt(0)
  {
  }

  virtual
  ~TimerTestObserver()
  {
  }

  virtual bool
  EventHandler(const zEvent::EventNotification* notification_)
  {
    std::cout << "Test timer event handler" << std::endl;
    this->_cnt++;
  }

  int GetCnt()
  {
    return(this->_cnt);
  }

protected:
private:
  int _cnt;
};

#endif /* _ZTIMERTEST_H_ */
