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

  std::string ExpVal;
  std::string ObsVal;

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
  ExpVal = "Elvis";
  TEST_TRUE(ParentData.PutValue(FirstNamePath, ExpVal));
  TEST_TRUE(ParentData.GetValue(FirstNamePath, ObsVal));
  TEST_EQ(ObsVal, ExpVal);

  // Set second value and validate
  ExpVal = "Presley";
  TEST_TRUE(ParentData.PutValue(LastNamePath, ExpVal));
  TEST_TRUE(ParentData.GetValue(LastNamePath, ObsVal));
  TEST_EQ(ObsVal, ExpVal);
//  ParentData.DisplayJson();

  // Get the data object for the child "employee"
  zData::Data ChildData;
  TEST_TRUE(ParentData.GetChild(NamePath, ChildData));
  TEST_EQ(ChildData.Key(), NamePath.Key());
//  ChildData.DisplayPath();
//  ChildData.DisplayJson();

  // validate first value
  TEST_EQ(ParentPath.Key(), FirstNamePath.PopFront());
  ExpVal = "Elvis";
  TEST_TRUE(ChildData.GetValue(FirstNamePath, ObsVal));
  TEST_EQ(ObsVal, ExpVal);
  TEST_EQ(ParentData("Name").GetValue<std::string>(std::string("First")), ExpVal);

  // validate second value
  TEST_EQ(ParentPath.Key(), LastNamePath.PopFront());
  ExpVal = "Presley";
  TEST_TRUE(ChildData.GetValue(LastNamePath, ObsVal));
  TEST_EQ(ObsVal, ExpVal);
  TEST_EQ(ParentData("Name").GetValue<std::string>(std::string("Last")), ExpVal);

  // Return success
  return (0);

}

int
zDataTest_GetChildMultiple(void* arg_)
{

  ZLOG_DEBUG("#############################################################");
  ZLOG_DEBUG("# zDataTest_GetChildMultiple()");
  ZLOG_DEBUG("#############################################################");

  std::string ExpVal;
  std::string ObsVal;

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
  ExpVal = "Elvis";
  TEST_TRUE(ParentData.PutValue(FirstNamePath, ExpVal));
  TEST_TRUE(ParentData.GetValue(FirstNamePath, ObsVal));
  TEST_EQ(ObsVal, ExpVal);

  // Set second value and validate
  ExpVal = "Presley";
  TEST_TRUE(ParentData.PutValue(LastNamePath, ExpVal));
  TEST_TRUE(ParentData.GetValue(LastNamePath, ObsVal));
  TEST_EQ(ObsVal, ExpVal);

  // Set first value and validate
  ExpVal = "123 Some St.";
  TEST_TRUE(ParentData.PutValue(StreetAddressPath, ExpVal));
  TEST_TRUE(ParentData.GetValue(StreetAddressPath, ObsVal));
  TEST_EQ(ObsVal, ExpVal);

  // Set second value and validate
  ExpVal = "Colorado";
  TEST_TRUE(ParentData.PutValue(StateAddressPath, ExpVal));
  TEST_TRUE(ParentData.GetValue(StateAddressPath, ObsVal));
  TEST_EQ(ObsVal, ExpVal);

  // Get the data object for the child "employee"
  zData::Data NameData;
  TEST_TRUE(ParentData.GetChild(NamePath, NameData));
  TEST_EQ(NameData.Key(), NamePath.Key());
  zData::Data AddressData;
  TEST_TRUE(ParentData.GetChild(AddressPath, AddressData));
  TEST_EQ(AddressData.Key(), AddressPath.Key());

  // Adjust the child paths to account for parent removal
  TEST_EQ(ParentPath.Key(), NamePath.PopFront());
  TEST_EQ(ParentPath.Key(), FirstNamePath.PopFront());
  TEST_EQ(ParentPath.Key(), LastNamePath.PopFront());
  TEST_EQ(ParentPath.Key(), AddressPath.PopFront());
  TEST_EQ(ParentPath.Key(), StreetAddressPath.PopFront());
  TEST_EQ(ParentPath.Key(), StateAddressPath.PopFront());

  // validate first value
  ExpVal = "Elvis";
  TEST_TRUE(NameData.GetValue(FirstNamePath, ObsVal));
  TEST_EQ(ObsVal, ExpVal);
  TEST_EQ(ParentData("Name").GetValue<std::string>(std::string("First")), ExpVal);

  // validate second value
  ExpVal = "Presley";
  TEST_TRUE(NameData.GetValue(LastNamePath, ObsVal));
  TEST_EQ(ObsVal, ExpVal);
  TEST_EQ(ParentData("Name").GetValue<std::string>(std::string("Last")), ExpVal);

  // validate first value
  ExpVal = "123 Some St.";
  TEST_TRUE(AddressData.GetValue(StreetAddressPath, ObsVal));
  TEST_EQ(ObsVal, ExpVal);
  TEST_EQ(ParentData("Address").GetValue<std::string>(std::string("Street")), ExpVal);

  // validate second value
  ExpVal = "Colorado";
  TEST_TRUE(AddressData.GetValue(StateAddressPath, ObsVal));
  TEST_EQ(ObsVal, ExpVal);
  TEST_EQ(ParentData("Address").GetValue<std::string>(std::string("State")), ExpVal);

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
  zData::DataPath ParentPath("Employee");
  TEST_EQ(std::string("Employee"), ParentPath.Root());

  zData::DataPath NamePath("Name");
  TEST_EQ(std::string("Name"), NamePath.Root());

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
  TEST_TRUE(NameData.PutValue(FirstNamePath, expVal));
  TEST_TRUE(NameData.GetValue(FirstNamePath, obsVal));
  TEST_EQ(obsVal, expVal);

  // Set second value and validate
  expVal = "Presley";
  TEST_TRUE(NameData.PutValue(LastNamePath, expVal));
  TEST_TRUE(NameData.GetValue(LastNamePath, obsVal));
  TEST_EQ(obsVal, expVal);
//  NameData.DisplayJson();

  // Set the child and validate
  TEST_TRUE_MSG(ParentData.PutChild(NamePath, NameData), ParentData.GetJson());
  ParentData.DisplayJson();

  // Get child and validate
  zData::Data ObsData;
  TEST_TRUE(NamePath.Prepend("Employee"));
  TEST_TRUE_MSG(ParentData.GetChild(NamePath, ObsData), ParentData.GetJson());

  // Get first value and validate
  expVal = "Elvis";
  TEST_TRUE_MSG(ObsData.GetValue(FirstNamePath, obsVal), ObsData.GetJson());
  TEST_EQ(obsVal, expVal);
  TEST_TRUE(FirstNamePath.Prepend("Employee"));
  TEST_TRUE_MSG(ParentData.GetValue(FirstNamePath, obsVal), ParentData.GetJson());
  TEST_EQ(obsVal, expVal);

  // Get second value and validate
  expVal = "Presley";
  TEST_TRUE_MSG(ObsData.GetValue(LastNamePath, obsVal), ObsData.GetJson());
  TEST_EQ(obsVal, expVal);
  TEST_TRUE(LastNamePath.Prepend("Employee"));
  TEST_TRUE_MSG(ParentData.GetValue(LastNamePath, obsVal), ParentData.GetJson());
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
  TEST_TRUE(NameData.PutValue(FirstNamePath, expVal));
  TEST_TRUE(NameData.GetValue(FirstNamePath, obsVal));
  TEST_EQ(obsVal, expVal);

  // Set second value and validate
  expVal = "Presley";
  TEST_TRUE(NameData.PutValue(LastNamePath, expVal));
  TEST_TRUE(NameData.GetValue(LastNamePath, obsVal));
  TEST_EQ(obsVal, expVal);
//  NameData.DisplayJson();

  // Create child data object and validate
  zData::Data AddressData(AddressPath);
  TEST_EQ(AddressData.Key(), AddressPath.Key());

  // Set first value and validate
  expVal = "123 Some St.";
  TEST_TRUE(AddressData.PutValue(StreetAddressPath, expVal));
  TEST_TRUE(AddressData.GetValue(StreetAddressPath, obsVal));
  TEST_EQ(obsVal, expVal);

  // Set second value and validate
  expVal = "Colorado";
  TEST_TRUE(AddressData.PutValue(StateAddressPath, expVal));
  TEST_TRUE(AddressData.GetValue(StateAddressPath, obsVal));
  TEST_EQ(obsVal, expVal);
//  AddressData.DisplayJson();

  // Set the child and validate
  TEST_TRUE_MSG(ParentData.PutChild(NameData), ParentData.GetJson());
  TEST_TRUE_MSG(ParentData.PutChild(AddressData), ParentData.GetJson());
//  ParentData.DisplayJson();

  // Get first value and validate
  TEST_TRUE(FirstNamePath.Prepend("Employee"));
  expVal = "Elvis";
  TEST_TRUE_MSG(ParentData.GetValue(FirstNamePath, obsVal), ParentData.GetJson());
  TEST_EQ(obsVal, expVal);

  // Get second value and validate
  TEST_TRUE(LastNamePath.Prepend("Employee"));
  expVal = "Presley";
  TEST_TRUE_MSG(ParentData.GetValue(LastNamePath, obsVal), ParentData.GetJson());
  TEST_EQ(obsVal, expVal);

  // Get first value and validate
  TEST_TRUE(StreetAddressPath.Prepend("Employee"));
  expVal = "123 Some St.";
  TEST_TRUE_MSG(ParentData.GetValue(StreetAddressPath, obsVal), ParentData.GetJson());
  TEST_EQ(obsVal, expVal);

  // Get second value and validate
  TEST_TRUE(StateAddressPath.Prepend("Employee"));
  expVal = "Colorado";
  TEST_TRUE_MSG(ParentData.GetValue(StateAddressPath, obsVal), ParentData.GetJson());
  TEST_EQ(obsVal, expVal);

  // Return success
  return (0);

}

