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

#include <zutils/zDisplay.h>

#include "UnitTest.h"
#include "zDisplayTest.h"

using namespace Test;
using namespace zUtils;

int
zDisplayTest_DisplayBufferDefaults(void *arg_)
{

  ZLOG_DEBUG("#############################################################");
  ZLOG_DEBUG("# zDisplayTest_DisplayBufferDefaults()");
  ZLOG_DEBUG("#############################################################");

  // Create new display buffer and verify
  zDisplay::DisplayBuffer *MyBuffer = new zDisplay::DisplayBuffer(0);
  TEST_ISNOT_NULL(MyBuffer);
  TEST_EQ(0, MyBuffer->GetColumns());
  TEST_EQ(1, MyBuffer->GetRows());
  TEST_EQ(0, MyBuffer->GetSize());
  TEST_TRUE(MyBuffer->GetString().empty());

  // Cleanup
  delete (MyBuffer);

  // Return success
  return (0);

}

int
zDisplayTest_DisplayVarDefaults(void *arg_)
{

  ZLOG_DEBUG("#############################################################");
  ZLOG_DEBUG("# zDisplayTest_DisplayVarDefaults()");
  ZLOG_DEBUG("#############################################################");

  // Create new display variable and verify
  zDisplay::DisplayVar *MyVar = new zDisplay::DisplayVar("var", 0);
  TEST_ISNOT_NULL(MyVar);
  TEST_EQ(std::string("var"), MyVar->GetName());
  TEST_EQ(std::string(""), MyVar->GetString());
  TEST_EQ(0, MyVar->GetRow());
  TEST_EQ(0, MyVar->GetColumn());
  TEST_EQ(0, MyVar->GetSize());

  // Cleanup
  delete (MyVar);

  // Return success
  return (0);

}

int
zDisplayTest_DisplayDefaults(void *arg_)
{
  // Create new display device and verify
  TestDisplay *MyDisplay = new TestDisplay(0);
  TEST_ISNOT_NULL(MyDisplay);

  // Return success
  return (0);

}
