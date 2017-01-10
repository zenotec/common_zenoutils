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

#ifndef _ZCOMMANDTEST_H_
#define _ZCOMMANDTEST_H_

#include "UnitTest.h"

#include <zutils/zCommand.h>

int
zCommandTest_CommandDefaults(void* arg_);
int
zCommandTest_TestCommandDefaults(void* arg_);
int
zCommandTest_CommandGetSet(void* arg_);
int
zCommandTest_CommandExecute(void* arg_);
//int
//zCommandTest_CommandHandlerNoArg(void* arg_);
//int
//zCommandTest_CommandHandlerSingleArg(void* arg_);
//int
//zCommandTest_CommandHandlerMultiArg(void* arg_);
//int
//zCommandTest_CommandHandlerBadCommand(void* arg_);

using namespace Test;
using namespace zUtils;

class TestCommand : public zCommand::Command
{
public:

  TestCommand(const std::string &name_ = std::string("")) :
      zCommand::Command(name_)
  {
//    std::cout << "TestCommand::TestCommand() " << std::endl;
  }

  virtual
  ~TestCommand()
  {

  }

  virtual bool
  Execute(zCommand::Command &cmd_)
  {
//    std::cout << "TestCommand::Execute() " << std::endl;
    std::string output = cmd_.GetName() + ":";
    std::map<std::string, zCommand::CommandOption> opts = cmd_.GetOptions();
    FOREACH(auto& opt, opts)
    {
      output += std::string(" ") + opt.first + std::string("=") + opt.second.GetArgument();
    }
    return (this->SetOutput(output));
  }

protected:

private:

};

#endif /* _ZCOMMANDTEST_H_ */
