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

#include <list>
#include <mutex>

#include <zutils/zLog.h>
#include <zutils/zQueue.h>
#include <zutils/zThread.h>
#include <zutils/zData.h>
#include <zutils/zEvent.h>
#include <zutils/zConfig.h>
#include <zutils/zSerial.h>

#include "UnitTest.h"
#include "zSerialTest.h"

using namespace Test;
using namespace zUtils;

int
zSerialTest_PortSendRecvChar(void *arg_)
{

  ZLOG_DEBUG("#############################################################");
  ZLOG_DEBUG("# zSerialTest_PortSendRecvChar()");
  ZLOG_DEBUG("#############################################################");

  bool status = false;

  // Create new port and validate
  TestPort *MyPort = new TestPort;
  TEST_ISNOT_NULL(MyPort);

  // Create new observer and validate
  TestObserver *MyObserver = new TestObserver;
  TEST_ISNOT_NULL(MyObserver);
  TEST_FALSE(MyObserver->RxSem.TryWait());
  TEST_FALSE(MyObserver->TxSem.TryWait());
  TEST_FALSE(MyObserver->ErrSem.TryWait());

  // Create new handler and validate
  zEvent::Handler *MyHandler = new zEvent::Handler;
  TEST_ISNOT_NULL(MyHandler);

  // Register observer
  status = MyHandler->RegisterObserver(MyObserver);
  TEST_TRUE(status);

  // Register port with handler
  status = MyHandler->RegisterEvent(MyPort);
  TEST_TRUE(status);

  // Open port
  status = MyPort->Open();
  TEST_TRUE(status);

  // Send byte
  status = MyPort->SendChar('a');
  TEST_TRUE(status);

  // Wait for byte to be sent
  status = MyObserver->TxSem.TimedWait(100);
  TEST_TRUE(status);
  TEST_EQ('a', MyObserver->TxSem.Front());
  MyObserver->TxSem.Pop();

  // Wait for byte to be received
  status = MyObserver->RxSem.TimedWait(100);
  TEST_TRUE(status);
  TEST_EQ('a', MyObserver->RxSem.Front());
  MyObserver->RxSem.Pop();

  // Verify no errors
  status = MyObserver->ErrSem.TryWait();
  TEST_FALSE(status);

  // Unregister observer
  status = MyHandler->UnregisterObserver(MyObserver);
  TEST_TRUE(status);

  // Cleanup
  delete (MyHandler);
  delete (MyObserver);
  MyPort->Close();
  delete (MyPort);

  // Return success
  return (0);

}

int
zSerialTest_PortSendRecvBuf(void *arg_)
{

  ZLOG_DEBUG("#############################################################");
  ZLOG_DEBUG("# zSerialTest_PortSendRecvBuf()");
  ZLOG_DEBUG("#############################################################");

  int cnt = 0;
  bool status = false;
  char exp_buf[256] = { 0 };
  char obs_buf[256] = { 0 };
  ssize_t bytes = 0;
  zEvent::Event *MyEvent = NULL;

  // Create new test port and validate
  TestPort *MyPort = new TestPort;
  TEST_ISNOT_NULL(MyPort);

  // Create new observer and validate
  TestObserver *MyObserver = new TestObserver;
  TEST_ISNOT_NULL(MyObserver);
  TEST_FALSE(MyObserver->RxSem.TryWait());
  TEST_FALSE(MyObserver->TxSem.TryWait());
  TEST_FALSE(MyObserver->ErrSem.TryWait());

  // Create new handler and validate
  zEvent::Handler *MyHandler = new zEvent::Handler;
  TEST_ISNOT_NULL(MyHandler);

  // Register observer
  status = MyHandler->RegisterObserver(MyObserver);
  TEST_TRUE(status);

  // Register port with handler
  status = MyHandler->RegisterEvent(MyPort);
  TEST_TRUE(status);

  // Open port
  status = MyPort->Open();
  TEST_TRUE(status);

  // Send data
  for (cnt = 0; cnt < sizeof(exp_buf); cnt++)
  {
    exp_buf[cnt] = (cnt + 7);
  }
  bytes = MyPort->SendBuf(exp_buf, sizeof(exp_buf));
  TEST_EQ(sizeof(exp_buf), bytes);

  // Verify data was sent
  for (cnt = 0; MyObserver->TxSem.TimedWait(100); cnt++)
  {
    TEST_EQ(exp_buf[cnt], MyObserver->TxSem.Front());
    MyObserver->TxSem.Pop();
  }
  TEST_EQ(cnt, sizeof(exp_buf));

  // Verify data was received
  for (cnt = 0; MyObserver->RxSem.TimedWait(100); cnt++)
  {
    TEST_EQ(exp_buf[cnt], MyObserver->RxSem.Front());
    MyObserver->RxSem.Pop();
  }
  TEST_EQ(cnt, sizeof(exp_buf));

  // Verify no errors
  status = MyObserver->ErrSem.TryWait();
  TEST_FALSE(status);

  // Unregister observer
  status = MyHandler->UnregisterObserver(MyObserver);
  TEST_TRUE(status);

  // Cleanup
  delete (MyHandler);
  delete (MyObserver);
  MyPort->Close();
  delete (MyPort);

  // Return success
  return (0);

}
