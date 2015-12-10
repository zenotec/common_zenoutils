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
zEventTest_EventHandler(void* arg_)
{
  // Create new event and validate
  zEvent::Event *MyEvent = new zEvent::Event;

  // Create new event list and validate
  zEvent::EventHandler MyList;
  TEST_FALSE(MyList.TimedWait(1));

  // Register event with list
  MyList.RegisterEvent(MyEvent);
  TEST_FALSE(MyList.TimedWait(1));

  // Notify
  MyEvent->Notify();
  TEST_TRUE(MyList.TimedWait(1));

  // Unregister
  MyList.UnregisterEvent(MyEvent);
  TEST_FALSE(MyList.TimedWait(1));

  // Cleanup
  delete (MyEvent);

  // Return success
  return (0);
}

