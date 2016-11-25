
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
  // Create test thread and validate
  TestFunction *myFunc = new TestFunction;
  TEST_ISNOT_NULL(myFunc);
  TEST_IS_ZERO(myFunc->Id());
  TEST_EQ(1, myFunc->GetCount());
  delete (myFunc);
  return (0);
}

int
zThreadTest_RunOnce(void* arg_)
{
  // Create test thread and validate
  TestFunction *myFunc = new TestFunction(1);
  TEST_ISNOT_NULL(myFunc);
  TEST_IS_ZERO(myFunc->Id());
  TEST_EQ(1, myFunc->GetCount());

  // Run thread function until it exits
  TEST_TRUE(myFunc->Run());
  TEST_ISNOT_ZERO(myFunc->Id());

  // Wait for the thread to finish
  usleep(2 * 1 * 1000); // wait twice the period

  // Join thread and validate
  TEST_TRUE(myFunc->Join());
  TEST_IS_ZERO(myFunc->Id());
  TEST_IS_ZERO(myFunc->GetCount());

  // Cleanup
  delete (myFunc);

  // Return success
  return (0);
}

int
zThreadTest_RunMultiple(void* arg_)
{
  // Create test thread and validate
  TestFunction *myFunc = new TestFunction(50);
  TEST_ISNOT_NULL(myFunc);
  TEST_IS_ZERO(myFunc->Id());
  TEST_EQ(50, myFunc->GetCount());

  // Run thread function until it exits
  TEST_TRUE(myFunc->Run());
  TEST_ISNOT_ZERO(myFunc->Id());

  // Wait for the thread to finish
  usleep(2 * 50 * 1000); // wait twice the period

  // Join thread and validate
  TEST_TRUE(myFunc->Join());
  TEST_IS_ZERO(myFunc->Id());
  TEST_IS_ZERO(myFunc->GetCount());

  // Cleanup
  delete (myFunc);

  // Return success
  return (0);
}
