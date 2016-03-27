
#include <list>
#include <mutex>

#include <zutils/zLog.h>
#include <zutils/zQueue.h>
#include <zutils/zThread.h>
#include <zutils/zData.h>
#include <zutils/zEvent.h>
#include <zutils/zConfig.h>
#include <zutils/zSerial.h>

#include "UnitTest.h"
#include "zSerialTest.h"

int
main(int argc, const char **argv)
{

  // Setup logging for testing
  zUtils::zLog::FileConnector conn("zSerialTest.zlog");
  zUtils::zLog::Log::Instance().RegisterConnector(zUtils::zLog::CRIT, &conn);
  zUtils::zLog::Log::Instance().RegisterConnector(zUtils::zLog::ERROR, &conn);
  zUtils::zLog::Log::Instance().RegisterConnector(zUtils::zLog::WARN, &conn);
  zUtils::zLog::Log::Instance().RegisterConnector(zUtils::zLog::INFO, &conn);
  zUtils::zLog::Log::Instance().RegisterConnector(zUtils::zLog::DBG, &conn);
  zUtils::zLog::Log::Instance().SetMaxLevel(zUtils::zLog::DBG);

  // Test all classes
  UTEST_INIT();
  UTEST_TEST(zSerialTest_PortDefaults, 0);
  UTEST_TEST(zSerialTest_TtyPortDefaults, 0);
  UTEST_TEST(zSerialTest_PortSendRecvChar, 0);
  UTEST_TEST(zSerialTest_PortSendRecvBuf, 0);
  UTEST_TEST(zSerialTest_PortSendRecvString, 0);
  UTEST_TEST(zSerialTest_TtyPortSendRecvChar, 0);
  UTEST_TEST(zSerialTest_TtyPortSendRecvBuf, 0);
  UTEST_TEST(zSerialTest_TtyPortSendRecvString, 0);
  UTEST_FINI();

  // Exit
  exit(0);

}
