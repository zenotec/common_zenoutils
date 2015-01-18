#include <zutils/zLog.h>
#include "zMessageTest.h"

int
main(int argc, const char **argv)
{

  // Setup logging for testing
  zUtils::zLog::FileConnector conn("zMessageTest.zlog");
  zUtils::zLog::Log::Instance().RegisterConnector(zUtils::zLog::CRIT, &conn);
  zUtils::zLog::Log::Instance().RegisterConnector(zUtils::zLog::ERROR, &conn);
  zUtils::zLog::Log::Instance().RegisterConnector(zUtils::zLog::WARN, &conn);
  zUtils::zLog::Log::Instance().RegisterConnector(zUtils::zLog::INFO, &conn);
  zUtils::zLog::Log::Instance().RegisterConnector(zUtils::zLog::DBG, &conn);
  zUtils::zLog::Log::Instance().SetMaxLevel(zUtils::zLog::DBG);

  // Test all classes
  UTEST_INIT();

  UTEST_TEST(zMessageTest_Defaults, 0);

  UTEST_TEST(zMessageTest_FactoryError, 0);
  UTEST_TEST(zMessageTest_FactoryAuth, 0);
  UTEST_TEST(zMessageTest_FactoryHello, 0);
  UTEST_TEST(zMessageTest_FactoryAck, 0);
  UTEST_TEST(zMessageTest_FactoryBye, 0);
  UTEST_TEST(zMessageTest_FactoryCfg, 0);
  UTEST_TEST(zMessageTest_FactoryCmd, 0);
  UTEST_TEST(zMessageTest_FactoryData, 0);

  UTEST_TEST(zMessageTest_GetSet, 0);

  UTEST_TEST(zMessageTest_Handler, 0);

  UTEST_FINI();

}
