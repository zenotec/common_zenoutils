
#include <zutils/zLog.h>
#include "zSemTest.h"

using namespace Test;
using namespace zUtils;

int
zSemTest_MutexDefaults(void* arg_)
{

  ZLOG_DEBUG("#############################################################");
  ZLOG_DEBUG("# zSemTest_Defaults()");
  ZLOG_DEBUG("#############################################################");

  // Create new mutex with defaults and validate
  zSem::Mutex MyMutex1(zSem::Mutex::LOCKED);
  TEST_EQ(zSem::Mutex::LOCKED, MyMutex1.State());

  // Create new mutex with defaults and validate
  zSem::Mutex MyMutex2(zSem::Mutex::UNLOCKED);
  TEST_EQ(zSem::Mutex::UNLOCKED, MyMutex2.State());

  // Return success
  return (0);
}

int
zSemTest_SemaphoreDefaults(void* arg_)
{

  ZLOG_DEBUG("#############################################################");
  ZLOG_DEBUG("# zSemTest_SemaphoreDefaults()");
  ZLOG_DEBUG("#############################################################");

  // Create new semaphore with defaults and validate
  zSem::Semaphore MySem1;
  TEST_IS_ZERO(MySem1.Value());

  // Create new semaphore with defaults and validate
  zSem::Semaphore MySem2(10);
  TEST_EQ(10, MySem2.Value());

  // Return success
  return (0);
}

