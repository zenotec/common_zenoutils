#include "zEventTest.h"

int
zEventTest_Defaults(void * arg_)
{
  // Create new event and validate
  zEvent::Event MyEvent;

  // Return success
  return (0);
}

int
zEventTest_EventList(void* arg_)
{
  // Create new event and validate
  TestEvent MyEvent;

  // Create new event list and validate
  zEvent::EventList MyList;
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

