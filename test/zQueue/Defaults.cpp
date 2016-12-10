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

#include "zQueueTest.h"

using namespace Test;
using namespace zUtils;

int
zQueueTest_Defaults(void* arg_)
{

  std::string exp_str = "TestString";
  std::string obs_str;

  // Create new queue and validate
  zQueue<std::string> MyQueue;
  TEST_TRUE(MyQueue.Empty());
  TEST_FALSE(MyQueue.TryWait());
  TEST_IS_ZERO(MyQueue.Size());

  // Push string on to back of queue and validate
  MyQueue.Push(exp_str + "_1");
  TEST_TRUE(MyQueue.TryWait());
  TEST_FALSE(MyQueue.Empty());
  TEST_EQ(1, MyQueue.Size());
  obs_str = MyQueue.Front();
  TEST_FALSE(MyQueue.Empty());
  TEST_EQ(1, MyQueue.Size());
  TEST_EQ(exp_str + "_1", obs_str);
  obs_str = MyQueue.Back();
  TEST_FALSE(MyQueue.Empty());
  TEST_EQ(1, MyQueue.Size());
  TEST_EQ(exp_str + "_1", obs_str);

  // Push second string on to back of queue and validate
  MyQueue.Push(exp_str + "_2");
  TEST_TRUE(MyQueue.TryWait());
  TEST_FALSE(MyQueue.Empty());
  TEST_EQ(2, MyQueue.Size());
  obs_str = MyQueue.Front();
  TEST_FALSE(MyQueue.Empty());
  TEST_EQ(2, MyQueue.Size());
  TEST_EQ(exp_str + "_1", obs_str);
  obs_str = MyQueue.Back();
  TEST_FALSE(MyQueue.Empty());
  TEST_EQ(2, MyQueue.Size());
  TEST_EQ(exp_str + "_2", obs_str);

  // Pop first string off the front of the queue
  MyQueue.Pop();
  TEST_FALSE(MyQueue.Empty());
  TEST_EQ(1, MyQueue.Size());

  // Pop second string off the front of the queue
  MyQueue.Pop();
  TEST_TRUE(MyQueue.Empty());
  TEST_IS_ZERO(MyQueue.Size());

  TEST_FALSE(MyQueue.TryWait());

  // Return success
  return (0);
}
