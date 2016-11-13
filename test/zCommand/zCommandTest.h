/*
 * zCommandTest.h
 *
 *  Created on: Jan 10, 2016
 *      Author: kmahoney
 */

#ifndef _ZCOMMANDTEST_H_
#define _ZCOMMANDTEST_H_

#include "UnitTest.h"

#include <zutils/zCommand.h>

int
zCommandTest_CommandDefaults(void* arg_);
int
zCommandTest_TestCommandDefaults(void* arg_);
int
zCommandTest_HandlerDefaults(void* arg_);
int
zCommandTest_CommandGetSet(void* arg_);
int
zCommandTest_CommandExecute(void* arg_);
int
zCommandTest_CommandHandlerNoArg( void* arg_ );
int
zCommandTest_CommandHandlerSingleArg( void* arg_ );
int
zCommandTest_CommandHandlerMultiArg( void* arg_ );
int
zCommandTest_CommandHandlerBadCommand( void* arg_ );

using namespace Test;
using namespace zUtils;

class TestCommand : public zCommand::Command
{
public:

  TestCommand(const std::string &name_ = std::string("")) :
      zCommand::Command(name_)
  {
    std::cout << "TestCommand::TestCommand() " << std::endl;
  }

  virtual
  ~TestCommand()
  {

  }

  virtual bool
  Execute(zCommand::Command &cmd_)
  {
    std::cout << "TestCommand::Execute() " << std::endl;
    std::string output = cmd_.GetName() + ":";
    std::vector<zCommand::CommandOption>opts = cmd_.GetOptions();
    for (int i = 0; i < opts.size(); i++)
    {
      output += std::string(" ") + opts[i].GetOption() + std::string("=") + opts[i].GetArgument();
    }
    return(this->SetOutput(output));
  }

protected:

private:

};

#endif /* _ZCOMMANDTEST_H_ */
