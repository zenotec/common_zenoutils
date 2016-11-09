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
  TEST_IS_ZERO(myCommand->Argc());
  TEST_TRUE(myCommand->GetArgv().empty());

  // Setup argument vector
  std::vector<std::string>argv;
  TEST_TRUE(argv.empty());
  argv.push_back("CommandGetSet");
  TEST_EQ(1, argv.size());
  argv.push_back("Arg1");
  TEST_EQ(2, argv.size());

  // Set name and validate
  TEST_TRUE(myCommand->SetName(std::string("CommandGetSet")));
  TEST_EQ(std::string("CommandGetSet"), myCommand->GetName());

  // Set argument and validate
  TEST_TRUE(myCommand->SetArgv(argv));
  TEST_EQ(2, myCommand->Argc());
  TEST_EQ(std::string("CommandGetSet"), myCommand->GetArgv()[0]);
  TEST_EQ(std::string("Arg1"), myCommand->GetArgv()[1]);

  // Cleanup
  delete (myCommand);

  // Return success
  return (0);
}

int
zCommandTest_CommandExecute(void* arg_)
{
  // Create new command and verify
  TestCommand *myCommand = new TestCommand("CommandExecute Arg1 Arg2");
  TEST_ISNOT_NULL(myCommand);
  TEST_EQ(std::string("CommandExecute"), myCommand->GetName());
  TEST_EQ(3, myCommand->Argc());
  TEST_FALSE(myCommand->GetArgv().empty());

  // Execute command and validate
  TEST_TRUE(myCommand->Execute(myCommand->Argc(), myCommand->GetArgv()));
  TEST_EQ(std::string("CommandExecute: Arg1 Arg2"), myCommand->GetString());

  // Cleanup
  delete (myCommand);

  // Return success
  return (0);
}
