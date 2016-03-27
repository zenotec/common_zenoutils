#include "zEventTest.h"

int
zEventTest_EventManagerTest(void* arg_)
{
  bool status = false;

  // Create new event and validate
  zEvent::Event *MyEvent = new zEvent::Event(zEvent::Event::TYPE_TEST);
  TEST_ISNOT_NULL(MyEvent);
  TEST_EQ(zEvent::Event::TYPE_TEST, MyEvent->Type());

  // Register event with handler
  zEvent::EventManager::Instance().RegisterEvent(MyEvent);

  // Create new observer and validate
  TestObserver *MyObserver = new TestObserver;
  TEST_ISNOT_NULL(MyObserver);

  // Register observer with handler
  zEvent::EventManager::Instance().RegisterObserver(MyObserver);
  TEST_FALSE(MyObserver->TimedWait(1));

  // Notify
  MyEvent->Notify(NULL);
  TEST_TRUE(MyObserver->TimedWait(100));

  // Unregister observer with handler
  zEvent::EventManager::Instance().UnregisterObserver(MyObserver);
  TEST_FALSE(MyObserver->TimedWait(1));

  // Unregister event with handler
  zEvent::EventManager::Instance().UnregisterEvent(MyEvent);

  // Cleanup
  delete (MyObserver);
  delete (MyEvent);

  // Return success
  return (0);
}

