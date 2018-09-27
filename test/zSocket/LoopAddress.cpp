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

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <unistd.h>

#include <sstream>
#include <string>
#include <list>
#include <mutex>
#include <memory>

#include <zutils/zLog.h>
using namespace zUtils;
ZLOG_MODULE_INIT(zLog::Log::MODULE_TEST);

#include <zutils/zLoopSocket.h>

#include "zSocketTest.h"

using namespace Test;
using namespace zUtils;

int
zSocketTest_LoopAddressGetSet(void* arg_)
{

  ZLOG_DEBUG("#############################################################");
  ZLOG_DEBUG("# zSocketTest_LoopAddressGetSet()");
  ZLOG_DEBUG("#############################################################");

  // Create new socket address and validate
  LoopAddress myAddr;
  TEST_EQ(Address::TYPE_LOOP, myAddr.GetType());
  TEST_EQ(std::string(""), myAddr.GetAddress());

  // Set socket address
  TEST_TRUE(myAddr.SetAddress(std::string("loop")));
  TEST_EQ(std::string("loop"), myAddr.GetAddress());

  // Return success
  return (0);

}

int
zSocketTest_LoopAddressCompare(void* arg_)
{

  ZLOG_DEBUG("#############################################################");
  ZLOG_DEBUG("# zSocketTest_LoopAddressCompare()");
  ZLOG_DEBUG("#############################################################");

  // Create new socket address and validate
  LoopAddress myAddr1;
  TEST_EQ(Address::TYPE_LOOP, myAddr1.GetType());
  TEST_EQ(std::string(""), myAddr1.GetAddress());

  // Create second socket address and validate
  LoopAddress myAddr2;
  TEST_EQ(Address::TYPE_LOOP, myAddr2.GetType());
  TEST_EQ(std::string(""), myAddr2.GetAddress());

  // Compare address (match)
  TEST_TRUE(myAddr1 == myAddr2);
  TEST_FALSE(myAddr1 != myAddr2);

  // Set socket address
  TEST_TRUE(myAddr1.SetAddress("loop"));
  TEST_EQ(std::string("loop"), myAddr1.GetAddress());

  // Compare address (no match)
  TEST_FALSE(myAddr1 == myAddr2);
  TEST_TRUE(myAddr1 != myAddr2);

  // Set socket address
  TEST_TRUE(myAddr2.SetAddress("loop"));
  TEST_EQ(std::string("loop"), myAddr2.GetAddress());

  // Compare address (match)
  TEST_TRUE(myAddr1 == myAddr2);
  TEST_FALSE(myAddr1 != myAddr2);

  // Return success
  return (0);

}

