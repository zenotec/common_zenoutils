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
zDataTest_GetChild(void* arg_)
{

  ZLOG_DEBUG("#############################################################");
  ZLOG_DEBUG("# zDataTest_GetChild()");
  ZLOG_DEBUG("#############################################################");

  std::string FirstName = "Elvis";
  std::string LastName = "Presley";
  std::string Street = "1034 Audubon Drive";
  std::string City = "Memphis";
  std::string State = "TN";
  std::string ObsVal;
  zData::DataPath Path;

  // Create data paths and validate
  zData::DataPath ParentPath("Employee");

  zData::DataPath NamePath;
  TEST_TRUE(NamePath.Append("Name"));

  zData::DataPath FirstNamePath;
  TEST_TRUE(FirstNamePath.Append("First"));

  zData::DataPath LastNamePath;
  TEST_TRUE(LastNamePath.Append("Last"));

  zData::DataPath AddressPath;
  TEST_TRUE(AddressPath.Append("Address"));

  zData::DataPath StreetPath;
  TEST_TRUE(StreetPath.Append("Street"));

  zData::DataPath CityPath;
  TEST_TRUE(CityPath.Append("City"));

  zData::DataPath StatePath;
  TEST_TRUE(StatePath.Append("State"));

  // Create parent data object and validate
  zData::Data ParentData(ParentPath);
  TEST_EQ(ParentData.Key(), ParentPath.Key());

  // Set first value and validate
  Path = ParentPath + NamePath + FirstNamePath;
  TEST_TRUE(ParentData.PutValue(Path, FirstName));
  TEST_TRUE(ParentData.GetValue(Path, ObsVal));
  TEST_EQ(ObsVal, FirstName);
//  ParentData.DisplayPath();
//  ParentData.DisplayJson();

  // Set second value and validate
  Path = ParentPath + NamePath + LastNamePath;
  TEST_TRUE(ParentData.PutValue(Path, LastName));
  TEST_TRUE(ParentData.GetValue(Path, ObsVal));
  TEST_EQ(ObsVal, LastName);
//  ParentData.DisplayPath();
//  ParentData.DisplayJson();

  // Set Street value and validate
  Path = ParentPath + AddressPath + StreetPath;
  TEST_TRUE(ParentData.PutValue(Path, Street));
  TEST_TRUE(ParentData.GetValue(Path, ObsVal));
  TEST_EQ(ObsVal, Street);
//  ParentData.DisplayPath();
//  ParentData.DisplayJson();

  // Set fourth value and validate
  Path = ParentPath + AddressPath + CityPath;
  TEST_TRUE(ParentData.PutValue(Path, City));
  TEST_TRUE(ParentData.GetValue(Path, ObsVal));
  TEST_EQ(ObsVal, City);
//  ParentData.DisplayPath();
//  ParentData.DisplayJson();

  /**************************************************************************************
   * GetChild(src = root, dst = root)
   *************************************************************************************/

  // Get the root data object from the parent, copy to the root of the child
  zData::Data ChildData;
  TEST_TRUE(ParentData.GetChild(ChildData));
//  ChildData.DisplayPath();
//  ChildData.DisplayJson();

  // validate first name
  Path = NamePath + FirstNamePath;
  TEST_TRUE(ChildData.GetValue(Path, ObsVal));
  TEST_EQ(ObsVal, FirstName);
  TEST_EQ(ParentData("Name").GetValue<std::string>(std::string("First")), FirstName);

  // validate last name
  Path = NamePath + LastNamePath;
  TEST_TRUE(ChildData.GetValue(Path, ObsVal));
  TEST_EQ(ObsVal, LastName);
  TEST_EQ(ParentData("Name").GetValue<std::string>(std::string("Last")), LastName);

  // validate street
  Path = AddressPath + StreetPath;
  TEST_TRUE(ChildData.GetValue(Path, ObsVal));
  TEST_EQ(ObsVal, Street);
  TEST_EQ(ParentData("Address").GetValue<std::string>(std::string("Street")), Street);

  // validate city
  Path = AddressPath + CityPath;
  TEST_TRUE(ChildData.GetValue(Path, ObsVal));
  TEST_EQ(ObsVal, City);
  TEST_EQ(ParentData("Address").GetValue<std::string>(std::string("City")), City);

  /**************************************************************************************
   * GetChild(src = path, dst = root)
   *************************************************************************************/

  // Get "name" child
  Path = ParentPath + NamePath;
  TEST_TRUE(ParentData.GetChild(Path, ChildData));
//  ChildData.DisplayPath();
//  ChildData.DisplayJson();

  // validate first value
  TEST_TRUE(ChildData.GetValue(FirstNamePath, ObsVal));
  TEST_EQ(ObsVal, FirstName);

  // validate second value
  TEST_TRUE(ChildData.GetValue(LastNamePath, ObsVal));
  TEST_EQ(ObsVal, LastName);

  // Get "address" child
  Path = ParentPath + AddressPath;
  TEST_TRUE(ParentData.GetChild(Path, ChildData));
//  ChildData.DisplayPath();
//  ChildData.DisplayJson();

  // validate third value
  TEST_TRUE(ChildData.GetValue(StreetPath, ObsVal));
  TEST_EQ(ObsVal, Street);
  TEST_EQ(ParentData("Address").GetValue<std::string>(std::string("Street")), Street);

  // validate fourth value
  TEST_TRUE(ChildData.GetValue(CityPath, ObsVal));
  TEST_EQ(ObsVal, City);
  TEST_EQ(ParentData("Address").GetValue<std::string>(std::string("City")), City);

  /**************************************************************************************
   * GetChild(src = path, dst = path)
   *************************************************************************************/

  // Get "name" child
  Path = ParentPath + NamePath;
  TEST_TRUE(ParentData.GetChild(Path, NamePath, ChildData));
//  ChildData.DisplayPath();
//  ChildData.DisplayJson();

  // validate first value
  Path = NamePath + FirstNamePath;
  TEST_TRUE(ChildData.GetValue(Path, ObsVal));
  TEST_EQ(ObsVal, FirstName);

  // validate second value
  Path = NamePath + LastNamePath;
  TEST_TRUE(ChildData.GetValue(Path, ObsVal));
  TEST_EQ(ObsVal, LastName);

  // Get "address" child
  Path = ParentPath + AddressPath;
  TEST_TRUE(ParentData.GetChild(Path, AddressPath, ChildData));
//  ChildData.DisplayPath();
//  ChildData.DisplayJson();

  // validate third value
  Path = AddressPath + StreetPath;
  TEST_TRUE(ChildData.GetValue(Path, ObsVal));
  TEST_EQ(ObsVal, Street);
  TEST_EQ(ParentData("Address").GetValue<std::string>(std::string("Street")), Street);

  // validate fourth value
  Path = AddressPath + CityPath;
  TEST_TRUE(ChildData.GetValue(Path, ObsVal));
  TEST_EQ(ObsVal, City);
  TEST_EQ(ParentData("Address").GetValue<std::string>(std::string("City")), City);

  // Return success
  return (0);

}

int
zDataTest_PutChild(void* arg_)
{

  ZLOG_DEBUG("#############################################################");
  ZLOG_DEBUG("# zDataTest_PutChild()");
  ZLOG_DEBUG("#############################################################");

  std::string FirstName = "Elvis";
  std::string LastName = "Presley";
  std::string Street = "1034 Audubon Drive";
  std::string City = "Memphis";
  std::string State = "TN";
  std::string ObsVal;
  zData::DataPath Path;

  // Create data paths and validate
  zData::DataPath ParentPath("Employee");

  zData::DataPath NamePath;
  TEST_TRUE(NamePath.Append("Name"));

  zData::DataPath FirstNamePath;
  TEST_TRUE(FirstNamePath.Append("First"));

  zData::DataPath LastNamePath;
  TEST_TRUE(LastNamePath.Append("Last"));

  zData::DataPath AddressPath;
  TEST_TRUE(AddressPath.Append("Address"));

  zData::DataPath StreetPath;
  TEST_TRUE(StreetPath.Append("Street"));

  zData::DataPath CityPath;
  TEST_TRUE(CityPath.Append("City"));

  zData::DataPath StatePath;
  TEST_TRUE(StatePath.Append("State"));

  // Create parent data object and validate
  zData::Data ParentData(ParentPath);
  TEST_EQ(ParentData.Key(), ParentPath.Key());

  // Create name data object and validate
  zData::Data NameData(NamePath);
  TEST_EQ(NameData.Key(), NamePath.Key());

  // Set first name and validate
  Path = NamePath + FirstNamePath;
  TEST_TRUE(NameData.PutValue(Path, FirstName));
  TEST_TRUE(NameData.GetValue(Path, ObsVal));
  TEST_EQ(ObsVal, FirstName);
//  NameData.DisplayPath();
//  NameData.DisplayJson();

  // Set last name and validate
  Path = NamePath + LastNamePath;
  TEST_TRUE(NameData.PutValue(Path, LastName));
  TEST_TRUE(NameData.GetValue(Path, ObsVal));
  TEST_EQ(ObsVal, LastName);
//  NameData.DisplayPath();
//  NameData.DisplayJson();

  // Create address data object and validate
  zData::Data AddressData(AddressPath);
  TEST_EQ(AddressData.Key(), AddressPath.Key());

  // Set street and validate
  Path = AddressPath + StreetPath;
  TEST_TRUE(AddressData.PutValue(Path, Street));
  TEST_TRUE(AddressData.GetValue(Path, ObsVal));
  TEST_EQ(ObsVal, Street);
//  AddressData.DisplayPath();
//  AddressData.DisplayJson();

  // Set city and validate
  Path = AddressPath + CityPath;
  TEST_TRUE(AddressData.PutValue(Path, City));
  TEST_TRUE(AddressData.GetValue(Path, ObsVal));
  TEST_EQ(ObsVal, City);
//  AddressData.DisplayPath();
//  AddressData.DisplayJson();

  /**************************************************************************************
   * PutChild (src = root, dst = root)
   *************************************************************************************/

  // Put the child and validate
  TEST_TRUE_MSG(ParentData.PutChild(NameData), ParentData.GetJson());
  TEST_TRUE_MSG(ParentData.PutChild(AddressData), ParentData.GetJson());
//  ParentData.DisplayPath();
//  ParentData.DisplayJson();

  // validate first name
  Path = ParentPath + NamePath + FirstNamePath;
  TEST_FALSE(ParentData.GetValue(Path, ObsVal));

  // validate last name
  Path = ParentPath + NamePath + LastNamePath;
  TEST_FALSE(ParentData.GetValue(Path, ObsVal));

  // validate street
  Path = ParentPath + AddressPath + StreetPath;
  TEST_TRUE(ParentData.GetValue(Path, ObsVal));
  TEST_EQ(ObsVal, Street);

  // validate city
  Path = ParentPath + AddressPath + CityPath;
  TEST_TRUE(ParentData.GetValue(Path, ObsVal));
  TEST_EQ(ObsVal, City);

  /**************************************************************************************
   * PutChild (src = root, dst = root) (reversed order)
   *************************************************************************************/

  // Put the child and validate
  TEST_TRUE_MSG(ParentData.PutChild(AddressData), ParentData.GetJson());
  TEST_TRUE_MSG(ParentData.PutChild(NameData), ParentData.GetJson());
//  ParentData.DisplayPath();
//  ParentData.DisplayJson();

  // validate first name
  Path = ParentPath + NamePath + FirstNamePath;
  TEST_TRUE(ParentData.GetValue(Path, ObsVal));
  TEST_EQ(ObsVal, FirstName);

  // validate last name
  Path = ParentPath + NamePath + LastNamePath;
  TEST_TRUE(ParentData.GetValue(Path, ObsVal));
  TEST_EQ(ObsVal, LastName);

  // validate street
  Path = ParentPath + AddressPath + StreetPath;
  TEST_FALSE(ParentData.GetValue(Path, ObsVal));

  // validate city
  Path = ParentPath + AddressPath + CityPath;
  TEST_FALSE(ParentData.GetValue(Path, ObsVal));

  // Return success
  return (0);

}


int
zDataTest_AddChild(void* arg_)
{

  ZLOG_DEBUG("#############################################################");
  ZLOG_DEBUG("# zDataTest_AddChild()");
  ZLOG_DEBUG("#############################################################");

  std::string FirstName[2] = { "Bud", "Lou" };
  std::string LastName[2] = { "Abbott", "Costello" };
  std::string ObsVal;
  zData::DataPath Path;
  zData::Data Data;

  // Create data paths and validate
  zData::DataPath ParentPath("Employee");

  zData::DataPath NamePath;
  TEST_TRUE(NamePath.Append("Name"));

  zData::DataPath FirstNamePath;
  TEST_TRUE(FirstNamePath.Append("First"));

  zData::DataPath LastNamePath;
  TEST_TRUE(LastNamePath.Append("Last"));

  // Create parent data object and validate
  zData::Data ParentData(ParentPath);
  TEST_EQ(ParentData.Key(), ParentPath.Key());

  // Create name data object and validate
  zData::Data NameData[2](NamePath);
  TEST_EQ(NameData[0].Key(), NamePath.Key());
  TEST_EQ(NameData[1].Key(), NamePath.Key());

  // Set first name and validate
  Path = NamePath + FirstNamePath;
  TEST_TRUE(NameData[0].PutValue(Path, FirstName[0]));
  TEST_TRUE(NameData[0].GetValue(Path, ObsVal));
  TEST_EQ(ObsVal, FirstName[0]);
//  NameData[0].DisplayPath();
//  NameData[0].DisplayJson();
  TEST_TRUE(NameData[1].PutValue(Path, FirstName[1]));
  TEST_TRUE(NameData[1].GetValue(Path, ObsVal));
  TEST_EQ(ObsVal, FirstName[1]);
//  NameData[1].DisplayPath();
//  NameData[1].DisplayJson();

  // Set last name and validate
  Path = NamePath + LastNamePath;
  TEST_TRUE(NameData[0].PutValue(Path, LastName[0]));
  TEST_TRUE(NameData[0].GetValue(Path, ObsVal));
  TEST_EQ(ObsVal, LastName[0]);
//  NameData[0].DisplayPath();
//  NameData[0].DisplayJson();
  TEST_TRUE(NameData[1].PutValue(Path, LastName[1]));
  TEST_TRUE(NameData[1].GetValue(Path, ObsVal));
  TEST_EQ(ObsVal, LastName[1]);
//  NameData[1].DisplayPath();
//  NameData[1].DisplayJson();

  /**************************************************************************************
   * AddChild (src = root, dst = root)
   *************************************************************************************/

  // Put the child and validate
  TEST_TRUE_MSG(ParentData.AddChild(NameData[0]), ParentData.GetJson());
  TEST_TRUE_MSG(ParentData.AddChild(NameData[1]), ParentData.GetJson());
//  ParentData.DisplayPath();
//  ParentData.DisplayJson();

  // Get first child
  Data = ParentData[0];

  // validate first name
  Path = NamePath + FirstNamePath;
  TEST_TRUE(Data.GetValue(Path, ObsVal));
  TEST_EQ(ObsVal, FirstName[0]);

  // validate last name
  Path = NamePath + LastNamePath;
  TEST_TRUE(Data.GetValue(Path, ObsVal));
  TEST_EQ(ObsVal, LastName[0]);

  // Get second child
  Data = ParentData[1];

  // validate first name
  Path = NamePath + FirstNamePath;
  TEST_TRUE(Data.GetValue(Path, ObsVal));
  TEST_EQ(ObsVal, FirstName[1]);

  // validate last name
  Path = NamePath + LastNamePath;
  TEST_TRUE(Data.GetValue(Path, ObsVal));
  TEST_EQ(ObsVal, LastName[1]);

  // Return success
  return (0);

}

