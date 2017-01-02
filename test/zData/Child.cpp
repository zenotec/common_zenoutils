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

int
zDataTest_GetChildSingle(void* arg_)
{

  ZLOG_DEBUG("#############################################################");
  ZLOG_DEBUG("# zDataTest_GetChildSingle()");
  ZLOG_DEBUG("#############################################################");

  std::string expVal;
  std::string obsVal;

  // Create data paths and validate
  zData::DataPath ParentPath;
  TEST_TRUE(ParentPath.Append("Employee"));

  zData::DataPath NamePath(ParentPath);
  TEST_TRUE(NamePath.Append("Name"));

  zData::DataPath FirstNamePath(NamePath);
  TEST_TRUE(FirstNamePath.Append("First"));

  zData::DataPath LastNamePath(NamePath);
  TEST_TRUE(LastNamePath.Append("Last"));

  // Create new data object and validate
  zData::Data ParentData(ParentPath);
  TEST_EQ(ParentData.Key(), ParentPath.Key());

  // Set first value and validate
  expVal = "Elvis";
  TEST_TRUE(ParentData.Put(FirstNamePath, expVal));
  TEST_TRUE(ParentData.Get(FirstNamePath, obsVal));
  TEST_EQ(obsVal, expVal);

  // Set second value and validate
  expVal = "Presley";
  TEST_TRUE(ParentData.Put(LastNamePath, expVal));
  TEST_TRUE(ParentData.Get(LastNamePath, obsVal));
  TEST_EQ(obsVal, expVal);
//  ParentData.DisplayJson();

  // Get the data object for the child "employee"
  zData::Data ChildData;
  TEST_TRUE(ParentData.Get(NamePath, ChildData));
  TEST_EQ(ChildData.Key(), NamePath.Key());
//  ChildData.DisplayPath();
//  ChildData.DisplayJson();

  // validate first value
  TEST_EQ(ParentPath.Key(), FirstNamePath.PopFront());
  expVal = "Elvis";
  TEST_TRUE(ChildData.Get(FirstNamePath, obsVal));
  TEST_EQ(obsVal, expVal);

  // validate second value
  TEST_EQ(ParentPath.Key(), LastNamePath.PopFront());
  expVal = "Presley";
  TEST_TRUE(ChildData.Get(LastNamePath, obsVal));
  TEST_EQ(obsVal, expVal);

  // Return success
  return (0);

}

int
zDataTest_GetChildMultiple(void* arg_)
{

  ZLOG_DEBUG("#############################################################");
  ZLOG_DEBUG("# zDataTest_GetChildMultiple()");
  ZLOG_DEBUG("#############################################################");

  std::string expVal;
  std::string obsVal;

  // Create data paths and validate
  zData::DataPath ParentPath;
  TEST_TRUE(ParentPath.Append("Employee"));

  zData::DataPath NamePath;
  TEST_TRUE(NamePath.Append("Employee"));
  TEST_TRUE(NamePath.Append("Name"));

  zData::DataPath FirstNamePath;
  TEST_TRUE(FirstNamePath.Append("Employee"));
  TEST_TRUE(FirstNamePath.Append("Name"));
  TEST_TRUE(FirstNamePath.Append("First"));

  zData::DataPath LastNamePath;
  TEST_TRUE(LastNamePath.Append("Employee"));
  TEST_TRUE(LastNamePath.Append("Name"));
  TEST_TRUE(LastNamePath.Append("Last"));

  zData::DataPath AddressPath;
  TEST_TRUE(AddressPath.Append("Employee"));
  TEST_TRUE(AddressPath.Append("Address"));

  zData::DataPath StreetAddressPath;
  TEST_TRUE(StreetAddressPath.Append("Employee"));
  TEST_TRUE(StreetAddressPath.Append("Address"));
  TEST_TRUE(StreetAddressPath.Append("Street"));

  zData::DataPath StateAddressPath;
  TEST_TRUE(StateAddressPath.Append("Employee"));
  TEST_TRUE(StateAddressPath.Append("Address"));
  TEST_TRUE(StateAddressPath.Append("State"));

  // Create new data object and validate
  zData::Data ParentData(ParentPath);
  TEST_EQ(ParentData.Key(), ParentPath.Key());

  // Set first value and validate
  expVal = "Elvis";
  TEST_TRUE(ParentData.Put(FirstNamePath, expVal));
  TEST_TRUE(ParentData.Get(FirstNamePath, obsVal));
  TEST_EQ(obsVal, expVal);

  // Set second value and validate
  expVal = "Presley";
  TEST_TRUE(ParentData.Put(LastNamePath, expVal));
  TEST_TRUE(ParentData.Get(LastNamePath, obsVal));
  TEST_EQ(obsVal, expVal);

  // Set first value and validate
  expVal = "123 Some St.";
  TEST_TRUE(ParentData.Put(StreetAddressPath, expVal));
  TEST_TRUE(ParentData.Get(StreetAddressPath, obsVal));
  TEST_EQ(obsVal, expVal);

  // Set second value and validate
  expVal = "Colorado";
  TEST_TRUE(ParentData.Put(StateAddressPath, expVal));
  TEST_TRUE(ParentData.Get(StateAddressPath, obsVal));
  TEST_EQ(obsVal, expVal);

  // Get the data object for the child "employee"
  zData::Data NameData;
  TEST_TRUE(ParentData.Get(NamePath, NameData));
  TEST_EQ(NameData.Key(), NamePath.Key());
  zData::Data AddressData;
  TEST_TRUE(ParentData.Get(AddressPath, AddressData));
  TEST_EQ(AddressData.Key(), AddressPath.Key());

  // Adjust the child paths to account for parent removal
  TEST_EQ(ParentPath.Key(), NamePath.PopFront());
  TEST_EQ(ParentPath.Key(), FirstNamePath.PopFront());
  TEST_EQ(ParentPath.Key(), LastNamePath.PopFront());
  TEST_EQ(ParentPath.Key(), AddressPath.PopFront());
  TEST_EQ(ParentPath.Key(), StreetAddressPath.PopFront());
  TEST_EQ(ParentPath.Key(), StateAddressPath.PopFront());

  // validate first value
  expVal = "Elvis";
  TEST_TRUE(NameData.Get(FirstNamePath, obsVal));
  TEST_EQ(obsVal, expVal);

  // validate second value
  expVal = "Presley";
  TEST_TRUE(NameData.Get(LastNamePath, obsVal));
  TEST_EQ(obsVal, expVal);

  // validate first value
  expVal = "123 Some St.";
  TEST_TRUE(AddressData.Get(StreetAddressPath, obsVal));
  TEST_EQ(obsVal, expVal);

  // validate second value
  expVal = "Colorado";
  TEST_TRUE(AddressData.Get(StateAddressPath, obsVal));
  TEST_EQ(obsVal, expVal);

  // Return success
  return (0);

}

int
zDataTest_PutChildSingle(void* arg_)
{

  ZLOG_DEBUG("#############################################################");
  ZLOG_DEBUG("# zDataTest_PutChildSingle()");
  ZLOG_DEBUG("#############################################################");

  std::string expVal;
  std::string obsVal;

  // Create data paths and validate
  zData::DataPath ParentPath;
  TEST_TRUE(ParentPath.Append("Employee"));

  zData::DataPath NamePath;
  TEST_TRUE(NamePath.Append("Name"));

  zData::DataPath FirstNamePath(NamePath);
  TEST_TRUE(FirstNamePath.Append("First"));

  zData::DataPath LastNamePath(NamePath);
  TEST_TRUE(LastNamePath.Append("Last"));

  // Create parent data object and validate
  zData::Data ParentData(ParentPath);
  TEST_EQ(ParentData.Key(), ParentPath.Key());

  // Create child data object and validate
  zData::Data NameData(NamePath);
  TEST_EQ(NameData.Key(), NamePath.Key());

  // Set first value and validate
  expVal = "Elvis";
  TEST_TRUE(NameData.Put(FirstNamePath, expVal));
  TEST_TRUE(NameData.Get(FirstNamePath, obsVal));
  TEST_EQ(obsVal, expVal);

  // Set second value and validate
  expVal = "Presley";
  TEST_TRUE(NameData.Put(LastNamePath, expVal));
  TEST_TRUE(NameData.Get(LastNamePath, obsVal));
  TEST_EQ(obsVal, expVal);
//  NameData.DisplayJson();

  // Set the child and validate
  TEST_TRUE_MSG(ParentData.Put(NameData), ParentData.GetJson());
//  ParentData.DisplayJson();

  // Get child and validate
  zData::Data ObsData;
  TEST_TRUE(NamePath.Prepend("Employee"));
  TEST_TRUE_MSG(ParentData.Get(NamePath, ObsData), ParentData.GetJson());

  // Get first value and validate
  expVal = "Elvis";
  TEST_TRUE_MSG(ObsData.Get(FirstNamePath, obsVal), ObsData.GetJson());
  TEST_EQ(obsVal, expVal);
  TEST_TRUE(FirstNamePath.Prepend("Employee"));
  TEST_TRUE_MSG(ParentData.Get(FirstNamePath, obsVal), ParentData.GetJson());
  TEST_EQ(obsVal, expVal);

  // Get second value and validate
  expVal = "Presley";
  TEST_TRUE_MSG(ObsData.Get(LastNamePath, obsVal), ObsData.GetJson());
  TEST_EQ(obsVal, expVal);
  TEST_TRUE(LastNamePath.Prepend("Employee"));
  TEST_TRUE_MSG(ParentData.Get(LastNamePath, obsVal), ParentData.GetJson());
  TEST_EQ(obsVal, expVal);

  // Return success
  return (0);

}

int
zDataTest_PutChildMultiple(void* arg_)
{

  ZLOG_DEBUG("#############################################################");
  ZLOG_DEBUG("# zDataTest_PutChildMultiple()");
  ZLOG_DEBUG("#############################################################");

  std::string expVal;
  std::string obsVal;

  // Create data paths and validate
  zData::DataPath ParentPath("Employee");

  zData::DataPath NamePath;
  TEST_TRUE(NamePath.Append("Name"));

  zData::DataPath FirstNamePath(NamePath);
  TEST_TRUE(FirstNamePath.Append("First"));

  zData::DataPath LastNamePath(NamePath);
  TEST_TRUE(LastNamePath.Append("Last"));

  zData::DataPath AddressPath;
  TEST_TRUE(AddressPath.Append("Address"));

  zData::DataPath StreetAddressPath(AddressPath);
  TEST_TRUE(StreetAddressPath.Append("Street"));

  zData::DataPath StateAddressPath(AddressPath);
  TEST_TRUE(StateAddressPath.Append("State"));

  // Create parent data object and validate
  zData::Data ParentData(ParentPath);
  TEST_EQ(ParentData.Key(), ParentPath.Key());

  // Create child data object and validate
  zData::Data NameData(NamePath);
  TEST_EQ(NameData.Key(), NamePath.Key());

  // Set first value and validate
  expVal = "Elvis";
  TEST_TRUE(NameData.Put(FirstNamePath, expVal));
  TEST_TRUE(NameData.Get(FirstNamePath, obsVal));
  TEST_EQ(obsVal, expVal);

  // Set second value and validate
  expVal = "Presley";
  TEST_TRUE(NameData.Put(LastNamePath, expVal));
  TEST_TRUE(NameData.Get(LastNamePath, obsVal));
  TEST_EQ(obsVal, expVal);
//  NameData.DisplayJson();

  // Create child data object and validate
  zData::Data AddressData(AddressPath);
  TEST_EQ(AddressData.Key(), AddressPath.Key());

  // Set first value and validate
  expVal = "123 Some St.";
  TEST_TRUE(AddressData.Put(StreetAddressPath, expVal));
  TEST_TRUE(AddressData.Get(StreetAddressPath, obsVal));
  TEST_EQ(obsVal, expVal);

  // Set second value and validate
  expVal = "Colorado";
  TEST_TRUE(AddressData.Put(StateAddressPath, expVal));
  TEST_TRUE(AddressData.Get(StateAddressPath, obsVal));
  TEST_EQ(obsVal, expVal);
//  AddressData.DisplayJson();

  // Set the child and validate
  TEST_TRUE_MSG(ParentData.Put(NameData), ParentData.GetJson());
  TEST_TRUE_MSG(ParentData.Put(AddressData), ParentData.GetJson());
//  ParentData.DisplayJson();

  // Get first value and validate
  TEST_TRUE(FirstNamePath.Prepend("Employee"));
  expVal = "Elvis";
  TEST_TRUE_MSG(ParentData.Get(FirstNamePath, obsVal), ParentData.GetJson());
  TEST_EQ(obsVal, expVal);

  // Get second value and validate
  TEST_TRUE(LastNamePath.Prepend("Employee"));
  expVal = "Presley";
  TEST_TRUE_MSG(ParentData.Get(LastNamePath, obsVal), ParentData.GetJson());
  TEST_EQ(obsVal, expVal);

  // Get first value and validate
  TEST_TRUE(StreetAddressPath.Prepend("Employee"));
  expVal = "123 Some St.";
  TEST_TRUE_MSG(ParentData.Get(StreetAddressPath, obsVal), ParentData.GetJson());
  TEST_EQ(obsVal, expVal);

  // Get second value and validate
  TEST_TRUE(StateAddressPath.Prepend("Employee"));
  expVal = "Colorado";
  TEST_TRUE_MSG(ParentData.Get(StateAddressPath, obsVal), ParentData.GetJson());
  TEST_EQ(obsVal, expVal);

  // Return success
  return (0);

}

