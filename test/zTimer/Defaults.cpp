
#include <signal.h>

#include <unistd.h>

#include <mutex>

#include "zTimerTest.h"

using namespace Test;
using namespace zUtils;

int
zTimerTest_Defaults(void* arg_)
{
  ZLOG_DEBUG("#############################################################");
  ZLOG_DEBUG("# zTimerTest_Defaults()");
  ZLOG_DEBUG("#############################################################");

  // Create new timer and validate
  zTimer::Timer *MyTimer = new zTimer::Timer;

  // Cleanup
  delete (MyTimer);

  // Return success
  return (0);
}

