#include "UnitTest.h"
#include "zutils/zTimer.h"

using namespace std;
using namespace Test;
using namespace zUtils;

class TimerTestObserver : public zTimerHandler
{
public:
  TimerTestObserver() :
      Cnt(0)
  {
  }
  ~TimerTestObserver()
  {
  }
  virtual void
  TimerTick()
  {
    this->Cnt++;
  }
  int Cnt;
};

static int
UT_zTimer(int arg_)
{
  uint32_t interval = 50000;
  int n = 50;

  // Create new timer and validate
  zTimer *MyTimer = new zTimer;

  // Create new timer observer and register with timer
  TimerTestObserver *MyObsvr = new TimerTestObserver;
  TEST_IS_ZERO(MyObsvr->Cnt);
  MyTimer->Register(MyObsvr);

  // Start the timer
  MyTimer->Start(interval);

  // Delay for n intervals
  usleep((n * interval) + (interval / 2));

  // Stop the timer
  MyTimer->Start(0);

  // Validate ((n - 2) < cnt < (n + 1))
  TEST_GT(MyObsvr->Cnt, (n - 2));
  TEST_LT(MyObsvr->Cnt, (n + 1));

  // Cleanup
  MyTimer->Unregister(MyObsvr);
  delete (MyObsvr);
  delete (MyTimer);

  // Return success
  return (0);
}

int
zTimer_utest(void)
{
  INIT();
  UTEST( UT_zTimer, 0);
  FINI();
}

