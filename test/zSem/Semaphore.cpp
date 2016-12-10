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

int
zSemTest_Semaphore(void* arg_)
{

  ZLOG_DEBUG("#############################################################");
  ZLOG_DEBUG("# zSemTest_Semaphore()");
  ZLOG_DEBUG("#############################################################");

  // Create new semaphore with defaults and validate
  zSem::Semaphore MySemaphore;
  TEST_FALSE(MySemaphore.TryWait());

  // Post and reWait
  TEST_TRUE(MySemaphore.Post());
  TEST_TRUE(MySemaphore.TryWait());

  // Timed Wait
  TEST_FALSE(MySemaphore.TimedWait(100));
  TEST_TRUE(MySemaphore.Post());
  TEST_TRUE(MySemaphore.TimedWait(100));

  // Return success
  return (0);
}
