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
using namespace zUtils;
ZLOG_MODULE_INIT(zLog::Log::MODULE_TEST);

#include "UnitTest.h"
#include "zConfTest.h"

using namespace Test;
using namespace zUtils;

int
zConfigTest_FileLoadStore(void* arg_)
{

  ZLOG_DEBUG("#############################################################");
  ZLOG_DEBUG("# zConfigTest_FileLoadStore()");
  ZLOG_DEBUG("#############################################################");

  zConfig::ConfigPath MyPath1;
  TEST_TRUE(MyPath1.Append("Key1"));

  zConfig::ConfigPath MyPath2;
  TEST_TRUE(MyPath2.Append("Key2"));

  // Create new configuration data item and verify
  zConfig::ConfigData *ExpData = new zConfig::ConfigData;
  TEST_ISNOT_NULL(ExpData);
  zConfig::ConfigData *ObsData = new zConfig::ConfigData;
  TEST_ISNOT_NULL(ObsData);

  // Create new configuration data connector and verify
  TestConnector *MyConnector = new TestConnector;
  TEST_ISNOT_NULL(MyConnector);

  // Attempt to load configuration and verify
  TEST_FALSE(MyConnector->Load(*ExpData));

  // Attempt to store configuration and verify
  TEST_TRUE(MyConnector->Store(*ExpData));

  // Attempt to load configuration and verify
  TEST_TRUE(MyConnector->Load(*ObsData));

  // Update configuration data
  std::string obs1;
  std::string val1 = "Value1";
  TEST_TRUE(ExpData->PutValue(MyPath1.GetDataPath(), val1));
  TEST_TRUE(ExpData->GetValue(MyPath1.GetDataPath(), obs1));
  TEST_EQ(val1, obs1);

  std::string obs2;
  std::string val2 = "Value2";
  TEST_TRUE(ExpData->PutValue(MyPath2.GetDataPath(), val2));
  TEST_TRUE(ExpData->GetValue(MyPath2.GetDataPath(), obs2));
  TEST_EQ(val2, obs2);

  // Verify data is not equal
  TEST_TRUE(*ObsData != *ExpData);

  // Attempt to store configuration and verify
  TEST_TRUE(MyConnector->Store(*ExpData));
//  expData->DisplayJson();

  // Attempt to load configuration and verify
  TEST_TRUE(MyConnector->Load(*ObsData));
//  obsData->DisplayJson();

  // Verify data is equal
  TEST_TRUE(*ObsData == *ExpData);

  // Cleanup
  delete (MyConnector);
  delete (ObsData);
  delete (ExpData);

  // Return success
  return (0);

}
