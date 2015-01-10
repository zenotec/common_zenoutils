#include "zSemTest.h"

using namespace Test;
using namespace zUtils;

int
zSemTest_Defaults(void* arg_)
{
  // Create new mutex with defaults and validate
  zSem::Mutex MyMutex;
  TEST_EQ(zSem::Mutex::LOCKED, MyMutex.State());

  // Create new semaphore with defaults and validate
  zSem::Semaphore MySem;
  TEST_IS_ZERO(MySem.Value());

  // Return success
  return (0);
}

