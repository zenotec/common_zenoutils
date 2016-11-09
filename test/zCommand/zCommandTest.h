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
int
zCommandTest_CommandHandlerMultiCommand( void* arg_ );

using namespace Test;
using namespace zUtils;

class TestCommand : public zCommand::Command
{
public:

  TestCommand(const std::string &cmd_ = std::string("")) :
      zCommand::Command(cmd_)
  {

  }

  virtual
  ~TestCommand()
  {

  }

  std::string
  GetString()
  {
    return(this->_string);
  }

  virtual bool
  Execute(int argc, const std::vector<std::string> &argv_)
  {
    //this->SetArgument(arg_);
    //this->_string = this->GetName() + ":" + this->GetArgument();
    return(true);
  }

protected:

private:

  std::string _string;

};

#endif /* _ZCOMMANDTEST_H_ */
