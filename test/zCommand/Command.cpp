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

  ZLOG_DEBUG("#############################################################");
  ZLOG_DEBUG("# zCommandTest_CommandDataGetSet()");
  ZLOG_DEBUG("#############################################################");

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

  ZLOG_DEBUG("#############################################################");
  ZLOG_DEBUG("# zCommandTest_CommandExecute()");
  ZLOG_DEBUG("#############################################################");

  // Create new command data and verify
  zCommand::CommandData MyData;
  TEST_EQ(std::string(""), MyData.GetName());
  TEST_TRUE(MyData.GetOptions().empty());
  TEST_EQ(std::string(""), MyData.GetOutput());

  // Set name and validate
  TEST_TRUE(MyData.SetName(std::string("CommandExecute")));
  TEST_EQ(std::string("CommandExecute"), MyData.GetName());

  // Create new command option and verify
  zCommand::CommandOption *MyOption = new zCommand::CommandOption;
  TEST_ISNOT_NULL(MyOption);
  TEST_EQ(std::string(""), MyOption->GetName());
  TEST_EQ(std::string(""), MyOption->GetArgument());

  // Set command option and argument and verify
  TEST_TRUE(MyOption->SetName(std::string("Option1")));
  TEST_EQ(std::string("Option1"), MyOption->GetName());
  TEST_TRUE(MyOption->SetArgument(std::string("Arg1")));
  TEST_EQ(std::string("Arg1"), MyOption->GetArgument());

  // Add command option
  TEST_TRUE(MyData.AddOption(*MyOption));
  TEST_FALSE(MyData.GetOptions().empty());
  TEST_EQ(1, MyData.GetOptions().size());

  // Create new command and verify
  zCommand::Command *MyCommand = new TestCommand;
  TEST_ISNOT_NULL(MyCommand);
  TEST_EQ(std::string(""), MyCommand->GetName());
  TEST_TRUE(MyCommand->GetOptions().empty());

  // Set name and validate
  TEST_TRUE(MyCommand->SetName(std::string("CommandExecute")));
  TEST_EQ(std::string("CommandExecute"), MyCommand->GetName());

  // Execute command and validate
  TEST_TRUE(MyCommand->Execute(MyData));
  TEST_EQ(std::string("CommandExecute: Option1=Arg1"), MyCommand->GetOutput());

  // Cleanup
  delete (MyOption);
  delete (MyCommand);

  // Return success
  return (0);
}
