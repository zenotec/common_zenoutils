#include <zutils/zLog.h>
#include "zSemTest.h"

using namespace Test;
using namespace zUtils;

int
zSemTest_Semaphore(void* arg_)
{

  ZLOG_DEBUG("#############################################################");
  ZLOG_DEBUG("# zSemTest_Semaphore()");
  ZLOG_DEBUG("#############################################################");

  // Create new semaphore with defaults and validate
  zSem::Semaphore MySemaphore;
  TEST_FALSE(MySemaphore.TryWait());

  // Post and reWait
  TEST_TRUE(MySemaphore.Post());
  TEST_TRUE(MySemaphore.TryWait());

  // Timed Wait
  TEST_FALSE(MySemaphore.TimedWait(100));
  TEST_TRUE(MySemaphore.Post());
  TEST_TRUE(MySemaphore.TimedWait(100));

  // Return success
  return (0);
}
