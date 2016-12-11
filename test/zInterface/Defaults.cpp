/*
 * Copyright (c) 2016 Cable Television Laboratories, Inc. ("CableLabs")
 *                    and others.  All rights reserved.
 *
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

#include "UnitTest.h"
#include "zInterfaceTest.h"

#include <zutils/zCompatibility.h>
#include <zutils/zInterface.h>

using namespace Test;
using namespace zUtils;
using namespace zUtils::zInterface;

int
zInterfaceTest_InterfaceConfigurationDefaults(void* arg)
{

  InterfaceConfigData *MyConfig = new InterfaceConfigData;
  TEST_ISNOT_NULL(MyConfig);
  TEST_EQ(InterfaceConfigData::ConfigNameDefault, MyConfig->GetName());
  TEST_EQ(InterfaceConfigData::ConfigTypeDefault, MyConfig->GetType());
  TEST_EQ(InterfaceConfigData::ConfigStateDefault, MyConfig->GetState());
  TEST_EQ(InterfaceConfigData::ConfigRateDefault, MyConfig->GetRate());

  // Cleanup
  delete (MyConfig);

  // Return success
  return (0);

}

int
zInterfaceTest_InterfaceDefaults(void* arg)
{

  Interface *MyInterface = new zInterface::Interface;
  TEST_ISNOT_NULL(MyInterface);
  TEST_EQ(InterfaceConfigData::ConfigNameDefault, MyInterface->GetName());
  TEST_EQ(InterfaceConfigData::ConfigTypeDefault, MyInterface->GetType());
  TEST_EQ(InterfaceConfigData::ConfigStateDefault, MyInterface->GetState());
  TEST_EQ(InterfaceConfigData::ConfigRateDefault, MyInterface->GetRate());

  // Cleanup
  delete (MyInterface);

  // Return success
  return (0);

}

int
zInterfaceTest_InterfaceHandlerDefaults(void* arg)
{

  InterfaceHandler *MyHandler = new InterfaceHandler;
  TEST_ISNOT_NULL(MyHandler);

  // Cleanup
  delete (MyHandler);

  // Return success
  return (0);

}

