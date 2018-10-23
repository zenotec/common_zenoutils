/*
 * Copyright (c) 2018 ZenoTec LLC (http://www.zenotec.net)
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

#include <zutils/zUdpSocket.h>

#include "zSocketTest.h"

using namespace Test;
using namespace zUtils;

int
zSocketTest_UdpSocketDefault(void* arg_)
{

  ZLOG_DEBUG("#############################################################");
  ZLOG_DEBUG("# zSocketTest_UdpSocketDefault()");
  ZLOG_DEBUG("#############################################################");

  // Create new socket address and validate
  zSocket::Ipv4Address MyAddr(std::string("127.0.0.1:0"));
  TEST_EQ(std::string("127.0.0.1:0"), MyAddr.GetAddress());

  // Set socket address
  TEST_TRUE(MyAddr.SetAddress(std::string("127.0.0.1:9876")));
  TEST_EQ(std::string("127.0.0.1:9876"), MyAddr.GetAddress());

  // Create new socket and validate
  zSocket::UdpSocket *MySock = new zSocket::UdpSocket;
  TEST_ISNOT_NULL(MySock);

  // Cleanup
  delete (MySock);

  // Return success
  return (0);

}

int
zSocketTest_UdpSocketSendReceiveLoop(void* arg_)
{

  ZLOG_DEBUG("#############################################################");
  ZLOG_DEBUG("# zSocketTest_UdpSocketSendReceiveLoop()");
  ZLOG_DEBUG("#############################################################");

  bool status = false;

  // Create new socket address and validate
  zSocket::Ipv4Address *SrcAddr = new zSocket::Ipv4Address;
  TEST_EQ(std::string("0.0.0.0:0"), SrcAddr->GetAddress());
  TEST_TRUE(SrcAddr->SetAddress("127.0.0.1:9800"));
  TEST_EQ(std::string("127.0.0.1:9800"), SrcAddr->GetAddress());

  // Create new socket address and validate
  zSocket::Ipv4Address *DstAddr = new zSocket::Ipv4Address;
  TEST_EQ(std::string("0.0.0.0:0"), DstAddr->GetAddress());
  TEST_TRUE(DstAddr->SetAddress("127.0.0.1:9800"));
  TEST_EQ(std::string("127.0.0.1:9800"), DstAddr->GetAddress());

  // Create new socket and validate
  zSocket::UdpSocket *MySock = new zSocket::UdpSocket;
  TEST_ISNOT_NULL(MySock);
  TEST_TRUE(MySock->Bind(*SrcAddr));

  // Create new socket handler and validate
  zSocket::Handler* MyHandler = new zSocket::Handler;
  TEST_ISNOT_NULL(MyHandler);

  // Add socket to handler
  MyHandler->RegisterSocket(MySock);

  // Create new observer and validate
  TestObserver *MyObserver = new TestObserver;
  TEST_ISNOT_NULL(MyObserver);

  // Register observer with socket handler
  MyHandler->RegisterObserver(MyObserver);

  // Send string and validate
  uint8_t buf[] = { 0x48, 0x65, 0x6c, 0x6c, 0x6f, 0x20, 0x55, 0x6e, 0x69, 0x76, 0x65, 0x72, 0x73, 0x65 }; // "Hello Universe"
  zSocket::Buffer sb;
  memcpy(sb.Head(), buf, sizeof(buf));
  TEST_TRUE(sb.Put(sizeof(buf)));
  TEST_TRUE(MySock->Send(*DstAddr, sb));

  // Verify no errors
  status = MyObserver->ErrSem.TryWait();
  TEST_FALSE(status);

  // Wait for packet to be received
  status = MyObserver->RxSem.TimedWait(100);
  TEST_TRUE(status);
  SHARED_PTR(zSocket::Notification) rxn(MyObserver->RxSem.Front());
  TEST_ISNOT_NULL(rxn.get());
  MyObserver->RxSem.Pop();

  // Verify no errors
  status = MyObserver->ErrSem.TryWait();
  TEST_FALSE(status);

  // Validate messages match
  TEST_TRUE_MSG((*STATIC_CAST(LoopAddress)(rxn->GetSrcAddress()) == *SrcAddr), rxn->GetSrcAddress()->GetAddress());
  TEST_TRUE_MSG((*STATIC_CAST(LoopAddress)(rxn->GetDstAddress()) == *DstAddr), rxn->GetDstAddress()->GetAddress());
  TEST_TRUE(sb == *rxn->GetBuffer());
  uint8_t* p = rxn->GetBuffer()->Head();
  for (int i = 0; i < sizeof(buf); i++)
  {
    TEST_EQ_MSG(buf[i], *p++, zUtils::zToStr(i));
  }

  // Unregister observer with socket handler
  MyHandler->UnregisterSocket(MySock);
  MyHandler->UnregisterObserver(MyObserver);

  // Cleanup
  delete (MyHandler);
  delete (MyObserver);
  delete (MySock);
  delete (DstAddr);
  delete (SrcAddr);

  // Return success
  return (UTEST_PASS);

}

int
zSocketTest_UdpSocketSendReceiveSock2Sock(void* arg_)
{
  ZLOG_DEBUG("#############################################################");
  ZLOG_DEBUG("# zSocketTest_UdpSocketSendReceiveSock2Sock()");
  ZLOG_DEBUG("#############################################################");

  bool status = false;

  // Create new socket address and validate
  zSocket::Ipv4Address *SrcAddr = new zSocket::Ipv4Address;
  TEST_EQ(std::string("0.0.0.0:0"), SrcAddr->GetAddress());

  // Create new socket address and validate
  zSocket::Ipv4Address *DstAddr = new zSocket::Ipv4Address;
  TEST_EQ(std::string("0.0.0.0:0"), DstAddr->GetAddress());

  // Set socket address
  TEST_TRUE(SrcAddr->SetAddress("127.0.0.1:9800"));
  TEST_EQ(std::string("127.0.0.1:9800"), SrcAddr->GetAddress());

  // Set socket address
  TEST_TRUE(DstAddr->SetAddress("127.0.0.1:9900"));
  TEST_EQ(std::string("127.0.0.1:9900"), DstAddr->GetAddress());

  // Verify addresses are different
  TEST_TRUE(SrcAddr != DstAddr);

  // Create new socket handler and validate
  zSocket::Handler* MyHandler = new zSocket::Handler;
  TEST_ISNOT_NULL(MyHandler);

  // Create new observer and validate
  TestObserver *MyObserver = new TestObserver;
  TEST_ISNOT_NULL(MyObserver);

  // Register observer
  MyHandler->RegisterObserver(MyObserver);

  // Create new socket and validate
  zSocket::UdpSocket *MySock1 = new zSocket::UdpSocket;
  TEST_ISNOT_NULL(MySock1);
  TEST_TRUE(MySock1->Bind(*SrcAddr));

  // Register socket with handler
  MyHandler->RegisterSocket(MySock1);

  // Create new socket and validate
  zSocket::UdpSocket *MySock2 = new zSocket::UdpSocket;
  TEST_ISNOT_NULL(MySock2);
  TEST_TRUE(MySock2->Bind(*DstAddr));

  // Register socket with handler
  MyHandler->RegisterSocket(MySock2);

  // Send string and validate
  uint8_t buf[] = { 0x48, 0x65, 0x6c, 0x6c, 0x6f, 0x20, 0x55, 0x6e, 0x69, 0x76, 0x65, 0x72, 0x73, 0x65 }; // "Hello Universe"
  zSocket::Buffer sb;
  memcpy(sb.Head(), buf, sizeof(buf));
  TEST_TRUE(sb.Put(sizeof(buf)));
  TEST_TRUE(MySock1->Send(*DstAddr, sb));

  // Verify no errors
  status = MyObserver->ErrSem.TryWait();
  TEST_FALSE(status);

  // Wait for packet to be received
  status = MyObserver->RxSem.TimedWait(100);
  TEST_TRUE(status);
  SHARED_PTR(zSocket::Notification) rxn(MyObserver->RxSem.Front());
  TEST_ISNOT_NULL(rxn.get());
  MyObserver->RxSem.Pop();

  // Verify no errors
  status = MyObserver->ErrSem.TryWait();
  TEST_FALSE(status);

  // Validate messages match
  TEST_TRUE_MSG((*STATIC_CAST(LoopAddress)(rxn->GetSrcAddress()) == *SrcAddr), rxn->GetSrcAddress()->GetAddress());
  TEST_TRUE_MSG((*STATIC_CAST(LoopAddress)(rxn->GetDstAddress()) == *DstAddr), rxn->GetDstAddress()->GetAddress());
  TEST_TRUE(sb == *rxn->GetBuffer());
  uint8_t* p = rxn->GetBuffer()->Head();
  for (int i = 0; i < sizeof(buf); i++)
  {
    TEST_EQ_MSG(buf[i], *p++, zUtils::zToStr(i));
  }

  // Unregister observer with socket handler
  MyHandler->UnregisterSocket(MySock1);
  MyHandler->UnregisterSocket(MySock2);
  MyHandler->UnregisterObserver(MyObserver);

  // Cleanup
  delete (MySock1);
  delete (MySock2);
  delete (DstAddr);
  delete (SrcAddr);
  delete (MyObserver);
  delete (MyHandler);

  // Return success
  return (UTEST_PASS);

}

