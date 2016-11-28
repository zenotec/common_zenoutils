
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
zSerialTest_TtyPortSendRecvChar(void *arg_)
{

  ZLOG_DEBUG( "#############################################################" );
  ZLOG_DEBUG( "# zSerialTest_TtyPortSendRecvChar()" );
  ZLOG_DEBUG( "#############################################################" );

  bool status = false;

  // Create new TTY test port and validate
  TtyTestPort *MyPort = new TtyTestPort;
  TEST_ISNOT_NULL(MyPort);

  // Create new TTY test thread for looping back data and validate
  TtyTestThread *MyThread = new TtyTestThread(MyPort);
  TEST_ISNOT_NULL(MyThread);

  // Create new observer and validate
  TestObserver *MyObserver = new TestObserver;
  TEST_ISNOT_NULL(MyObserver);
  TEST_FALSE(MyObserver->RxSem.TryWait());
  TEST_FALSE(MyObserver->TxSem.TryWait());
  TEST_FALSE(MyObserver->ErrSem.TryWait());

  // Create new handler and validate
  zEvent::EventHandler *MyHandler = new zEvent::EventHandler;
  TEST_ISNOT_NULL(MyHandler);

  // Register observer
  MyHandler->RegisterObserver(MyObserver);

  // Register port with handler
  MyHandler->RegisterEvent(MyPort);

  // Open port
  status = MyPort->Open(MyPort->Dev());
  TEST_TRUE(status);

  // Send byte
  status = MyPort->SendChar('a');
  TEST_TRUE(status);

  // Wait for byte to be sent
  status = MyObserver->TxSem.TimedWait(100000);
  TEST_TRUE(status);
  TEST_EQ('a', MyObserver->TxSem.Front());
  MyObserver->TxSem.Pop();

  // Wait for byte to be received
  status = MyObserver->RxSem.TimedWait(100000);
  TEST_TRUE(status);
  TEST_EQ('a', MyObserver->RxSem.Front());
  MyObserver->RxSem.Pop();

  // Verify no errors
  status = MyObserver->ErrSem.TryWait();
  TEST_FALSE(status);

  // Unregister observer
  MyHandler->UnregisterObserver(MyObserver);

  // Cleanup
  delete(MyHandler);
  delete(MyObserver);
  delete(MyThread);
  MyPort->Close();
  delete (MyPort);

  // Return success
  return (0);

}

int
zSerialTest_TtyPortSendRecvBuf(void *arg_)
{

  ZLOG_DEBUG( "#############################################################" );
  ZLOG_DEBUG( "# zSerialTest_TtyPortSendRecvBuf()" );
  ZLOG_DEBUG( "#############################################################" );

  int cnt = 0;
  bool status = false;
  char exp_buf[256] = { 0 };
  char obs_buf[256] = { 0 };
  ssize_t bytes = 0;

  // Create new TTY test port and validate
  TtyTestPort *MyPort = new TtyTestPort;
  TEST_ISNOT_NULL(MyPort);

  // Create new TTY test thread for looping back data and validate
  TtyTestThread *MyThread = new TtyTestThread(MyPort);
  TEST_ISNOT_NULL(MyThread);

  // Create new observer and validate
  TestObserver *MyObserver = new TestObserver;
  TEST_ISNOT_NULL(MyObserver);
  TEST_FALSE(MyObserver->RxSem.TryWait());
  TEST_FALSE(MyObserver->TxSem.TryWait());
  TEST_FALSE(MyObserver->ErrSem.TryWait());

  // Create new handler and validate
  zEvent::EventHandler *MyHandler = new zEvent::EventHandler;
  TEST_ISNOT_NULL(MyHandler);

  // Register observer
  MyHandler->RegisterObserver(MyObserver);

  // Register port with handler
  MyHandler->RegisterEvent(MyPort);

  // Open port
  status = MyPort->Open(MyPort->Dev());
  TEST_TRUE(status);

  // Send data
  for (cnt = 0; cnt < sizeof(exp_buf); cnt++)
  {
    exp_buf[cnt] = (cnt + 7);
  }
  bytes = MyPort->SendBuf(exp_buf, sizeof(exp_buf));
  TEST_EQ(sizeof(exp_buf), bytes);

  // Verify data was sent
  for (cnt = 0; MyObserver->TxSem.TimedWait(100000); cnt++)
  {
    TEST_EQ(exp_buf[cnt], MyObserver->TxSem.Front());
    MyObserver->TxSem.Pop();
  }
  TEST_EQ(cnt, sizeof(exp_buf));

  // Verify data was received
  for (cnt = 0; MyObserver->RxSem.TimedWait(100000); cnt++)
  {
    TEST_EQ(exp_buf[cnt], MyObserver->RxSem.Front());
    MyObserver->RxSem.Pop();
  }
  TEST_EQ(cnt, sizeof(exp_buf));

  // Verify no errors
  status = MyObserver->ErrSem.TryWait();
  TEST_FALSE(status);

  // Unregister observer
  MyHandler->UnregisterObserver(MyObserver);

  // Cleanup
  delete(MyHandler);
  delete(MyObserver);
  delete(MyThread);
  MyPort->Close();
  delete (MyPort);

  // Return success
  return (0);

}

