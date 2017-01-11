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
zProgramTest_ProgramOptionString(void* arg_)
{
  // Create new option and validate
  ProgramOption myOpt;
  TEST_IS_ZERO(myOpt.Count());
  TEST_TRUE(ProgramOption::FLAGS_NONE == myOpt.GetFlags());
  TEST_EQ('\0', myOpt.GetShortName());
  TEST_EQ(std::string(""), myOpt.GetLongName());
  TEST_EQ(std::string(""), myOpt.GetDescription());
  TEST_EQ(std::string(""), myOpt.GetDefault());
  TEST_EQ(std::string(""), myOpt.GetArguments()[0]);

  // Set default argument value
  TEST_TRUE(myOpt.SetDefault(std::string("test")));
  TEST_EQ(std::string("test"), myOpt.GetArguments()[0]);

  // Return success
  return (0);

}


