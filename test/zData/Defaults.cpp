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
zDataTest_Defaults(void* arg)
{

  ZLOG_DEBUG("#############################################################");
  ZLOG_DEBUG("# zDataTest_Defaults()");
  ZLOG_DEBUG("#############################################################");

  std::string key;
  zData::Data *MyData;

  // Create new data object and validate
  key = "";
  MyData = new zData::Data;
  TEST_EQ(MyData->Key(), key);
  TEST_EQ(MyData->GetJson(), std::string("{\n    \"Key\": \"\"\n}\n"));
  TEST_EQ(MyData->GetXml(),
      std::string("<?xml version=\"1.0\" encoding=\"utf-8\"?>\n<Key/>"));
  delete (MyData);

  // Create new data object and validate
  key = "TestKey";
  MyData = new zData::Data(key);
  TEST_EQ(MyData->Key(), key);
  TEST_EQ(MyData->GetJson(),
      std::string("{\n    \"Key\": \"TestKey\",\n    \"TestKey\": \"\"\n}\n"));
  TEST_EQ(MyData->GetXml(),
      std::string(
          "<?xml version=\"1.0\" encoding=\"utf-8\"?>\n<Key>TestKey</Key><TestKey/>"));
  delete (MyData);

  // Return success
  return (0);

}

