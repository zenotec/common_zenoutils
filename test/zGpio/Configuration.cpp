#include <string>
#include <list>
#include <mutex>

#include <zutils/zLog.h>
#include <zutils/zSem.h>
#include <zutils/zThread.h>
#include <zutils/zData.h>
#include <zutils/zEvent.h>
#include <zutils/zConfig.h>
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

  // Create new GPIO port configuration and verify
  zGpio::GpioConfiguration *MyConfig = new zGpio::GpioConfiguration();
  TEST_ISNOT_NULL(MyConfig);
  TEST_EQ(MyConfig->Identifier(), 0);
  TEST_EQ(MyConfig->ExportFilename(), zGpio::GpioConfiguration::ConfigExportFilenameDefault);
  TEST_EQ(MyConfig->UnexportFilename(), zGpio::GpioConfiguration::ConfigUnexportFilenameDefault);
  TEST_EQ(MyConfig->DirectionFilename(), zGpio::GpioConfiguration::ConfigDirectionFilenameDefault);
  TEST_EQ(MyConfig->Direction(), zGpio::GpioConfiguration::ConfigDirectionValueDefault);
  TEST_EQ(MyConfig->StateFilename(), zGpio::GpioConfiguration::ConfigStateFilenameDefault);
  TEST_EQ(MyConfig->State(), zGpio::GpioConfiguration::ConfigStateValueDefault);
  TEST_EQ(MyConfig->EdgeFilename(), zGpio::GpioConfiguration::ConfigEdgeFilenameDefault);
  TEST_EQ(MyConfig->Edge(), zGpio::GpioConfiguration::ConfigEdgeValueDefault);

  // Clean up
  delete (MyConfig);

  zConfig::Configuration config;
  path = zData::Data::PathConcat(zGpio::GpioConfiguration::ConfigRoot,
      zGpio::GpioConfiguration::ConfigIdentifierPath);
  config.Put("8", path);
  path = zData::Data::PathConcat(zGpio::GpioConfiguration::ConfigRoot,
      zGpio::GpioConfiguration::ConfigExportFilenamePath);
  config.Put("export", path);
  path = zData::Data::PathConcat(zGpio::GpioConfiguration::ConfigRoot,
      zGpio::GpioConfiguration::ConfigUnexportFilenamePath);
  config.Put("unexport", path);
  path = zData::Data::PathConcat(zGpio::GpioConfiguration::ConfigRoot,
      zGpio::GpioConfiguration::ConfigDirectionFilenamePath);
  config.Put("direction", path);
  path = zData::Data::PathConcat(zGpio::GpioConfiguration::ConfigRoot,
      zGpio::GpioConfiguration::ConfigDirectionValuePath);
  config.Put("out", path);
  path = zData::Data::PathConcat(zGpio::GpioConfiguration::ConfigRoot,
      zGpio::GpioConfiguration::ConfigStateFilenamePath);
  config.Put("state", path);
  path = zData::Data::PathConcat(zGpio::GpioConfiguration::ConfigRoot,
      zGpio::GpioConfiguration::ConfigStateValuePath);
  config.Put("active", path);
  path = zData::Data::PathConcat(zGpio::GpioConfiguration::ConfigRoot,
      zGpio::GpioConfiguration::ConfigEdgeFilenamePath);
  config.Put("edge", path);
  path = zData::Data::PathConcat(zGpio::GpioConfiguration::ConfigRoot,
      zGpio::GpioConfiguration::ConfigEdgeValuePath);
  config.Put("both", path);
  config.Commit();

  // Create new GPIO port configuration and verify
  MyConfig = new zGpio::GpioConfiguration(config);
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
  zGpio::GpioConfiguration *MyConfig = new zGpio::GpioConfiguration;
  TEST_ISNOT_NULL(MyConfig);
  TEST_EQ(MyConfig->Identifier(), zGpio::GpioConfiguration::ConfigIdentifierDefault);
  TEST_EQ(MyConfig->ExportFilename(), zGpio::GpioConfiguration::ConfigExportFilenameDefault);
  TEST_EQ(MyConfig->UnexportFilename(), zGpio::GpioConfiguration::ConfigUnexportFilenameDefault);
  TEST_EQ(MyConfig->DirectionFilename(), zGpio::GpioConfiguration::ConfigDirectionFilenameDefault);
  TEST_EQ(MyConfig->Direction(), zGpio::GpioConfiguration::ConfigDirectionValueDefault);
  TEST_EQ(MyConfig->StateFilename(), zGpio::GpioConfiguration::ConfigStateFilenameDefault);
  TEST_EQ(MyConfig->State(), zGpio::GpioConfiguration::ConfigStateValueDefault);
  TEST_EQ(MyConfig->EdgeFilename(), zGpio::GpioConfiguration::ConfigEdgeFilenameDefault);
  TEST_EQ(MyConfig->Edge(), zGpio::GpioConfiguration::ConfigEdgeValueDefault);

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
