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
#include "zConfTest.h"

using namespace Test;
using namespace zUtils;

int
zConfigTest_DataGetPutValue(void* arg_)
{

  ZLOG_DEBUG("#############################################################");
  ZLOG_DEBUG("# zConfigTest_DataGetPutValue()");
  ZLOG_DEBUG("#############################################################");

  std::string exp, obs;

  // Create path
  zConfig::ConfigPath MyPath;

  // Create new configuration data item and verify
  zConfig::ConfigData *MyData = new zConfig::ConfigData(MyPath);
  TEST_ISNOT_NULL(MyData);
  TEST_TRUE(MyData->Get(MyPath, obs));
  TEST_EQ(obs, exp);
//  MyData->DisplayJson();
//  MyData->DisplayPath();

  // Put some data
  exp = std::string("data");
  TEST_TRUE(MyData->Put(MyPath, exp));
  TEST_TRUE(MyData->Get(MyPath, obs));
  TEST_EQ(obs, exp);
//  MyData->DisplayJson();
//  MyData->DisplayPath();

  // Cleanup
  delete (MyData);

  // Return success
  return (0);
}

int
zConfigTest_DataGetPutChild(void* arg_)
{

  ZLOG_DEBUG("#############################################################");
  ZLOG_DEBUG("# zConfigTest_DataGetPutChild()");
  ZLOG_DEBUG("#############################################################");

  std::string exp, obs;

  // Create path
  zConfig::ConfigPath ParentPath("Parent");

  zConfig::ConfigPath ChildPath;
  TEST_TRUE(ChildPath.Append("Child"));

  // Create new configuration data item and verify
  zConfig::ConfigData *ParentData = new zConfig::ConfigData(ParentPath);
  TEST_ISNOT_NULL(ParentData);
  TEST_TRUE(ParentData->Get(ParentPath, obs));
  TEST_EQ(obs, exp);
//  ParentData->DisplayJson();

  // Create new configuration data item and verify
  zConfig::ConfigData *ChildData = new zConfig::ConfigData(ChildPath);
  TEST_ISNOT_NULL(ChildData);
  TEST_TRUE(ChildData->Get(ChildPath, obs));
  TEST_EQ(obs, exp);
//  ChildData->DisplayJson();

  // Put some data
  exp = std::string("data");
  TEST_TRUE(ChildData->Put(ChildPath, exp));
  TEST_TRUE(ChildData->Get(ChildPath, obs));
  TEST_EQ(obs, exp);
//  ChildData->DisplayJson();

  // Put the child and verify
  TEST_TRUE(ChildPath.Prepend("Parent"));
  TEST_TRUE_MSG(ParentData->Put(ChildData->GetData()), ParentData->GetJson());
//  ParentData->DisplayJson();
//  ChildPath.DisplayPath();
  TEST_TRUE_MSG(ParentData->Get(ChildPath, obs), ParentData->GetJson());
  TEST_EQ(obs, exp);

  // Cleanup
  delete (ChildData);
  delete (ParentData);

  // Return success
  return (0);
}

