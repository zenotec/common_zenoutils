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

#include <zutils/zLog.h>
#include "zSemTest.h"

using namespace Test;
using namespace zUtils;

ZLOG_MODULE_INIT(zLog::Log::MODULE_TEST);

int
zSemTest_Mutex(void* arg_)
{

  ZLOG_DEBUG("#############################################################");
  ZLOG_DEBUG("# zSemTest_Mutex()");
  ZLOG_DEBUG("#############################################################");

  // Create new mutex with defaults and validate
  zSem::Mutex MyMutex;
  TEST_FALSE(MyMutex.TryLock());

  // Unlock and relock
  TEST_TRUE(MyMutex.Unlock());
  TEST_TRUE(MyMutex.TryLock());

  // Timed lock
  TEST_FALSE(MyMutex.TimedLock(100));
  TEST_TRUE(MyMutex.Unlock());
  TEST_TRUE(MyMutex.TimedLock(100));

  // Return success
  return (0);
}

