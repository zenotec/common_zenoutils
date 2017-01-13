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
zCommandTest_CommandDataGetSet(void* arg_)
{

  // Create new command and verify
  zCommand::CommandData *MyCommandData = new zCommand::CommandData;
  TEST_ISNOT_NULL(MyCommandData);
  TEST_EQ(std::string(""), MyCommandData->GetName());
  TEST_TRUE(MyCommandData->GetOptions().empty());
  TEST_EQ(std::string(""), MyCommandData->GetOutput());

  // Set name and validate
  TEST_TRUE(MyCommandData->SetName(std::string("CommandGetSet")));
  TEST_EQ(std::string("CommandGetSet"), MyCommandData->GetName());

  // Create new command option and verify
  zCommand::CommandOption *myOption = new zCommand::CommandOption;
  TEST_ISNOT_NULL(myOption);
  TEST_EQ(std::string(""), myOption->GetName());
  TEST_EQ(std::string(""), myOption->GetArgument());

  // Set command option and argument and verify
  TEST_TRUE(myOption->SetName(std::string("Option1")));
  TEST_EQ(std::string("Option1"), myOption->GetName());
  TEST_TRUE(myOption->SetArgument(std::string("Arg1")));
  TEST_EQ(std::string("Arg1"), myOption->GetArgument());

  // Add command option
  TEST_TRUE(MyCommandData->AddOption(*myOption));
  TEST_FALSE(MyCommandData->GetOptions().empty());
  TEST_EQ(1, MyCommandData->GetOptions().size());

  // Cleanup
  delete (myOption);
  delete (MyCommandData);

  // Return success
  return (0);
}

int
zCommandTest_CommandExecute(void* arg_)
{

  // Create new command and verify
  zCommand::Command *myCommand = new TestCommand;
  TEST_ISNOT_NULL(myCommand);
  TEST_EQ(std::string(""), myCommand->GetName());
  TEST_TRUE(myCommand->GetOptions().empty());

  // Set name and validate
  TEST_TRUE(myCommand->SetName(std::string("CommandExecute")));
  TEST_EQ(std::string("CommandExecute"), myCommand->GetName());

  // Create new command option and verify
  zCommand::CommandOption *myOption = new zCommand::CommandOption;
  TEST_ISNOT_NULL(myOption);
  TEST_EQ(std::string(""), myOption->GetName());
  TEST_EQ(std::string(""), myOption->GetArgument());

  // Set command option and argument and verify
  TEST_TRUE(myOption->SetName(std::string("Option1")));
  TEST_EQ(std::string("Option1"), myOption->GetName());
  TEST_TRUE(myOption->SetArgument(std::string("Arg1")));
  TEST_EQ(std::string("Arg1"), myOption->GetArgument());

  // Add command option
  TEST_TRUE(myCommand->AddOption(*myOption));
  TEST_FALSE(myCommand->GetOptions().empty());
  TEST_EQ(1, myCommand->GetOptions().size());

  // Execute command and validate
  TEST_TRUE(myCommand->Execute());
  TEST_EQ(std::string("CommandExecute: Option1=Arg1"), myCommand->GetOutput());

  // Cleanup
  delete (myOption);
  delete (myCommand);

  // Return success
  return (0);
}
