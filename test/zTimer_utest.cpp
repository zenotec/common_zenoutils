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

protected:

private:
};

static int
UT_zTimer(int arg_)
{
  // Create new timer and validate
  zTimer MyTimer;

  // Create new timer observer and register with timer
  TimerTestObserver MyObsvr;
  TEST_IS_ZERO(MyObsvr.Cnt);
  MyTimer.Register(&MyObsvr);

  // Start the timer
  MyTimer.Start(1000);

  // Delay
  usleep(10000);
  TEST_ISNOT_ZERO(MyObsvr.Cnt);

  // Stop the timer
  MyTimer.Stop();

  // Cleanup
  MyTimer.Unregister(&MyObsvr);

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

