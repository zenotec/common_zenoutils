
#include <string>
#include <mutex>
#include <list>
#include <map>

#include <zutils/zLog.h>
#include <zutils/zSem.h>
#include <zutils/zQueue.h>
#include <zutils/zEvent.h>
#include <zutils/zThermo.h>

#include "UnitTest.h"
#include "zThermoTest.h"

int
main(int argc, const char **argv)
{

  // Setup logging for testing
  zUtils::zLog::FileConnector conn("zThermoTest.zlog");
  zUtils::zLog::Log::Instance().RegisterConnector(zUtils::zLog::CRIT, &conn);
  zUtils::zLog::Log::Instance().RegisterConnector(zUtils::zLog::ERROR, &conn);
  zUtils::zLog::Log::Instance().RegisterConnector(zUtils::zLog::WARN, &conn);
  zUtils::zLog::Log::Instance().RegisterConnector(zUtils::zLog::INFO, &conn);
  zUtils::zLog::Log::Instance().RegisterConnector(zUtils::zLog::DBG, &conn);
  zUtils::zLog::Log::Instance().SetMaxLevel(zUtils::zLog::INFO);

  // Test all classes
  UTEST_INIT();
  UTEST_TEST(zThermoTest_Defaults, 0);
  UTEST_FINI();

  // Exit
  exit(0);

}
