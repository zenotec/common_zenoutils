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
zDataTest_PathAppend(void* arg)
{

  ZLOG_DEBUG("#############################################################");
  ZLOG_DEBUG("# zDataTest_PathAppend()");
  ZLOG_DEBUG("#############################################################");

  // Create new data path and validate
  zData::DataPath MyPath;
  TEST_EQ(zData::DataPath::DataRoot, MyPath.Root());
  TEST_EQ(zData::DataPath::DataRoot, MyPath.Base());
  TEST_EQ(std::string(""), MyPath.Key());
  TEST_EQ(zData::DataPath::DataRoot, MyPath.Path());

  // Root
  TEST_TRUE(MyPath.Append(std::string("root")));
  TEST_EQ(zData::DataPath::DataRoot, MyPath.Root());
  TEST_EQ(zData::DataPath::DataRoot, MyPath.Base());
  TEST_EQ(std::string("root"), MyPath.Key());
  TEST_EQ((zData::DataPath::DataRoot + std::string(".root")), MyPath.Path());

  // Base
  TEST_TRUE(MyPath.Append(std::string("base")));
  TEST_EQ(zData::DataPath::DataRoot, MyPath.Root());
  TEST_EQ((zData::DataPath::DataRoot + std::string(".root")), MyPath.Base());
  TEST_EQ(std::string("base"), MyPath.Key());
  TEST_EQ((zData::DataPath::DataRoot + std::string(".root.base")), MyPath.Path());

  // Key
  TEST_TRUE(MyPath.Append(std::string("key")));
  TEST_EQ(zData::DataPath::DataRoot, MyPath.Root());
  TEST_EQ((zData::DataPath::DataRoot + std::string(".root.base")), MyPath.Base());
  TEST_EQ(std::string("key"), MyPath.Key());
  TEST_EQ((zData::DataPath::DataRoot + std::string(".root.base.key")), MyPath.Path());

  // Return success
  return (0);
}

int
zDataTest_PathPrepend(void* arg)
{

  ZLOG_DEBUG("#############################################################");
  ZLOG_DEBUG("# zDataTest_PathPrepend()");
  ZLOG_DEBUG("#############################################################");

  // Create new data path and validate
  zData::DataPath MyPath;
  TEST_EQ(zData::DataPath::DataRoot, MyPath.Root());
  TEST_EQ(zData::DataPath::DataRoot, MyPath.Base());
  TEST_EQ(std::string(""), MyPath.Key());
  TEST_EQ(zData::DataPath::DataRoot, MyPath.Path());

  // Key
  TEST_TRUE(MyPath.Prepend(std::string("key")));
  TEST_EQ(zData::DataPath::DataRoot, MyPath.Root());
  TEST_EQ(zData::DataPath::DataRoot, MyPath.Base());
  TEST_EQ(std::string("key"), MyPath.Key());
  TEST_EQ((zData::DataPath::DataRoot + std::string(".key")), MyPath.Path());

  // Base
  TEST_TRUE(MyPath.Prepend(std::string("base")));
  TEST_EQ(zData::DataPath::DataRoot, MyPath.Root());
  TEST_EQ((zData::DataPath::DataRoot + std::string(".base")), MyPath.Base());
  TEST_EQ(std::string("key"), MyPath.Key());
  TEST_EQ((zData::DataPath::DataRoot + std::string(".base.key")), MyPath.Path());

  // Root
  TEST_TRUE(MyPath.Prepend(std::string("root")));
  TEST_EQ(zData::DataPath::DataRoot, MyPath.Root());
  TEST_EQ((zData::DataPath::DataRoot + std::string(".root.base")), MyPath.Base());
  TEST_EQ(std::string("key"), MyPath.Key());
  TEST_EQ((zData::DataPath::DataRoot + std::string(".root.base.key")), MyPath.Path());

  return (0);
}


int zDataTest_PopFront(void* arg) /////
{
  
  ZLOG_DEBUG("#############################################################");
  ZLOG_DEBUG("# zDataTest_PopFront()");
  ZLOG_DEBUG("#############################################################");

  zData::DataPath MyPath;
  TEST_TRUE(MyPath.Append(std::string("root")));
  TEST_TRUE(MyPath.Append(std::string("base")));
  TEST_TRUE(MyPath.Append(std::string("key")));
  MyPath.PopFront();
  //std::cout <<std::endl<<MyPath.Path() << std::endl;
  
  TEST_EQ((zData::DataPath::DataRoot + std::string(".base.key")), MyPath.Path());

  return (0);

}

int
zDataTest_PopBack(void* arg)
{
  ZLOG_DEBUG("#############################################################");
  ZLOG_DEBUG("# zDataTest_PopBack()");
  ZLOG_DEBUG("#############################################################");
   
  zData::DataPath MyPath;
  TEST_TRUE(MyPath.Append(std::string("root")));
  TEST_TRUE(MyPath.Append(std::string("base")));
  TEST_TRUE(MyPath.Append(std::string("key")));
  MyPath.PopBack();
  TEST_EQ((zData::DataPath::DataRoot + std::string(".root.base")), MyPath.Path());

  return (0);
}

int
zDataTest_PathCompare(void* arg)
{

  ZLOG_DEBUG("#############################################################");
  ZLOG_DEBUG("# zDataTest_PathDefaults()");
  ZLOG_DEBUG("#############################################################");

  zData::DataPath MyPath1;
  TEST_EQ(zData::DataPath::DataRoot, MyPath1.Root());
  TEST_EQ(zData::DataPath::DataRoot, MyPath1.Base());
  TEST_EQ(std::string(""), MyPath1.Key());
  TEST_EQ(zData::DataPath::DataRoot, MyPath1.Path());

  zData::DataPath MyPath2;
  TEST_EQ(zData::DataPath::DataRoot, MyPath2.Root());
  TEST_EQ(zData::DataPath::DataRoot, MyPath2.Base());
  TEST_EQ(std::string(""), MyPath2.Key());
  TEST_EQ(zData::DataPath::DataRoot, MyPath2.Path());

  // Compare (equal)
  TEST_TRUE(MyPath1 == MyPath2);
  TEST_FALSE(MyPath1 != MyPath2);

  // Update second path to be different then first path
  TEST_TRUE(MyPath2.Append(std::string("key")));

  // Compare (not equal)
  TEST_FALSE(MyPath1 == MyPath2);
  TEST_TRUE(MyPath1 != MyPath2);

  // Update first path to match second path
  TEST_TRUE(MyPath1.Prepend(std::string("key")));

  // Compare (equal)
  TEST_TRUE(MyPath1 == MyPath2);
  TEST_FALSE(MyPath1 != MyPath2);

  // Return success
  return (0);
}

