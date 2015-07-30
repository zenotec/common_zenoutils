#include "zSwitchTest.h"

using namespace Test;
using namespace zUtils;

int
zSwitchTest_Defaults(void* arg_)
{
  // Create test switch
  TestSwitch *mySwitch = new TestSwitch(zSwitch::Switch::STATE_OFF);
  TEST_ISNOT_NULL(mySwitch);
  TEST_FALSE(mySwitch->IsOn());
  TEST_TRUE(mySwitch->IsOff());
  delete (mySwitch);
  return (0);
}

