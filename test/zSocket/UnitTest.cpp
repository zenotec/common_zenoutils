#include "zutils/zLog.h"
#include "zSocketTest.h"
#include "UnitTest.h"

int
main(int argc, const char **argv)
{

  // Setup logging for testing
  zUtils::zLog::FileConnector conn("zSocketTest.zlog");
  zUtils::zLog::Log::Instance().RegisterConnector(zUtils::zLog::CRIT, &conn);
  zUtils::zLog::Log::Instance().RegisterConnector(zUtils::zLog::ERROR, &conn);
  zUtils::zLog::Log::Instance().RegisterConnector(zUtils::zLog::WARN, &conn);
  zUtils::zLog::Log::Instance().RegisterConnector(zUtils::zLog::INFO, &conn);
  zUtils::zLog::Log::Instance().RegisterConnector(zUtils::zLog::DBG, &conn);
  zUtils::zLog::Log::Instance().SetMaxLevel(zUtils::zLog::DBG);

  // Test all classes
  UTEST_INIT();

  UTEST_TEST(zSocketTest_BufferDefaults, 0);
  UTEST_TEST(zSocketTest_AddressDefaults, 0);
  UTEST_TEST(zSocketTest_ObserverDefaults, 0);
  UTEST_TEST(zSocketTest_SocketDefaults, 0);
  UTEST_TEST(zSocketTest_HandlerDefaults, 0);

  UTEST_TEST(zSocketTest_BufferCompare, 0);
  UTEST_TEST(zSocketTest_BufferString, 0);

  UTEST_TEST(zSocketTest_AddressGetSet, 0);
  UTEST_TEST(zSocketTest_AddressCompare, 0);

  UTEST_TEST(zSocketTest_InetAddressGet, 0);
  UTEST_TEST(zSocketTest_InetAddressSet, 0);
  UTEST_TEST(zSocketTest_InetAddressCompare, 0);

  UTEST_TEST(zSocketTest_InetSocketDefault, 0);
  UTEST_TEST(zSocketTest_InetSocketSendReceiveLoop, 0);
  UTEST_TEST(zSocketTest_InetSocketSendReceiveSock2Sock, 0);
  UTEST_TEST(zSocketTest_InetSocketObserver, 0);

  UTEST_TEST(zSocketTest_HandlerRegister, 0);
  UTEST_TEST(zSocketTest_HandlerStartStop, 0);
  UTEST_TEST(zSocketTest_HandlerSendRecv, 0);

  UTEST_FINI();

}
