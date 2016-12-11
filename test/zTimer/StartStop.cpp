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

#include <unistd.h>
#include <signal.h>

#include <mutex>

#include "zTimerTest.h"

using namespace Test;
using namespace zUtils;

int
zTimerTest_StartStop(void* arg_)
{
  ZLOG_DEBUG("#############################################################");
  ZLOG_DEBUG("# zTimerTest_StartStop()");
  ZLOG_DEBUG("#############################################################");

  uint32_t interval = 50000;
  int n = 10;

  // Create new timer and validate
  zTimer::Timer *MyTimer = new zTimer::Timer;
  TEST_ISNOT_NULL(MyTimer);

  // Create new timer observer and register with timer
  TimerTestObserver *MyObserver = new TimerTestObserver;
  TEST_ISNOT_NULL(MyObserver);
  TEST_IS_ZERO(MyObserver->GetCnt());
  MyTimer->RegisterObserver(MyObserver);

  // Start the timer
  MyTimer->Start(interval);

  // Delay for n intervals
  for (int i = 0; i < n; i++)
  {
    usleep(interval);
  }

  // Stop the timer
  MyTimer->Stop();

  usleep((interval));

  // Validate
  TEST_EQ(n, MyObserver->GetCnt());

  // Cleanup
  MyTimer->UnregisterObserver(MyObserver);
  delete (MyObserver);
  delete (MyTimer);

  // Return success
  return (0);
}

