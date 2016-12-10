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
zOptTest_OptionString(void* arg_)
{
  // Create new option and validate
  zProgOpt::Option opt_f(
      (zProgOpt::Option::FLAGS_ISOPTIONAL | zProgOpt::Option::FLAGS_ARG_ISOPTIONAL));
  TEST_EQ('\0', opt_f.ShortName());
  TEST_EQ(std::string(""), opt_f.LongName());
  TEST_EQ(std::string(""), opt_f.HelpMsg());
  TEST_EQ(std::string(""), opt_f.Arg<std::string>());

  // Set default argument value
  TEST_TRUE(opt_f.Default(std::string("test")));
  TEST_EQ(std::string("test"), opt_f.Arg<std::string>());

  // Return success
  return (0);

}

int
zOptTest_OptionInt(void* arg_)
{
  // Create new option and validate
  zProgOpt::Option opt_f(
      (zProgOpt::Option::FLAGS_ISOPTIONAL | zProgOpt::Option::FLAGS_ARG_ISOPTIONAL));
  TEST_EQ('\0', opt_f.ShortName());
  TEST_EQ(std::string(""), opt_f.LongName());
  TEST_EQ(std::string(""), opt_f.HelpMsg());
  TEST_EQ(0, opt_f.Arg<int>());

  // Set default argument value
  TEST_TRUE(opt_f.Default<int>(-8));
  TEST_EQ(-8, opt_f.Arg<int>());

  // Return success
  return (0);

}

int
zOptTest_OptionUint(void* arg_)
{
  // Create new option and validate
  zProgOpt::Option opt_f(
      (zProgOpt::Option::FLAGS_ISOPTIONAL | zProgOpt::Option::FLAGS_ARG_ISOPTIONAL));
  TEST_EQ('\0', opt_f.ShortName());
  TEST_EQ(std::string(""), opt_f.LongName());
  TEST_EQ(std::string(""), opt_f.HelpMsg());
  TEST_EQ(0, opt_f.Arg<unsigned int>());

  // Set default argument value
  TEST_TRUE(opt_f.Default(8));
  TEST_EQ(8, opt_f.Arg<unsigned int>());

  // Return success
  return (0);

}

int
zOptTest_OptionDouble(void* arg_)
{
  // Create new option and validate
  zProgOpt::Option opt_f(
      (zProgOpt::Option::FLAGS_ISOPTIONAL | zProgOpt::Option::FLAGS_ARG_ISOPTIONAL));
  TEST_EQ('\0', opt_f.ShortName());
  TEST_EQ(std::string(""), opt_f.LongName());
  TEST_EQ(std::string(""), opt_f.HelpMsg());
  TEST_EQ(0.0, opt_f.Arg<double>());

  // Set default argument value
  TEST_TRUE(opt_f.Default(8.0098));
  TEST_EQ(8.0098, opt_f.Arg<double>());

  // Return success
  return (0);

}

