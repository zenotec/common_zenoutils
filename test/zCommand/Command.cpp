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
  TEST_EQ(std::string(""), myCommand->GetArgument());

  // Set name and validate
  TEST_TRUE(myCommand->SetName("TestName"));
  TEST_EQ(std::string("TestName"), myCommand->GetName());

  // Set argument and validate
  TEST_TRUE(myCommand->SetArgument("TestArg"));
  TEST_EQ(std::string("TestArg"), myCommand->GetArgument());

  // Cleanup
  delete (myCommand);

  // Return success
  return (0);
}

int
zCommandTest_CommandExecute(void* arg_)
{
  // Create new command and verify
  TestCommand *myCommand = new TestCommand;
  TEST_ISNOT_NULL(myCommand);
  TEST_EQ(std::string(""), myCommand->GetName());
  TEST_EQ(std::string(""), myCommand->GetArgument());

  // Set name and argument
  TEST_TRUE(myCommand->SetName("TestName"));
  TEST_TRUE(myCommand->SetArgument("TestArg"));

  // Execute command and validate
  TEST_TRUE(myCommand->Execute(" "));
  TEST_EQ(std::string("TestName: "), myCommand->GetString());

  // Cleanup
  delete (myCommand);

  // Return success
  return (0);
}
