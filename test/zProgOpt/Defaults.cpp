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

#include "zProgOptTest.h"

using namespace Test;
using namespace zUtils;

int
zOptTest_OptionDefaults(void* arg_)
{

  // Create new option and validate
  zProgOpt::Option myOpt(
      (zProgOpt::Option::FLAGS_ISOPTIONAL | zProgOpt::Option::FLAGS_ARG_ISOPTIONAL));
  TEST_EQ((zProgOpt::Option::FLAGS_ISOPTIONAL | zProgOpt::Option::FLAGS_ARG_ISOPTIONAL),
      myOpt.Flags());
  TEST_EQ('\0', myOpt.ShortName());
  TEST_EQ(std::string(""), myOpt.LongName());
  TEST_EQ(std::string(""), myOpt.HelpMsg());
  TEST_EQ(std::string(""), myOpt.Arg<std::string>());

  // Return success
  return (0);

}

int
zOptTest_TableDefaults(void* arg_)
{

  // Create new table and validate
  zProgOpt::Table myTable;
  TEST_EQ(-1, myTable.Count("f"));
  TEST_EQ(std::string("USAGE: \n"), myTable.Usage());
  TEST_EQ(std::string(""), myTable.ErrorString());

  // Return success
  return (0);

}

