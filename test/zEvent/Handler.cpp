#include "zEventTest.h"

int
zEventTest_EventHandlerTest(void* arg_)
{
  // Create new event and validate
  zEvent::Event *MyEvent = new zEvent::Event(zEvent::Event::TYPE_TEST);
  TEST_ISNOT_NULL(MyEvent);
  TEST_EQ(zEvent::Event::TYPE_TEST, MyEvent->GetType());
  TEST_EQ(0, MyEvent->GetId());

  // Create new event handler and validate
  zEvent::EventHandler *MyHandler = new zEvent::EventHandler;
  TEST_ISNOT_NULL(MyHandler);
  TEST_TRUE(MyHandler->Empty());
  TEST_IS_ZERO(MyHandler->Size());
  TEST_FALSE(MyHandler->TimedWait(1));

  // Register event with handler
  MyHandler->RegisterEvent(MyEvent);
  TEST_FALSE(MyHandler->TimedWait(1));

  // Create new observer and validate
  TestObserver *MyObserver = new TestObserver;
  TEST_ISNOT_NULL(MyObserver);

  // Register observer with handler
  MyHandler->RegisterObserver(MyObserver);
  TEST_FALSE(MyObserver->TimedWait(1));

  // Notify
  MyEvent->Notify(NULL);
  TEST_TRUE(MyHandler->TimedWait(100));
  TEST_TRUE(MyObserver->TimedWait(100));
  TEST_FALSE(MyHandler->Empty());
  TEST_EQ(1, MyHandler->Size());
  TEST_TRUE(MyEvent == MyHandler->GetEvent());

  // Unregister observer with handler
  MyHandler->UnregisterObserver(MyObserver);
  TEST_FALSE(MyObserver->TimedWait(1));

  // Unregister event with handler
  MyHandler->UnregisterEvent(MyEvent);
  TEST_FALSE(MyHandler->TimedWait(1));

  // Cleanup
  delete (MyObserver);
  delete (MyHandler);
  delete (MyEvent);

  // Return success
  return (0);
}

