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
  uint32_t interval = 10000;

  // Create new timer and validate
  zTimer *MyTimer = new zTimer;

  // Create new timer observer and register with timer
  TimerTestObserver *MyObsvr = new TimerTestObserver;
  TEST_IS_ZERO(MyObsvr->Cnt);
  MyTimer->Register(MyObsvr);

  // Start the timer
  MyTimer->Start(interval);

  // Delay for n intervals
  usleep((10 * interval) + (interval / 2));

  // Stop the timer
  MyTimer->Stop();

  // Validate
  TEST_EQ(10, MyObsvr->Cnt);

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

