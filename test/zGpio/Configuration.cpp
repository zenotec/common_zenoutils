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

  ZLOG_DEBUG("#############################################################");
  ZLOG_DEBUG("# zGpioTest_ConfigurationCtor()");
  ZLOG_DEBUG("#############################################################");

  zGpio::GpioConfigPath path;

  // Create new GPIO port configuration and verify
  zGpio::GpioConfigData *MyConfig = new zGpio::GpioConfigData();
  TEST_ISNOT_NULL(MyConfig);
  TEST_EQ(MyConfig->Identifier(), zGpio::GpioConfigData::ConfigIdentifierDefault);
  TEST_EQ(MyConfig->ExportFilename(), zGpio::GpioConfigData::ConfigExportFilenameDefault);
  TEST_EQ(MyConfig->UnexportFilename(), zGpio::GpioConfigData::ConfigUnexportFilenameDefault);
  TEST_EQ(MyConfig->DirectionFilename(), zGpio::GpioConfigData::ConfigDirectionFilenameDefault);
  TEST_EQ(MyConfig->Direction(), zGpio::GpioConfigData::ConfigDirectionValueDefault);
  TEST_EQ(MyConfig->StateFilename(), zGpio::GpioConfigData::ConfigStateFilenameDefault);
  TEST_EQ(MyConfig->State(), zGpio::GpioConfigData::ConfigStateValueDefault);
  TEST_EQ(MyConfig->EdgeFilename(), zGpio::GpioConfigData::ConfigEdgeFilenameDefault);
  TEST_EQ(MyConfig->Edge(), zGpio::GpioConfigData::ConfigEdgeValueDefault);
//  MyConfig->Display();

  // Clean up
  delete (MyConfig);

  zConfig::ConfigData config;
  config.Put(path.Identifier(), "8");
  config.Put(path.ExportFilename(), "export");
  config.Put(path.UnexportFilename(), "unexport");
  config.Put(path.DirectionFilename(), "direction");
  config.Put(path.Direction(), "out");
  config.Put(path.StateFilename(), "state");
  config.Put(path.State(), "active");
  config.Put(path.EdgeFilename(), "edge");
  config.Put(path.Edge(), "both");

  // Create new GPIO port configuration and verify
  MyConfig = new zGpio::GpioConfigData(config);
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
  zGpio::GpioConfigData *MyConfig = new zGpio::GpioConfigData;
  TEST_ISNOT_NULL(MyConfig);
  TEST_EQ(MyConfig->Identifier(), zGpio::GpioConfigData::ConfigIdentifierDefault);
  TEST_EQ(MyConfig->ExportFilename(), zGpio::GpioConfigData::ConfigExportFilenameDefault);
  TEST_EQ(MyConfig->UnexportFilename(), zGpio::GpioConfigData::ConfigUnexportFilenameDefault);
  TEST_EQ(MyConfig->DirectionFilename(), zGpio::GpioConfigData::ConfigDirectionFilenameDefault);
  TEST_EQ(MyConfig->Direction(), zGpio::GpioConfigData::ConfigDirectionValueDefault);
  TEST_EQ(MyConfig->StateFilename(), zGpio::GpioConfigData::ConfigStateFilenameDefault);
  TEST_EQ(MyConfig->State(), zGpio::GpioConfigData::ConfigStateValueDefault);
  TEST_EQ(MyConfig->EdgeFilename(), zGpio::GpioConfigData::ConfigEdgeFilenameDefault);
  TEST_EQ(MyConfig->Edge(), zGpio::GpioConfigData::ConfigEdgeValueDefault);

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
//  MyConfig->DisplayJson();

  // Commity and verify
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
