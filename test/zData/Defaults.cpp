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

#include "zDataTest.h"

using namespace zUtils;

int
zDataTest_PathDefaults(void* arg)
{

  ZLOG_DEBUG("#############################################################");
  ZLOG_DEBUG("# zDataTest_PathDefaults()");
  ZLOG_DEBUG("#############################################################");

  zData::DataPath MyPath1;
  TEST_EQ(zData::DataPath::DataRoot, MyPath1.Root());
  TEST_EQ(zData::DataPath::DataRoot, MyPath1.Base());
  TEST_EQ(std::string(""), MyPath1.Key());
  TEST_EQ(zData::DataPath::DataRoot, MyPath1.Path());

  zData::DataPath MyPath2("key");
  TEST_EQ((zData::DataPath::DataRoot + std::string(".key")), MyPath2.Root());
  TEST_EQ((zData::DataPath::DataRoot + std::string(".key")), MyPath2.Base());
  TEST_EQ(std::string(""), MyPath2.Key());
  TEST_EQ((zData::DataPath::DataRoot + std::string(".key")), MyPath2.Path());

  // Return success
  return (0);
}

int
zDataTest_Defaults(void* arg)
{

  ZLOG_DEBUG("#############################################################");
  ZLOG_DEBUG("# zDataTest_Defaults()");
  ZLOG_DEBUG("#############################################################");

  zData::Data *MyData;

  // Create new data object and validate
  MyData = new zData::Data;
  TEST_EQ(MyData->GetJson(), std::string("{\"zData\":\"\"}\n"));
  TEST_EQ(MyData->GetXml(),
      std::string("<?xml version=\"1.0\" encoding=\"utf-8\"?>\n<zData/>"));
  delete (MyData);

  // Create new data object and validate
  MyData = new zData::Data("TestKey");
  TEST_EQ(MyData->GetJson(), std::string("{\"zData\":{\"TestKey\":\"\"}}\n"));
  TEST_EQ(MyData->GetXml(),
      std::string("<?xml version=\"1.0\" encoding=\"utf-8\"?>\n<zData><TestKey/></zData>"));
  delete (MyData);

  // Return success
  return (0);

}

