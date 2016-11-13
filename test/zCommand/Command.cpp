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
zCommandTest_CommandGetSet(void* arg_)
{

  // Create new command and verify
  zCommand::Command *myCommand = new zCommand::Command;
  TEST_ISNOT_NULL(myCommand);
  TEST_EQ(std::string(""), myCommand->GetName());
  TEST_TRUE(myCommand->GetOptions().empty());

  // Set name and validate
  TEST_TRUE(myCommand->SetName(std::string("CommandGetSet")));
  TEST_EQ(std::string("CommandGetSet"), myCommand->GetName());

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

  // Cleanup
  delete (myOption);
  delete (myCommand);

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

  // Execute command and validate
  TEST_TRUE(myCommand->Execute(*myCommand));
  TEST_EQ(std::string("CommandExecute: Option1=Arg1"), myCommand->GetOutput());

  // Cleanup
  delete (myOption);
  delete (myCommand);

  // Return success
  return (0);
}
