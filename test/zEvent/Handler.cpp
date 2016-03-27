#include "zEventTest.h"

int
zEventTest_EventHandlerTest(void* arg_)
{
  ZLOG_DEBUG("#############################################################");
  ZLOG_DEBUG("# zEventTest_EventHandlerTest()");
  ZLOG_DEBUG("#############################################################");

  // Create new event and validate
  zEvent::Event *MyEvent = new zEvent::Event(zEvent::Event::TYPE_TEST);
  TEST_ISNOT_NULL(MyEvent);
  TEST_EQ(zEvent::Event::TYPE_TEST, MyEvent->Type());

  // Create new event handler and validate
  zEvent::EventHandler *MyHandler = new zEvent::EventHandler;
  TEST_ISNOT_NULL(MyHandler);

  // Register event with handler
  MyHandler->RegisterEvent(MyEvent);

  // Create new observer and validate
  TestObserver *MyObserver = new TestObserver;
  TEST_ISNOT_NULL(MyObserver);
  TEST_FALSE(MyObserver->TryWait());
  TEST_IS_ZERO(MyObserver->Size());
  TEST_TRUE(MyObserver->Empty());

  // Register observer with handler
  MyHandler->RegisterObserver(MyObserver);
  TEST_FALSE(MyObserver->TryWait());
  TEST_IS_ZERO(MyObserver->Size());
  TEST_TRUE(MyObserver->Empty());

  // Notify
  zEvent::EventNotification MyNotification;
  MyEvent->Notify(&MyNotification);
  TEST_TRUE(MyObserver->TryWait());
  TEST_EQ(MyObserver->Size(), 1);
  TEST_FALSE(MyObserver->Empty());
  TEST_TRUE(MyObserver->Front().Type() == zEvent::Event::TYPE_TEST);
  MyObserver->Pop();

  // Unregister observer with handler
  MyHandler->UnregisterObserver(MyObserver);
  TEST_FALSE(MyObserver->TryWait());
  TEST_IS_ZERO(MyObserver->Size());
  TEST_TRUE(MyObserver->Empty());

  // Unregister event with handler
  MyHandler->UnregisterEvent(MyEvent);

  // Cleanup
  delete (MyObserver);
  delete (MyHandler);
  delete (MyEvent);

  // Return success
  return (0);
}

