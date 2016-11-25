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
zLcdTest_LcdCreateVar(void *arg_)
{
  // Create new LCD device and verify
  TestLcd *MyLcd = new TestLcd;
  TEST_ISNOT_NULL(MyLcd);

  // Configure LCD
  MyLcd->SetLimits(2, 16);

  // Create new LCD variable and verify
  zLcd::LcdVar *MyVar = MyLcd->CreateVar("var", 1, 2, 3);
  TEST_ISNOT_NULL(MyVar);
  TEST_EQ("var", MyVar->GetName());
  TEST_EQ("", MyVar->GetValue());
  TEST_EQ(1, MyVar->GetRow());
  TEST_EQ(2, MyVar->GetCol());
  TEST_EQ(3, MyVar->GetLength());

  // Cleanup
  delete (MyLcd);

  // Return success
  return (0);

}

int
zLcdTest_LcdUpdate(void *arg_)
{
  // Create new LCD device and verify
  TestLcd *MyLcd = new TestLcd;
  TEST_ISNOT_NULL(MyLcd);

  // Configure LCD
  MyLcd->SetLimits(2, 16);
  MyLcd->SetRefresh(60);

  // Update LCD with string and verify
  TEST_TRUE(MyLcd->Update("RPM: ", 0, 0));
  usleep(100000);
  TEST_EQ("RPM: \n\n", MyLcd->GetBuffer());

  // Update LCD with string and verify
  TEST_TRUE(MyLcd->Update("MPH: ", 1, 0));
  usleep(100000);
  TEST_EQ("RPM: \nMPH: \n", MyLcd->GetBuffer());

  // Create new LCD variable and verify
  zLcd::LcdVar *MyVar1 = MyLcd->CreateVar("rpm", 0, 5, 5);
  TEST_ISNOT_NULL(MyVar1);
  TEST_EQ("rpm", MyVar1->GetName());
  TEST_EQ("", MyVar1->GetValue());
  TEST_EQ(0, MyVar1->GetRow());
  TEST_EQ(5, MyVar1->GetCol());
  TEST_EQ(5, MyVar1->GetLength());

  // Update variable
  TEST_TRUE(MyVar1->SetValue(" 9999"));
  usleep(100000);
  TEST_EQ("RPM:  9999\nMPH: \n", MyLcd->GetBuffer());

  // Create new LCD variable and verify
  zLcd::LcdVar *MyVar2 = MyLcd->CreateVar("mph", 1, 5, 3);
  TEST_ISNOT_NULL(MyVar2);
  TEST_EQ("mph", MyVar2->GetName());
  TEST_EQ("", MyVar2->GetValue());
  TEST_EQ(1, MyVar2->GetRow());
  TEST_EQ(5, MyVar2->GetCol());
  TEST_EQ(3, MyVar2->GetLength());

  // Update variable
  TEST_TRUE(MyVar1->SetValue(" 8888"));
  TEST_TRUE(MyVar2->SetValue("123"));
  usleep(100000);
  TEST_EQ("RPM:  8888\nMPH: 123\n", MyLcd->GetBuffer());

  // Cleanup
  MyLcd->DeleteVar(MyVar1);
  MyLcd->DeleteVar(MyVar2);
  delete (MyLcd);

  // Return success
  return (0);

}
