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

#include "zSemTest.h"

using namespace Test;
using namespace zUtils;

int
zSemTest_Semaphore(void* arg_)
{

  // Create new semaphore with defaults and validate
  zSem::Semaphore MySemaphore; //
  TEST_ISNOT_ZERO(MySemaphore.GetFd());
  TEST_FALSE(MySemaphore.TryWait()); //
  
  // Post and Wait
  TEST_TRUE(MySemaphore.Post(2)); //
  TEST_TRUE(MySemaphore.Wait());
  TEST_TRUE(MySemaphore.Wait());
  TEST_FALSE(MySemaphore.TryWait()); //

  // Timed Wait
  TEST_FALSE(MySemaphore.TimedWait(10)); //
  TEST_TRUE(MySemaphore.Post()); //
  TEST_TRUE(MySemaphore.TimedWait(10)); //

  // Return success
  return (0);
}
