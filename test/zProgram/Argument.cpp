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
zProgramTest_ProgramArgumentString(void* arg_)
{
  // Create new argument and validate
  ProgramArgument myArg;
  TEST_IS_ZERO(myArg.Count());
  TEST_TRUE(ProgramArgument::FLAGS_NONE == myArg.GetFlags());
  TEST_IS_ZERO(myArg.GetPosition());
  TEST_EQ(std::string(""), myArg.GetName());
  TEST_EQ(std::string(""), myArg.GetDescription());
  TEST_EQ(std::string(""), myArg.GetDefault());
  TEST_EQ(std::string(""), myArg.GetValue());

  // Set default argument value
  TEST_TRUE(myArg.SetDefault(std::string("test")));
  TEST_EQ(std::string("test"), myArg.GetValue());

  // Return success
  return (0);

}


