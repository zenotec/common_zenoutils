/*
 * UnitTest.cpp
 *
 *  Created on: May 9, 2014
 *      Author: Kevin Mahoney
 */

#include "zutils/zLog.h"

#include "UnitTest.h"

int
zLog_utest(void);
int
zSem_utest(void);
int
zQueue_utest(void);
int
zData_utest(void);
int
zEvent_utest(void);
int
zTimer_utest(void);
int
zSelect_utest(void);
int
zNode_utest(void);

int
main(int argc, const char **argv)
{
  int ret = 0;

  // Test logging first before continuing
  if ((ret = zLog_utest()) != 0)
  {
    return (ret);
  } // end if

  // Setup logging for remainder of testing
  zUtils::zLog::FileConnector conn("/tmp/zutils-utest.log");
  zUtils::zLog::Log::Instance().RegisterConnector(zUtils::zLog::CRIT, &conn);
  zUtils::zLog::Log::Instance().RegisterConnector(zUtils::zLog::ERROR, &conn);
  zUtils::zLog::Log::Instance().RegisterConnector(zUtils::zLog::WARN, &conn);
  zUtils::zLog::Log::Instance().RegisterConnector(zUtils::zLog::INFO, &conn);
  zUtils::zLog::Log::Instance().RegisterConnector(zUtils::zLog::DBG, &conn);
  zUtils::zLog::Log::Instance().SetMaxLevel(zUtils::zLog::INFO);

  // Test all classes
  ret |= zSem_utest();
  ret |= zEvent_utest();
  ret |= zQueue_utest();
  ret |= zTimer_utest();
  ret |= zData_utest();
  ret |= zNode_utest();

  // Return status
  return (ret);

}
