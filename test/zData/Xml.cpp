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
zDataTest_XmlSimple(void* arg_)
{

  ZLOG_DEBUG("#############################################################");
  ZLOG_DEBUG("# zDataTest_XmlSimple()");
  ZLOG_DEBUG("#############################################################");

  std::string key("TestKey");
  std::string name("TestName");
  std::string val("TestValue");
  std::string obs;

  // Create new data object and validate
  zData::Data MyData1(key);
  TEST_EQ(MyData1.Key(), key);
  TEST_EQ(MyData1.GetJson(),
      std::string("{\n    \"Key\": \"TestKey\",\n    \"TestKey\": \"\"\n}\n"));
  TEST_EQ(MyData1.GetXml(),
      std::string(
          "<?xml version=\"1.0\" encoding=\"utf-8\"?>\n<Key>TestKey</Key><TestKey/>"));

  // Set value and validate
  TEST_TRUE(MyData1.Put(val, name));
  TEST_TRUE(MyData1.Get(obs, name));
  TEST_EQ(obs, val);

  // Create second empty data object and validate
  zData::Data MyData2;
  TEST_EQ(MyData2.Key(), "");
  TEST_EQ(MyData2.GetJson(), std::string("{\n    \"Key\": \"\"\n}\n"));
  TEST_EQ(MyData2.GetXml(),
      std::string("<?xml version=\"1.0\" encoding=\"utf-8\"?>\n<Key/>"));

  // Set the second data object from the first using only XML
  MyData2.SetXml(MyData1.GetXml());
  TEST_EQ(MyData2.Key(), key);
  TEST_TRUE(MyData2.Get(obs, name));
  TEST_EQ(obs, val);
  TEST_EQ(MyData2.GetJson(), MyData1.GetJson());
  TEST_EQ(MyData2.GetXml(), MyData1.GetXml());

  // Return success
  return (0);

}

