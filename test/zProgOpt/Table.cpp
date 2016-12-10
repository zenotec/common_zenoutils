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
zOptTest_ParseSimpleShort(void* arg_)
{

  int argc = 3;
  const char *argv[] = { "program", "-f", "filename.ext" };

  // Create new option and validate
  zProgOpt::Option myOpt(
      (zProgOpt::Option::FLAGS_ISOPTIONAL | zProgOpt::Option::FLAGS_ARG_ISOPTIONAL));
  TEST_EQ((zProgOpt::Option::FLAGS_ISOPTIONAL | zProgOpt::Option::FLAGS_ARG_ISOPTIONAL),
      myOpt.Flags());
  TEST_EQ('\0', myOpt.ShortName());
  TEST_EQ(std::string(""), myOpt.LongName());
  TEST_EQ(std::string(""), myOpt.HelpMsg());
  TEST_EQ(std::string(""), myOpt.Arg<std::string>());

  // Set short name and verify
  TEST_TRUE(myOpt.ShortName('f'));
  TEST_EQ('f', myOpt.ShortName());

  // Set long option and verify
  TEST_TRUE(myOpt.LongName("file"));
  TEST_EQ(std::string("file"), myOpt.LongName());

  // Set default argument value
  TEST_TRUE(myOpt.Default(std::string("test")));
  TEST_EQ(std::string("test"), myOpt.Arg<std::string>());

  // Set option help message
  myOpt.HelpMsg("File");
  TEST_EQ(std::string("File"), myOpt.HelpMsg());

  // Create new program options table and validate
  zProgOpt::Table myTable;
  TEST_EQ(std::string("USAGE: \n"), myTable.Usage());
  TEST_TRUE(myTable.AddOption(myOpt));

  // Parse command line and validate
  TEST_TRUE(myTable.Parse(argc, argv));
  TEST_EQ(std::string("USAGE: program\n\t-f,--file\t\tFile\n"), myTable.Usage());
  TEST_EQ(1, myTable.Count("f"));
  TEST_EQ(std::string("filename.ext"), myTable["file"].Arg<std::string>());

  // Return success
  return (0);

}

int
zOptTest_ParseSimpleLong(void* arg_)
{

  int argc = 3;
  const char *argv[] = { "program", "--file", "filename.ext" };

  // Create new option and validate
  zProgOpt::Option myOpt(
      (zProgOpt::Option::FLAGS_ISOPTIONAL | zProgOpt::Option::FLAGS_ARG_ISOPTIONAL));
  TEST_EQ((zProgOpt::Option::FLAGS_ISOPTIONAL | zProgOpt::Option::FLAGS_ARG_ISOPTIONAL),
      myOpt.Flags());
  TEST_EQ('\0', myOpt.ShortName());
  TEST_EQ(std::string(""), myOpt.LongName());
  TEST_EQ(std::string(""), myOpt.HelpMsg());
  TEST_EQ(std::string(""), myOpt.Arg<std::string>());

  // Set short name and verify
  TEST_TRUE(myOpt.ShortName('f'));
  TEST_EQ('f', myOpt.ShortName());

  // Set long option and verify
  TEST_TRUE(myOpt.LongName("file"));
  TEST_EQ(std::string("file"), myOpt.LongName());

  // Set default argument value
  TEST_TRUE(myOpt.Default(std::string("test")));
  TEST_EQ(std::string("test"), myOpt.Arg<std::string>());

  // Set option help message
  myOpt.HelpMsg("File");
  TEST_EQ(std::string("File"), myOpt.HelpMsg());

  // Create new program options table and validate
  zProgOpt::Table myTable;
  TEST_EQ(std::string("USAGE: \n"), myTable.Usage());
  TEST_TRUE(myTable.AddOption(myOpt));

  // Parse command line and validate
  TEST_TRUE(myTable.Parse(argc, argv));
  TEST_EQ(std::string("USAGE: program\n\t-f,--file\t\tFile\n"), myTable.Usage());
  TEST_EQ(1, myTable.Count("file"));
  TEST_EQ(std::string("filename.ext"), myTable["file"].Arg<std::string>());

  // Return success
  return (0);

}

