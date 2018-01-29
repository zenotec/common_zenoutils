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
using namespace zUtils;
ZLOG_MODULE_INIT(zLog::Log::MODULE_TEST);

#include <zutils/zSocket.h>
#include <zutils/zLoopSocket.h>

#include "zSocketTest.h"

using namespace Test;
using namespace zUtils;
using namespace zSocket;

int
zSocketTest_BufferDefaults(void* arg_)
{

  ZLOG_DEBUG("#############################################################");
  ZLOG_DEBUG("# zSocketTest_BufferDefaults()");
  ZLOG_DEBUG("#############################################################");

  // Create new packet and validate
  Buffer mySb1;
  TEST_ISNOT_NULL(mySb1.Head());
  TEST_EQ(mySb1.Head(), mySb1.Data());
  TEST_IS_ZERO(mySb1.Length());
  TEST_IS_ZERO(mySb1.Size());
  TEST_EQ(mySb1.TotalSize(), 1500);
  TEST_EQ(std::string(""), mySb1.Str());

  // Create new packet of set size and validate
  Buffer mySb2(500);
  TEST_ISNOT_NULL(mySb2.Head());
  TEST_EQ(mySb1.Head(), mySb1.Data());
  TEST_ISNOT_NULL(mySb2.Data());
  TEST_IS_ZERO(mySb2.Length());
  TEST_IS_ZERO(mySb2.Size());
  TEST_EQ(mySb2.TotalSize(), 500);
  TEST_EQ(std::string(""), mySb2.Str());

  // Return success
  return (0);

}

int
zSocketTest_AddressDefaults(void* arg_)
{

  ZLOG_DEBUG("#############################################################");
  ZLOG_DEBUG("# zSocketTest_AddressDefaults()");
  ZLOG_DEBUG("#############################################################");

  // Create new socket address and validate
  TestAddress *myAddr = new TestAddress;
  TEST_ISNOT_NULL(myAddr);
  TEST_EQ(SOCKET_TYPE::TYPE_TEST, myAddr->GetType());
  TEST_EQ(std::string(""), myAddr->GetAddress());
  delete (myAddr);

  // Return success
  return (0);
}

int
zSocketTest_ObserverDefaults(void* arg_)
{

  ZLOG_DEBUG("#############################################################");
  ZLOG_DEBUG("# zSocketTest_ObserverDefaults()");
  ZLOG_DEBUG("#############################################################");

  TestObserver *myObserver = new TestObserver;
  TEST_ISNOT_NULL(myObserver);
  delete (myObserver);

  // Return success
  return (0);
}

int
zSocketTest_SocketDefaults(void* arg_)
{

  ZLOG_DEBUG("#############################################################");
  ZLOG_DEBUG("# zSocketTest_SocketDefaults()");
  ZLOG_DEBUG("#############################################################");

  TestSocket *mySocket = new TestSocket;
  TEST_ISNOT_NULL(mySocket);
  delete (mySocket);

  // Return success
  return (0);

}
