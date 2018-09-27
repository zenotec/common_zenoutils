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
zDisplayTest_DisplayVarGetSet(void *arg_)
{

  ZLOG_DEBUG("#############################################################");
  ZLOG_DEBUG("# zDisplayTest_DisplayVarGetSet()");
  ZLOG_DEBUG("#############################################################");

  // Create new LCD variable and verify
  zDisplay::DisplayVar *MyVar = new zDisplay::DisplayVar("var", 10);
  TEST_ISNOT_NULL(MyVar);
  TEST_EQ(std::string("var"), MyVar->GetName());
  TEST_EQ(std::string(""), MyVar->GetString());
  TEST_EQ(0, MyVar->GetRow());
  TEST_EQ(0, MyVar->GetColumn());
  TEST_EQ(10, MyVar->GetSize());

  // Set column, row and verify
  TEST_TRUE(MyVar->SetRow(1));
  TEST_EQ(1, MyVar->GetRow());
  TEST_TRUE(MyVar->SetColumn(5));
  TEST_EQ(5, MyVar->GetColumn());

  // Set value (too big)
  TEST_EQ(10, MyVar->Update(std::string("value-0123456789")));
  TEST_EQ(std::string("value-0123"), MyVar->GetString());

  // Set value (exact)
  TEST_EQ(10, MyVar->Update(std::string("0123456789")));
  TEST_EQ(std::string("0123456789"), MyVar->GetString());

  // Set value (partial)
  TEST_EQ(5, MyVar->Update(std::string("value")));
  TEST_EQ(std::string("value56789"), MyVar->GetString());

  // Clear and verify
  MyVar->Clear();
  TEST_EQ(std::string(""), MyVar->GetString());

  // Cleanup
  delete (MyVar);

  // Return success
  return (0);

}
