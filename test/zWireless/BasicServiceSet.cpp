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

#include <unistd.h>

#include "UnitTest.h"
#include "zWirelessTest.h"

#include <zutils/zCompatibility.h>
#include <zutils/zWireless.h>
#include <zutils/zAccessPointInterface.h>
#include <zutils/zBasicServiceSet.h>

#include <ListInterfacesCommand.h>
#include <ListPhysCommand.h>

using namespace Test;
using namespace zUtils;

int
zWirelessTest_BasicServiceSet(void* arg)
{

  ZLOG_DEBUG("#############################################################");
  ZLOG_DEBUG("# zWirelessTest_BasicServiceSet()");
  ZLOG_DEBUG("#############################################################");

  // First step is to determine if we were executed with root privileges, if not, bypass test
  uid_t uid = getuid(), euid = geteuid();
  if (uid != 0 || uid != euid)
  {
    UTEST_BYPASS;
  }

  // Second step is to find an radio we can test with, if none exist, bypass test
  nl80211::ListPhysCommand physcmd;
  TEST_TRUE(physcmd.Exec());
  std::map<int, std::string> phys = physcmd();
  if (phys.empty())
  {
    UTEST_BYPASS;
  }

  zWireless::AccessPointInterface *MyInterface = NULL;

  FOREACH(auto& phy, phys)
  {
    std::string ifname = std::string("vap") + zToStr(phy.first);
    MyInterface = new zWireless::AccessPointInterface(ifname, phy.first);
    TEST_ISNOT_NULL(MyInterface);

    if (zInterface::ConfigData::ConfigIndexDefault == MyInterface->GetIfIndex())
    {
      TEST_EQ(zInterface::ConfigData::ConfigIndexDefault, MyInterface->GetIfIndex());
      TEST_NEQ(zInterface::ConfigData::ConfigNameDefault, MyInterface->GetIfName());
      TEST_EQ(ifname, MyInterface->GetIfName());
      TEST_EQ(zInterface::ConfigData::IFTYPE_IEEE80211, MyInterface->GetIfType());
      TEST_EQ(zInterface::ConfigData::ConfigHwAddressDefault, MyInterface->GetHwAddress());
      TEST_EQ(zInterface::ConfigData::ConfigMtuDefault, MyInterface->GetMtu());
//    TEST_NEQ(zInterface::ConfigData::ConfigIpAddressDefault, MyInterface->GetIpAddress());
//    TEST_NEQ(zInterface::ConfigData::ConfigNetmaskDefault, MyInterface->GetNetmask());
      TEST_EQ(zInterface::ConfigData::STATE_DEF, MyInterface->GetAdminState());
      TEST_NEQ(zWireless::ConfigData::ConfigPhyIndexDefault, MyInterface->GetPhyIndex());
      TEST_EQ(phy.first, MyInterface->GetPhyIndex());
      TEST_NEQ(zWireless::ConfigData::ConfigPhyNameDefault, MyInterface->GetPhyName());
      TEST_NEQ(zWireless::ConfigData::HWMODE_ERR, MyInterface->GetHwMode());
      TEST_NEQ(zWireless::ConfigData::HTMODE_ERR, MyInterface->GetHtMode());
      TEST_NEQ(zWireless::ConfigData::OPMODE_ERR, MyInterface->GetOpMode());
      TEST_EQ(zWireless::ConfigData::ConfigChannelDefault, MyInterface->GetChannel());
      TEST_EQ(zWireless::ConfigData::ConfigTxPowerDefault, MyInterface->GetTxPower());

      TEST_TRUE(MyInterface->Create());
    }

    // Verify
    TEST_NEQ(zInterface::ConfigData::ConfigIndexDefault, MyInterface->GetIfIndex());
    TEST_NEQ(zInterface::ConfigData::ConfigNameDefault, MyInterface->GetIfName());
//    TEST_EQ(ifname, MyInterface->GetIfName());
    TEST_EQ(zInterface::ConfigData::IFTYPE_IEEE8023, MyInterface->GetIfType());
    TEST_NEQ(zInterface::ConfigData::ConfigHwAddressDefault, MyInterface->GetHwAddress());
    TEST_EQ(zInterface::ConfigData::ConfigMtuDefault, MyInterface->GetMtu());
//    TEST_NEQ(zInterface::ConfigData::ConfigIpAddressDefault, MyInterface->GetIpAddress());
//    TEST_NEQ(zInterface::ConfigData::ConfigNetmaskDefault, MyInterface->GetNetmask());
    TEST_EQ(zInterface::ConfigData::STATE_DOWN, MyInterface->GetAdminState());
    TEST_NEQ(zWireless::ConfigData::ConfigPhyIndexDefault, MyInterface->GetPhyIndex());
    TEST_EQ(phy.first, MyInterface->GetPhyIndex());
    TEST_NEQ(zWireless::ConfigData::ConfigPhyNameDefault, MyInterface->GetPhyName());
    TEST_NEQ(zWireless::ConfigData::HWMODE_ERR, MyInterface->GetHwMode());
    TEST_NEQ(zWireless::ConfigData::HTMODE_ERR, MyInterface->GetHtMode());
    TEST_NEQ(zWireless::ConfigData::OPMODE_ERR, MyInterface->GetOpMode());
    TEST_NEQ(zWireless::ConfigData::ConfigChannelDefault, MyInterface->GetChannel());
    TEST_NEQ(zWireless::ConfigData::ConfigTxPowerDefault, MyInterface->GetTxPower());

    // Setup interface; commit each independently so we know which one failed, if any
    TEST_TRUE(MyInterface->SetAdminState(zWireless::ConfigData::STATE_UP));
    TEST_TRUE(MyInterface->Commit());
    TEST_TRUE(MyInterface->SetChannel(1));
    TEST_TRUE(MyInterface->Commit());
    TEST_TRUE(MyInterface->SetTxPower(2100));
    TEST_TRUE(MyInterface->Commit());

    // Verify
    TEST_NEQ(zInterface::ConfigData::ConfigIndexDefault, MyInterface->GetIfIndex());
    TEST_NEQ(zInterface::ConfigData::ConfigNameDefault, MyInterface->GetIfName());
    TEST_EQ(ifname, MyInterface->GetIfName());
    TEST_EQ(zInterface::ConfigData::IFTYPE_IEEE8023, MyInterface->GetIfType());
    TEST_NEQ(zInterface::ConfigData::ConfigHwAddressDefault, MyInterface->GetHwAddress());
    TEST_EQ(zInterface::ConfigData::ConfigMtuDefault, MyInterface->GetMtu());
//    TEST_NEQ(zInterface::ConfigData::ConfigIpAddressDefault, MyInterface->GetIpAddress());
//    TEST_NEQ(zInterface::ConfigData::ConfigNetmaskDefault, MyInterface->GetNetmask());
    TEST_EQ(zInterface::ConfigData::STATE_UP, MyInterface->GetAdminState());
    TEST_NEQ(zWireless::ConfigData::ConfigPhyIndexDefault, MyInterface->GetPhyIndex());
    TEST_EQ(phy.first, MyInterface->GetPhyIndex());
    TEST_NEQ(zWireless::ConfigData::ConfigPhyNameDefault, MyInterface->GetPhyName());
    TEST_NEQ(zWireless::ConfigData::HWMODE_ERR, MyInterface->GetHwMode());
    TEST_NEQ(zWireless::ConfigData::HTMODE_ERR, MyInterface->GetHtMode());
    TEST_EQ(zWireless::ConfigData::OPMODE_AP, MyInterface->GetOpMode());
//    TEST_EQ(1, MyInterface->GetChannel());
//    TEST_EQ(2100, MyInterface->GetTxPower());

    // Create BSS
    std::string ssid = std::string("TestSsid-") + zToStr(phy.first);
    zWireless::BasicServiceSet* MyBss = new zWireless::BasicServiceSet(ifname, ssid);
    TEST_ISNOT_NULL(MyBss);
    TEST_TRUE(MyBss->Create());

    // Cleanup
    TEST_TRUE(MyBss->Destroy());
    TEST_TRUE(MyInterface->Destroy());
    delete (MyInterface);
  }

  // Return success
  UTEST_RETURN;

}

