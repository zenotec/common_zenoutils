
#include <unistd.h>
#include <signal.h>

#include <mutex>

#include "zTimerTest.h"

using namespace Test;
using namespace zUtils;

int
zTimerTest_StartStop(void* arg_)
{
  ZLOG_DEBUG("#############################################################");
  ZLOG_DEBUG("# zTimerTest_StartStop()");
  ZLOG_DEBUG("#############################################################");

  uint32_t interval = 50000;
  int n = 10;

  // Create new event handler and validate
  zEvent::EventHandler *MyHandler = new zEvent::EventHandler;
  TEST_ISNOT_ZERO(MyHandler);

  // Create new timer and validate
  zTimer::Timer *MyTimer = new zTimer::Timer;
  TEST_ISNOT_ZERO(MyTimer);
  MyHandler->RegisterEvent(MyTimer);

  // Create new timer observer and register with timer
  TimerTestObserver *MyObserver = new TimerTestObserver;
  TEST_ISNOT_ZERO(MyObserver);
  TEST_IS_ZERO(MyObserver->GetCnt());
  MyHandler->RegisterObserver(MyObserver);

  // Start the timer
  MyTimer->Start(interval);

  // Delay for n intervals
  for (int i = 0; i < n; i++)
  {
    usleep(interval);
  }

  // Stop the timer
  MyTimer->Stop();

  usleep((interval));

  // Validate
  TEST_EQ(n, MyObserver->GetCnt());

  // Cleanup
  MyHandler->UnregisterObserver(MyObserver);
  MyHandler->UnregisterEvent(MyTimer);
  delete (MyObserver);
  delete (MyHandler);
  delete (MyTimer);

  // Return success
  return (0);
}

