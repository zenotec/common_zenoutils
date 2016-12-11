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

#include <zutils/zLog.h>
#include <zutils/zData.h>

#include "UnitTest.h"
#include "zDataTest.h"

using namespace zUtils;
using namespace Test;

int
zDataTest_DataCopy(void* arg)
{

  ZLOG_DEBUG("#############################################################");
  ZLOG_DEBUG("# zDataTest_DataCopy()");
  ZLOG_DEBUG("#############################################################");

  // Create new data object and validate
  std::string key1 = "key1";
  zData::Data *MyData1 = new zData::Data;
  TEST_ISNOT_NULL(MyData1);
  TEST_TRUE(MyData1->Append(key1));
  TEST_EQ(key1, MyData1->Key());
//  MyData1->DisplayPath();
//  MyData1->DisplayJson();

  // Create new data object and validate
  std::string key2 = "key2";
  zData::Data *MyData2 = new zData::Data;
  TEST_ISNOT_NULL(MyData2);
  TEST_TRUE(MyData2->Append(key2));
  TEST_EQ(key2, MyData2->Key());
//  MyData2->DisplayPath();
//  MyData2->DisplayJson();

  // Verify data is NOT equal
  TEST_FALSE_MSG(*MyData1 == *MyData2, std::string(MyData1->GetJson() + MyData2->GetJson()));
  TEST_TRUE_MSG(*MyData1 != *MyData2, std::string(MyData1->GetJson() + MyData2->GetJson()));

  // Copy
  *MyData2 = *MyData1;
//  MyData1->DisplayPath();
//  MyData1->DisplayJson();
//  MyData2->DisplayPath();
//  MyData2->DisplayJson();

  // Verify data is equal
  TEST_TRUE_MSG(*MyData1 == *MyData2, std::string(MyData1->GetJson() + MyData2->GetJson()));
  TEST_FALSE_MSG(*MyData1 != *MyData2, std::string(MyData1->GetJson() + MyData2->GetJson()));

  // Clean up
  delete (MyData1);
  delete (MyData2);

  // Return success
  return (0);

}

