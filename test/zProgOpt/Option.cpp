#include "zOptTest.h"

using namespace Test;
using namespace zUtils;

int
zOptTest_OptionString(void* arg_)
{
  // Create new option and validate
  zProgOpt::Option opt_f(
      (zProgOpt::Option::FLAGS_ISOPTIONAL | zProgOpt::Option::FLAGS_ARG_ISOPTIONAL), "f");
  TEST_EQ(std::string("f"), opt_f.Name());
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
      (zProgOpt::Option::FLAGS_ISOPTIONAL | zProgOpt::Option::FLAGS_ARG_ISOPTIONAL), "f");
  TEST_EQ(std::string("f"), opt_f.Name());
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
      (zProgOpt::Option::FLAGS_ISOPTIONAL | zProgOpt::Option::FLAGS_ARG_ISOPTIONAL), "f");
  TEST_EQ(std::string("f"), opt_f.Name());
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
      (zProgOpt::Option::FLAGS_ISOPTIONAL | zProgOpt::Option::FLAGS_ARG_ISOPTIONAL), "f");
  TEST_EQ(std::string("f"), opt_f.Name());
  TEST_EQ(std::string(""), opt_f.HelpMsg());
  TEST_EQ(0.0, opt_f.Arg<double>());

  // Set default argument value
  TEST_TRUE(opt_f.Default(8.0098));
  TEST_EQ(8.0098, opt_f.Arg<double>());

  // Return success
  return (0);

}

