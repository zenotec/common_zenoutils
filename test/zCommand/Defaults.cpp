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

int
zCommandTest_CommandDefaults(void* arg_)
{
  // Create new empty command and verify
  zCommand::Command *myCommand = new zCommand::Command;
  TEST_ISNOT_NULL(myCommand);
  TEST_EQ(std::string(""), myCommand->GetName());
  TEST_TRUE(myCommand->GetOptions().empty());

  // Cleanup
  delete (myCommand);

  // Create new command from command string and verify
  myCommand = new zCommand::Command("CommandDefaults");
  TEST_ISNOT_NULL(myCommand);
  TEST_EQ(std::string("CommandDefaults"), myCommand->GetName());
  TEST_TRUE(myCommand->GetOptions().empty());

  // Cleanup
  delete (myCommand);

  // Return success
  return (0);
}

int
zCommandTest_TestCommandDefaults(void* arg_)
{
  // Create new configuration data item and verify
  TestCommand *myCommand = new TestCommand;
  TEST_ISNOT_NULL(myCommand);
  TEST_EQ(std::string(""), myCommand->GetName());
  TEST_TRUE(myCommand->GetOptions().empty());

  // Cleanup
  delete (myCommand);

  // Return success
  return (0);
}

int
zCommandTest_HandlerDefaults(void* arg_)
{
  // Create new handler and verify
  zCommand::CommandHandler *myHandler = new zCommand::CommandHandler;
  TEST_ISNOT_NULL(myHandler);

  // Cleanup
  delete (myHandler);

  // Return success
  return (0);
}

