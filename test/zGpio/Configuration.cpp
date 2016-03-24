#include <string>
#include <list>
#include <mutex>

#include <zutils/zLog.h>
#include <zutils/zSem.h>
#include <zutils/zThread.h>
#include <zutils/zData.h>
#include <zutils/zEvent.h>
#include <zutils/zConf.h>
#include <zutils/zSwitch.h>

#include <zutils/zGpio.h>

#include "zGpioTest.h"

using namespace zUtils;
using namespace Test;

int
zGpioTest_ConfigurationCtor(void* arg)
{

  std::string path;

  ZLOG_DEBUG("#############################################################");
  ZLOG_DEBUG("# zGpioTest_ConfigurationCtor()");
  ZLOG_DEBUG("#############################################################");

  zConf::Configuration config;
  path = zData::Data::PathConcat(zGpio::Configuration::ConfigRoot,
      zGpio::Configuration::ConfigIdentifierValuePath);
  config.Put("8", path);
  path = zData::Data::PathConcat(zGpio::Configuration::ConfigRoot,
      zGpio::Configuration::ConfigExportFilenamePath);
  config.Put("export", path);
  path = zData::Data::PathConcat(zGpio::Configuration::ConfigRoot,
      zGpio::Configuration::ConfigUnexportFilenamePath);
  config.Put("unexport", path);
  path = zData::Data::PathConcat(zGpio::Configuration::ConfigRoot,
      zGpio::Configuration::ConfigDirectionFilenamePath);
  config.Put("direction", path);
  path = zData::Data::PathConcat(zGpio::Configuration::ConfigRoot,
      zGpio::Configuration::ConfigDirectionValuePath);
  config.Put("out", path);
  path = zData::Data::PathConcat(zGpio::Configuration::ConfigRoot,
      zGpio::Configuration::ConfigStateFilenamePath);
  config.Put("state", path);
  path = zData::Data::PathConcat(zGpio::Configuration::ConfigRoot,
      zGpio::Configuration::ConfigStateValuePath);
  config.Put("active", path);
  path = zData::Data::PathConcat(zGpio::Configuration::ConfigRoot,
      zGpio::Configuration::ConfigEdgeFilenamePath);
  config.Put("edge", path);
  path = zData::Data::PathConcat(zGpio::Configuration::ConfigRoot,
      zGpio::Configuration::ConfigEdgeValuePath);
  config.Put("both", path);
  config.Commit();

  // Create new GPIO port configuration and verify
  zGpio::Configuration *MyConfig = new zGpio::Configuration(config);
  TEST_ISNOT_NULL(MyConfig);
  TEST_EQ(MyConfig->Identifier(), 8);
  TEST_EQ(MyConfig->ExportFilename(), "export");
  TEST_EQ(MyConfig->UnexportFilename(), "unexport");
  TEST_EQ(MyConfig->DirectionFilename(), "direction");
  TEST_EQ(MyConfig->Direction(), "out");
  TEST_EQ(MyConfig->StateFilename(), "state");
  TEST_EQ(MyConfig->State(), "active");
  TEST_EQ(MyConfig->EdgeFilename(), "edge");
  TEST_EQ(MyConfig->Edge(), "both");

  // Clean up
  delete (MyConfig);
  return (0);
}


int
zGpioTest_ConfigurationGetSet(void* arg)
{
  // Create new GPIO port configuration and verify
  zGpio::Configuration *MyConfig = new zGpio::Configuration;
  TEST_ISNOT_NULL(MyConfig);
  TEST_EQ(MyConfig->Identifier(), zGpio::Configuration::ConfigIdentifierValueDefault);
  TEST_EQ(MyConfig->ExportFilename(), zGpio::Configuration::ConfigExportFilenameDefault);
  TEST_EQ(MyConfig->UnexportFilename(), zGpio::Configuration::ConfigUnexportFilenameDefault);
  TEST_EQ(MyConfig->DirectionFilename(), zGpio::Configuration::ConfigDirectionFilenameDefault);
  TEST_EQ(MyConfig->Direction(), zGpio::Configuration::ConfigDirectionValueDefault);
  TEST_EQ(MyConfig->StateFilename(), zGpio::Configuration::ConfigStateFilenameDefault);
  TEST_EQ(MyConfig->State(), zGpio::Configuration::ConfigStateValueDefault);
  TEST_EQ(MyConfig->EdgeFilename(), zGpio::Configuration::ConfigEdgeFilenameDefault);
  TEST_EQ(MyConfig->Edge(), zGpio::Configuration::ConfigEdgeValueDefault);

  // Set all configuration parameters
  TEST_TRUE(MyConfig->Identifier(8));
  TEST_TRUE(MyConfig->ExportFilename("export"));
  TEST_TRUE(MyConfig->UnexportFilename("unexport"));
  TEST_TRUE(MyConfig->DirectionFilename("direction"));
  TEST_TRUE(MyConfig->Direction("out"));
  TEST_TRUE(MyConfig->StateFilename("state"));
  TEST_TRUE(MyConfig->State("active"));
  TEST_TRUE(MyConfig->EdgeFilename("edge"));
  TEST_TRUE(MyConfig->Edge("both"));

  // Verify
  TEST_EQ(MyConfig->Identifier(), 8);
  TEST_EQ(MyConfig->ExportFilename(), "export");
  TEST_EQ(MyConfig->UnexportFilename(), "unexport");
  TEST_EQ(MyConfig->DirectionFilename(), "direction");
  TEST_EQ(MyConfig->Direction(), "out");
  TEST_EQ(MyConfig->StateFilename(), "state");
  TEST_EQ(MyConfig->State(), "active");
  TEST_EQ(MyConfig->EdgeFilename(), "edge");
  TEST_EQ(MyConfig->Edge(), "both");

  // Clean up
  delete (MyConfig);
  return (0);
}
