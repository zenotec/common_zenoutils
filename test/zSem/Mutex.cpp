#include <zutils/zLog.h>
#include "zSemTest.h"

using namespace Test;
using namespace zUtils;

int
zSemTest_Mutex(void* arg_)
{

  ZLOG_DEBUG("#############################################################");
  ZLOG_DEBUG("# zSemTest_Mutex()");
  ZLOG_DEBUG("#############################################################");

  // Create new mutex with defaults and validate
  zSem::Mutex MyMutex1;
  TEST_EQ(zSem::Mutex::LOCKED, MyMutex1.State());

  // Create new mutex specifying initial value and validate
  zSem::Mutex MyMutex2(zSem::Mutex::LOCKED);
  TEST_EQ(zSem::Mutex::LOCKED, MyMutex2.State());
  TEST_FALSE(MyMutex2.TryLock());
  TEST_EQ(zSem::Mutex::LOCKED, MyMutex2.State());
  TEST_TRUE(MyMutex2.Unlock());
  TEST_EQ(zSem::Mutex::UNLOCKED, MyMutex2.State());
  TEST_TRUE(MyMutex2.TryLock());
  TEST_EQ(zSem::Mutex::LOCKED, MyMutex2.State());

  // Create new mutex specifying initial value and validate
  zSem::Mutex MyMutex3(zSem::Mutex::UNLOCKED);
  TEST_EQ(zSem::Mutex::UNLOCKED, MyMutex3.State());
  TEST_TRUE(MyMutex3.Lock());
  TEST_EQ(zSem::Mutex::LOCKED, MyMutex3.State());
  TEST_FALSE(MyMutex3.TimedLock(1000));
  TEST_TRUE(MyMutex3.Unlock());
  TEST_EQ(zSem::Mutex::UNLOCKED, MyMutex3.State());
  TEST_TRUE(MyMutex3.TimedLock(1000));

  // Return success
  return (0);
}

