#include "zSemTest.h"

using namespace Test;
using namespace zUtils;

int
zSemTest_Semaphore(void* arg_)
{
  // Create new semaphore with defaults and validate
  zSem::Semaphore MySem1;
  TEST_IS_ZERO(MySem1.Value());
  TEST_TRUE(MySem1.Post());
  TEST_EQ(1, MySem1.Value());
  TEST_TRUE(MySem1.Wait());
  TEST_IS_ZERO(MySem1.Value());

  // Create new semaphore specifying initial value and validate
  zSem::Semaphore MySem2(1);
  TEST_EQ(1, MySem2.Value());
  TEST_TRUE(MySem2.Post(1));
  TEST_EQ(2, MySem2.Value());
  TEST_TRUE(MySem2.TryWait());
  TEST_EQ(1, MySem2.Value());
  TEST_TRUE(MySem2.TimedWait(1000));
  TEST_IS_ZERO(MySem2.Value());
  TEST_FALSE(MySem2.TryWait());
  TEST_FALSE(MySem2.TimedWait(1000));

  // Return success
  return (0);
}
