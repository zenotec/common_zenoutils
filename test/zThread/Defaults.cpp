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
  TestThread *myThread = new TestThread("Defaults", 0);
  TEST_ISNOT_NULL(myThread);
  TEST_EQ(std::string("Defaults"), myThread->Name());
  TEST_IS_ZERO(myThread->TestArg.LoopCnt);
  delete (myThread);
  return (0);
}

