#include "zutils/zLog.h"
#include "zOptTest.h"

int
main(int argc, const char **argv)
{

  // Setup logging for testing
  zUtils::zLog::FileConnector conn("zOptTest.zlog");
  zUtils::zLog::Log::Instance().RegisterConnector(zUtils::zLog::CRIT, &conn);
  zUtils::zLog::Log::Instance().RegisterConnector(zUtils::zLog::ERROR, &conn);
  zUtils::zLog::Log::Instance().RegisterConnector(zUtils::zLog::WARN, &conn);
  zUtils::zLog::Log::Instance().RegisterConnector(zUtils::zLog::INFO, &conn);
  zUtils::zLog::Log::Instance().RegisterConnector(zUtils::zLog::DBG, &conn);
  zUtils::zLog::Log::Instance().SetMaxLevel(zUtils::zLog::INFO);

  // Test all classes
  UTEST_INIT();

  UTEST_TEST(zOptTest_OptionDefaults, 0);
  UTEST_TEST(zOptTest_TableDefaults, 0);

  UTEST_TEST(zOptTest_OptionString, 0);
  UTEST_TEST(zOptTest_OptionInt, 0);
  UTEST_TEST(zOptTest_OptionUint, 0);
  UTEST_TEST(zOptTest_OptionDouble, 0);

  UTEST_TEST(zOptTest_ParseSimpleShort, 0);
  UTEST_TEST(zOptTest_ParseSimpleLong, 0);

  UTEST_FINI();

  // Exit
  exit(0);

}
