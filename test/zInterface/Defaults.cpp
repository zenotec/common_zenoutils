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

  ZLOG_DEBUG("#############################################################");
  ZLOG_DEBUG("# zInterfaceTest_InterfaceConfigurationDefaults()");
  ZLOG_DEBUG("#############################################################");

  ConfigData *MyConfig = new ConfigData;
  TEST_ISNOT_NULL(MyConfig);
  TEST_EQ(ConfigData::ConfigNameDefault, MyConfig->GetIfName());
  TEST_EQ(ConfigData::IFTYPE_DEF, MyConfig->GetIfType());
  TEST_EQ(ConfigData::ConfigHwAddressDefault, MyConfig->GetHwAddress());
  TEST_EQ(ConfigData::ConfigMtuDefault, MyConfig->GetMtu());
  TEST_EQ(ConfigData::ConfigIpAddressDefault, MyConfig->GetIpAddress());
  TEST_EQ(ConfigData::ConfigNetmaskDefault, MyConfig->GetNetmask());
  TEST_EQ(ConfigData::STATE_DEF, MyConfig->GetAdminState());

  // Cleanup
  delete (MyConfig);

  // Return success
  return (0);

}

int
zInterfaceTest_InterfaceDefaults(void* arg)
{

  ZLOG_DEBUG("#############################################################");
  ZLOG_DEBUG("# zInterfaceTest_InterfaceDefaults()");
  ZLOG_DEBUG("#############################################################");

  ConfigData *MyConfig = new ConfigData;
  TEST_ISNOT_NULL(MyConfig);
  TEST_EQ(ConfigData::ConfigNameDefault, MyConfig->GetIfName());
  TEST_EQ(ConfigData::IFTYPE_DEF, MyConfig->GetIfType());
  TEST_EQ(ConfigData::ConfigHwAddressDefault, MyConfig->GetHwAddress());
  TEST_EQ(ConfigData::ConfigMtuDefault, MyConfig->GetMtu());
  TEST_EQ(ConfigData::ConfigIpAddressDefault, MyConfig->GetIpAddress());
  TEST_EQ(ConfigData::ConfigNetmaskDefault, MyConfig->GetNetmask());
  TEST_EQ(ConfigData::STATE_DEF, MyConfig->GetAdminState());

  Interface *MyInterface = new zInterface::Interface(*MyConfig);
  TEST_ISNOT_NULL(MyInterface);
  TEST_EQ(ConfigData::ConfigNameDefault, MyInterface->GetIfName());
  TEST_EQ(ConfigData::IFTYPE_DEF, MyInterface->Config.GetIfType());
  TEST_EQ(ConfigData::ConfigHwAddressDefault, MyInterface->Config.GetHwAddress());
  TEST_EQ(ConfigData::ConfigMtuDefault, MyInterface->Config.GetMtu());
  TEST_EQ(ConfigData::ConfigIpAddressDefault, MyInterface->Config.GetIpAddress());
  TEST_EQ(ConfigData::ConfigNetmaskDefault, MyInterface->Config.GetNetmask());
  TEST_EQ(ConfigData::STATE_ERR, MyInterface->GetAdminState());

  // Cleanup
  delete (MyInterface);
  delete (MyConfig);

  // Return success
  return (0);

}

