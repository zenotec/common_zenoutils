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

#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <netinet/in.h>
#include <string.h>

#include <string>
#include <list>
#include <mutex>
#include <memory>

#include <zutils/zLog.h>
using namespace zUtils;
ZLOG_MODULE_INIT(zLog::Log::MODULE_TEST);

#include <zutils/zSocket.h>
#include <zutils/zLoopSocket.h>

#include "zSocketTest.h"

using namespace Test;
using namespace zUtils;

int
zSocketTest_BufferString(void* arg_)
{

  std::string expStr = "Test Buffer String";

  // Create new packet and validate
  zSocket::Buffer myBuffer(expStr);
  TEST_ISNOT_NULL(myBuffer.Head());
  TEST_EQ(myBuffer.Head(), myBuffer.Data());
  TEST_NEQ(myBuffer.Head(), myBuffer.Tail());
  TEST_ISNOT_NULL(myBuffer.Data());
  TEST_EQ(int(expStr.size()), myBuffer.Length());
  TEST_EQ(int(expStr.size()), myBuffer.Size());
  TEST_EQ(int(expStr.size()), myBuffer.TotalSize());
  TEST_EQ(expStr, myBuffer.Str());

  // Return success
  UTEST_RETURN;

}

int
zSocketTest_BufferCompare(void* arg_)
{

  // Create new buffer and validate
  zSocket::Buffer mySb1;
  TEST_ISNOT_NULL(mySb1.Head());
  TEST_EQ(mySb1.Head(), mySb1.Data());
  TEST_EQ(mySb1.Head(), mySb1.Tail());
  TEST_ISNOT_NULL(mySb1.Data());
  TEST_IS_ZERO(mySb1.Length());
  TEST_IS_ZERO(mySb1.Size());
  TEST_EQ(mySb1.TotalSize(), 1500);
  TEST_EQ(std::string(""), mySb1.Str());

  // Create new buffer of set size and validate
  zSocket::Buffer mySb2;
  TEST_ISNOT_NULL(mySb2.Head());
  TEST_EQ(mySb2.Head(), mySb2.Data());
  TEST_EQ(mySb2.Head(), mySb2.Tail());
  TEST_ISNOT_NULL(mySb2.Data());
  TEST_IS_ZERO(mySb2.Length());
  TEST_IS_ZERO(mySb2.Size());
  TEST_EQ(mySb2.TotalSize(), 1500);
  TEST_EQ(std::string(""), mySb2.Str());

  // Validate
  TEST_TRUE((mySb1 == mySb2));
  TEST_EQ(mySb1.Str(), mySb2.Str());

  // Write to first buffer and validate
  TEST_TRUE(mySb1.Data() == memset(mySb1.Data(), 0xed, 100));
  TEST_TRUE(mySb1.Put(100));
  TEST_TRUE((mySb1 != mySb2));
  TEST_NEQ(mySb1.Str(), mySb2.Str());

  // Write to second buffer and validate
  TEST_TRUE(mySb2.Data() == memset(mySb2.Data(), 0xde, 100));
  TEST_TRUE(mySb2.Put(100));
  TEST_TRUE((mySb1 != mySb2));
  TEST_NEQ(mySb1.Str(), mySb2.Str());

  // Write to second buffer and validate
  TEST_TRUE(mySb2.Data() == memset(mySb2.Data(), 0xed, 100));
  TEST_TRUE((mySb1 == mySb2));
  TEST_EQ(mySb1.Str(), mySb2.Str());

  // Return success
  UTEST_RETURN;

}

int
zSocketTest_BufferCopy(void* arg_)
{

  // Create new buffer and validate
  zSocket::Buffer mySb1;
  TEST_ISNOT_NULL(mySb1.Head());
  TEST_TRUE(mySb1.Head() == mySb1.Data());
  TEST_TRUE(mySb1.Head() == mySb1.Tail());
  TEST_ISNOT_NULL(mySb1.Data());
  TEST_IS_ZERO(mySb1.Length());
  TEST_IS_ZERO(mySb1.Size());
  TEST_EQ(1500, mySb1.TotalSize());
  TEST_EQ(std::string(""), mySb1.Str());

  // Write to first buffer and validate
  TEST_TRUE(mySb1.Data() == memset(mySb1.Data(), 0xed, 100));
  TEST_TRUE(mySb1.Put(100));
  TEST_EQ(mySb1.Head(), mySb1.Data());
  TEST_NEQ(mySb1.Head(), mySb1.Tail());

  // Create new buffer from first buffer and validate
  zSocket::Buffer mySb2(mySb1);
  TEST_TRUE(mySb1.Head() == mySb2.Head());
  TEST_TRUE(mySb2.Head() == mySb2.Data());
  TEST_TRUE(mySb2.Head() == mySb2.Tail());
  TEST_ISNOT_NULL(mySb2.Data());
  TEST_IS_ZERO(mySb2.Length());
  TEST_IS_ZERO(mySb2.Size());
  TEST_EQ(mySb2.TotalSize(), 1500);
  TEST_EQ(std::string(""), mySb2.Str());

  // Return success
  UTEST_RETURN;
}
