#include "UnitTest.h"
#include "zutils/zSemaphore.h"

using namespace Test;
using namespace zUtils;

static int
UT_zSem_Mutex(int arg_)
{
  // Create new mutex with defaults and validate
  zMutex MyMutex1;
  TEST_EQ(zMutex::LOCKED, MyMutex1.State());

  // Create new mutex specifying initial value and validate
  zMutex MyMutex2(zMutex::LOCKED);
  TEST_EQ(zMutex::LOCKED, MyMutex2.State());
  TEST_FALSE(MyMutex2.TryLock());
  TEST_EQ(zMutex::LOCKED, MyMutex2.State());
  TEST_TRUE(MyMutex2.Unlock());
  TEST_EQ(zMutex::UNLOCKED, MyMutex2.State());
  TEST_TRUE(MyMutex2.TryLock());
  TEST_EQ(zMutex::LOCKED, MyMutex2.State());

  // Create new mutex specifying initial value and validate
  zMutex MyMutex3(zMutex::UNLOCKED);
  TEST_EQ(zMutex::UNLOCKED, MyMutex3.State());
  TEST_TRUE(MyMutex3.Lock());
  TEST_EQ(zMutex::LOCKED, MyMutex3.State());
  TEST_FALSE(MyMutex3.TimedLock(1000));
  TEST_TRUE(MyMutex3.Unlock());
  TEST_EQ(zMutex::UNLOCKED, MyMutex3.State());
  TEST_TRUE(MyMutex3.TimedLock(1000));

  // Return success
  return (0);
}

static int
UT_zSem_Semaphore(int arg_)
{
  // Create new semaphore with defaults and validate
  zSemaphore MySem1;
  TEST_IS_ZERO(MySem1.Value());
  TEST_TRUE(MySem1.Post());
  TEST_EQ(1, MySem1.Value());
  TEST_TRUE(MySem1.Wait());
  TEST_IS_ZERO(MySem1.Value());

  // Create new semaphore specifying initial value and validate
  zSemaphore MySem2(1);
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

int
zSem_utest(void)
{
  INIT();
  UTEST( UT_zSem_Mutex, 0);
  UTEST( UT_zSem_Semaphore, 0);
  FINI();
}

