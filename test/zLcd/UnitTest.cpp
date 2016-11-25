#include <signal.h>

#include <string>
#include <mutex>
#include <list>
#include <vector>
#include <map>

#include <zutils/zLog.h>
#include <zutils/zSem.h>
#include <zutils/zQueue.h>
#include <zutils/zData.h>
#include <zutils/zEvent.h>
#include <zutils/zTimer.h>
#include <zutils/zSocket.h>
#include <zutils/zMessage.h>
#include <zutils/zCommand.h>

#include <zutils/zLcd.h>

#include "UnitTest.h"
#include "zLcdTest.h"

int
main(int argc, const char **argv)
{

  // Setup logging for testing
  zUtils::zLog::FileConnector conn("zLcdTest.zlog");
  zUtils::zLog::Log::Instance().RegisterConnector(zUtils::zLog::CRIT, &conn);
  zUtils::zLog::Log::Instance().RegisterConnector(zUtils::zLog::ERROR, &conn);
  zUtils::zLog::Log::Instance().RegisterConnector(zUtils::zLog::WARN, &conn);
  zUtils::zLog::Log::Instance().RegisterConnector(zUtils::zLog::INFO, &conn);
  zUtils::zLog::Log::Instance().RegisterConnector(zUtils::zLog::DBG, &conn);
  zUtils::zLog::Log::Instance().SetMaxLevel(zUtils::zLog::DBG);

  // Test all classes
  UTEST_INIT();
  UTEST_TEST(zLcdTest_LcdVarDefaults, 0);
  UTEST_TEST(zLcdTest_LcdDefaults, 0);
  UTEST_TEST(zLcdTest_LcdVarGetSet, 0);
  UTEST_TEST(zLcdTest_LcdCreateVar, 0);
  UTEST_TEST(zLcdTest_LcdUpdate, 0);
  UTEST_FINI();

  // Exit
  exit(0);

}
