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
  zCommand::Command *myCommand = new TestCommand;
  TEST_ISNOT_NULL(myCommand);
  TEST_EQ(std::string(""), myCommand->GetName());
  TEST_TRUE(myCommand->GetOptions().empty());

  // Set name and validate
  TEST_TRUE(myCommand->SetName(std::string("CommandHandlerNoArg")));
  TEST_EQ(std::string("CommandHandlerNoArg"), myCommand->GetName());

  // Create new command option and verify
  zCommand::CommandOption *myOption = new zCommand::CommandOption;
  TEST_ISNOT_NULL(myOption);
  TEST_EQ(std::string(""), myOption->GetOption());
  TEST_EQ(std::string(""), myOption->GetArgument());

  // Create new handler and verify
  zCommand::CommandHandler *myHandler = new zCommand::CommandHandler;
  TEST_ISNOT_NULL(myHandler);

  // Register command with handler and verify
  TEST_TRUE(myHandler->RegisterCommand(myCommand));

  // Execute command and validate
  TEST_TRUE(myHandler->ProcessCommand(*myCommand));

  // Verify command output
  TEST_EQ(std::string("CommandHandlerNoArg:"), myCommand->GetOutput());

  // Cleanup
  myHandler->UnregisterCommand(myCommand);
  delete (myCommand);
  delete (myHandler);

  // Return success
  return (0);

}

int
zCommandTest_CommandHandlerSingleArg(void* arg_)
{

  // Create new command and verify
  zCommand::Command *myCommand = new TestCommand;
  TEST_ISNOT_NULL(myCommand);
  TEST_EQ(std::string(""), myCommand->GetName());
  TEST_TRUE(myCommand->GetOptions().empty());

  // Set name and validate
  TEST_TRUE(myCommand->SetName(std::string("CommandHandlerSingleArg")));
  TEST_EQ(std::string("CommandHandlerSingleArg"), myCommand->GetName());

  // Create new command option and verify
  zCommand::CommandOption *myOption = new zCommand::CommandOption;
  TEST_ISNOT_NULL(myOption);
  TEST_EQ(std::string(""), myOption->GetOption());
  TEST_EQ(std::string(""), myOption->GetArgument());

  // Set command option and argument and verify
  TEST_TRUE(myOption->SetOption(std::string("Option1")));
  TEST_EQ(std::string("Option1"), myOption->GetOption());
  TEST_TRUE(myOption->SetArgument(std::string("Arg1")));
  TEST_EQ(std::string("Arg1"), myOption->GetArgument());

  // Add command option
  TEST_TRUE(myCommand->AddOption(*myOption));
  TEST_FALSE(myCommand->GetOptions().empty());
  TEST_EQ(1, myCommand->GetOptions().size());

  // Create new handler and verify
  zCommand::CommandHandler *myHandler = new zCommand::CommandHandler;
  TEST_ISNOT_NULL(myHandler);

  // Register command with handler and verify
  TEST_TRUE(myHandler->RegisterCommand(myCommand));

  // Execute command and validate
  TEST_TRUE(myHandler->ProcessCommand(*myCommand));

  // Verify command output
  TEST_EQ(std::string("CommandHandlerSingleArg: Option1=Arg1"), myCommand->GetOutput());

  // Cleanup
  myHandler->UnregisterCommand(myCommand);
  delete (myOption);
  delete (myCommand);
  delete (myHandler);

  // Return success
  return (0);

}

int
zCommandTest_CommandHandlerMultiArg(void* arg_)
{

  // Create new command and verify
  zCommand::Command *myCommand = new TestCommand;
  TEST_ISNOT_NULL(myCommand);
  TEST_EQ(std::string(""), myCommand->GetName());
  TEST_TRUE(myCommand->GetOptions().empty());

  // Set name and validate
  TEST_TRUE(myCommand->SetName(std::string("CommandHandlerMultiArg")));
  TEST_EQ(std::string("CommandHandlerMultiArg"), myCommand->GetName());

  // Create new command option and verify
  zCommand::CommandOption *myOption1 = new zCommand::CommandOption;
  TEST_ISNOT_NULL(myOption1);
  TEST_EQ(std::string(""), myOption1->GetOption());
  TEST_EQ(std::string(""), myOption1->GetArgument());

  // Set command option and argument and verify
  TEST_TRUE(myOption1->SetOption(std::string("Option1")));
  TEST_EQ(std::string("Option1"), myOption1->GetOption());
  TEST_TRUE(myOption1->SetArgument(std::string("Arg1")));
  TEST_EQ(std::string("Arg1"), myOption1->GetArgument());

  // Add command option
  TEST_TRUE(myCommand->AddOption(*myOption1));
  TEST_FALSE(myCommand->GetOptions().empty());
  TEST_EQ(1, myCommand->GetOptions().size());

  // Create new command option and verify
  zCommand::CommandOption *myOption2 = new zCommand::CommandOption;
  TEST_ISNOT_NULL(myOption2);
  TEST_EQ(std::string(""), myOption2->GetOption());
  TEST_EQ(std::string(""), myOption2->GetArgument());

  // Set command option and argument and verify
  TEST_TRUE(myOption2->SetOption(std::string("Option2")));
  TEST_EQ(std::string("Option2"), myOption2->GetOption());
  TEST_TRUE(myOption2->SetArgument(std::string("Arg2")));
  TEST_EQ(std::string("Arg2"), myOption2->GetArgument());

  // Add command option
  TEST_TRUE(myCommand->AddOption(*myOption2));
  TEST_FALSE(myCommand->GetOptions().empty());
  TEST_EQ(2, myCommand->GetOptions().size());

  // Create new handler and verify
  zCommand::CommandHandler *myHandler = new zCommand::CommandHandler;
  TEST_ISNOT_NULL(myHandler);

  // Register command with handler and verify
  TEST_TRUE(myHandler->RegisterCommand(myCommand));

  // Execute command and validate
  TEST_TRUE(myHandler->ProcessCommand(*myCommand));

  // Verify command output
  TEST_EQ(std::string("CommandHandlerMultiArg: Option1=Arg1 Option2=Arg2"), myCommand->GetOutput());

  // Cleanup
  myHandler->UnregisterCommand(myCommand);
  delete (myOption1);
  delete (myOption2);
  delete (myCommand);
  delete (myHandler);

  // Return success
  return (0);

}

int
zCommandTest_CommandHandlerBadCommand(void* arg_)
{
  // Create new command and verify
  //TestCommand *myCommand = new TestCommand;
  //TEST_ISNOT_NULL(myCommand);
  //TEST_EQ(std::string(""), myCommand->GetName());

  // Set name and argument
  //TEST_TRUE(myCommand->SetName("TestName"));

  // Create new handler and verify
  //zCommand::CommandHandler *myHandler = new zCommand::CommandHandler;
  //TEST_ISNOT_NULL(myHandler);

  // Register command with handler and verify
  //TEST_TRUE(myHandler->RegisterCommand(myCommand));

  // Execute command and validate
  //TEST_FALSE(myHandler->ProcessCommand(std::string("TestName1 TestArg1")));

  // Cleanup
  //myHandler->UnregisterCommand(myCommand);
  //delete (myCommand);

  // Return success
  return (0);
}

