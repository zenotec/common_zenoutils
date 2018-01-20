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
zSemTest_MutexDefaults(void* arg_)
{

  // Create new mutex with defaults and validate
  zSem::Mutex MyMutex1;
  TEST_FALSE(MyMutex1.TryLock());

  // Create new mutex with defaults and validate
  zSem::Mutex MyMutex2(zSem::Mutex::LOCKED);
  TEST_FALSE(MyMutex2.TryLock());

  // Create new mutex with defaults and validate
  zSem::Mutex MyMutex3(zSem::Mutex::UNLOCKED);
  TEST_TRUE(MyMutex3.TryLock());

  // Return success
  return (0);
}

int
zSemTest_SemaphoreDefaults(void* arg_)
{

  // Create new semaphore with defaults and validate
  zSem::Semaphore MySem1;
  TEST_FALSE(MySem1.TryWait());

  // Create new semaphore with defaults and validate
  zSem::Semaphore MySem2(0);
  TEST_FALSE(MySem2.TryWait());

  // Create new semaphore with defaults and validate
  zSem::Semaphore MySem3(1);
  TEST_TRUE(MySem3.TryWait());

  // Return success
  return (0);
}

