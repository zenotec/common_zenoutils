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
zGpioTest_ConfigurationDefaults(void* arg)
{

  ZLOG_DEBUG("#############################################################");
  ZLOG_DEBUG("# zGpioTest_ConfigurationDefaults()");
  ZLOG_DEBUG("#############################################################");

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

  // Clean up
  delete (MyConfig);
  return (0);
}

int
zGpioTest_PortDefaults(void* arg)
{

  ZLOG_DEBUG("#############################################################");
  ZLOG_DEBUG("# zGpioTest_PortDefaults()");
  ZLOG_DEBUG("#############################################################");

  // Create new GPIO Port test configuration
  TestPortConfig *MyConfig = new TestPortConfig(1);
  TEST_ISNOT_NULL(MyConfig);

  // Create new GPIO port and verify
  zGpio::GpioPort *MyPort = new zGpio::GpioPort(*MyConfig);
  TEST_ISNOT_NULL(MyPort);

  // Clean up
  delete (MyPort);
  delete (MyConfig);
  return (0);
}

int
zGpioTest_HandlerDefaults(void* arg)
{

  ZLOG_DEBUG("#############################################################");
  ZLOG_DEBUG("# zGpioTest_HandlerDefaults()");
  ZLOG_DEBUG("#############################################################");

  // Create new GPIO handler and verify
  zGpio::GpioHandler *myHandler = new zGpio::GpioHandler;
  TEST_ISNOT_NULL(myHandler);

  // Clean up
  delete (myHandler);
  return (0);
}

int
zGpioTest_SwitchDefaults(void* arg)
{

  ZLOG_DEBUG("#############################################################");
  ZLOG_DEBUG("# zGpioTest_SwitchDefaults()");
  ZLOG_DEBUG("#############################################################");
#if 0
    // Create new Switch and verify
    zGpio::Switch* mySwitch = new zGpio::Switch( zSwitch::Switch::STATE_OFF );
    TEST_ISNOT_NULL( mySwitch );
    TEST_FALSE( mySwitch->IsOn() );
    TEST_TRUE( mySwitch->IsOff() );
    TEST_IS_NULL( mySwitch->GetPort( 1 ) );

// Clean up
    delete (mySwitch);
  return (0);
#else
  return (1);
#endif
}
