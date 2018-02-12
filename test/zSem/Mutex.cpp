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
zSemTest_Mutex(void* arg_)
{

  // Create new mutex with defaults and validate
  zSem::Mutex MyMutex;
  TEST_EQ(zSem::Mutex::LOCKED, MyMutex.State());
  TEST_FALSE(MyMutex.TryLock());

  // Unlock and relock
  TEST_TRUE(MyMutex.Unlock());
  TEST_EQ(zSem::Mutex::UNLOCKED, MyMutex.State());
  TEST_TRUE(MyMutex.Lock());
  TEST_EQ(zSem::Mutex::LOCKED, MyMutex.State());

  // Double lock (deadlock)
  TEST_TRUE(MyMutex.Lock());
  TEST_EQ(zSem::Mutex::LOCKED, MyMutex.State());
  TEST_TRUE(MyMutex.Unlock());
  TEST_EQ(zSem::Mutex::LOCKED, MyMutex.State());
  TEST_TRUE(MyMutex.Unlock());
  TEST_EQ(zSem::Mutex::UNLOCKED, MyMutex.State());
  TEST_FALSE(MyMutex.Unlock());
  TEST_EQ(zSem::Mutex::UNLOCKED, MyMutex.State());

  // Try lock
  TEST_TRUE(MyMutex.TryLock());
  TEST_EQ(zSem::Mutex::LOCKED, MyMutex.State());
  TEST_FALSE(MyMutex.TryLock());
  TEST_EQ(zSem::Mutex::LOCKED, MyMutex.State());
  TEST_TRUE(MyMutex.Unlock());
  TEST_EQ(zSem::Mutex::UNLOCKED, MyMutex.State());
  TEST_FALSE(MyMutex.Unlock());
  TEST_EQ(zSem::Mutex::UNLOCKED, MyMutex.State());

  // Timed lock
  TEST_TRUE(MyMutex.TimedLock(50));
  TEST_EQ(zSem::Mutex::LOCKED, MyMutex.State());
  TEST_FALSE(MyMutex.TimedLock(50));
  TEST_EQ(zSem::Mutex::LOCKED, MyMutex.State());
  TEST_TRUE(MyMutex.Unlock());
  TEST_EQ(zSem::Mutex::UNLOCKED, MyMutex.State());
  TEST_FALSE(MyMutex.Unlock());
  TEST_EQ(zSem::Mutex::UNLOCKED, MyMutex.State());

  // Return success
  return (0);
}

