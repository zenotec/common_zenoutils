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

#include <zutils/zDisplay.h>

#include "UnitTest.h"
#include "zDisplayTest.h"

int
main(int argc, const char **argv)
{

  // Setup logging for testing
  zUtils::zLog::FileConnector conn("zDisplayTest.zlog");
  zUtils::zLog::Log::Instance().RegisterConnector(zUtils::zLog::CRIT, &conn);
  zUtils::zLog::Log::Instance().RegisterConnector(zUtils::zLog::ERROR, &conn);
  zUtils::zLog::Log::Instance().RegisterConnector(zUtils::zLog::WARN, &conn);
  zUtils::zLog::Log::Instance().RegisterConnector(zUtils::zLog::INFO, &conn);
  zUtils::zLog::Log::Instance().RegisterConnector(zUtils::zLog::DBG, &conn);
  zUtils::zLog::Log::Instance().SetMaxLevel(zUtils::zLog::DBG);

  // Test all classes
  UTEST_INIT();
  UTEST_TEST(zDisplayTest_DisplayBufferDefaults, 0);
  UTEST_TEST(zDisplayTest_DisplayVarDefaults, 0);
  UTEST_TEST(zDisplayTest_DisplayDefaults, 0);
  UTEST_TEST(zDisplayTest_DisplayBufferUpdate, 0);
  UTEST_TEST(zDisplayTest_DisplayBufferCopy, 0);
  UTEST_TEST(zDisplayTest_DisplayVarGetSet, 0);
  UTEST_TEST(zDisplayTest_DisplayCreateVar, 0);
  UTEST_TEST(zDisplayTest_DisplayUpdate, 0);
  UTEST_FINI();

  // Exit
  exit(0);

}
