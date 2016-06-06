
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
  TEST_ISNOT_ZERO(MyPort);

  // Create new TTY test thread for looping back data and validate
  TtyTestThread *MyThread = new TtyTestThread(MyPort);
  TEST_ISNOT_ZERO(MyThread);

  // Create new observer and validate
  TestObserver *MyObserver = new TestObserver;
  TEST_ISNOT_ZERO(MyObserver);
  TEST_FALSE(MyObserver->RxSem.TryWait());
  TEST_FALSE(MyObserver->TxSem.TryWait());
  TEST_FALSE(MyObserver->ErrSem.TryWait());

  // Create new handler and validate
  zSerial::SerialHandler *MyHandler = new zSerial::SerialHandler;
  TEST_ISNOT_ZERO(MyHandler);

  // Register observer
  MyHandler->RegisterObserver(MyObserver);

  // Register port with handler
  MyHandler->Add(MyPort);

  // Open port
  status = MyPort->Open(MyPort->Dev());
  TEST_TRUE(status);

  // Send byte
  status = MyPort->SendChar('a');
  TEST_TRUE(status);

  // Wait for byte to be sent
  status = MyObserver->TxSem.TimedWait(100000);
  TEST_TRUE(status);

  // Wait for byte to be received
  status = MyObserver->RxSem.TimedWait(100000);
  TEST_TRUE(status);

  // Verify no errors
  status = MyObserver->ErrSem.TryWait();
  TEST_FALSE(status);

  // Receive byte back
  char c = 0;
  status = MyPort->RecvChar(&c);
  TEST_TRUE(status);
  TEST_EQ('a', c);

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
  TEST_ISNOT_ZERO(MyPort);

  // Create new TTY test thread for looping back data and validate
  TtyTestThread *MyThread = new TtyTestThread(MyPort);
  TEST_ISNOT_ZERO(MyThread);

  // Create new observer and validate
  TestObserver *MyObserver = new TestObserver;
  TEST_ISNOT_ZERO(MyObserver);
  TEST_FALSE(MyObserver->RxSem.TryWait());
  TEST_FALSE(MyObserver->TxSem.TryWait());
  TEST_FALSE(MyObserver->ErrSem.TryWait());

  // Create new handler and validate
  zSerial::SerialHandler *MyHandler = new zSerial::SerialHandler;
  TEST_ISNOT_ZERO(MyHandler);

  // Register observer
  MyHandler->RegisterObserver(MyObserver);

  // Register port with handler
  MyHandler->Add(MyPort);

  // Open port
  status = MyPort->Open(MyPort->Dev());
  TEST_TRUE(status);

  // Send data
  memset(exp_buf, 0x55, 100);
  bytes = MyPort->SendBuf(exp_buf, 100);
  TEST_EQ(100, bytes);

  cnt = 0;
  // Verify data was sent
  while (MyObserver->TxSem.TimedWait(100000))
  {
    ++cnt;
  }
  TEST_EQ(cnt, 100);

  cnt = 0;
  // Verify data was received
  while (MyObserver->RxSem.TimedWait(100000))
  {
    ++cnt;
  }
  TEST_EQ(cnt, 100);

  // Verify no errors
  status = MyObserver->ErrSem.TryWait();
  TEST_FALSE(status);

  // Receive data
  bytes = MyPort->RecvBuf(obs_buf, 256);
  TEST_EQ(100, bytes);

  // Verify data
  TEST_IS_ZERO(memcmp(exp_buf, obs_buf, bytes));

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
zSerialTest_TtyPortSendRecvString(void *arg_)
{

  ZLOG_DEBUG( "#############################################################" );
  ZLOG_DEBUG( "# zSerialTest_TtyPortSendRecvString()" );
  ZLOG_DEBUG( "#############################################################" );

  int cnt = 0;
  bool status = false;
  zEvent::Event *MyEvent = NULL;

  // Create new TTY test port and validate
  TtyTestPort *MyPort = new TtyTestPort;
  TEST_ISNOT_ZERO(MyPort);

  // Create new TTY test thread for looping back data and validate
  TtyTestThread *MyThread = new TtyTestThread(MyPort);
  TEST_ISNOT_ZERO(MyThread);

  // Create new observer and validate
  TestObserver *MyObserver = new TestObserver;
  TEST_ISNOT_ZERO(MyObserver);
  TEST_FALSE(MyObserver->RxSem.TryWait());
  TEST_FALSE(MyObserver->TxSem.TryWait());
  TEST_FALSE(MyObserver->ErrSem.TryWait());

  // Create new handler and validate
  zSerial::SerialHandler *MyHandler = new zSerial::SerialHandler;
  TEST_ISNOT_ZERO(MyHandler);

  // Register observer
  MyHandler->RegisterObserver(MyObserver);

  // Register port with handler
  MyHandler->Add(MyPort);

  // Open port
  status = MyPort->Open(MyPort->Dev());
  TEST_TRUE(status);

  // Send string
  status = MyPort->SendString("test string\n");
  TEST_TRUE(status);

  cnt = 0;
  // Verify data was sent
  while (MyObserver->TxSem.TimedWait(100000))
  {
    ++cnt;
  }
  TEST_EQ(cnt, 12);

  cnt = 0;
  // Verify data was received
  while (MyObserver->RxSem.TimedWait(100000))
  {
    ++cnt;
  }
  TEST_EQ(cnt, 12);

  // Verify no errors
  status = MyObserver->ErrSem.TryWait();
  TEST_FALSE(status);

  // Receive string
  std::string str;
  status = MyPort->RecvString(str);
  TEST_TRUE(status);
  TEST_EQ(std::string("test string"), str);

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
