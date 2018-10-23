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

#include <zutils/zUtils.h>
#include <zutils/zLog.h>
using namespace zUtils;
ZLOG_MODULE_INIT(zLog::Log::MODULE_TEST);

#include <zutils/zUnixSocket.h>

#include "zSocketTest.h"

using namespace Test;
using namespace zUtils;

int
zSocketTest_UnixSocketDefault(void* arg_)
{

  ZLOG_DEBUG("#############################################################");
  ZLOG_DEBUG("# zSocketTest_UnixSocketDefault()");
  ZLOG_DEBUG("#############################################################");

  // Create new socket address and validate
  zSocket::UnixAddress MyAddr;
  TEST_EQ(Address::TYPE_UNIX, MyAddr.GetType());
  TEST_EQ(std::string(""), MyAddr.GetAddress());

  // Set socket address
  TEST_TRUE(MyAddr.SetAddress(std::string("/tmp/UnixTestSock")));
  TEST_EQ(std::string("/tmp/UnixTestSock"), MyAddr.GetAddress());

  // Create new socket and validate
  zSocket::UnixSocket *MySock = new zSocket::UnixSocket;
  TEST_ISNOT_NULL(MySock);

  // Cleanup
  delete (MySock);

  // Return success
  return (UTEST_PASS);

}

int
zSocketTest_UnixSocketSendReceive(void* arg_)
{

  ZLOG_DEBUG("#############################################################");
  ZLOG_DEBUG("# zSocketTest_UnixSocketSendReceive()");
  ZLOG_DEBUG("#############################################################");

  bool status = false;

  // Create new socket address and validate
  zSocket::UnixAddress *SrcAddr = new zSocket::UnixAddress;
  TEST_EQ(Address::TYPE_UNIX, SrcAddr->GetType());
  TEST_EQ(std::string(""), SrcAddr->GetAddress());
  TEST_TRUE(SrcAddr->SetAddress(std::string("/tmp/UnixSrcSock")));
  TEST_EQ(std::string("/tmp/UnixSrcSock"), SrcAddr->GetAddress());

  // Create new socket and validate
  zSocket::UnixSocket *MySrcSock = new zSocket::UnixSocket;
  TEST_ISNOT_NULL(MySrcSock);
  TEST_TRUE(MySrcSock->Bind(*SrcAddr));

  // Create new socket address and validate
  zSocket::UnixAddress *DstAddr = new zSocket::UnixAddress;
  TEST_EQ(Address::TYPE_UNIX, DstAddr->GetType());
  TEST_EQ(std::string(""), DstAddr->GetAddress());
  TEST_TRUE(DstAddr->SetAddress(std::string("/tmp/UnixDstSock")));
  TEST_EQ(std::string("/tmp/UnixDstSock"), DstAddr->GetAddress());

  // Create new socket and validate
  zSocket::UnixSocket *MyDstSock = new zSocket::UnixSocket;
  TEST_ISNOT_NULL(MyDstSock);
  TEST_TRUE(MyDstSock->Bind(*DstAddr));

  // Create new socket handler and validate
  zSocket::Handler* MyHandler = new zSocket::Handler;
  TEST_ISNOT_NULL(MyHandler);

  // Add socket to handler
  MyHandler->RegisterSocket(MySrcSock);
  MyHandler->RegisterSocket(MyDstSock);

  // Create new observer and validate
  TestObserver *MyObserver = new TestObserver;
  TEST_ISNOT_NULL(MyObserver);

  // Register observer with socket handler
  MyHandler->RegisterObserver(MyObserver);

  // Send string and validate
  std::string ExpStr = "Hello Universe";
  TEST_TRUE(MySrcSock->Send(*DstAddr, ExpStr));

  // Verify no errors
  status = MyObserver->ErrSem.TryWait();
  TEST_FALSE(status);

  // Wait for packet to be received
  status = MyObserver->RxSem.TimedWait(100);
  TEST_TRUE(status);
  SHARED_PTR(zSocket::Notification) rxn(MyObserver->RxSem.Front());
  MyObserver->RxSem.Pop();

  // Verify no errors
  status = MyObserver->ErrSem.TryWait();
  TEST_FALSE(status);

  // Validate messages match
  TEST_TRUE_MSG((*rxn->GetSrcAddress() == *SrcAddr), rxn->GetSrcAddress()->GetAddress());
  TEST_TRUE_MSG((*rxn->GetDstAddress() == *DstAddr), rxn->GetDstAddress()->GetAddress());
  TEST_EQ(ExpStr, rxn->GetBuffer()->String());

  // Unregister observer with socket handler
  MyHandler->UnregisterSocket(MySrcSock);
  MyHandler->UnregisterSocket(MyDstSock);
  MyHandler->UnregisterObserver(MyObserver);

  // Cleanup
  delete (MyObserver);
  delete (MyHandler);
  delete (MySrcSock);
  delete (MyDstSock);
  delete (DstAddr);
  delete (SrcAddr);

  // Return success
  return (UTEST_PASS);

}
