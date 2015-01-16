#include "zOptTest.h"

using namespace Test;
using namespace zUtils;

int
zOptTest_ParseSimpleShort(void* arg_)
{

  int argc = 3;
  const char *argv[] = { "program", "-f", "filename.ext" };

  // Create new option and validate
  zProgOpt::Option myOpt(
      (zProgOpt::Option::FLAGS_ISOPTIONAL | zProgOpt::Option::FLAGS_ARG_ISOPTIONAL), "f");
  TEST_EQ((zProgOpt::Option::FLAGS_ISOPTIONAL | zProgOpt::Option::FLAGS_ARG_ISOPTIONAL),
      myOpt.Flags());
  TEST_EQ(std::string("f"), myOpt.Name());
  TEST_EQ(std::string(""), myOpt.HelpMsg());
  TEST_EQ(std::string(""), myOpt.Arg<std::string>());

  // Set default argument value
  TEST_TRUE(myOpt.Default(std::string("test")));
  TEST_EQ(std::string("test"), myOpt.Arg<std::string>());

  // Set option help message
  myOpt.HelpMsg("File");
  TEST_EQ(std::string("File"), myOpt.HelpMsg());

  // Create new program options table and validate
  zProgOpt::Table myTable;
  TEST_TRUE(myTable.AddOption(myOpt));

  // Parse command line and validate
  TEST_TRUE(myTable.Parse(argc, argv));
  TEST_EQ(1, myTable.Count("f"));
  TEST_EQ(std::string("filename.ext"), myTable["f"].Arg<std::string>());

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
      (zProgOpt::Option::FLAGS_ISOPTIONAL | zProgOpt::Option::FLAGS_ARG_ISOPTIONAL), "file");
  TEST_EQ((zProgOpt::Option::FLAGS_ISOPTIONAL | zProgOpt::Option::FLAGS_ARG_ISOPTIONAL),
      myOpt.Flags());
  TEST_EQ(std::string("file"), myOpt.Name());
  TEST_EQ(std::string(""), myOpt.HelpMsg());
  TEST_EQ(std::string(""), myOpt.Arg<std::string>());

  // Set default argument value
  TEST_TRUE(myOpt.Default(std::string("test")));
  TEST_EQ(std::string("test"), myOpt.Arg<std::string>());

  // Set option help message
  myOpt.HelpMsg("File");
  TEST_EQ(std::string("File"), myOpt.HelpMsg());

  // Create new program options table and validate
  zProgOpt::Table myTable;
  TEST_TRUE(myTable.AddOption(myOpt));

  // Parse command line and validate
  TEST_TRUE(myTable.Parse(argc, argv));
  TEST_EQ(1, myTable.Count("file"));
  TEST_EQ(std::string("filename.ext"), myTable["file"].Arg<std::string>());

  // Return success
  return (0);

}

