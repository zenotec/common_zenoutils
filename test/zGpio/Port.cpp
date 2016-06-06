#include <string>
#include <list>
#include <mutex>

#include <zutils/zLog.h>
#include <zutils/zSem.h>
#include <zutils/zThread.h>
#include <zutils/zData.h>
#include <zutils/zEvent.h>
#include <zutils/zConfig.h>
#include <zutils/zSwitch.h>

#include <zutils/zGpio.h>

#include "zGpioTest.h"

using namespace zUtils;
using namespace Test;

int
zGpioTest_PortGetSet(void* arg)
{

  ZLOG_DEBUG("#############################################################");
  ZLOG_DEBUG("# zGpioTest_PortGetSet()");
  ZLOG_DEBUG("#############################################################");

  // Create new GPIO Port test configuration
  TestPortConfig *MyConfig = new TestPortConfig(1);
  TEST_ISNOT_NULL(MyConfig);

  // Create new GPIO port and verify
  zGpio::GpioPort *MyPort = new zGpio::GpioPort(*MyConfig);
  TEST_ISNOT_NULL(MyPort);

  // Open port
  TEST_TRUE(MyPort->Open() >= 0);

  // Verify configured state and current state match
  TEST_TRUE(MyPort->State() == MyPort->Get());
  TEST_TRUE(zGpio::GpioPort::STATE_INACTIVE == MyPort->Get());

  // Modify current state and verify
  TEST_TRUE(MyPort->Set(zGpio::GpioPort::STATE_ACTIVE));
  TEST_FALSE(MyPort->State() == MyPort->Get());
  TEST_TRUE(zGpio::GpioPort::STATE_ACTIVE == MyPort->Get());

  // Modify current state back and verify
  TEST_TRUE(MyPort->Set(zGpio::GpioPort::STATE_INACTIVE));
  TEST_TRUE(MyPort->State() == MyPort->Get());
  TEST_TRUE(zGpio::GpioPort::STATE_INACTIVE == MyPort->Get());

  // Close port
  TEST_TRUE(MyPort->Close());

  // Clean up
  delete (MyPort);
  delete (MyConfig);
  return (0);
}
