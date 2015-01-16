#include "zOptTest.h"

using namespace Test;
using namespace zUtils;

int
zOptTest_OptionDefaults(void* arg_)
{

  // Create new option and validate
  zProgOpt::Option myOpt(
      (zProgOpt::Option::FLAGS_ISOPTIONAL | zProgOpt::Option::FLAGS_ARG_ISOPTIONAL), "f");
  TEST_EQ((zProgOpt::Option::FLAGS_ISOPTIONAL | zProgOpt::Option::FLAGS_ARG_ISOPTIONAL),
      myOpt.Flags());
  TEST_EQ(std::string("f"), myOpt.Name());
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
  TEST_EQ(std::string(""), myTable.Usage());
  TEST_EQ(std::string(""), myTable.ErrorString());

  // Return success
  return (0);

}

