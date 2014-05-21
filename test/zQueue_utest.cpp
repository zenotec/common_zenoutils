#include "UnitTest.h"
#include "zutils/zQueue.h"

using namespace Test;
using namespace zUtils;

static int
UT_zQueue(int arg_)
{

  std::string exp_str = "TestString";
  std::string obs_str;

  // Create new queue and validate
  zQueue<std::string> MyQueue;
  TEST_TRUE(MyQueue.Empty());
  TEST_IS_ZERO(MyQueue.Size());

  // Push string on to back of queue and validate
  MyQueue.Push(exp_str + "_1");
  TEST_FALSE(MyQueue.Empty());
  TEST_EQ(1, MyQueue.Size());
  obs_str = MyQueue.Front();
  TEST_FALSE(MyQueue.Empty());
  TEST_EQ(1, MyQueue.Size());
  TEST_EQ(exp_str + "_1", obs_str);
  obs_str = MyQueue.Back();
  TEST_FALSE(MyQueue.Empty());
  TEST_EQ(1, MyQueue.Size());
  TEST_EQ(exp_str + "_1", obs_str);

  // Push second string on to back of queue and validate
  MyQueue.Push(exp_str + "_2");
  TEST_FALSE(MyQueue.Empty());
  TEST_EQ(2, MyQueue.Size());
  obs_str = MyQueue.Front();
  TEST_FALSE(MyQueue.Empty());
  TEST_EQ(2, MyQueue.Size());
  TEST_EQ(exp_str + "_1", obs_str);
  obs_str = MyQueue.Back();
  TEST_FALSE(MyQueue.Empty());
  TEST_EQ(2, MyQueue.Size());
  TEST_EQ(exp_str + "_2", obs_str);

  // Pop first string off the front of the queue
  MyQueue.Pop();
  TEST_FALSE(MyQueue.Empty());
  TEST_EQ(1, MyQueue.Size());

  // Pop second string off the front of the queue
  MyQueue.Pop();
  TEST_TRUE(MyQueue.Empty());
  TEST_IS_ZERO(MyQueue.Size());

  // Return success
  return (0);
}

int
zQueue_utest(void)
{
  INIT();
  UTEST( UT_zQueue, 0);
  FINI();
}
