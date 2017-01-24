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

#include <stdint.h>
#include <netinet/in.h>
#include <string.h>

#include <string>
#include <list>
#include <mutex>
#include <memory>

#include <zutils/zLog.h>
#include <zutils/zSem.h>
#include <zutils/zThread.h>
#include <zutils/zQueue.h>
#include <zutils/zEvent.h>

#include <zutils/zSocket.h>
#include <zutils/zLoopSocket.h>

#include "zSocketTest.h"

using namespace Test;
using namespace zUtils;
using namespace zSocket;

int
zSocketTest_AddressGetSet(void* arg_)
{

  ZLOG_DEBUG("#############################################################");
  ZLOG_DEBUG("# zSocketTest_AddressGetSet()");
  ZLOG_DEBUG("#############################################################");

  // Create new socket address and validate
  TestAddress myAddr;
  TEST_EQ(SocketType::TYPE_TEST, myAddr.Type());
  TEST_EQ(std::string(""), myAddr.Address());
  TEST_TRUE(myAddr.Address(std::string("some_address")));
  TEST_EQ(std::string("some_address"), myAddr.Address());

  // Return success
  return (0);
}

int
zSocketTest_AddressCompare(void* arg_)
{

  ZLOG_DEBUG("#############################################################");
  ZLOG_DEBUG("# zSocketTest_AddressCompare()");
  ZLOG_DEBUG("#############################################################");

  // Create new socket address and validate
  TestAddress myAddr1;
  TEST_EQ(SocketType::TYPE_TEST, myAddr1.Type());
  TEST_EQ(std::string(""), myAddr1.Address());

  // Create another new socket address and validate
  TestAddress myAddr2;
  TEST_EQ(SocketType::TYPE_TEST, myAddr2.Type());
  TEST_EQ(std::string(""), myAddr2.Address());

  // Compare the addresses
  TEST_TRUE(myAddr1 == myAddr2);
  TEST_FALSE(myAddr1 != myAddr2);

  // Set first address to new value and validate
  std::string expAddr = "some_address";
  TEST_TRUE(myAddr1.Address(expAddr));
  TEST_EQ(expAddr, myAddr1.Address());

  // Compare the addresses
  TEST_TRUE(myAddr1 != myAddr2);
  TEST_FALSE(myAddr1 == myAddr2);

  // Set first address to new value and validate
  TEST_TRUE(myAddr2.Address(expAddr));
  TEST_EQ(expAddr, myAddr2.Address());

  // Compare the addresses
  TEST_TRUE(myAddr1 == myAddr2);
  TEST_FALSE(myAddr1 != myAddr2);

  // Return success
  return (0);
}

