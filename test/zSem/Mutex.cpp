#include <zutils/zLog.h>
#include "zSemTest.h"

using namespace Test;
using namespace zUtils;

int
zSemTest_Mutex(void* arg_)
{

  ZLOG_DEBUG("#############################################################");
  ZLOG_DEBUG("# zSemTest_Mutex()");
  ZLOG_DEBUG("#############################################################");

  // Create new mutex with defaults and validate
  zSem::Mutex MyMutex;
  TEST_FALSE(MyMutex.TryLock());

  // Unlock and relock
  TEST_TRUE(MyMutex.Unlock());
  TEST_TRUE(MyMutex.TryLock());

  // Timed lock
  TEST_FALSE(MyMutex.TimedLock(100));
  TEST_TRUE(MyMutex.Unlock());
  TEST_TRUE(MyMutex.TimedLock(100));

  // Return success
  return (0);
}

