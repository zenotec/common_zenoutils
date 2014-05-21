#include "UnitTest.h"
#include "zutils/zEvent.h"

using namespace Test;
using namespace zUtils;

class TestEvent : public zEvent
{
public:
  void
  Notify()
  {
    this->_notify();
  }

protected:

private:

};

static int
UT_zEvent_Event(int arg_)
{
  // Create new event and validate
  zEvent MyEvent;

  // Return success
  return (0);
}

static int
UT_zEvent_EventList(int arg_)
{
  // Create new event and validate
  TestEvent MyEvent;

  // Create new event list and validate
  zEventList MyList;
  TEST_FALSE(MyList.Wait(1));

  // Register event with list
  MyList.Register(MyEvent);
  TEST_FALSE(MyList.Wait(1));

  // Notify
  MyEvent.Notify();
  TEST_TRUE(MyList.Wait(1));

  // Cleanup
  MyList.Unregister(MyEvent);
  TEST_FALSE(MyList.Wait(1));

  // Return success
  return (0);
}

int
zEvent_utest(void)
{
  INIT();
  UTEST( UT_zEvent_Event, 0);
  UTEST( UT_zEvent_EventList, 0);
  FINI();
}

