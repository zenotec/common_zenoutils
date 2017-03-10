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

int
zDataTest_JsonSimple(void* arg_)
{

  ZLOG_DEBUG("#############################################################");
  ZLOG_DEBUG("# zDataTest_JsonSimple()");
  ZLOG_DEBUG("#############################################################");

  std::string val("TestValue");
  std::string obs;

  zData::DataPath MyPath1;
  TEST_EQ(zData::DataPath::DataRoot, MyPath1.Root());
  TEST_EQ(zData::DataPath::DataRoot, MyPath1.Base());
  TEST_EQ(std::string(""), MyPath1.Key());
  TEST_EQ(zData::DataPath::DataRoot, MyPath1.Path());

  zData::DataPath MyPath2("TestKey");
  TEST_EQ((zData::DataPath::DataRoot + std::string(".TestKey")), MyPath2.Root());
  TEST_EQ((zData::DataPath::DataRoot + std::string(".TestKey")), MyPath2.Base());
  TEST_EQ(std::string(""), MyPath2.Key());
  TEST_EQ((zData::DataPath::DataRoot + std::string(".TestKey")), MyPath2.Path());

  // Create new data object and validate
  zData::Data MyData1(MyPath1);
  TEST_EQ(MyData1.Key(), MyPath1.Key());
  TEST_EQ(MyData1.GetJson(), std::string("{\"zData\":\"\"}\n"));
  TEST_EQ(MyData1.GetXml(), std::string("<?xml version=\"1.0\" encoding=\"utf-8\"?>\n<zData/>"));

  // Set value and validate
  TEST_TRUE(MyData1.PutValue(MyPath1, val));
  TEST_TRUE(MyData1.GetValue(MyPath1, obs));
  TEST_EQ(obs, val);

  // Create second empty data object and validate
  zData::Data MyData2(MyPath2);
  TEST_EQ(MyData2.Key(), MyPath2.Key());
  TEST_EQ(MyData2.GetJson(), std::string("{\"zData\":{\"TestKey\":\"\"}}\n"));
  TEST_EQ(MyData2.GetXml(),
      std::string("<?xml version=\"1.0\" encoding=\"utf-8\"?>\n<zData><TestKey/></zData>"));

  // Set the second data object from the first using only JSON
  TEST_TRUE(MyData2.SetJson(MyData1.GetJson()));
//  TEST_EQ(MyData2.Key(), MyPath1.Key());
  TEST_TRUE(MyData1.GetValue(MyPath1, obs));
  TEST_EQ(obs, val);
  TEST_EQ(MyData2.GetJson(), MyData1.GetJson());
  TEST_EQ(MyData2.GetXml(), MyData1.GetXml());

  // Return success
  return (0);

}
