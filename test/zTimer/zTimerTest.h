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

class TimerTestObserver : public zEvent::Observer
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
  Notify(SHARED_PTR(zEvent::Notification) noti_)
  {
    this->_cnt++;
  }

  int
  GetCnt()
  {
    return (this->_cnt);
  }

protected:
private:
  int _cnt;
};

#endif /* _ZTIMERTEST_H_ */
