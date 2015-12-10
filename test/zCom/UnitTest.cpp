#include <zutils/zLog.h>

#include "UnitTest.h"
#include "zComTest.h"

int
main(int argc, const char **argv)
{

  // Setup logging for testing
  zUtils::zLog::FileConnector conn("zComTest.zlog");
  zUtils::zLog::Log::Instance().RegisterConnector(zUtils::zLog::CRIT, &conn);
  zUtils::zLog::Log::Instance().RegisterConnector(zUtils::zLog::ERROR, &conn);
  zUtils::zLog::Log::Instance().RegisterConnector(zUtils::zLog::WARN, &conn);
  zUtils::zLog::Log::Instance().RegisterConnector(zUtils::zLog::INFO, &conn);
  zUtils::zLog::Log::Instance().RegisterConnector(zUtils::zLog::DBG, &conn);
  zUtils::zLog::Log::Instance().SetMaxLevel(zUtils::zLog::DBG);

  // Test all classes
  UTEST_INIT();
  UTEST_TEST(zComTest_PortDefaults, 0);
  UTEST_TEST(zComTest_TtyPortDefaults, 0);
  UTEST_TEST(zComTest_PortSendRecvChar, 0);
  UTEST_TEST(zComTest_PortSendRecvBuf, 0);
  UTEST_TEST(zComTest_PortSendRecvString, 0);
  UTEST_TEST(zComTest_TtyPortSendRecvChar, 0);
  UTEST_TEST(zComTest_TtyPortSendRecvBuf, 0);
  UTEST_TEST(zComTest_TtyPortSendRecvString, 0);
  UTEST_FINI();

  // Exit
  exit(0);

}
