#include "unistd.h"

#include "zTimerTest.h"

using namespace Test;
using namespace zUtils;

int
zTimerTest_Defaults(void* arg_)
{

  // Create new timer and validate
  zTimer::Timer *MyTimer = new zTimer::Timer;

  // Cleanup
  delete (MyTimer);

  // Return success
  return (0);
}

