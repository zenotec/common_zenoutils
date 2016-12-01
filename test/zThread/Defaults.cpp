#include <list>
#include <mutex>

#include <zutils/zLog.h>
#include <zutils/zSem.h>

#include "zThreadTest.h"

using namespace Test;
using namespace zUtils;

int
zThreadTest_Defaults(void* arg_)
{

  ZLOG_DEBUG("#############################################################");
  ZLOG_DEBUG("# zThreadTest_Defaults()");
  ZLOG_DEBUG("#############################################################");

  // Create test thread and validate
  TestThread *myThread = new TestThread(0);
  TEST_ISNOT_NULL(myThread);
  TEST_EQ(std::string(""), myThread->Name());
  TEST_IS_ZERO(myThread->TestFunc.GetCount());
  delete (myThread);
  return (0);
}

int
zThreadTest_RunOnce(void* arg_)
{

  ZLOG_DEBUG("#############################################################");
  ZLOG_DEBUG("# zThreadTest_RunOnce()");
  ZLOG_DEBUG("#############################################################");

  // Create test thread and validate
  TestThread *myThread = new TestThread(1);
  TEST_ISNOT_NULL(myThread);
  TEST_EQ(std::string(""), myThread->Name());
  TEST_EQ(1, myThread->TestFunc.GetCount());

  // Run thread function until it exits
  TEST_TRUE(myThread->Start());
  TEST_TRUE(myThread->Join());

  // Verify
  TEST_IS_ZERO(myThread->TestFunc.GetCount());

  // Cleanup
  delete (myThread);

  // Return success
  return (0);
}

int
zThreadTest_RunMultiple(void* arg_)
{

  ZLOG_DEBUG("#############################################################");
  ZLOG_DEBUG("# zThreadTest_RunMultiple()");
  ZLOG_DEBUG("#############################################################");

  // Create test thread and validate
  TestThread *myThread = new TestThread(1);
  TEST_ISNOT_NULL(myThread);
  TEST_EQ(1, myThread->TestFunc.GetCount());

  // Run thread function until it exits
  TEST_TRUE(myThread->Start());

  // Wait for the thread to finish
  sleep(1);

  // Join thread and validate
  TEST_TRUE(myThread->Stop());

  // Verify
  TEST_IS_ZERO(myThread->TestFunc.GetCount());

  // Cleanup
  delete (myThread);

  // Return success
  return (0);
}
