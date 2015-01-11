#include "zOptTest.h"

using namespace Test;
using namespace zUtils;

int
zOptTest_Defaults(void* arg_)
{

  // Create new option and validate
  zOpt::Option opt_f("f");
  TEST_EQ(std::string("f"), opt_f.GetName());
  TEST_EQ(std::string(""), opt_f.GetDesc());
  TEST_EQ(std::string(""), opt_f.GetArg());
  TEST_FALSE(opt_f.GetPresent());

  // Create new getopt and validate
  zOpt::GetOpt opt;
  opt.AddOption(opt_f);

  // Return success
  return (0);

}

int
zOptTest_ParseSimpleShort(void* arg_)
{

  int argc = 3;
  const char *argv[] = { "program", "-f", "filename.ext" };

  // Create new option and validate
  zOpt::Option opt_f("f");
  TEST_EQ(std::string("f"), opt_f.GetName());
  TEST_EQ(std::string(""), opt_f.GetDesc());
  TEST_EQ(std::string(""), opt_f.GetArg());
  TEST_FALSE(opt_f.GetPresent());

  // Set option description
  opt_f.SetDesc("File");

  // Create new getopt and validate
  zOpt::GetOpt opts;
  opts.AddOption(opt_f);

  // Parse command line and validate
  opts.Parse(argc, argv);
  TEST_TRUE(opts["f"].GetPresent());
  TEST_EQ(std::string("filename.ext"), opts["f"].GetArg());

  // Return success
  return (0);

}

int
zOptTest_ParseSimpleLong(void* arg_)
{

  int argc = 3;
  const char *argv[] = { "program", "--file", "filename.ext" };

  // Create new option and validate
  zOpt::Option opt_f("file");
  TEST_EQ("file", opt_f.GetName());
  TEST_EQ("", opt_f.GetDesc());
  TEST_EQ("", opt_f.GetArg());
  TEST_FALSE(opt_f.GetPresent());

  // Create new getopt and validate
  zOpt::GetOpt opts;
  opts.AddOption(opt_f);

  // Parse command line and validate
  opts.Parse(argc, argv);
  TEST_TRUE(opts["file"].GetPresent());
  TEST_EQ(std::string("filename.ext"), opts["file"].GetArg());

  // Return success
  return (0);

}

