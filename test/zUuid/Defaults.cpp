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

#include "zUuidTest.h"

using namespace Test;
using namespace zUtils;

ZLOG_MODULE_INIT(zLog::Log::MODULE_TEST);

int
zUuidTest_Defaults(void* arg_)
{

  zUuid::Uuid uuid1;
  zUuid::Uuid uuid2;

  TEST_FALSE(uuid1 == uuid2);
  TEST_TRUE(uuid1 != uuid2);

  // Return success
  UTEST_RETURN;
}

int
zUuidTest_Copy(void* arg_)
{

  zUuid::Uuid uuid1;
  zUuid::Uuid uuid2;

  TEST_FALSE(uuid1 == uuid2);
  TEST_TRUE(uuid1 != uuid2);

  uuid2 = uuid1;
  TEST_TRUE(uuid1 == uuid2);
  TEST_FALSE(uuid1 != uuid2);
  TEST_FALSE(uuid1 < uuid2);
  TEST_FALSE(uuid1 > uuid2);
  TEST_TRUE(uuid1 <= uuid2);
  TEST_TRUE(uuid1 >= uuid2);

  // Return success
  UTEST_RETURN;
}

int
zUuidTest_Compare(void* arg_)
{

  uint8_t val8[16] = { 0x0, 0x1, 0x2, 0x3, 0x4, 0x5, 0x6, 0x7, 0x8, 0x9, 0xa, 0xb, 0xc, 0xd, 0xe, 0xf };
  uint32_t val32[4] = { 0x0, 0x4, 0x8, 0xc };
  uint64_t val64[2] = { 0x0, 0x8 };
  zUuid::Uuid uuid1(val8);
  zUuid::Uuid uuid2(uuid1);

  // Compare equal to
  TEST_EQ(std::string("00010203-0405-0607-0809-0a0b0c0d0e0f"), uuid1.Str());
  TEST_EQ(std::string("00010203-0405-0607-0809-0a0b0c0d0e0f"), uuid2.Str());
  TEST_TRUE(uuid1 == uuid2);
  TEST_FALSE(uuid1 != uuid2);
  TEST_FALSE(uuid1 < uuid2);
  TEST_FALSE(uuid1 > uuid2);
  TEST_TRUE(uuid1 <= uuid2);
  TEST_TRUE(uuid1 >= uuid2);

  // Compare greater than
  uuid2 = val32;
  TEST_EQ(std::string("00010203-0405-0607-0809-0a0b0c0d0e0f"), uuid1.Str());
  TEST_EQ(std::string("00000000-0400-0000-0800-00000c000000"), uuid2.Str());
  TEST_FALSE(uuid1 == uuid2);
  TEST_TRUE(uuid1 != uuid2);
  TEST_FALSE(uuid1 < uuid2);
  TEST_TRUE(uuid1 > uuid2);
  TEST_FALSE(uuid1 <= uuid2);
  TEST_TRUE(uuid1 >= uuid2);

  // Compare less than
  uuid1 = val64;
  TEST_EQ(std::string("00000000-0000-0000-0800-000000000000"), uuid1.Str());
  TEST_EQ(std::string("00000000-0400-0000-0800-00000c000000"), uuid2.Str());
  TEST_FALSE(uuid1 == uuid2);
  TEST_TRUE(uuid1 != uuid2);
  TEST_TRUE(uuid1 < uuid2);
  TEST_FALSE(uuid1 > uuid2);
  TEST_TRUE(uuid1 <= uuid2);
  TEST_FALSE(uuid1 >= uuid2);

  // Return success
  UTEST_RETURN;
}
