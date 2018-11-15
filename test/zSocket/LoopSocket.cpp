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

#include <zutils/zUtils.h>
#include <zutils/zLog.h>
using namespace zUtils;
ZLOG_MODULE_INIT(zLog::Log::MODULE_TEST);

#include <zutils/zLoopSocket.h>

#include "zSocketTest.h"

using namespace Test;
using namespace zUtils;

int
zSocketTest_LoopSocketDefault(void* arg_)
{

  ZLOG_DEBUG("#############################################################");
  ZLOG_DEBUG("# zSocketTest_LoopSocketDefault()");
  ZLOG_DEBUG("#############################################################");

  // Create new socket and validate
  zSocket::LoopSocket *MySock = new zSocket::LoopSocket;
  TEST_ISNOT_NULL(MySock);

  // Cleanup
  delete (MySock);

  // Return success
  return (0);

}

int
zSocketTest_LoopSocketSendReceive(void* arg_)
{

  ZLOG_DEBUG("#############################################################");
  ZLOG_DEBUG("# zSocketTest_LoopSocketSendReceiveLoop()");
  ZLOG_DEBUG("#############################################################");

  bool status = false;

  // Create new socket address and validate
  zSocket::LoopAddress *MyAddr = new zSocket::LoopAddress;
  TEST_EQ(Address::TYPE_LOOP, MyAddr->GetType());
  TEST_EQ(std::string(""), MyAddr->GetAddress());

  // Create new socket and validate
  zSocket::LoopSocket *MySock = new zSocket::LoopSocket;
  TEST_ISNOT_NULL(MySock);
  TEST_TRUE(MySock->Bind(*MyAddr));

  // Create new socket handler and validate
  zSocket::Handler* MyHandler = new zSocket::Handler;
  TEST_ISNOT_NULL(MyHandler);

  // Add socket to handler
  MyHandler->RegisterEvent(MySock);

  // Create new observer and validate
  TestObserver *MyObserver = new TestObserver;
  TEST_ISNOT_NULL(MyObserver);

  // Register observer with socket handler
  MyHandler->RegisterObserver(MyObserver);

  // Send string and validate
  std::string ExpStr = "Hello Universe";
  TEST_TRUE(MySock->Send(*MyAddr, ExpStr));

  // Verify no errors
  status = MyObserver->ErrSem.TryWait();
  TEST_FALSE(status);

  // Wait for packet to be sent
  status = MyObserver->TxSem.TimedWait(100);
  TEST_TRUE(status);
  SHARED_PTR(zSocket::Notification) txn(MyObserver->TxSem.Front());
  TEST_ISNOT_NULL(txn.get());
  MyObserver->TxSem.Pop();

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
  TEST_TRUE_MSG((*rxn->GetSrcAddress() == *MyAddr), rxn->GetSrcAddress()->GetAddress());
  TEST_TRUE_MSG((*rxn->GetDstAddress() == *MyAddr), rxn->GetDstAddress()->GetAddress());
  TEST_TRUE(ExpStr == rxn->GetBuffer()->String());

  // Unregister observer with socket handler
  MyHandler->UnregisterEvent(MySock);
  MyHandler->UnregisterObserver(MyObserver);

  // Cleanup
  delete (MyHandler);
  delete (MyObserver);
  delete (MySock);
  delete (MyAddr);

  // Return success
  return (UTEST_PASS);

}
