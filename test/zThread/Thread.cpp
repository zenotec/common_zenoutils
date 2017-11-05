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

#include <list>
#include <mutex>

#include <zutils/zLog.h>
#include <zutils/zSem.h>

#include "zThreadTest.h"

using namespace Test;
using namespace zUtils;

int
zThreadTest_RunOnce(void* arg_)
{

  ZLOG_DEBUG("#############################################################");
  ZLOG_DEBUG("# zThreadTest_RunOnce()");
  ZLOG_DEBUG("#############################################################");

  int loops = 1;

  // Create test thread and validate
  TestThread *myThread = new TestThread("RunOnce", loops);
  TEST_ISNOT_NULL(myThread);
  TEST_EQ(std::string("RunOnce"), myThread->Name());
  TEST_EQ(loops, myThread->TestArg.LoopCnt);
  TEST_IS_ZERO(myThread->TestArg.ErrorCnt);

  // Run thread function until it exits
  TEST_TRUE(myThread->Start());

  TEST_TRUE(myThread->TestArg.TimedWait(100));
  TEST_FALSE(myThread->TestArg.TimedWait(100));

  // Verify
  TEST_IS_ZERO(myThread->TestArg.LoopCnt);
  TEST_IS_ZERO(myThread->TestArg.ErrorCnt);

  // Cleanup
  TEST_TRUE(myThread->Stop());
  delete (myThread);

  // Return success
  return (0);
}

int
zThreadTest_RunMultiple(void* arg_)
{

  ZLOG_DEBUG("#############################################################");
  ZLOG_DEBUG("# zThreadTest_RunMultiple()");
  ZLOG_DEBUG("#############################################################");

  int loops = 10;

  // Create test thread and validate
  TestThread *myThread = new TestThread("RunMultiple", loops);
  TEST_ISNOT_NULL(myThread);
  TEST_EQ(std::string("RunMultiple"), myThread->Name());
  TEST_EQ(loops, myThread->TestArg.LoopCnt);
  TEST_IS_ZERO(myThread->TestArg.ErrorCnt);

  // Run thread function until it exits
  TEST_TRUE(myThread->Start());
  for (; loops; --loops)
  {
    TEST_TRUE(myThread->TestArg.TimedWait(100));
  }
  TEST_FALSE(myThread->TestArg.TimedWait(100));

  // Verify
  TEST_IS_ZERO(myThread->TestArg.LoopCnt);
  TEST_IS_ZERO(myThread->TestArg.ErrorCnt);

  // Cleanup
  TEST_TRUE(myThread->Stop());
  delete (myThread);

  // Return success
  return (0);
}

int
zThreadTest_Synchronize(void* arg_)
{

  ZLOG_DEBUG("#############################################################");
  ZLOG_DEBUG("# zThreadTest_Synchronize()");
  ZLOG_DEBUG("#############################################################");

  int loops = 10;

  // Create test thread and validate
  TestThread *myThread1 = new TestThread("Synchronize1", loops);
  TEST_ISNOT_NULL(myThread1);
  TEST_EQ(std::string("Synchronize1"), myThread1->Name());
  TEST_EQ(loops, myThread1->TestArg.LoopCnt);
  TEST_IS_ZERO(myThread1->TestArg.ErrorCnt);
  TEST_FALSE(myThread1->Join());

  // Create second test thread and validate
  TestThread *myThread2 = new TestThread("Synchronize2", loops);
  TEST_ISNOT_NULL(myThread2);
  TEST_EQ(std::string("Synchronize2"), myThread2->Name());
  TEST_EQ(loops, myThread2->TestArg.LoopCnt);
  TEST_IS_ZERO(myThread2->TestArg.ErrorCnt);
  TEST_FALSE(myThread2->Join());

  // Share sync arg
  TEST_TRUE(myThread1->TestFunc.SetSyncArg(&myThread2->TestArg));
  TEST_TRUE(&myThread2->TestArg == myThread1->TestFunc.GetSyncArg());
  TEST_TRUE(myThread2->TestFunc.SetSyncArg(&myThread1->TestArg));
  TEST_TRUE(&myThread1->TestArg == myThread2->TestFunc.GetSyncArg());

  // Run first thread function, force it to stop and verify
  TEST_TRUE(myThread1->Start());
  usleep(10000);
  TEST_TRUE(myThread1->Stop());
  TEST_FALSE(myThread1->Join());
  TEST_EQ((loops - 1), myThread1->TestArg.LoopCnt);
  TEST_EQ(1, myThread1->TestArg.ErrorCnt);

  // Reset counters and verify
  myThread1->TestArg.LoopCnt = loops;
  myThread1->TestArg.ErrorCnt = 0;
  TEST_EQ(loops, myThread1->TestArg.LoopCnt);
  TEST_IS_ZERO(myThread1->TestArg.ErrorCnt);
  TEST_FALSE(myThread1->Join());

  // Run both threads, allow them to sync
  TEST_TRUE(myThread1->Start());
  TEST_TRUE(myThread2->Start());
  usleep(10000);
  TEST_TRUE(myThread1->Join());
  TEST_TRUE(myThread2->Join());

  // Verify both threads ran to completion
  TEST_IS_ZERO(myThread1->TestArg.LoopCnt);
  TEST_IS_ZERO(myThread1->TestArg.ErrorCnt);
  TEST_IS_ZERO(myThread2->TestArg.LoopCnt);
  TEST_IS_ZERO(myThread2->TestArg.ErrorCnt);

  // Cleanup
  TEST_FALSE(myThread1->Stop());
  delete (myThread1);
  TEST_FALSE(myThread2->Stop());
  delete (myThread2);

  // Return success
  return (0);
}
