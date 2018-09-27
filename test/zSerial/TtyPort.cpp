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

TtyTestThread::TtyTestThread(TtyTestPort *port_) :
    _port(port_)
{

  ZLOG_INFO("Created new test thread");
}

TtyTestThread::~TtyTestThread()
{
  ZLOG_INFO("Destroying test thread");
}

void
TtyTestThread::Run(zThread::ThreadArg *arg_)
{

  int usecs = 1000000;
  fd_set rxFdSet;
  struct timeval to = { 0 };
  ssize_t bytes = -1;
  char c = 0;

  if (!this->_port)
  {
    return;
  }

  // Setup for poll loop
  struct pollfd fds[1];
  fds[0].fd = this->_port->_master_fd;
  fds[0].events = (POLLIN | POLLERR);

  while (!this->Exit())
  {

    int ret = poll(fds, 1, 100);
    if (ret > 0 && ((fds[0].revents & POLLIN) == POLLIN))
    {
      bytes = read(this->_port->_master_fd, &c, sizeof(c));
      if (bytes == sizeof(c))
      {
        ZLOG_INFO("Received: '" + ZLOG_CHAR(c) + "'");
        bytes = write(this->_port->_master_fd, &c, sizeof(c));
        if (bytes == sizeof(c))
        {
          ZLOG_INFO("Sent: '" + ZLOG_CHAR(c) + "'");
        }
      }
    }
  }

  return;
}

int
zSerialTest_TtyPortSendRecvChar(void *arg_)
{

  ZLOG_DEBUG("#############################################################");
  ZLOG_DEBUG("# zSerialTest_TtyPortSendRecvChar()");
  ZLOG_DEBUG("#############################################################");

  bool status = false;

  // Create new TTY test port and validate
  TtyTestPort *MyPort = new TtyTestPort;
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
  MyHandler->RegisterObserver(MyObserver);

  // Register port with handler
  MyHandler->RegisterEvent(MyPort);

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
  MyHandler->UnregisterObserver(MyObserver);

  // Cleanup
  delete (MyHandler);
  delete (MyObserver);
  MyPort->Close();
  delete (MyPort);

  // Return success
  return (0);

}

int
zSerialTest_TtyPortSendRecvBuf(void *arg_)
{

  ZLOG_DEBUG("#############################################################");
  ZLOG_DEBUG("# zSerialTest_TtyPortSendRecvBuf()");
  ZLOG_DEBUG("#############################################################");

  int cnt = 0;
  char c = 0;
  bool status = false;
  char exp_buf[100] = { 0 };
  char obs_buf[100] = { 0 };
  ssize_t bytes = 0;

  // Create new TTY test port and validate
  TtyTestPort *MyPort = new TtyTestPort;
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
  MyHandler->RegisterObserver(MyObserver);

  // Register port with handler
  MyHandler->RegisterEvent(MyPort);

  // Open port
  status = MyPort->Open();
  TEST_TRUE(status);

  // Initialize data buffer
  for (cnt = 0; cnt < sizeof(exp_buf); cnt++)
  {
    exp_buf[cnt] = (cnt + 1);
  }

  // Send data
  bytes = MyPort->SendBuf(exp_buf, sizeof(exp_buf));
  TEST_EQ(sizeof(exp_buf), bytes);

  // Verify data was sent
  for (cnt = 0; MyObserver->TxSem.TimedWait(100); cnt++)
  {
    c = MyObserver->TxSem.Front();
    MyObserver->TxSem.Pop();
    TEST_EQ_MSG(exp_buf[cnt], c, (ZLOG_INT(cnt) + ": " + zLog::CharStr(c)));
  }
  TEST_EQ(cnt, sizeof(exp_buf));

  // Verify data was received
  for (cnt = 0; MyObserver->RxSem.TimedWait(100); cnt++)
  {
    c = MyObserver->RxSem.Front();
    MyObserver->RxSem.Pop();
    TEST_EQ_MSG(exp_buf[cnt], c, (ZLOG_INT(cnt) + ": " + zLog::CharStr(c)));
  }
  TEST_EQ(cnt, sizeof(exp_buf));

  // Verify no errors
  status = MyObserver->ErrSem.TryWait();
  TEST_FALSE(status);

  // Unregister observer
  MyHandler->UnregisterObserver(MyObserver);

  // Cleanup
  delete (MyHandler);
  delete (MyObserver);
  MyPort->Close();
  delete (MyPort);

  // Return success
  return (0);

}

