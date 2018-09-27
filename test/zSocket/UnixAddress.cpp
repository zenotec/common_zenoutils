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
using namespace zUtils;
ZLOG_MODULE_INIT(zLog::Log::MODULE_TEST);

#include <zutils/zUnixSocket.h>

#include "zSocketTest.h"

using namespace Test;
using namespace zUtils;

int
zSocketTest_UnixAddressGetSet(void* arg_)
{

  ZLOG_DEBUG("#############################################################");
  ZLOG_DEBUG("# zSocketTest_UnixAddressGetSet()");
  ZLOG_DEBUG("#############################################################");

  // Create new socket address and validate
  UnixAddress myAddr;
  TEST_EQ(Address::TYPE_UNIX, myAddr.GetType());
  TEST_EQ(std::string(""), myAddr.GetAddress());

  // Set socket address
  TEST_TRUE(myAddr.SetAddress(std::string("unix")));
  TEST_EQ(std::string("unix"), myAddr.GetAddress());

  // Return success
  return (0);

}

int
zSocketTest_UnixAddressCompare(void* arg_)
{

  ZLOG_DEBUG("#############################################################");
  ZLOG_DEBUG("# zSocketTest_UnixAddressCompare()");
  ZLOG_DEBUG("#############################################################");

  // Create new socket address and validate
  UnixAddress myAddr1;
  TEST_EQ(Address::TYPE_UNIX, myAddr1.GetType());
  TEST_EQ(std::string(""), myAddr1.GetAddress());

  // Create second socket address and validate
  UnixAddress myAddr2;
  TEST_EQ(Address::TYPE_UNIX, myAddr2.GetType());
  TEST_EQ(std::string(""), myAddr2.GetAddress());

  // Compare address (match)
  TEST_TRUE(myAddr1 == myAddr2);
  TEST_FALSE(myAddr1 != myAddr2);

  // Set socket address
  TEST_TRUE(myAddr1.SetAddress("unix"));
  TEST_EQ(std::string("unix"), myAddr1.GetAddress());

  // Compare address (no match)
  TEST_FALSE(myAddr1 == myAddr2);
  TEST_TRUE(myAddr1 != myAddr2);

  // Set socket address
  TEST_TRUE(myAddr2.SetAddress("unix"));
  TEST_EQ(std::string("unix"), myAddr2.GetAddress());

  // Compare address (match)
  TEST_TRUE(myAddr1 == myAddr2);
  TEST_FALSE(myAddr1 != myAddr2);

  // Set socket address
  TEST_FALSE(myAddr1.SetAddress(""));
  TEST_EQ(std::string("unix"), myAddr1.GetAddress());

  // Compare address (match)
  TEST_TRUE(myAddr1 == myAddr2);
  TEST_FALSE(myAddr1 != myAddr2);

  // Return success
  UTEST_RETURN;

}

