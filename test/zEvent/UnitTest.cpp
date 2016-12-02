#include <zutils/zLog.h>

#include "zEventTest.h"

int
main(int argc, const char **argv)
{

  UTEST_INIT();

  // Setup logging for testing
  zUtils::zLog::FileConnector conn("zEventTest.zlog");
  zUtils::zLog::Log::Instance().RegisterConnector(zUtils::zLog::CRIT, &conn);
  zUtils::zLog::Log::Instance().RegisterConnector(zUtils::zLog::ERROR, &conn);
  zUtils::zLog::Log::Instance().RegisterConnector(zUtils::zLog::WARN, &conn);
  zUtils::zLog::Log::Instance().RegisterConnector(zUtils::zLog::INFO, &conn);
  zUtils::zLog::Log::Instance().RegisterConnector(zUtils::zLog::DBG, &conn);
  zUtils::zLog::Log::Instance().SetMaxLevel(zUtils::zLog::DBG);

  // Test all classes
  UTEST_TEST(zEventTest_EventDefaults, 0);
  UTEST_TEST(zEventTest_EventNotificationDefaults, 0);
  UTEST_TEST(zEventTest_EventHandlerDefaults, 0);
  UTEST_TEST(zEventTest_EventManagerDefaults, 0);
  UTEST_TEST(zEventTest_EventTest, 0);
  UTEST_TEST(zEventTest_EventHandlerTest, 0);
//  UTEST_TEST(zEventTest_EventManagerTest, 0);

  zUtils::zLog::Log::Instance().UnregisterConnector(zUtils::zLog::CRIT);
  zUtils::zLog::Log::Instance().UnregisterConnector(zUtils::zLog::ERROR);
  zUtils::zLog::Log::Instance().UnregisterConnector(zUtils::zLog::WARN);
  zUtils::zLog::Log::Instance().UnregisterConnector(zUtils::zLog::INFO);
  zUtils::zLog::Log::Instance().UnregisterConnector(zUtils::zLog::DBG);

  UTEST_FINI();

}
