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

#include <zutils/zInetSocket.h>

#include "zSocketTest.h"

using namespace Test;
using namespace zUtils;

int
zSocketTest_InetAddressGetSet(void* arg_)
{

  ZLOG_DEBUG("#############################################################");
  ZLOG_DEBUG("# zSocketTest_InetAddressGetSet()");
  ZLOG_DEBUG("#############################################################");

  // Create new socket address and validate
  InetAddress myAddr;
  TEST_EQ(Address::TYPE_IPv4, myAddr.GetType());
  TEST_EQ(std::string("0.0.0.0:0"), myAddr.GetAddress());

  // Set socket address using string notation (bad address, bad port)
  TEST_FALSE(myAddr.SetAddress(std::string("1:a")));
  TEST_EQ(std::string("0.0.0.0:0"), myAddr.GetAddress());

  // Set socket address using string notation (good address, no port)
  TEST_FALSE(myAddr.SetAddress(std::string("1.2.3.4")));
  TEST_EQ(std::string("0.0.0.0:0"), myAddr.GetAddress());

  // Set socket address using string notation (good address, bad port)
  TEST_FALSE(myAddr.SetAddress(std::string("1.2.3.4:123456")));
  TEST_EQ(std::string("0.0.0.0:0"), myAddr.GetAddress());

  // Set socket address using string notation (good address, good port)
  TEST_TRUE(myAddr.SetAddress(std::string("1.2.3.4:5")));
  TEST_EQ(std::string("1.2.3.4:5"), myAddr.GetAddress());

  // Return success
  return (0);

}

int
zSocketTest_InetAddressCompare(void* arg_)
{

  ZLOG_DEBUG("#############################################################");
  ZLOG_DEBUG("# zSocketTest_InetAddressCompare()");
  ZLOG_DEBUG("#############################################################");

  // Create new socket address and validate
  InetAddress myAddr1;
  TEST_EQ(Address::TYPE_IPv4, myAddr1.GetType());
  TEST_EQ(std::string("0.0.0.0:0"), myAddr1.GetAddress());

  // Create second socket address and validate
  InetAddress myAddr2;
  TEST_EQ(Address::TYPE_IPv4, myAddr2.GetType());
  TEST_EQ(std::string("0.0.0.0:0"), myAddr2.GetAddress());

  // Compare address (match)
  TEST_TRUE(myAddr1 == myAddr2);
  TEST_FALSE(myAddr1 != myAddr2);

  // Set socket address using string notation
  myAddr1.SetAddress("1.2.3.4:56789");
  TEST_EQ(std::string("1.2.3.4:56789"), myAddr1.GetAddress());

  // Compare address (no match)
  TEST_FALSE(myAddr1 == myAddr2);
  TEST_TRUE(myAddr1 != myAddr2);

  // Set socket address using string notation
  myAddr2.SetAddress("1.2.3.4:56789");
  TEST_EQ(std::string("1.2.3.4:56789"), myAddr2.GetAddress());

  // Compare address (match)
  TEST_TRUE(myAddr1 == myAddr2);
  TEST_FALSE(myAddr1 != myAddr2);

  // Set socket address using string notation
  myAddr1.SetAddress("1.2.3.4:56788");
  TEST_EQ(std::string("1.2.3.4:56788"), myAddr1.GetAddress());

  // Compare address (no match)
  TEST_FALSE(myAddr1 == myAddr2);
  TEST_TRUE(myAddr1 != myAddr2);

  // Return success
  return (0);

}

