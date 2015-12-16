#include "zEventTest.h"

int
zEventTest_EventManagerTest(void* arg_)
{
  bool status = false;

  // Create new event and validate
  zEvent::Event *MyEvent = new zEvent::Event(zEvent::Event::TYPE_TEST);
  TEST_ISNOT_NULL(MyEvent);
  TEST_EQ(zEvent::Event::TYPE_TEST, MyEvent->GetType());
  TEST_EQ(0, MyEvent->GetId());

  // Register event with handler
  status = zEvent::EventManager::GetInstance().RegisterEvent(MyEvent);
  TEST_TRUE(status);

  // Create new observer and validate
  TestObserver *MyObserver = new TestObserver;
  TEST_ISNOT_NULL(MyObserver);

  // Register observer with handler
  status = zEvent::EventManager::GetInstance().RegisterObserver(zEvent::Event::TYPE_TEST, MyObserver);
  TEST_TRUE(status);
  TEST_FALSE(MyObserver->TimedWait(1));

  // Notify
  MyEvent->Notify(NULL);
  TEST_TRUE(MyObserver->TimedWait(100));

  // Unregister observer with handler
  status = zEvent::EventManager::GetInstance().UnregisterObserver(zEvent::Event::TYPE_TEST, MyObserver);
  TEST_TRUE(status);
  TEST_FALSE(MyObserver->TimedWait(1));

  // Unregister event with handler
  status = zEvent::EventManager::GetInstance().UnregisterEvent(MyEvent);
  TEST_TRUE(status);

  // Cleanup
  delete (MyObserver);
  delete (MyEvent);

  // Return success
  return (0);
}

