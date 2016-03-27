#include "zEventTest.h"

int
zEventTest_EventTest(void* arg_)
{

  bool status = false;

  ZLOG_DEBUG("#############################################################");
  ZLOG_DEBUG("# zEventTest_EventTest()");
  ZLOG_DEBUG("#############################################################");

  // Create new event and validate
  zEvent::Event *MyEvent = new zEvent::Event(zEvent::Event::TYPE_TEST);
  TEST_ISNOT_NULL(MyEvent);
  TEST_EQ(zEvent::Event::TYPE_TEST, MyEvent->Type());

  // Cleanup
  delete (MyEvent);

  // Return success
  return (0);
}

