#include "zEventTest.h"

int
zEventTest_EventTest(void* arg_)
{

  bool status = false;

  ZLOG_DEBUG("#############################################################");
  ZLOG_DEBUG("# zEventTest_EventTest()");
  ZLOG_DEBUG("#############################################################");

  // Create new event and validate
  zEvent::Event *MyEvent = new zEvent::Event(zEvent::Event::TYPE_NONE);
  TEST_ISNOT_NULL(MyEvent);
  TEST_EQ(zEvent::Event::TYPE_NONE, MyEvent->GetType());
  TEST_EQ(0, MyEvent->GetId());

  // Set type and validate
  status = MyEvent->SetType(zEvent::Event::TYPE_TEST);
  TEST_TRUE(status);
  TEST_EQ(zEvent::Event::TYPE_TEST, MyEvent->GetType());

  // Set id and validate
  status = MyEvent->SetId(1);
  TEST_TRUE(status);
  TEST_EQ(1, MyEvent->GetId());

  // Cleanup
  delete (MyEvent);

  // Return success
  return (0);
}

