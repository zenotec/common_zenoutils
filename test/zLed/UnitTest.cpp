#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>

#include <zutils/zLog.h>

#include "zLedTest.h"

int
main(int argc, const char **argv)
{
  struct stat st = { 0 };

  // Setup logging for testing
  zUtils::zLog::FileConnector conn("zLedTest.zlog");
  zUtils::zLog::Log::Instance().RegisterConnector(zUtils::zLog::CRIT, &conn);
  zUtils::zLog::Log::Instance().RegisterConnector(zUtils::zLog::ERROR, &conn);
  zUtils::zLog::Log::Instance().RegisterConnector(zUtils::zLog::WARN, &conn);
  zUtils::zLog::Log::Instance().RegisterConnector(zUtils::zLog::INFO, &conn);
  zUtils::zLog::Log::Instance().RegisterConnector(zUtils::zLog::DBG, &conn);
  zUtils::zLog::Log::Instance().SetMaxLevel(zUtils::zLog::INFO);

  // Test all classes
  UTEST_INIT();

  if (stat(TESTDIR.c_str(), &st) == -1)
  {
    mkdir(TESTDIR.c_str(), 0777);
  }

  UTEST_TEST(zLedTest_LedDefaults, 0);
  UTEST_TEST(zLedTest_HandlerDefaults, 0);

  UTEST_TEST(zLedTest_HandlerAddRemove, 0);
  UTEST_TEST(zLedTest_HandlerOnOffToggle, 0);

  if (stat(TESTDIR.c_str(), &st) != -1)
  {
    rmdir(TESTDIR.c_str());
  }

  UTEST_FINI();

}
