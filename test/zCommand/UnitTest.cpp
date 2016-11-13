#include <string>
#include <mutex>
#include <list>
#include <map>

#include <zutils/zLog.h>
#include <zutils/zEvent.h>
#include <zutils/zData.h>
#include <zutils/zQueue.h>
#include <zutils/zSocket.h>
#include <zutils/zMessage.h>

#include "zCommandTest.h"
#include "UnitTest.h"

int
main(int argc, const char **argv)
{

  // Setup logging for testing
  zUtils::zLog::FileConnector conn("zCommandTest.zlog");
  zUtils::zLog::Log::Instance().RegisterConnector(zUtils::zLog::CRIT, &conn);
  zUtils::zLog::Log::Instance().RegisterConnector(zUtils::zLog::ERROR, &conn);
  zUtils::zLog::Log::Instance().RegisterConnector(zUtils::zLog::WARN, &conn);
  zUtils::zLog::Log::Instance().RegisterConnector(zUtils::zLog::INFO, &conn);
  zUtils::zLog::Log::Instance().RegisterConnector(zUtils::zLog::DBG, &conn);
  zUtils::zLog::Log::Instance().SetMaxLevel(zUtils::zLog::DBG);

  // Test all classes
  UTEST_INIT();
  UTEST_TEST(zCommandTest_CommandDefaults, 0);
  UTEST_TEST(zCommandTest_TestCommandDefaults, 0);
  UTEST_TEST(zCommandTest_HandlerDefaults, 0);
  UTEST_TEST(zCommandTest_CommandGetSet, 0);
  UTEST_TEST(zCommandTest_CommandExecute, 0);
  UTEST_TEST(zCommandTest_CommandHandlerNoArg, 0);
  UTEST_TEST(zCommandTest_CommandHandlerSingleArg, 0);
  UTEST_TEST(zCommandTest_CommandHandlerMultiArg, 0);
  UTEST_TEST(zCommandTest_CommandHandlerBadCommand, 0);
  UTEST_FINI();

  // Exit
  exit(0);

}
