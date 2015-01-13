#include "zThreadTest.h"

using namespace Test;
using namespace zUtils;

int
zThreadTest_Defaults(void* arg_)
{
  // Create test thread and validate
  TestFunction *myFunc = new TestFunction;
  TEST_ISNOT_NULL(myFunc);
  delete (myFunc);
  return (0);
}

int
zThreadTest_RunOnce(void* arg_)
{
  // Create test thread and validate
  TestFunction *myFunc = new TestFunction(1);
  TEST_ISNOT_NULL(myFunc);
  TEST_EQ(1, myFunc->GetCount());
  TEST_TRUE(myFunc->Run());
  sleep(1);
  TEST_TRUE(myFunc->Join(1000));
  TEST_IS_ZERO(myFunc->GetCount());
  delete (myFunc);
  return (0);
}

int
zThreadTest_RunMultiple(void* arg_)
{
  // Create test thread and validate
  TestFunction *myFunc = new TestFunction(50);
  TEST_EQ(50, myFunc->GetCount());
  TEST_ISNOT_NULL(myFunc);
  TEST_TRUE(myFunc->Run());
  sleep(1);
  TEST_TRUE(myFunc->Join(1000));
  TEST_IS_ZERO(myFunc->GetCount());
  delete (myFunc);
  return (0);
}
