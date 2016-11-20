#include "zEventTest.h"

int
zEventTest_EventDefaults(void * arg_)
{
  // Create new event and validate
  TestEvent *MyEvent = new TestEvent;
  TEST_ISNOT_NULL(MyEvent);
  TEST_EQ(zEvent::Event::TYPE_TEST, MyEvent->Type());

  // Cleanup
  delete (MyEvent);

  // Return success
  return (0);
}

int
zEventTest_EventNotificationDefaults(void * arg_)
{
  // Create new event and validate
  TestEvent *MyEvent = new TestEvent;
  TEST_ISNOT_NULL(MyEvent);
  TEST_EQ(zEvent::Event::TYPE_TEST, MyEvent->Type());

  // Create new event notification and validate
  TestEventNotification *MyNotification = new TestEventNotification(MyEvent);
  TEST_ISNOT_NULL(MyNotification);
  TEST_EQ(zEvent::Event::TYPE_TEST, MyNotification->Type());

  // Cleanup
  delete (MyNotification);
  delete (MyEvent);

  // Return success
  return (0);
}


int
zEventTest_EventHandlerDefaults(void * arg_)
{
  // Create new event handler and validate
  zEvent::EventHandler *MyHandler = new zEvent::EventHandler;
  TEST_ISNOT_NULL(MyHandler);

  // Cleanup
  delete (MyHandler);

  // Return success
  return (0);
}


int
zEventTest_EventManagerDefaults(void * arg_)
{

  // Get instance of event manager
  zEvent::EventManager &manager = zEvent::EventManager::Instance();

  // Return success
  return (0);
}


