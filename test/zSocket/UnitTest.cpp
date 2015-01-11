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
  zUtils::zLog::Log::Instance().SetMaxLevel(zUtils::zLog::INFO);

  // Test all classes
  UTEST_INIT();
  UTEST_TEST(zSocketTest_SocketBufferDefaults, 0);
  UTEST_TEST(zSocketTest_SocketAddrDefaults, 0);
  UTEST_TEST(zSocketTest_SocketDefaults, 0);
  UTEST_TEST(zSocketTest_ListenerDefaults, 0);
  UTEST_TEST(zSocketTest_SocketBufferCompare, 0);
  UTEST_TEST(zSocketTest_SocketAddrGet, 0);
  UTEST_TEST(zSocketTest_SocketAddrSet, 0);
  UTEST_TEST(zSocketTest_SocketAddrCompare, 0);
  UTEST_TEST(zSocketTest_SocketListener, 0);
  UTEST_FINI();

  // Exit
  exit(0);

}
