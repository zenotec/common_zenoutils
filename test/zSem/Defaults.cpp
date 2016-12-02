
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
  zSem::Mutex MyMutex1;
  TEST_FALSE(MyMutex1.TryLock());

  // Create new mutex with defaults and validate
  zSem::Mutex MyMutex2(zSem::Mutex::LOCKED);
  TEST_FALSE(MyMutex2.TryLock());

  // Create new mutex with defaults and validate
  zSem::Mutex MyMutex3(zSem::Mutex::UNLOCKED);
  TEST_TRUE(MyMutex3.TryLock());

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
  TEST_FALSE(MySem1.TryWait());

  // Create new semaphore with defaults and validate
  zSem::Semaphore MySem2(0);
  TEST_FALSE(MySem2.TryWait());

  // Create new semaphore with defaults and validate
  zSem::Semaphore MySem3(1);
  TEST_TRUE(MySem3.TryWait());

  // Return success
  return (0);
}

