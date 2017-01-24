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
#include <zutils/zSem.h>
#include <zutils/zThread.h>
#include <zutils/zQueue.h>
#include <zutils/zEvent.h>
#include <zutils/zSocket.h>
#include <zutils/zLoopSocket.h>
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
  TEST_EQ(SocketType::TYPE_UNIX, MyAddr.Type());
  TEST_EQ(std::string(""), MyAddr.Address());

  // Set socket address
  TEST_TRUE(MyAddr.Address(std::string("/tmp/UnixTestSock")));
  TEST_EQ(std::string("/tmp/UnixTestSock"), MyAddr.Address());

  // Create new socket and validate
  zSocket::UnixSocket *MySock = new zSocket::UnixSocket;
  TEST_ISNOT_NULL(MySock);

  // Cleanup
  MySock->Close();
  delete (MySock);

  // Return success
  return (0);

}

int
zSocketTest_UnixSocketSendReceive(void* arg_)
{

  ZLOG_DEBUG("#############################################################");
  ZLOG_DEBUG("# zSocketTest_UnixSocketSendReceiveUnix()");
  ZLOG_DEBUG("#############################################################");

  bool status = false;

  // Create new socket address and validate
  zSocket::UnixAddress *SrcAddr = new zSocket::UnixAddress;
  TEST_EQ(SocketType::TYPE_UNIX, SrcAddr->Type());
  TEST_EQ(std::string(""), SrcAddr->Address());
  TEST_TRUE(SrcAddr->Address(std::string("/tmp/UnixSrcSock")));
  TEST_EQ(std::string("/tmp/UnixSrcSock"), SrcAddr->Address());

  // Create new socket and validate
  zSocket::UnixSocket *MySrcSock = new zSocket::UnixSocket;
  TEST_ISNOT_NULL(MySrcSock);
  TEST_TRUE(MySrcSock->Open());
  TEST_TRUE(MySrcSock->Bind(*SrcAddr));

  // Create new socket address and validate
  zSocket::UnixAddress *DstAddr = new zSocket::UnixAddress;
  TEST_EQ(SocketType::TYPE_UNIX, DstAddr->Type());
  TEST_EQ(std::string(""), DstAddr->Address());
  TEST_TRUE(DstAddr->Address(std::string("/tmp/UnixDstSock")));
  TEST_EQ(std::string("/tmp/UnixDstSock"), DstAddr->Address());

  // Create new socket and validate
  zSocket::UnixSocket *MyDstSock = new zSocket::UnixSocket;
  TEST_ISNOT_NULL(MyDstSock);
  TEST_TRUE(MyDstSock->Open());
  TEST_TRUE(MyDstSock->Bind(*DstAddr));

  // Create new socket handler and validate
  zEvent::EventHandler* MyHandler = new zEvent::EventHandler;
  TEST_ISNOT_NULL(MyHandler);

  // Add socket to handler
  MyHandler->RegisterEvent(MySrcSock);
  MyHandler->RegisterEvent(MyDstSock);

  // Create new observer and validate
  TestObserver *MyObserver = new TestObserver;
  TEST_ISNOT_NULL(MyObserver);

  // Register observer with socket handler
  MyHandler->RegisterObserver(MyObserver);

  // Send string and validate
  std::string ExpStr = "Hello Universe";
  TEST_EQ((int )MySrcSock->Send(*DstAddr, ExpStr), (int )ExpStr.size());

  // Wait for packet to be sent
  status = MyObserver->TxSem.TimedWait(100000);
  TEST_TRUE(status);
  zSocket::SocketAddressBufferPair txp = MyObserver->TxSem.Front();
  MyObserver->TxSem.Pop();

  // Verify no errors
  status = MyObserver->ErrSem.TryWait();
  TEST_FALSE(status);

  // Wait for packet to be received
  status = MyObserver->RxSem.TimedWait(100000);
  TEST_TRUE(status);
  zSocket::SocketAddressBufferPair rxp = MyObserver->RxSem.Front();
  MyObserver->RxSem.Pop();

  // Verify no errors
  status = MyObserver->ErrSem.TryWait();
  TEST_FALSE(status);

  // Validate messages match
  TEST_EQ(txp.first->Address(), DstAddr->Address());
  TEST_EQ(rxp.first->Address(), SrcAddr->Address());
  TEST_EQ(txp.second->Str(), rxp.second->Str());

  // Unregister observer with socket handler
  MyHandler->UnregisterEvent(MySrcSock);
  MyHandler->UnregisterEvent(MyDstSock);
  MyHandler->UnregisterObserver(MyObserver);

  // Cleanup
  delete (MyHandler);
  delete (MyObserver);
  delete (MySrcSock);
  delete (MyDstSock);
  delete (DstAddr);
  delete (SrcAddr);

  // Return success
  return (0);

}
