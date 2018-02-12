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

#include <zutils/zLog.h>
using namespace zUtils;
ZLOG_MODULE_INIT(zLog::Log::MODULE_TEST);

#include "zConfTest.h"

using namespace Test;
using namespace zUtils;

int
zConfigTest_PathDefaults(void* arg_)
{

  ZLOG_DEBUG("#############################################################");
  ZLOG_DEBUG("# zConfigTest_PathDefaults()");
  ZLOG_DEBUG("#############################################################");

  // Create new configuration data item and verify
  zConfig::ConfigPath *MyPath = new zConfig::ConfigPath;
  TEST_ISNOT_NULL(MyPath);

  // Cleanup
  delete (MyPath);

  // Return success
  return (0);
}

int
zConfigTest_DataDefaults(void* arg_)
{

  ZLOG_DEBUG("#############################################################");
  ZLOG_DEBUG("# zConfigTest_DataDefaults()");
  ZLOG_DEBUG("#############################################################");

  // Create new configuration data item and verify
  zConfig::ConfigData *MyData = new zConfig::ConfigData;
  TEST_ISNOT_NULL(MyData);

  // Cleanup
  delete (MyData);

  // Return success
  return (0);
}

int
zConfigTest_ConfigurationDefaults(void* arg_)
{

  ZLOG_DEBUG("#############################################################");
  ZLOG_DEBUG("# zConfigTest_ConfigurationDefaults()");
  ZLOG_DEBUG("#############################################################");

  // Create new configuration data item and verify
  zConfig::Configuration *MyConfig = new zConfig::Configuration;
  TEST_ISNOT_NULL(MyConfig);

  // Cleanup
  delete (MyConfig);

  // Return success
  return (0);
}

int
zConfigTest_ConnectorDefaults(void* arg_)
{

  ZLOG_DEBUG("#############################################################");
  ZLOG_DEBUG("# zConfigTest_ConnectorDefaults()");
  ZLOG_DEBUG("#############################################################");

  // Create new configuration data connector and verify
  TestConnector *myConnector = new TestConnector;
  TEST_ISNOT_NULL(myConnector);

  // Cleanup
  delete (myConnector);

  // Return success
  return (0);
}

