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
zCommandTest_CommandHandlerNoArg(void* arg_)
{
  // Create new command and verify
  TestCommand *myCommand = new TestCommand;
  TEST_ISNOT_NULL(myCommand);
  TEST_EQ(std::string(""), myCommand->GetName());

  // Set name and argument
  TEST_TRUE(myCommand->SetName("CommandHandlerNoArg"));

  // Create new handler and verify
  zCommand::CommandHandler *myHandler = new zCommand::CommandHandler;
  TEST_ISNOT_NULL(myHandler);

  // Register command with handler and verify
  TEST_TRUE(myHandler->RegisterCommand(myCommand));

  // Execute command and validate
  std::string cmdString = "CommandHandlerNoArg";
  TEST_TRUE(myHandler->ProcessCommand(cmdString));
  TEST_EQ(std::string("CommandHandlerNoArg:"), myCommand->GetString());

  // Cleanup
  myHandler->UnregisterCommand(myCommand);
  delete (myCommand);

  // Return success
  return (0);
}

int
zCommandTest_CommandHandlerSingleArg(void* arg_)
{
  // Create new command and verify
  TestCommand *myCommand = new TestCommand;
  TEST_ISNOT_NULL(myCommand);
  TEST_EQ(std::string(""), myCommand->GetName());

  // Set name and argument
  TEST_TRUE(myCommand->SetName("CommandHandlerSingleArg"));

  // Create new handler and verify
  zCommand::CommandHandler *myHandler = new zCommand::CommandHandler;
  TEST_ISNOT_NULL(myHandler);

  // Register command with handler and verify
  TEST_TRUE(myHandler->RegisterCommand(myCommand));

  // Execute command and validate
  std::string cmdString = "CommandHandlerSingleArg Arg1";
  TEST_TRUE(myHandler->ProcessCommand(cmdString));
  TEST_EQ(std::string("CommandHandlerSingleArg:Arg1"), myCommand->GetString());

  // Cleanup
  myHandler->UnregisterCommand(myCommand);
  delete (myCommand);

  // Return success
  return (0);
}

int
zCommandTest_CommandHandlerMultiArg(void* arg_)
{
  // Create new command and verify
  TestCommand *myCommand = new TestCommand;
  TEST_ISNOT_NULL(myCommand);
  TEST_EQ(std::string(""), myCommand->GetName());

  // Set name and argument
  TEST_TRUE(myCommand->SetName("CommandHandlerMultiArg"));

  // Create new handler and verify
  zCommand::CommandHandler *myHandler = new zCommand::CommandHandler;
  TEST_ISNOT_NULL(myHandler);

  // Register command with handler and verify
  TEST_TRUE(myHandler->RegisterCommand(myCommand));

  // Execute command and validate
  std::string cmdString = "CommandHandlerMultiArg Arg1 Arg2";
  TEST_TRUE(myHandler->ProcessCommand(cmdString));
  TEST_EQ(std::string("CommandHandlerMultiArg:Arg1 Arg2"), myCommand->GetString());

  // Cleanup
  myHandler->UnregisterCommand(myCommand);
  delete (myCommand);

  // Return success
  return (0);
}

int
zCommandTest_CommandHandlerBadCommand(void* arg_)
{
  // Create new command and verify
  TestCommand *myCommand = new TestCommand;
  TEST_ISNOT_NULL(myCommand);
  TEST_EQ(std::string(""), myCommand->GetName());

  // Set name and argument
  TEST_TRUE(myCommand->SetName("TestName"));

  // Create new handler and verify
  zCommand::CommandHandler *myHandler = new zCommand::CommandHandler;
  TEST_ISNOT_NULL(myHandler);

  // Register command with handler and verify
  TEST_TRUE(myHandler->RegisterCommand(myCommand));

  // Execute command and validate
  TEST_FALSE(myHandler->ProcessCommand(std::string("TestName1 TestArg1")));

  // Cleanup
  myHandler->UnregisterCommand(myCommand);
  delete (myCommand);

  // Return success
  return (0);
}

int
zCommandTest_CommandHandlerMultiCommand(void* arg_)
{
  // Create new command and verify
  TestCommand *myCommand1 = new TestCommand;
  TEST_ISNOT_NULL(myCommand1);
  TEST_EQ(std::string(""), myCommand1->GetName());

  // Set name and argument
  TEST_TRUE(myCommand1->SetName("TestName1"));

  // Create new command and verify
  TestCommand *myCommand2 = new TestCommand;
  TEST_ISNOT_NULL(myCommand2);
  TEST_EQ(std::string(""), myCommand2->GetName());

  // Set name and argument
  TEST_TRUE(myCommand2->SetName("TestName2"));

  // Create new handler and verify
  zCommand::CommandHandler *myHandler = new zCommand::CommandHandler;
  TEST_ISNOT_NULL(myHandler);

  // Register commands with handler and verify
  TEST_TRUE(myHandler->RegisterCommand(myCommand1));
  TEST_TRUE(myHandler->RegisterCommand(myCommand2));

  // Execute command and validate
  std::string cmdString = "TestName1 TestArg1-1 TestArg1-2; TestName2 TestArg2-1 TestArg2-2";
  TEST_TRUE(myHandler->ProcessCommand(cmdString));
  TEST_EQ(std::string("TestName1:TestArg1-1 TestArg1-2"), myCommand1->GetString());
  TEST_EQ(std::string("TestName2:TestArg2-1 TestArg2-2"), myCommand2->GetString());

  // Cleanup
  myHandler->UnregisterCommand(myCommand2);
  delete (myCommand2);
  myHandler->UnregisterCommand(myCommand1);
  delete (myCommand1);

  // Return success
  return (0);
}
