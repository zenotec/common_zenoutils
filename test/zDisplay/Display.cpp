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
zDisplayTest_DisplayCreateVar(void *arg_)
{

  ZLOG_DEBUG("#############################################################");
  ZLOG_DEBUG("# zDisplayTest_DisplayCreateVar()");
  ZLOG_DEBUG("#############################################################");

  // Create new Display device and verify
  TestDisplay *MyDisplay = new TestDisplay(16, 2);
  TEST_ISNOT_NULL(MyDisplay);

  // Create new Display variable and verify
  zDisplay::DisplayVar *MyVar = MyDisplay->CreateVar("var", 3);
  TEST_ISNOT_NULL(MyVar);
  TEST_EQ(std::string("var"), MyVar->GetName());
  TEST_EQ(std::string(""), MyVar->GetString());
  TEST_EQ(0, MyVar->GetRow());
  TEST_EQ(0, MyVar->GetColumn());
  TEST_EQ(3, MyVar->GetSize());

  // Cleanup
  MyDisplay->DeleteVar(MyVar);
  delete (MyDisplay);

  // Return success
  return (0);

}

int
zDisplayTest_DisplayUpdate(void *arg_)
{

  ZLOG_DEBUG("#############################################################");
  ZLOG_DEBUG("# zDisplayTest_DisplayUpdate()");
  ZLOG_DEBUG("#############################################################");

  // Create new Display device and verify
  TestDisplay *MyDisplay = new TestDisplay(16, 2);
  TEST_ISNOT_NULL(MyDisplay);
  TEST_EQ(std::string(""), MyDisplay->GetBuffer());

  // Configure Display
  MyDisplay->SetRefresh(60);

  // Update Display with string and verify
  TEST_TRUE(MyDisplay->Update("RPM: ", 0, 0));
  usleep(100000);
  TEST_EQ(std::string("RPM: "), MyDisplay->GetBuffer());

  // Update Display with string and verify
  TEST_TRUE(MyDisplay->Update("MPH: ", 0, 1));
  usleep(100000);
  TEST_EQ(std::string("RPM: \nMPH: \n"), MyDisplay->GetBuffer());

  // Create new Display variable and verify
  zDisplay::DisplayVar *MyVar1 = MyDisplay->CreateVar("rpm", 5);
  TEST_ISNOT_NULL(MyVar1);
  TEST_EQ(std::string("rpm"), MyVar1->GetName());
  TEST_EQ(std::string(""), MyVar1->GetString());
  TEST_TRUE(MyVar1->SetRow(0));
  TEST_EQ(0, MyVar1->GetRow());
  TEST_TRUE(MyVar1->SetColumn(5));
  TEST_EQ(5, MyVar1->GetColumn());
  TEST_EQ(5, MyVar1->GetSize());

  // Update variable
  TEST_TRUE(MyVar1->Update(" 9999"));
  usleep(100000);
  TEST_EQ(std::string("RPM:  9999\nMPH: \n"), MyDisplay->GetBuffer());

  // Create new Display variable and verify
  zDisplay::DisplayVar *MyVar2 = MyDisplay->CreateVar("mph", 3);
  TEST_ISNOT_NULL(MyVar2);
  TEST_EQ(std::string("mph"), MyVar2->GetName());
  TEST_EQ(std::string(""), MyVar2->GetString());
  TEST_TRUE(MyVar2->SetRow(0));
  TEST_EQ(1, MyVar2->GetRow());
  TEST_TRUE(MyVar2->SetColumn(5));
  TEST_EQ(5, MyVar2->GetColumn());
  TEST_EQ(3, MyVar2->GetSize());

  // Update variable
  TEST_TRUE(MyVar1->Update(" 8888"));
  TEST_TRUE(MyVar2->Update("123"));
  usleep(100000);
  TEST_EQ(std::string("RPM:  8888\nMPH: 123\n"), MyDisplay->GetBuffer());

  // Cleanup
  MyDisplay->DeleteVar(MyVar1);
  MyDisplay->DeleteVar(MyVar2);
  delete (MyDisplay);

  // Return success
  return (0);

}
