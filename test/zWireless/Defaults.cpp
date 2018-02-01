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
#include "zWirelessTest.h"

#include <zutils/zCompatibility.h>

#include <zutils/zLog.h>
using namespace zUtils;
ZLOG_MODULE_INIT(zLog::Log::MODULE_TEST);

#include <zutils/zWireless.h>

using namespace Test;
using namespace zUtils;

int
zWirelessTest_WirelessConfigurationDefaults(void* arg)
{

  ZLOG_DEBUG("#############################################################");
  ZLOG_DEBUG("# zWirelessTest_WirelessConfigurationDefaults()");
  ZLOG_DEBUG("#############################################################");

  zWireless::ConfigData *MyConfig = new zWireless::ConfigData;
  TEST_ISNOT_NULL(MyConfig);
  TEST_EQ(zWireless::ConfigData::ConfigIfIndexDefault, MyConfig->GetIfIndex());
  TEST_EQ(zWireless::ConfigData::ConfigIfNameDefault, MyConfig->GetIfName());
  TEST_EQ(zWireless::ConfigData::IFTYPE_IEEE80211, MyConfig->GetIfType());
  TEST_EQ(zWireless::ConfigData::ConfigHwAddressDefault, MyConfig->GetHwAddress());
  TEST_EQ(zWireless::ConfigData::ConfigMtuDefault, MyConfig->GetMtu());
  TEST_EQ(zWireless::ConfigData::ConfigIpAddressDefault, MyConfig->GetIpAddress());
  TEST_EQ(zWireless::ConfigData::ConfigNetmaskDefault, MyConfig->GetNetmask());
  TEST_EQ(zWireless::ConfigData::STATE_DEF, MyConfig->GetAdminState());
  TEST_EQ(zWireless::ConfigData::ConfigPhyIndexDefault, MyConfig->GetPhyIndex());
  TEST_EQ(zWireless::ConfigData::ConfigPhyNameDefault, MyConfig->GetPhyName());
  TEST_EQ(zWireless::ConfigData::HWMODE_DEF, MyConfig->GetHwMode());
  TEST_EQ(zWireless::ConfigData::HTMODE_DEF, MyConfig->GetHtMode());
  TEST_EQ(zWireless::ConfigData::OPMODE_DEF, MyConfig->GetOpMode());
  TEST_EQ(zWireless::ConfigData::ConfigSsidDefault, MyConfig->GetSsid());
  TEST_EQ(zWireless::ConfigData::ConfigBssidDefault, MyConfig->GetBssid());
  TEST_EQ(zWireless::ConfigData::ConfigChannelDefault, MyConfig->GetChannel());
  TEST_EQ(zWireless::ConfigData::ConfigTxPowerDefault, MyConfig->GetTxPower());

  // Cleanup
  delete (MyConfig);

  // Return success
  return (0);

}

int
zWirelessTest_WirelessDefaults(void* arg)
{

  ZLOG_DEBUG("#############################################################");
  ZLOG_DEBUG("# zWirelessTest_WirelessDefaults()");
  ZLOG_DEBUG("#############################################################");

  zWireless::Interface *MyInterface = NULL;

  MyInterface = new zWireless::Interface(zWireless::ConfigData::ConfigIfNameDefault);
  TEST_ISNOT_NULL(MyInterface);
  TEST_EQ(zWireless::ConfigData::ConfigIfIndexDefault, MyInterface->GetIfIndex());
  TEST_EQ(zWireless::ConfigData::ConfigIfNameDefault, MyInterface->GetIfName());
  TEST_EQ(zWireless::ConfigData::IFTYPE_IEEE80211, MyInterface->GetIfType());
  TEST_EQ(zWireless::ConfigData::ConfigHwAddressDefault, MyInterface->GetHwAddress());
  TEST_EQ(zWireless::ConfigData::ConfigMtuDefault, MyInterface->GetMtu());
  TEST_EQ(zWireless::ConfigData::ConfigIpAddressDefault, MyInterface->GetIpAddress());
  TEST_EQ(zWireless::ConfigData::ConfigNetmaskDefault, MyInterface->GetNetmask());
  TEST_EQ(zWireless::ConfigData::STATE_DEF, MyInterface->GetAdminState());
  TEST_EQ(zWireless::ConfigData::ConfigPhyIndexDefault, MyInterface->GetPhyIndex());
  TEST_EQ(zWireless::ConfigData::ConfigPhyNameDefault, MyInterface->GetPhyName());
  TEST_EQ(zWireless::ConfigData::HWMODE_DEF, MyInterface->GetHwMode());
  TEST_EQ(zWireless::ConfigData::HTMODE_DEF, MyInterface->GetHtMode());
  TEST_EQ(zWireless::ConfigData::OPMODE_DEF, MyInterface->GetOpMode());
  TEST_EQ(zWireless::ConfigData::ConfigChannelDefault, MyInterface->GetChannel());
  TEST_EQ(zWireless::ConfigData::ConfigTxPowerDefault, MyInterface->GetTxPower());

  // Cleanup
  delete (MyInterface);

  // Return success
  return (0);

}

