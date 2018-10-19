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

#include <zutils/zRawAddress.h>
#include <zutils/zRawSocket.h>

#include "zSocketTest.h"

using namespace Test;
using namespace zUtils;

uint8_t pkt[] =
{
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, // ........
    0x00, 0x00, 0x00, 0x00, 0x08, 0x00, 0x45, 0x00, // ......E.
    0x00, 0x2a, 0xcb, 0x61, 0x40, 0x00, 0x40, 0x11, // .*.a@.@.
    0x71, 0x5f, 0x7f, 0x00, 0x00, 0x01, 0x7f, 0x00, // q_......
    0x00, 0x01, 0x26, 0x48, 0x26, 0x48, 0x00, 0x16, // ..&H&H..
    0xfe, 0x29, 0x48, 0x65, 0x6c, 0x6c, 0x6f, 0x20, // .)Hello.
    0x55, 0x6e, 0x69, 0x76, 0x65, 0x72, 0x73, 0x65  // Universe
};

int
zSocketTest_RawSocketDefault(void* arg_)
{

  ZLOG_DEBUG("#############################################################");
  ZLOG_DEBUG("# zSocketTest_RawSocketDefault()");
  ZLOG_DEBUG("#############################################################");

  // Create new socket and validate
  zSocket::RawSocket *MySock = new zSocket::RawSocket;
  TEST_ISNOT_NULL(MySock);
  TEST_EQ(zSocket::Socket::TYPE_RAW, MySock->GetType());
  TEST_EQ(std::string(""), MySock->GetAddress().GetAddress());

  // Cleanup
  delete (MySock);

  // Return success
  UTEST_RETURN;

}

int
zSocketTest_RawSocketSendReceiveLoop(void* arg_)
{

  bool status = false;

  ZLOG_DEBUG("#############################################################");
  ZLOG_DEBUG("# zSocketTest_RawSocketSendReceiveLoop()");
  ZLOG_DEBUG("#############################################################");

  // First step is to determine if we were executed with root privileges, if not, bypass test
  uid_t uid = getuid(), euid = geteuid();
  if (uid != 0 || uid != euid)
  {
    ZLOG_DEBUG("Test bypassed");
    UTEST_BYPASS;
  }

  // Create new socket address and validate
  zSocket::RawAddress *SrcAddr = new zSocket::RawAddress;
  TEST_ISNOT_NULL(SrcAddr);
  TEST_EQ(std::string(""), SrcAddr->GetAddress());
  TEST_TRUE(SrcAddr->SetAddress("lo"));
  TEST_EQ(std::string("lo"), SrcAddr->GetAddress());

  // Create new socket address and validate
  zSocket::RawAddress *DstAddr = new zSocket::RawAddress;
  TEST_ISNOT_NULL(DstAddr);
  TEST_EQ(std::string(""), DstAddr->GetAddress());
  TEST_TRUE(DstAddr->SetAddress("lo"));
  TEST_EQ(std::string("lo"), DstAddr->GetAddress());

  // Create new socket handler and validate
  zSocket::Handler* MyHandler = new zSocket::Handler;
  TEST_ISNOT_NULL(MyHandler);

  // Create new observer and validate
  TestObserver *MyObserver = new TestObserver;
  TEST_ISNOT_NULL(MyObserver);

  // Register observer with socket handler
  TEST_TRUE(MyHandler->RegisterObserver(MyObserver));

  // Create new socket and validate
  zSocket::RawSocket *MySock = new zSocket::RawSocket;
  TEST_ISNOT_NULL(MySock);
  TEST_TRUE(MySock->Bind(*SrcAddr));

  // Add socket to handler
  TEST_TRUE(MyHandler->RegisterSocket(MySock));

  // Send string and validate
  zSocket::Buffer sb(sizeof(pkt));
  memcpy(sb.Head(), pkt, sizeof(pkt));
  TEST_TRUE(sb.Put(sizeof(pkt)));
  SHARED_PTR(zSocket::Notification) txn(MySock->Send(*DstAddr, sb));
  TEST_ISNOT_NULL(txn.get());
  TEST_EQ(zSocket::Notification::SUBTYPE_PKT_SENT, txn->GetSubType());

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
  TEST_TRUE_MSG((*STATIC_CAST(RawAddress)(rxn->GetSrcAddress()) == *SrcAddr), rxn->GetSrcAddress()->GetAddress());
  TEST_TRUE_MSG((*STATIC_CAST(RawAddress)(rxn->GetDstAddress()) == *DstAddr), rxn->GetDstAddress()->GetAddress());
  TEST_TRUE(sb == *rxn->GetBuffer()); // TODO: Always receive a mysterious extra 14 bytes than we send to ourselves
  // See Ubuntu bug: #1791893 (https://bugs.launchpad.net/ubuntu/+source/linux/+bug/1791893)

  // Unregister observer with socket handler
  MyHandler->UnregisterSocket(MySock);
  MyHandler->UnregisterObserver(MyObserver);

  // Cleanup
  delete (MySock);
  delete (DstAddr);
  delete (SrcAddr);
  delete (MyObserver);
  delete (MyHandler);

  // Return success
  UTEST_RETURN;

}

int
zSocketTest_RawSocketSendReceiveSock2Sock(void* arg_)
{

  bool status = false;

  ZLOG_DEBUG("#############################################################");
  ZLOG_DEBUG("# zSocketTest_RawSocketSendReceiveSock2Sock()");
  ZLOG_DEBUG("#############################################################");

  // First step is to determine if we were executed with root privileges, if not, bypass test
  uid_t uid = getuid(), euid = geteuid();
  if (uid != 0 || uid != euid)
  {
    ZLOG_DEBUG("Test bypassed");
    UTEST_BYPASS;
  }

  // Create new socket address and validate
  zSocket::RawAddress *SrcAddr = new zSocket::RawAddress;
  TEST_ISNOT_NULL(SrcAddr);
  TEST_EQ(std::string(""), SrcAddr->GetAddress());
  TEST_TRUE(SrcAddr->SetAddress("lo"));
  TEST_EQ(std::string("lo"), SrcAddr->GetAddress());

  // Create new socket address and validate
  zSocket::RawAddress *DstAddr = new zSocket::RawAddress;
  TEST_ISNOT_NULL(DstAddr);
  TEST_EQ(std::string(""), DstAddr->GetAddress());
  TEST_TRUE(DstAddr->SetAddress("lo"));
  TEST_EQ(std::string("lo"), DstAddr->GetAddress());

  // Create new socket handler and validate
  zSocket::Handler* MyHandler = new zSocket::Handler;
  TEST_ISNOT_NULL(MyHandler);

  // Create new observer and validate
  TestObserver *MyObserver = new TestObserver;
  TEST_ISNOT_NULL(MyObserver);

  // Register observer with socket handler
  TEST_TRUE(MyHandler->RegisterObserver(MyObserver));

  // Create new socket and validate
  zSocket::RawSocket *MySock1 = new zSocket::RawSocket;
  TEST_ISNOT_NULL(MySock1);
  TEST_TRUE(MySock1->Bind(*SrcAddr));

  // Add socket to handler
  TEST_TRUE(MyHandler->RegisterSocket(MySock1));

  // Create new socket and validate
  zSocket::RawSocket *MySock2 = new zSocket::RawSocket;
  TEST_ISNOT_NULL(MySock2);
  TEST_TRUE(MySock2->Bind(*DstAddr));

  // Add socket to handler
  TEST_TRUE(MyHandler->RegisterSocket(MySock2));

  // Send string and validate
  zSocket::Buffer sb(sizeof(pkt));
  memcpy(sb.Head(), pkt, sizeof(pkt));
  TEST_TRUE(sb.Put(sizeof(pkt)));
  SHARED_PTR(zSocket::Notification) txn(MySock1->Send(*DstAddr, sb));
  TEST_ISNOT_NULL(txn.get());
  TEST_EQ(zSocket::Notification::SUBTYPE_PKT_SENT, txn->GetSubType());

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
  TEST_TRUE_MSG((*STATIC_CAST(RawAddress)(rxn->GetSrcAddress()) == *SrcAddr), rxn->GetSrcAddress()->GetAddress());
  TEST_TRUE_MSG((*STATIC_CAST(RawAddress)(rxn->GetDstAddress()) == *DstAddr), rxn->GetDstAddress()->GetAddress());
  TEST_TRUE(sb == *rxn->GetBuffer()); // TODO: Always receive an extra 14 bytes than we send to ourselves
  // See Ubuntu bug: #1791893 (https://bugs.launchpad.net/ubuntu/+source/linux/+bug/1791893)

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
  UTEST_RETURN;

}

