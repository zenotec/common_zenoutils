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
#include <errno.h>
#include <sys/time.h>
#include <sys/types.h>
#include <sys/poll.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <ifaddrs.h>

#include <sstream>

#include <string>
#include <list>
#include <mutex>
#include <memory>

#include <zutils/zLog.h>
using namespace zUtils;
ZLOG_MODULE_INIT(zLog::Log::MODULE_TEST);

#include <zutils/zEthSocket.h>

#include "zSocketTest.h"

using namespace Test;
using namespace zUtils;

int
zSocketTest_EthAddressGetSet(void* arg_)
{

  ZLOG_DEBUG("#############################################################");
  ZLOG_DEBUG("# zSocketTest_EthAddressGetSet()");
  ZLOG_DEBUG("#############################################################");

  // Create new socket address and validate
  EthAddress myAddr;
  TEST_EQ(SocketType::TYPE_ETH, myAddr.Type());
  TEST_EQ(std::string("00:00:00:00:00:00"), myAddr.Address());

  // Set socket address using string notation (bad address)
  TEST_FALSE(myAddr.Address(std::string("1:a")));
  TEST_EQ(std::string("00:00:00:00:00:00"), myAddr.Address());

  // Set socket address using string notation (good address)
  TEST_TRUE(myAddr.Address(std::string("00:11:22:33:44:55")));
  TEST_EQ(std::string("00:11:22:33:44:55"), myAddr.Address());

  // Return success
  return (0);

}

int
zSocketTest_EthAddressCompare(void* arg_)
{

  ZLOG_DEBUG("#############################################################");
  ZLOG_DEBUG("# zSocketTest_EthAddressCompare()");
  ZLOG_DEBUG("#############################################################");

  // Create new socket address and validate
  EthAddress myAddr1;
  TEST_EQ(SocketType::TYPE_ETH, myAddr1.Type());
  TEST_EQ(std::string("00:00:00:00:00:00"), myAddr1.Address());

  // Create second socket address and validate
  EthAddress myAddr2;
  TEST_EQ(SocketType::TYPE_ETH, myAddr2.Type());
  TEST_EQ(std::string("00:00:00:00:00:00"), myAddr2.Address());

  // Compare address (match)
  TEST_TRUE(myAddr1 == myAddr2);
  TEST_FALSE(myAddr1 != myAddr2);

  // Set socket address using string notation
  myAddr1.Address("00:11:22:33:44:55");
  TEST_EQ(std::string("00:11:22:33:44:55"), myAddr1.Address());

  // Compare address (no match)
  TEST_FALSE(myAddr1 == myAddr2);
  TEST_TRUE(myAddr1 != myAddr2);

  // Set socket address using string notation
  myAddr2.Address("00:11:22:33:44:55");
  TEST_EQ(std::string("00:11:22:33:44:55"), myAddr2.Address());

  // Compare address (match)
  TEST_TRUE(myAddr1 == myAddr2);
  TEST_FALSE(myAddr1 != myAddr2);

  // Set socket address using string notation
  myAddr1.Address("AA:11:22:33:44:55");
  TEST_EQ(std::string("AA:11:22:33:44:55"), myAddr1.Address());

  // Compare address (no match)
  TEST_FALSE(myAddr1 == myAddr2);
  TEST_TRUE(myAddr1 != myAddr2);

  // Return success
  return (0);

}

