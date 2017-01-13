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

#include "zProgramTest.h"

using namespace Test;
using namespace zUtils;
using namespace zUtils::zProgram;

int
zProgramTest_ParseCommandLine(void* arg_)
{

  const char *argv[3][6] = {
      { "program", "-f", "filename.ext", "addr", NULL, NULL },
      { "program", "--file", "filename.ext", "addr", NULL, NULL },
      { "program", "-f", "filename_1.txt", "-f", "filename_2.txt", "addr" }
  };

  // Create new option and validate
  ProgramOption MyOpt;
  TEST_IS_ZERO(MyOpt.Count());
  TEST_TRUE(ProgramOption::FLAGS_NONE == MyOpt.GetFlags());
  TEST_EQ('\0', MyOpt.GetShortName());
  TEST_EQ(std::string(""), MyOpt.GetLongName());
  TEST_EQ(std::string(""), MyOpt.GetDescription());
  TEST_EQ(std::string(""), MyOpt.GetDefault());
  TEST_EQ(std::string(""), MyOpt.GetArguments()[0]);

  // Set flags and verify
  TEST_TRUE(MyOpt.SetFlags(ProgramOption::FLAGS_HAVEARG));
  TEST_TRUE(ProgramOption::FLAGS_HAVEARG == MyOpt.GetFlags());

  // Set short name and verify
  TEST_TRUE(MyOpt.SetShortName('f'));
  TEST_EQ('f', MyOpt.GetShortName());

  // Set long option and verify
  TEST_TRUE(MyOpt.SetLongName("file"));
  TEST_EQ(std::string("file"), MyOpt.GetLongName());

  // Set default argument value
  TEST_TRUE(MyOpt.SetDefault(std::string("test.txt")));
  TEST_EQ(std::string("test.txt"), MyOpt.GetDefault());

  // Set option help message
  MyOpt.SetDescription("Filename");
  TEST_EQ(std::string("Filename"), MyOpt.GetDescription());

  // Create new argument and validate
  ProgramArgument MyArg;
  TEST_IS_ZERO(MyArg.Count());
  TEST_TRUE(ProgramArgument::FLAGS_NONE == MyArg.GetFlags());
  TEST_IS_ZERO(MyArg.GetPosition());
  TEST_EQ(std::string(""), MyArg.GetName());
  TEST_EQ(std::string(""), MyArg.GetDescription());
  TEST_EQ(std::string(""), MyArg.GetDefault());
  TEST_EQ(std::string(""), MyArg.GetValue());

  // Set order and verify
  TEST_TRUE(MyArg.SetPosition(0));
  TEST_EQ(0, MyArg.GetPosition());

  // Set name and verify
  TEST_TRUE(MyArg.SetName("Address"));
  TEST_EQ(std::string("Address"), MyArg.GetName());

  // Set option help message
  MyArg.SetDescription("Address Description");
  TEST_EQ(std::string("Address Description"), MyArg.GetDescription());

  // Set default argument value
  TEST_TRUE(MyArg.SetDefault(std::string("1.2.3.4")));
  TEST_EQ(std::string("1.2.3.4"), MyArg.GetDefault());

  // Create new test program and verify
  TestProgram *MyProg = new TestProgram;
  TEST_ISNOT_NULL(MyProg);

  // Add option and argument and verify
  TEST_TRUE(MyProg->AddOption(MyOpt));
  TEST_TRUE(MyProg->AddArgument(MyArg));
  TEST_EQ(0, MyProg->GetOption("file").Count());
  TEST_EQ(std::string("test.txt"), MyProg->GetOption("file").GetArguments()[0]);
  TEST_EQ(std::string("1.2.3.4"), MyProg->GetArgument(0).GetValue());

  // Parse command line and validate
  TEST_TRUE(MyProg->ParseCommandLine(4, argv[0]));
  TEST_EQ(1, MyProg->GetOption("file").Count());
  TEST_EQ(std::string("filename.ext"), MyProg->GetOption("file").GetArguments()[0]);
  TEST_EQ(std::string("addr"), MyProg->GetArgument(0).GetValue());

  delete (MyProg);

  // Create new test program and verify
  MyProg = new TestProgram;
  TEST_ISNOT_NULL(MyProg);

  // Add option and argument and verify
  TEST_TRUE(MyProg->AddOption(MyOpt));
  TEST_TRUE(MyProg->AddArgument(MyArg));
  TEST_EQ(0, MyProg->GetOption("file").Count());
  TEST_EQ(std::string("test.txt"), MyProg->GetOption("file").GetArguments()[0]);
  TEST_EQ(std::string("1.2.3.4"), MyProg->GetArgument(0).GetValue());

  // Parse command line and validate
  TEST_TRUE(MyProg->ParseCommandLine(4, argv[1]));
  TEST_EQ(1, MyProg->GetOption("file").Count());
  TEST_EQ(std::string("filename.ext"), MyProg->GetOption("file").GetArguments()[0]);
  TEST_EQ(std::string("addr"), MyProg->GetArgument(0).GetValue());

  delete (MyProg);

  // Create new test program and verify
  MyProg = new TestProgram;
  TEST_ISNOT_NULL(MyProg);

  // Add option and argument and verify
  TEST_TRUE(MyProg->AddOption(MyOpt));
  TEST_TRUE(MyProg->AddArgument(MyArg));
  TEST_EQ(0, MyProg->GetOption("file").Count());
  TEST_EQ(std::string("test.txt"), MyProg->GetOption("file").GetArguments()[0]);
  TEST_EQ(std::string("1.2.3.4"), MyProg->GetArgument(0).GetValue());

  // Parse command line and validate
  TEST_TRUE(MyProg->ParseCommandLine(6, argv[2]));
  TEST_EQ(2, MyProg->GetOption("file").Count());
  TEST_EQ(std::string("filename_1.txt"), MyProg->GetOption("file").GetArguments()[0]);
  TEST_EQ(std::string("filename_2.txt"), MyProg->GetOption("file").GetArguments()[1]);
  TEST_EQ(std::string("addr"), MyProg->GetArgument(0).GetValue());

  delete (MyProg);

  // Return success
  return (0);

}

