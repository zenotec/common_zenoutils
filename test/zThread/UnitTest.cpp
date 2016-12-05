#include <list>
#include <mutex>

#include <zutils/zLog.h>
#include <zutils/zSem.h>

#include "zThreadTest.h"
#include "UnitTest.h"

int
main(int argc, const char **argv)
{

  // Setup logging for testing
  zUtils::zLog::FileConnector conn("zThreadTest.zlog");
  zUtils::zLog::Log::Instance().RegisterConnector(zUtils::zLog::CRIT, &conn);
  zUtils::zLog::Log::Instance().RegisterConnector(zUtils::zLog::ERROR, &conn);
  zUtils::zLog::Log::Instance().RegisterConnector(zUtils::zLog::WARN, &conn);
  zUtils::zLog::Log::Instance().RegisterConnector(zUtils::zLog::INFO, &conn);
  zUtils::zLog::Log::Instance().RegisterConnector(zUtils::zLog::DBG, &conn);
  zUtils::zLog::Log::Instance().SetMaxLevel(zUtils::zLog::DBG);

  // Test all classes
  UTEST_INIT();
  UTEST_TEST(zThreadTest_Defaults, 0);
  UTEST_TEST(zThreadTest_RunOnce, 0);
  UTEST_TEST(zThreadTest_RunMultiple, 0);
  UTEST_TEST(zThreadTest_Synchronize, 0);
  UTEST_FINI();

}
