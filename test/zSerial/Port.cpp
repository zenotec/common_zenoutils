
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

  ZLOG_DEBUG( "#############################################################" );
  ZLOG_DEBUG( "# zComTest_PortSendRecvChar()" );
  ZLOG_DEBUG( "#############################################################" );

  zEvent::Event *MyEvent = NULL;
  bool status = false;

  // Create new port and validate
  TestPort *MyPort = new TestPort;
  TEST_ISNOT_ZERO(MyPort);

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

  // Open port
  status = MyPort->Open("/dev/MyPort");
  TEST_TRUE(status);

  // Send byte
  status = MyPort->SendChar('a');
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
  MyPort->Close();
  delete (MyPort);

  // Return success
  return (0);

}

int
zSerialTest_PortSendRecvBuf(void *arg_)
{

  ZLOG_DEBUG( "#############################################################" );
  ZLOG_DEBUG( "# zComTest_PortSendRecvBuf()" );
  ZLOG_DEBUG( "#############################################################" );

  int cnt = 0;
  bool status = false;
  char exp_buf[256] = { 0 };
  char obs_buf[256] = { 0 };
  ssize_t bytes = 0;
  zEvent::Event *MyEvent = NULL;

  // Create new test port and validate
  TestPort *MyPort = new TestPort;
  TEST_ISNOT_ZERO(MyPort);

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

  // Open port
  status = MyPort->Open("/dev/MyPort");
  TEST_TRUE(status);

  // Send data
  memset(exp_buf, 0x55, 100);
  bytes = MyPort->SendBuf(exp_buf, 100);
  TEST_EQ(100, bytes);

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

  // Receive data and validate
  bytes = MyPort->RecvBuf(obs_buf, 256);
  TEST_EQ(100, bytes);
  TEST_IS_ZERO(memcmp(exp_buf, obs_buf, bytes));

  // Unregister observer
  MyHandler->UnregisterObserver(MyObserver);

  // Cleanup
  delete(MyHandler);
  delete(MyObserver);
  MyPort->Close();
  delete (MyPort);

  // Return success
  return (0);

}

int
zSerialTest_PortSendRecvString(void *arg_)
{

  ZLOG_DEBUG( "#############################################################" );
  ZLOG_DEBUG( "# zComTest_PortSendRecvString()" );
  ZLOG_DEBUG( "#############################################################" );

  zEvent::Event *MyEvent = NULL;
  int cnt = 0;
  bool status = false;

  // Create new port and validate
  TestPort *MyPort = new TestPort;
  TEST_ISNOT_ZERO(MyPort);

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

  // Open port
  status = MyPort->Open("/dev/MyPort");
  TEST_TRUE(status);

  // Send string
  status = MyPort->SendString("test string\n");
  TEST_TRUE(status);

  // Verify data was received
  cnt = 0;
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
  MyPort->Close();
  delete (MyPort);

  // Return success
  return (0);

}
