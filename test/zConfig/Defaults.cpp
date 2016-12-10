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
#include "zConfTest.h"

using namespace Test;
using namespace zUtils;

int
zConfigTest_DataDefaults(void* arg_)
{

  ZLOG_DEBUG("#############################################################");
  ZLOG_DEBUG("# zConfigTest_DataDefaults()");
  ZLOG_DEBUG("#############################################################");

  // Create new configuration data item and verify
  zConfig::Configuration *myData = new zConfig::Configuration;
  TEST_ISNOT_NULL(myData);

  // Cleanup
  delete (myData);

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

int
zConfigTest_HandlerDefaults(void* arg_)
{

  ZLOG_DEBUG("#############################################################");
  ZLOG_DEBUG("# zConfigTest_HandlerDefaults()");
  ZLOG_DEBUG("#############################################################");

  // Create new configuration data connector and verify
  zConfig::ConfigurationHandler *MyHandler = new zConfig::ConfigurationHandler;
  TEST_ISNOT_NULL(MyHandler);

  // Cleanup
  delete (MyHandler);

  // Return success
  return (0);
}
