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

#include <string>
#include <mutex>
#include <list>
#include <map>

#include <zutils/zLog.h>
#include <zutils/zEvent.h>
#include <zutils/zData.h>
#include <zutils/zQueue.h>
#include <zutils/zSocket.h>
#include <zutils/zMessage.h>

#include "UnitTest.h"
#include "zCommandTest.h"

using namespace Test;
using namespace zUtils;
using namespace zUtils::zCommand;

int
zCommandTest_CommandPathDefaults(void* arg_)
{
  // Create new command path and validate
  CommandPath MyPath;
  TEST_EQ((zData::DataPath::DataRoot + "." + CommandPath::DataRoot), MyPath.Root());

  // Return success
  return (0);
}

int
zCommandTest_CommandDataDefaults(void* arg_)
{
  // Create new command data and validate
  CommandData MyData;
  TEST_EQ((zData::DataPath::DataRoot + "." + CommandPath::DataRoot), MyData.Root());
  TEST_EQ(std::string(""), MyData.GetName());
  TEST_TRUE(MyData.GetOptions().empty());
  TEST_EQ(std::string(""), MyData.GetOutput());

  // Return success
  return (0);
}

int
zCommandTest_CommandDefaults(void* arg_)
{
  // Create new test command and validate
  TestCommand MyCommand;
  TEST_EQ((zData::DataPath::DataRoot + "." + CommandPath::DataRoot), MyCommand.Root());
  TEST_EQ(std::string(""), MyCommand.GetName());
  TEST_TRUE(MyCommand.GetOptions().empty());
  TEST_EQ(std::string(""), MyCommand.GetOutput());

  // Return success
  return (0);
}

