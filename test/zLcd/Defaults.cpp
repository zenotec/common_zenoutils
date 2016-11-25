#include <signal.h>

#include <string>
#include <mutex>
#include <list>
#include <vector>
#include <map>

#include <zutils/zLog.h>
#include <zutils/zSem.h>
#include <zutils/zQueue.h>
#include <zutils/zData.h>
#include <zutils/zEvent.h>
#include <zutils/zTimer.h>
#include <zutils/zSocket.h>
#include <zutils/zMessage.h>
#include <zutils/zCommand.h>

#include <zutils/zLcd.h>

#include "UnitTest.h"
#include "zLcdTest.h"

using namespace Test;
using namespace zUtils;

int
zLcdTest_LcdVarDefaults(void *arg_)
{
  // Create new LCD variable and verify
  zLcd::LcdVar *MyVar = new zLcd::LcdVar("var");
  TEST_ISNOT_NULL(MyVar);
  TEST_EQ("var", MyVar->GetName());
  TEST_EQ("", MyVar->GetValue());
  TEST_EQ(0, MyVar->GetRow());
  TEST_EQ(0, MyVar->GetCol());
  TEST_EQ(0, MyVar->GetLength());

  // Cleanup
  delete (MyVar);

  // Return success
  return (0);

}

int
zLcdTest_LcdDefaults(void *arg_)
{
  // Create new LCD device and verify
  TestLcd *MyLcd = new TestLcd;
  TEST_ISNOT_NULL(MyLcd);

  // Return success
  return (0);

}
