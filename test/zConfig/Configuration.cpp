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
zConfigTest_ConfigurationCtor(void* arg_)
{
  ZLOG_DEBUG("#############################################################");
  ZLOG_DEBUG("# zConfigTest_ConfigurationCtor()");
  ZLOG_DEBUG("#############################################################");

  // Create new data item and verify
  zConfig::ConfigData *MyData = new zConfig::ConfigData;
  TEST_ISNOT_NULL(MyData);

  // Setup data item and verify
  zData::DataPath NamePath;
  TEST_TRUE(NamePath.Append("Employee"));
  TEST_TRUE(NamePath.Append("Name"));
  std::string val = "Elvis";
  std::string obs = "";
  TEST_TRUE(MyData->PutValue(NamePath, val));
  TEST_TRUE(MyData->GetValue(NamePath, obs));
  TEST_EQ(obs, val);

  // Create const configuration data item from data item and verify
  const zConfig::Configuration ConstConfig(*MyData);
  TEST_FALSE(ConstConfig.IsModified());

  // Create new configuration data item and verify
  zConfig::Configuration *MyConfig = new zConfig::Configuration(ConstConfig);
  TEST_ISNOT_NULL(MyConfig);
  TEST_FALSE(MyConfig->IsModified());

  return (0);

}

int
zConfigTest_ConfigurationGetSetData(void* arg_)
{
  ZLOG_DEBUG("#############################################################");
  ZLOG_DEBUG("# zConfigTest_ConfigurationGetSetData()");
  ZLOG_DEBUG("#############################################################");

  std::string exp, obs;

  zConfig::ConfigPath MyPath("Employee");

  // Create new configuration item and verify
  zConfig::Configuration *MyConfig = new zConfig::Configuration;
  TEST_ISNOT_NULL(MyConfig);
  TEST_FALSE(MyConfig->IsModified());

  // Create new data item and verify
  zConfig::ConfigData *MyData = new zConfig::ConfigData(MyPath);
  TEST_ISNOT_NULL(MyData);
//  MyData->DisplayJson();

// Setup data item and verify
  exp = "Elvis";
  TEST_TRUE(MyData->PutValue(MyData->GetDataPath(), exp));
  TEST_TRUE(MyData->GetValue(MyData->GetDataPath(), obs));
  TEST_EQ(exp, obs);
//  MyData->DisplayJson();

// Put the data and verify (only updates staging data)
  TEST_TRUE(MyConfig->Put(*MyData));
  TEST_TRUE(MyConfig->Get(MyConfig->GetConfigPath(), obs));
  TEST_EQ(std::string(""), obs);

  // Commit data and verify
  TEST_TRUE(MyConfig->Commit());
  TEST_TRUE(MyConfig->Get(MyConfig->GetConfigPath(), obs));
  TEST_EQ(exp, obs);

  // Cleanup
  delete (MyData);
  delete (MyConfig);

  // Return success
  return (0);

}

int
zConfigTest_ConfigurationCompare(void* arg_)
{
  ZLOG_DEBUG("#############################################################");
  ZLOG_DEBUG("# zConfigTest_ConfigurationCompare()");
  ZLOG_DEBUG("#############################################################");

  // Create new configuration data item and verify
  zConfig::Configuration *MyConfig1 = new zConfig::Configuration;
  TEST_ISNOT_NULL(MyConfig1);
  TEST_FALSE(MyConfig1->IsModified());

  // Create second configuration data item and verify
  zConfig::Configuration *MyConfig2 = new zConfig::Configuration;
  TEST_ISNOT_NULL(MyConfig2);
  TEST_FALSE(MyConfig2->IsModified());

  // Verify both are equal
  TEST_TRUE(*MyConfig1 == *MyConfig2);
  TEST_FALSE(*MyConfig1 != *MyConfig2);

  zConfig::ConfigData MyData("Employee");
  std::string val = "Elvis";
  TEST_TRUE(MyData.PutValue(MyData.GetDataPath(), val));

  // Modify first configuration data item
  TEST_TRUE(MyConfig1->Put(MyData));
  TEST_TRUE(MyConfig1->IsModified());

  // Verify both are still equal
  TEST_TRUE(*MyConfig1 == *MyConfig2);
  TEST_FALSE(*MyConfig1 != *MyConfig2);

  // Commit data and verify they are no longer equal
  TEST_TRUE(MyConfig1->Commit());
  TEST_FALSE(MyConfig1->IsModified());
  TEST_TRUE(MyConfig2->Commit());
  TEST_FALSE(MyConfig2->IsModified());
  TEST_FALSE(*MyConfig1 == *MyConfig2);
  TEST_TRUE(*MyConfig1 != *MyConfig2);

  // Modify second configuration data item
  TEST_TRUE(MyConfig2->Put(MyData));
  TEST_TRUE(MyConfig2->IsModified());

  // Verify they are still not equal
  TEST_FALSE(*MyConfig1 == *MyConfig2);
  TEST_TRUE(*MyConfig1 != *MyConfig2);

  // Commit and verify both are equal again
  TEST_TRUE(MyConfig1->Commit());
  TEST_FALSE(MyConfig1->IsModified());
  TEST_TRUE(MyConfig2->Commit());
  TEST_FALSE(MyConfig2->IsModified());
  TEST_TRUE(*MyConfig1 == *MyConfig2);
  TEST_FALSE(*MyConfig1 != *MyConfig2);

  // Cleanup
  delete (MyConfig2);
  delete (MyConfig1);

  // Return success
  return (0);

}

int
zConfigTest_ConfigurationDataArray(void* arg_)
{
  ZLOG_DEBUG("#############################################################");
  ZLOG_DEBUG("# zConfigTest_ConfigurationDataArray()");
  ZLOG_DEBUG("#############################################################");

  zConfig::ConfigPath MyPath("Employee");

  // Create new configuration data item and verify
  zConfig::Configuration *MyConfig = new zConfig::Configuration;
  TEST_ISNOT_NULL(MyConfig);
  TEST_FALSE(MyConfig->IsModified());

  // Create new data item and verify
  zConfig::ConfigData *MyData1 = new zConfig::ConfigData(MyPath);
  TEST_ISNOT_NULL(MyData1);

  // Setup data item and verify
  std::string val1 = "Moe";
  std::string obs1 = "";
  TEST_TRUE(MyData1->PutValue(MyData1->GetDataPath(), val1));
  TEST_TRUE(MyData1->GetValue(MyData1->GetDataPath(), obs1));
  TEST_EQ(obs1, val1);

  // Create new data item and verify
  zConfig::ConfigData *MyData2 = new zConfig::ConfigData(MyPath);
  TEST_ISNOT_NULL(MyData2);

  // Setup data item and verify
  std::string val2 = "Larry";
  std::string obs2 = "";
  TEST_TRUE(MyData2->PutValue(MyData2->GetDataPath(), val2));
  TEST_TRUE(MyData2->GetValue(MyData2->GetDataPath(), obs2));
  TEST_EQ(obs2, val2);

  // Create new data item and verify
  zConfig::ConfigData *MyData3 = new zConfig::ConfigData(MyPath);
  TEST_ISNOT_NULL(MyData3);

  // Setup data item and verify
  std::string val3 = "Curly";
  std::string obs3 = "";
  TEST_TRUE(MyData3->PutValue(MyData3->GetDataPath(), val3));
  TEST_TRUE(MyData3->GetValue(MyData3->GetDataPath(), obs3));
  TEST_EQ(obs3, val3);

  // Add the data items and verify (only updates staging data)
  TEST_TRUE(MyConfig->Add(*MyData1));
  TEST_TRUE(MyConfig->IsModified());
  TEST_TRUE(MyConfig->Add(*MyData2));
  TEST_TRUE(MyConfig->IsModified());
  TEST_TRUE(MyConfig->Add(*MyData3));
  TEST_TRUE(MyConfig->IsModified());
//  MyConfig->Display();

// Commit data and verify
  TEST_TRUE(MyConfig->Commit());
  TEST_FALSE(MyConfig->IsModified());
//  MyConfig->Display();
  zConfig::ConfigData ObsData;
  TEST_TRUE(MyConfig->Get(ObsData));

  zData::Data d;

  d = ObsData[0];
//  d->DisplayJson();
  TEST_TRUE(d.GetValue(d.GetDataPath(), obs1));
  TEST_EQ(obs1, val1);

  d = ObsData[1];
//  d->DisplayJson();
  TEST_TRUE(d.GetValue(d.GetDataPath(), obs2));
  TEST_EQ(obs2, val2);

  d = ObsData[2];
//  d.DisplayJson();
  TEST_TRUE(d.GetValue(d.GetDataPath(), obs3));
  TEST_EQ(obs3, val3);

  // Cleanup
  delete (MyData1);
  delete (MyData2);
  delete (MyData3);
  delete (MyConfig);

  // Return success
  return (0);

}

int
zConfigTest_ConfigurationLoadStore(void* arg_)
{
  ZLOG_DEBUG("#############################################################");
  ZLOG_DEBUG("# zConfigTest_ConfigurationLoadStore()");
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

  // Create new configuration data item and verify
  zConfig::Configuration *MyConfig = new zConfig::Configuration;
  TEST_ISNOT_NULL(MyConfig);
  TEST_FALSE(MyConfig->IsModified());

  // Create new configuration data connector and verify
  TestConnector *MyConnector = new TestConnector;
  TEST_ISNOT_NULL(MyConnector);

  // Register connector and verify
  TEST_TRUE(MyConfig->Connect(MyConnector));

  // Attempt to load configuration and verify
  TEST_FALSE(MyConfig->Load());

  // Attempt to store configuration and verify
  TEST_TRUE(MyConfig->Store());

  // Attempt to load configuration and verify
  TEST_TRUE(MyConfig->Load());

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
  TEST_FALSE(*ObsData == *ExpData);
  TEST_TRUE(*ObsData != *ExpData);

  // Put the data
  TEST_TRUE(MyConfig->Put(*ExpData));
//  MyConfig->Display();

  // Attempt to store configuration and verify
  TEST_TRUE(MyConfig->Store());

  // Attempt to load configuration and verify
  TEST_TRUE(MyConfig->Load());

  // Get data and verify
  TEST_TRUE(MyConfig->Get(*ObsData));

  // Verify data is not equal
  TEST_FALSE(*ObsData == *ExpData);
  TEST_TRUE(*ObsData != *ExpData);

  // Commit the data and verify
  TEST_TRUE(MyConfig->Put(*ExpData));
  TEST_TRUE(MyConfig->Commit());
//  MyConfig->Display();

  // Attempt to store configuration and verify
  TEST_TRUE(MyConfig->Store());

  // Attempt to load configuration and verify
  TEST_TRUE(MyConfig->Load());

  // Get data and verify
  TEST_TRUE(MyConfig->Get(*ObsData));

  // Verify data is equal
  TEST_TRUE(*ObsData == *ExpData);
  TEST_FALSE(*ObsData != *ExpData);

  // Cleanup
  MyConfig->Disconnect();
  delete (MyConnector);
  delete (MyConfig);
  delete (ObsData);
  delete (ExpData);

  // Return success
  return (0);

}

