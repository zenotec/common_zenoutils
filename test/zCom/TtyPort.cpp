#include "UnitTest.h"
#include "zComTest.h"

using namespace Test;
using namespace zUtils;

int
zComTest_TtyPortSendRecvChar(void *arg_)
{

  ZLOG_DEBUG( "#############################################################" );
  ZLOG_DEBUG( "# zComTest_TtyPortSendRecvChar()" );
  ZLOG_DEBUG( "#############################################################" );

  bool status = false;
  TtyTestPort *MyPort = new TtyTestPort;
  TEST_ISNOT_ZERO(MyPort);
  TtyTestThread *MyThread = new TtyTestThread(MyPort);
  TEST_ISNOT_ZERO(MyThread);
  zEvent::EventHandler *MyHandler = new zEvent::EventHandler;
  TEST_ISNOT_ZERO(MyHandler);

  MyHandler->RegisterEvent(MyPort);

  // Open port
  status = MyPort->Open(MyPort->Dev());
  TEST_TRUE(status);

  // Send byte
  status = MyPort->SendChar('a');
  TEST_TRUE(status);

  // Wait for byte
  status = MyHandler->TimedWait(100000);
  TEST_TRUE(status);

  // Receive byte back
  char c = 0;
  status = MyPort->RecvChar(&c);
  TEST_TRUE(status);
  TEST_EQ('a', c);

  // Cleanup
  delete(MyThread);
  MyPort->Close();
  MyHandler->UnregisterEvent(MyPort);
  delete (MyHandler);
  delete (MyPort);

  // Return success
  return (0);

}

int
zComTest_TtyPortSendRecvBuf(void *arg_)
{

  ZLOG_DEBUG( "#############################################################" );
  ZLOG_DEBUG( "# zComTest_TtyPortSendRecvBuf()" );
  ZLOG_DEBUG( "#############################################################" );

  bool status = false;
  TtyTestPort *MyPort = new TtyTestPort;
  TEST_ISNOT_ZERO(MyPort);
  TtyTestThread *MyThread = new TtyTestThread(MyPort);
  TEST_ISNOT_ZERO(MyThread);
  zEvent::EventHandler *MyHandler = new zEvent::EventHandler;
  TEST_ISNOT_ZERO(MyHandler);

  char exp_buf[256] = { 0 };
  char obs_buf[256] = { 0 };
  ssize_t bytes = 0;

  MyHandler->RegisterEvent(MyPort);

  // Open port
  status = MyPort->Open(MyPort->Dev());
  TEST_TRUE(status);

  // Send data
  memset(exp_buf, 0x55, 100);
  bytes = MyPort->SendBuf(exp_buf, 100);
  TEST_EQ(100, bytes);

  // Wait for data
  int cnt = 0;
  while (MyHandler->TimedWait(100000))
  {
    ++cnt;
  }
  TEST_EQ(cnt, 100);

  // Receive data
  bytes = MyPort->RecvBuf(obs_buf, 256);
  TEST_EQ(100, bytes);

  // Verify data
  TEST_IS_ZERO(memcmp(exp_buf, obs_buf, bytes));

  // Cleanup
  delete(MyThread);
  MyPort->Close();
  MyHandler->UnregisterEvent(MyPort);
  delete (MyHandler);
  delete (MyPort);

  // Return success
  return (0);

}

int
zComTest_TtyPortSendRecvString(void *arg_)
{

  ZLOG_DEBUG( "#############################################################" );
  ZLOG_DEBUG( "# zComTest_TtyPortSendRecvString()" );
  ZLOG_DEBUG( "#############################################################" );

  bool status = false;
  TtyTestPort *MyPort = new TtyTestPort;
  TEST_ISNOT_ZERO(MyPort);
  TtyTestThread *MyThread = new TtyTestThread(MyPort);
  TEST_ISNOT_ZERO(MyThread);
  zEvent::EventHandler *MyHandler = new zEvent::EventHandler;
  TEST_ISNOT_ZERO(MyHandler);

  MyHandler->RegisterEvent(MyPort);

  // Open port
  status = MyPort->Open(MyPort->Dev());
  TEST_TRUE(status);

  // Send string
  status = MyPort->SendString("test string");
  TEST_TRUE(status);

  // Wait for data
  int cnt = 0;
  while (MyHandler->TimedWait(100000))
  {
    ++cnt;
  }
  TEST_EQ(cnt, 11);

  // Receive string
  std::string str;
  status = MyPort->RecvString(str);
  TEST_TRUE(status);
  TEST_EQ(std::string("test string"), str);

  // Cleanup
  delete(MyThread);
  MyPort->Close();
  MyHandler->UnregisterEvent(MyPort);
  delete (MyHandler);
  delete (MyPort);

  // Return success
  return (0);

}
