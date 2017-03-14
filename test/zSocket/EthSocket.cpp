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
#include <zutils/zEthSocket.h>

#include "zSocketTest.h"

using namespace Test;
using namespace zUtils;

int
zSocketTest_EthSocketDefault(void* arg_)
{

  ZLOG_DEBUG("#############################################################");
  ZLOG_DEBUG("# zSocketTest_EthSocketDefault()");
  ZLOG_DEBUG("#############################################################");

  // Create new socket address and validate
  zSocket::EthAddress MyAddr(std::string("lo"));
  TEST_EQ(std::string("00:00:00:00:00:00"), MyAddr.Address());

  // Create new socket and validate
  zSocket::EthSocket *MySock = new zSocket::EthSocket;
  TEST_ISNOT_NULL(MySock);

  // Cleanup
  MySock->Close();
  delete (MySock);

  // Return success
  return (0);

}

int
zSocketTest_EthSocketSendReceiveLoop(void* arg_)
{

  ZLOG_DEBUG("#############################################################");
  ZLOG_DEBUG("# zSocketTest_EthSocketSendReceiveLoop()");
  ZLOG_DEBUG("#############################################################");

  bool status = false;

  // Create new socket address and validate
  zSocket::EthAddress *SrcAddr = new zSocket::EthAddress("lo");
  TEST_EQ(std::string("00:00:00:00:00:00"), SrcAddr->Address());
  TEST_TRUE(SrcAddr->Address("00:01:00:AA:AA:AA"));
  TEST_EQ(std::string("00:01:00:AA:AA:AA"), SrcAddr->Address());

  // Create new socket address and validate
  zSocket::EthAddress *DstAddr = new zSocket::EthAddress("lo");
  TEST_EQ(std::string("00:00:00:00:00:00"), DstAddr->Address());
  TEST_TRUE(DstAddr->Address("00:01:00:BB:BB:BB"));
  TEST_EQ(std::string("00:01:00:BB:BB:BB"), DstAddr->Address());

//  // Create new socket and validate
//  zSocket::EthSocket *MySock = new zSocket::EthSocket;
//  TEST_ISNOT_NULL(MySock);
//  TEST_TRUE(MySock->Open());
//  TEST_TRUE(MySock->Bind(*SrcAddr));
//
//  // Create new socket handler and validate
//  zEvent::EventHandler* MyHandler = new zEvent::EventHandler;
//  TEST_ISNOT_NULL(MyHandler);
//
//  // Add socket to handler
//  MyHandler->RegisterEvent(MySock);
//
//  // Create new observer and validate
//  TestObserver *MyObserver = new TestObserver;
//  TEST_ISNOT_NULL(MyObserver);
//
//  // Register observer with socket handler
//  MyHandler->RegisterObserver(MyObserver);
//
//  // Send string and validate
//  std::string ExpStr = "Hello Universe";
//  TEST_EQ((int )MySock->Send(*DstAddr, ExpStr), (int )ExpStr.size());
//
//  // Wait for packet to be sent
//  status = MyObserver->TxSem.TimedWait(100);
//  TEST_TRUE(status);
//  zSocket::SocketAddressBufferPair txp = MyObserver->TxSem.Front();
//  MyObserver->TxSem.Pop();
//
//  // Verify no errors
//  status = MyObserver->ErrSem.TryWait();
//  TEST_FALSE(status);
//
//  // Wait for packet to be received
//  status = MyObserver->RxSem.TimedWait(100);
//  TEST_TRUE(status);
//  zSocket::SocketAddressBufferPair rxp = MyObserver->RxSem.Front();
//  MyObserver->RxSem.Pop();
//
//  // Verify no errors
//  status = MyObserver->ErrSem.TryWait();
//  TEST_FALSE(status);
//
//  // Validate messages match
//  TEST_EQ(txp.first->Address(), rxp.first->Address());
//  TEST_EQ(txp.second->Str(), rxp.second->Str());

  // Unregister observer with socket handler
//  MyHandler->UnregisterEvent(MySock);
//  MyHandler->UnregisterObserver(MyObserver);
//
//  // Cleanup
//  delete (MyHandler);
//  delete (MyObserver);
//  delete (MySock);
  delete (DstAddr);
  delete (SrcAddr);

  // Return success
  return (0);

}

int
zSocketTest_EthSocketSendReceiveSock2Sock(void* arg_)
{
  ZLOG_DEBUG("#############################################################");
  ZLOG_DEBUG("# zSocketTest_EthSocketSendReceiveSock2Sock()");
  ZLOG_DEBUG("#############################################################");

  bool status = false;
//
//  // Create new socket address and validate
//  zSocket::EthAddress *SrcAddr = new zSocket::EthAddress;
//  TEST_EQ(std::string("0.0.0.0:0"), SrcAddr->Address());
//
//  // Create new socket address and validate
//  zSocket::EthAddress *DstAddr = new zSocket::EthAddress;
//  TEST_EQ(std::string("0.0.0.0:0"), DstAddr->Address());
//
//  // Set socket address
//  TEST_TRUE(SrcAddr->Address("127.0.0.1:9800"));
//  TEST_EQ(std::string("127.0.0.1:9800"), SrcAddr->Address());
//
//  // Set socket address
//  TEST_TRUE(DstAddr->Address("127.0.0.1:9900"));
//  TEST_EQ(std::string("127.0.0.1:9900"), DstAddr->Address());
//
//  // Verify addresses are different
//  TEST_TRUE(SrcAddr != DstAddr);
//
//  // Create new socket and validate
//  zSocket::EthSocket *MySock1 = new zSocket::EthSocket;
//  TEST_ISNOT_NULL(MySock1);
//  TEST_TRUE(MySock1->Open());
//  TEST_TRUE(MySock1->Bind(*SrcAddr));
//
//  // Create new socket handler and validate
//  zEvent::EventHandler* MyHandler1 = new zEvent::EventHandler;
//  TEST_ISNOT_NULL(MyHandler1);
//
//  // Create new socket handler and validate
//  zEvent::EventHandler* MyHandler2 = new zEvent::EventHandler;
//  TEST_ISNOT_NULL(MyHandler2);
//
//  // Register socket with handler
//  MyHandler1->RegisterEvent(MySock1);
//
//  // Create new observer and validate
//  TestObserver *MyObserver1 = new TestObserver;
//  TEST_ISNOT_NULL(MyObserver1);
//
//  // Register observer
//  MyHandler1->RegisterObserver(MyObserver1);
//
//  // Create new socket and validate
//  zSocket::EthSocket *MySock2 = new zSocket::EthSocket;
//  TEST_ISNOT_NULL(MySock2);
//  TEST_TRUE(MySock2->Open());
//  TEST_TRUE(MySock2->Bind(*DstAddr));
//
//  // Register socket with handler
//  MyHandler2->RegisterEvent(MySock2);
//
//  // Create new observer and validate
//  TestObserver *MyObserver2 = new TestObserver;
//  TEST_ISNOT_NULL(MyObserver2);
//
//  // Register observer
//  MyHandler2->RegisterObserver(MyObserver2);
//
//  // Send string and validate
//  std::string expStr = "Hello Universe";
//  TEST_EQ((int )MySock1->Send(*DstAddr, expStr), (int )expStr.size());
//
//  // Wait for packet to be sent
//  status = MyObserver1->TxSem.TimedWait(100);
//  TEST_TRUE(status);
//  status = MyObserver2->TxSem.TryWait();
//  TEST_FALSE(status);
//  zSocket::SocketAddressBufferPair txp = MyObserver1->TxSem.Front();
//  MyObserver1->TxSem.Pop();
//
//  // Verify no errors
//  status = MyObserver1->ErrSem.TryWait();
//  TEST_FALSE(status);
//
//  // Wait for packet to be received
//  status = MyObserver2->RxSem.TimedWait(100);
//  TEST_TRUE(status);
//  status = MyObserver1->RxSem.TryWait();
//  TEST_FALSE(status);
//  zSocket::SocketAddressBufferPair rxp = MyObserver2->RxSem.Front();
//  MyObserver2->RxSem.Pop();
//
//  // Verify no errors
//  status = MyObserver2->ErrSem.TryWait();
//  TEST_FALSE(status);
//
//  // Validate messages match
//  TEST_EQ(txp.first->Address(), DstAddr->Address());
//  TEST_EQ(rxp.first->Address(), SrcAddr->Address());
//  TEST_EQ(txp.second->Str(), rxp.second->Str());
//
//  // Cleanup
//  MyHandler1->UnregisterEvent(MySock1);
//  MyHandler1->UnregisterObserver(MyObserver1);
//  delete (MyObserver1);
//  MySock1->Close();
//  delete (MySock1);
//  delete (MyHandler1);
//
//  MyHandler1->UnregisterEvent(MySock2);
//  MyHandler1->UnregisterObserver(MyObserver2);
//  delete (MyObserver2);
//  MySock2->Close();
//  delete (MySock2);
//  delete (MyHandler2);
//
//  delete (DstAddr);
//  delete (SrcAddr);

  // Return success
  return (0);

}

