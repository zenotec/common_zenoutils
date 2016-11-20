#include <zutils/zLog.h>

#include "zEventTest.h"

int
main(int argc, const char **argv)
{

  // Setup logging for testing
  zUtils::zLog::FileConnector conn("zEventTest.zlog");
  zUtils::zLog::Log::Instance().RegisterConnector(zUtils::zLog::CRIT, &conn);
  zUtils::zLog::Log::Instance().RegisterConnector(zUtils::zLog::ERROR, &conn);
  zUtils::zLog::Log::Instance().RegisterConnector(zUtils::zLog::WARN, &conn);
  zUtils::zLog::Log::Instance().RegisterConnector(zUtils::zLog::INFO, &conn);
  zUtils::zLog::Log::Instance().RegisterConnector(zUtils::zLog::DBG, &conn);
  zUtils::zLog::Log::Instance().SetMaxLevel(zUtils::zLog::DBG);

  // Test all classes
  UTEST_INIT();
  UTEST_TEST(zEventTest_EventDefaults, 0);
  UTEST_TEST(zEventTest_EventNotificationDefaults, 0);
  UTEST_TEST(zEventTest_EventHandlerDefaults, 0);
  UTEST_TEST(zEventTest_EventManagerDefaults, 0);
  UTEST_TEST(zEventTest_EventTest, 0);
  UTEST_TEST(zEventTest_EventHandlerTest, 0);
//  UTEST_TEST(zEventTest_EventManagerTest, 0);
  UTEST_FINI();

}
