#include "zEventTest.h"

int
zEventTest_EventTest(void* arg_)
{

  bool status = false;

  ZLOG_DEBUG("#############################################################");
  ZLOG_DEBUG("# zEventTest_EventTest()");
  ZLOG_DEBUG("#############################################################");

  // Create new event and validate
  TestEvent *MyEvent = new TestEvent;
  TEST_ISNOT_NULL(MyEvent);
  TEST_EQ(zEvent::Event::TYPE_TEST, MyEvent->Type());

  // Cleanup
  delete (MyEvent);

  // Return success
  return (0);
}

