/*
 * Copyright (c) 2014-2016 ZenoTec LLC (http://www.zenotec.net)
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at:
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

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

  // Create new display page and verify
  zDisplay::DisplayPage *MyPage = MyDisplay->CreatePage("page1");
  TEST_ISNOT_NULL(MyPage);

  // Create new Display variable and verify
  zDisplay::DisplayVar *MyVar = MyPage->CreateVar("var", 3);
  TEST_ISNOT_NULL(MyVar);
  TEST_EQ(std::string("var"), MyVar->GetName());
  TEST_EQ(std::string(""), MyVar->GetString());
  TEST_EQ(0, MyVar->GetRow());
  TEST_EQ(0, MyVar->GetColumn());
  TEST_EQ(3, MyVar->GetSize());

  // Cleanup
  MyPage->DeleteVar(MyVar);
  MyDisplay->DeletePage(MyPage);
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
  TEST_EQ(std::string("................................"), MyDisplay->GetBuffer());

  // Create new display page and verify
  zDisplay::DisplayPage *MyPage1 = MyDisplay->CreatePage("page1");
  TEST_ISNOT_NULL(MyPage1);

  // Create new display page and verify
  zDisplay::DisplayPage *MyPage2 = MyDisplay->CreatePage("page2");
  TEST_ISNOT_NULL(MyPage2);

  // Configure Display
  TEST_TRUE(MyDisplay->SetRefreshRate(60));
  TEST_TRUE(MyDisplay->SetPageTimeout(1));

  // Update Display with string and verify
  TEST_EQ(5, MyPage1->Update("RPM: ", 0, 0));
  MyDisplay->Flush();
  TEST_EQ(std::string("RPM: ..........................."), MyDisplay->GetBuffer());

  // Update Display with string and verify
  TEST_EQ(5, MyPage2->Update("MPH: ", 0, 0));
  MyDisplay->Flush();
  TEST_EQ(std::string("RPM: ..........................."), MyDisplay->GetBuffer());

  // Create new Display variable and verify
  zDisplay::DisplayVar *MyVar1 = MyPage1->CreateVar("rpm", 5);
  TEST_ISNOT_NULL(MyVar1);
  TEST_EQ(std::string("rpm"), MyVar1->GetName());
  TEST_EQ(std::string(""), MyVar1->GetString());
  TEST_TRUE(MyVar1->SetRow(0));
  TEST_EQ(0, MyVar1->GetRow());
  TEST_TRUE(MyVar1->SetColumn(5));
  TEST_EQ(5, MyVar1->GetColumn());
  TEST_EQ(5, MyVar1->GetSize());

  // Update variable
  TEST_EQ(5, MyVar1->Update(" 9999"));
  usleep(100000);
  TEST_EQ(std::string("RPM:  9999......................"), MyDisplay->GetBuffer());

  // Create new Display variable and verify
  zDisplay::DisplayVar *MyVar2 = MyPage2->CreateVar("mph", 4);
  TEST_ISNOT_NULL(MyVar2);
  TEST_EQ(std::string("mph"), MyVar2->GetName());
  TEST_EQ(std::string(""), MyVar2->GetString());
  TEST_TRUE(MyVar2->SetRow(0));
  TEST_EQ(0, MyVar2->GetRow());
  TEST_TRUE(MyVar2->SetColumn(5));
  TEST_EQ(5, MyVar2->GetColumn());
  TEST_EQ(4, MyVar2->GetSize());

  // Update variable
  TEST_EQ(5, MyVar1->Update(" 8888"));
  TEST_EQ(4, MyVar2->Update(" 123"));
  usleep(100000);
  TEST_EQ(std::string("RPM:  8888......................"), MyDisplay->GetBuffer());
  usleep(1000000);
  TEST_EQ(std::string("MPH:  123......................."), MyDisplay->GetBuffer());
//TEST_EQ(std::string("MPH: 123........................"), MyDisplay->GetBuffer());

  // Cleanup
  MyPage1->DeleteVar(MyVar1);
  MyPage1->DeleteVar(MyVar2);
  MyDisplay->DeletePage(MyPage1);
  delete (MyDisplay);

  // Return success
  return (0);

}
