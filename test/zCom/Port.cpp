#include "UnitTest.h"
#include "zComTest.h"

using namespace Test;
using namespace zUtils;

int
zComTest_PortSendRecvChar(void *arg_)
{

  ZLOG_DEBUG( "#############################################################" );
  ZLOG_DEBUG( "# zComTest_PortSendRecvChar()" );
  ZLOG_DEBUG( "#############################################################" );

  bool status = false;
  TestPort *MyPort = new TestPort;
  TEST_ISNOT_ZERO(MyPort);
  zEvent::EventHandler *MyHandler = new zEvent::EventHandler;
  TEST_ISNOT_ZERO(MyHandler);

  MyHandler->RegisterEvent(MyPort);

  // Open port
  status = MyPort->Open("/dev/MyPort");
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
  MyPort->Close();
  MyHandler->UnregisterEvent(MyPort);
  delete (MyHandler);
  delete (MyPort);

  // Return success
  return (0);

}

int
zComTest_PortSendRecvBuf(void *arg_)
{

  ZLOG_DEBUG( "#############################################################" );
  ZLOG_DEBUG( "# zComTest_PortSendRecvBuf()" );
  ZLOG_DEBUG( "#############################################################" );

  bool status = false;
  TestPort *MyPort = new TestPort;
  TEST_ISNOT_ZERO(MyPort);
  zEvent::EventHandler *MyHandler = new zEvent::EventHandler;
  TEST_ISNOT_ZERO(MyHandler);

  char exp_buf[256] = { 0 };
  char obs_buf[256] = { 0 };
  ssize_t bytes = 0;

  MyHandler->RegisterEvent(MyPort);

  // Open port
  status = MyPort->Open("/dev/MyPort");
  TEST_TRUE(status);

  // Send byte
  memset(exp_buf, 0x55, 100);
  bytes = MyPort->SendBuf(exp_buf, 100);
  TEST_EQ(100, bytes);

  // Wait for byte
  status = MyHandler->TimedWait(100000);
  TEST_TRUE(status);

  // Receive byte back
  bytes = MyPort->RecvBuf(obs_buf, 256);
  TEST_EQ(100, bytes);

  // Verify
  TEST_IS_ZERO(memcmp(exp_buf, obs_buf, bytes));

  // Cleanup
  MyPort->Close();
  MyHandler->UnregisterEvent(MyPort);
  delete (MyHandler);
  delete (MyPort);

  // Return success
  return (0);

}

int
zComTest_PortSendRecvString(void *arg_)
{

  ZLOG_DEBUG( "#############################################################" );
  ZLOG_DEBUG( "# zComTest_PortSendRecvString()" );
  ZLOG_DEBUG( "#############################################################" );

  bool status = false;
  TestPort *MyPort = new TestPort;
  TEST_ISNOT_ZERO(MyPort);
  zEvent::EventHandler *MyHandler = new zEvent::EventHandler;
  TEST_ISNOT_ZERO(MyHandler);

  MyHandler->RegisterEvent(MyPort);

  // Open port
  status = MyPort->Open("/dev/MyPort");
  TEST_TRUE(status);

  // Send byte
  status = MyPort->SendString("test string");
  TEST_TRUE(status);

  // Wait for byte
  status = MyHandler->TimedWait(100000);
  TEST_TRUE(status);

  // Receive byte back
  std::string str;
  status = MyPort->RecvString(str);
  TEST_TRUE(status);
  TEST_EQ(std::string("test string"), str);

  // Cleanup
  MyPort->Close();
  MyHandler->UnregisterEvent(MyPort);
  delete (MyHandler);
  delete (MyPort);

  // Return success
  return (0);

}
