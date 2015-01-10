#include "unistd.h"

#include "zTimerTest.h"

using namespace Test;
using namespace zUtils;

int
zTimerTest_StartStop(void* arg_)
{
  uint32_t interval = 50000;
  int n = 50;

  // Create new timer and validate
  zTimer::Timer *MyTimer = new zTimer::Timer;

  // Create new timer observer and register with timer
  TimerTestObserver *MyObsvr = new TimerTestObserver;
  TEST_IS_ZERO(MyObsvr->GetCnt());
  MyTimer->Register(MyObsvr);

  // Start the timer
  MyTimer->Start(interval);

  // Delay for n intervals
  usleep((n * interval) + (interval / 2));

  // Stop the timer
  MyTimer->Stop();

  // Validate ((n - 3) < cnt < (n + 1))
  TEST_GT(MyObsvr->GetCnt(), (n - 3));
  TEST_LT(MyObsvr->GetCnt(), (n + 1));

  // Cleanup
  MyTimer->Unregister(MyObsvr);
  delete (MyObsvr);
  delete (MyTimer);

  // Return success
  return (0);
}

