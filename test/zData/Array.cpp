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

#include "zDataTest.h"

using namespace zUtils;
using namespace Test;

int
zDataTest_Array(void* arg)
{

  ZLOG_DEBUG("#############################################################");
  ZLOG_DEBUG("# zDataTest_Array()");
  ZLOG_DEBUG("#############################################################");

  const int num = 4;
  std::string key, obs;

  // Create new path and validate
  zData::DataPath ParentPath;
  TEST_TRUE(ParentPath.Append("Parent"));

  zData::DataPath ChildPath(ParentPath);
  TEST_TRUE(ChildPath.Append("Child"));

  // Create new data item and validate
  zData::Data *ParentData = new zData::Data(ParentPath);
  TEST_ISNOT_NULL(ParentData);

  // Create new value array and validate
  for (int i = 0; i < num; i++)
  {
    std::stringstream val;
    val << "value" << i;
    TEST_TRUE(ParentData->Add(ChildPath, val.str()));
  }
//  ParentData->DisplayJson();

  TEST_EQ(1, ParentData->Size());

  std::unique_ptr<zData::Data> ChildData = ParentData->operator [](0);
  TEST_TRUE(ChildData != NULL);
//  ChildData->DisplayJson();
//  ChildData->DisplayPath();

  // Parse value array and validate
  for (int i = 0; i < num; i++)
  {
    std::stringstream val;
    val << "value" << i;
    std::unique_ptr<zData::Data> MyData = ChildData->operator [](i);
    TEST_TRUE(MyData != NULL);
//    MyData->DisplayJson();
//    MyData->DisplayPath();
    TEST_TRUE(MyData->Get(MyData->GetDataPath(), obs));
    TEST_EQ(obs, val.str());
  }

  ParentData->Clear();
  TEST_IS_ZERO(ParentData->Size());

  // Cleanup
  if (ParentData)
    delete (ParentData);

  // Return success
  return (0);

}
