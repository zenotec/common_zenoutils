
#include "zutils/zLog.h"
#include "zDataTest.h"
#include "UnitTest.h"

int
main(int argc, const char **argv)
{

  // Setup logging for testing
  zUtils::zLog::FileConnector conn("zDataTest.log");
  zUtils::zLog::Log::Instance().RegisterConnector(zUtils::zLog::CRIT, &conn);
  zUtils::zLog::Log::Instance().RegisterConnector(zUtils::zLog::ERROR, &conn);
  zUtils::zLog::Log::Instance().RegisterConnector(zUtils::zLog::WARN, &conn);
  zUtils::zLog::Log::Instance().RegisterConnector(zUtils::zLog::INFO, &conn);
  zUtils::zLog::Log::Instance().RegisterConnector(zUtils::zLog::DBG, &conn);
  zUtils::zLog::Log::Instance().SetMaxLevel(zUtils::zLog::INFO);

  // Test all classes
  UTEST_INIT();
  UTEST_TEST( zDataTest_Defaults, 0);
  UTEST_TEST( zDataTest_SetValueSingle, 0);
  UTEST_TEST( zDataTest_SetValueMultiple, 0);
  UTEST_TEST( zDataTest_GetChildSingle, 0);
  UTEST_TEST( zDataTest_GetChildMultiple, 0);
  UTEST_TEST( zDataTest_PutChildSingle, 0);
  UTEST_TEST( zDataTest_PutChildMultiple, 0);
  UTEST_TEST( zDataTest_JsonSimple, 0);
  UTEST_TEST( zDataTest_XmlSimple, 0);
  UTEST_FINI();

  // Exit
  exit( 0 );


}
