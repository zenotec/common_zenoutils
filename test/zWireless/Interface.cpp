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
#include <zutils/zLog.h>
using namespace zUtils;
ZLOG_MODULE_INIT(zLog::Log::MODULE_TEST);
#include <zutils/zWireless.h>
#include <zutils/zAccessPointInterface.h>
#include <zutils/nl80211/ListInterfacesCommand.h>
#include <zutils/nl80211/ListPhysCommand.h>

using namespace Test;
using namespace zUtils;

int
zWirelessTest_WirelessInterface(void* arg)
{

  ZLOG_DEBUG("#############################################################");
  ZLOG_DEBUG("# zWirelessTest_WirelessInterface()");
  ZLOG_DEBUG("#############################################################");

  // First step is to find an interface we can test with, if none exist, bypass test
  nl80211::ListInterfacesCommand ifacescmd;
  TEST_TRUE(ifacescmd.Exec());
  std::map<uint32_t, std::string> ifaces = ifacescmd();
  if (ifaces.empty())
  {
    ZLOG_DEBUG("Test bypassed: no interfaces found");
    UTEST_BYPASS;
  }

  zWireless::Interface *MyInterface = NULL;

  MyInterface = new zWireless::Interface(zInterface::ConfigData::ConfigIfNameDefault);
  TEST_ISNOT_NULL(MyInterface);
  TEST_EQ(zInterface::ConfigData::ConfigIfIndexDefault, MyInterface->GetIfIndex());
  TEST_EQ(zInterface::ConfigData::ConfigIfNameDefault, MyInterface->GetIfName());
  TEST_EQ(zInterface::ConfigData::IFTYPE_IEEE80211, MyInterface->GetIfType());
  TEST_EQ(zInterface::ConfigData::ConfigHwAddressDefault, MyInterface->GetHwAddress());
  TEST_EQ(zInterface::ConfigData::ConfigMtuDefault, MyInterface->GetMtu());
  TEST_EQ(zInterface::ConfigData::ConfigIpAddressDefault, MyInterface->GetIpAddress());
  TEST_EQ(zInterface::ConfigData::ConfigNetmaskDefault, MyInterface->GetNetmask());
  TEST_EQ(zInterface::ConfigData::STATE_DEF, MyInterface->GetAdminState());
  TEST_EQ(zWireless::ConfigData::ConfigPhyIndexDefault, MyInterface->GetPhyIndex());
  TEST_EQ(zWireless::ConfigData::ConfigPhyNameDefault, MyInterface->GetPhyName());
  TEST_EQ(zWireless::ConfigData::HWMODE_DEF, MyInterface->GetHwMode());
  TEST_EQ(zWireless::ConfigData::HTMODE_DEF, MyInterface->GetHtMode());
  TEST_EQ(zWireless::ConfigData::OPMODE_DEF, MyInterface->GetOpMode());
  TEST_EQ(zWireless::ConfigData::ConfigChannelDefault, MyInterface->GetChannel());
//  TEST_EQ(zWireless::ConfigData::ConfigTxPowerDefault, MyInterface->GetTxPower());
  delete (MyInterface);

  //RKB
  TEST_EQ(zWireless::ConfigData::ConfigCenterFrequency1Default, MyInterface->GetCenterFrequency1());
//  TEST_EQ(zWireless::ConfigData::ConfigCenterFrequency2Default, MyInterface->GetCenterFrequency2());

  FOREACH(auto& iface, ifaces)
  {

    ZLOG_DEBUG("Testing interface: " + iface.second + "[" + ZLOG_INT(iface.first) + "]");

    MyInterface = new zWireless::Interface(iface.second);
    TEST_ISNOT_NULL(MyInterface);

    TEST_EQ(iface.first, MyInterface->GetIfIndex());
    TEST_EQ(iface.second, MyInterface->GetIfName());

    TEST_EQ(zInterface::ConfigData::IFTYPE_IEEE80211, MyInterface->GetIfType());
    TEST_EQ(zInterface::ConfigData::ConfigHwAddressDefault, MyInterface->GetHwAddress());
    TEST_EQ(zInterface::ConfigData::ConfigMtuDefault, MyInterface->GetMtu());
    TEST_EQ(zInterface::ConfigData::ConfigIpAddressDefault, MyInterface->GetIpAddress());
    TEST_EQ(zInterface::ConfigData::ConfigNetmaskDefault, MyInterface->GetNetmask());
    TEST_EQ(zInterface::ConfigData::STATE_DEF, MyInterface->GetAdminState());
    TEST_EQ(zWireless::ConfigData::ConfigPhyIndexDefault, MyInterface->GetPhyIndex());
    TEST_EQ(zWireless::ConfigData::ConfigPhyNameDefault, MyInterface->GetPhyName());
    TEST_EQ(zWireless::ConfigData::HWMODE_DEF, MyInterface->GetHwMode());
    TEST_EQ(zWireless::ConfigData::HTMODE_DEF, MyInterface->GetHtMode());
    TEST_EQ(zWireless::ConfigData::OPMODE_DEF, MyInterface->GetOpMode());
	TEST_EQ(zWireless::ConfigData::ConfigChannelDefault, MyInterface->GetChannel());
//    TEST_EQ(zWireless::ConfigData::ConfigTxPowerDefault, MyInterface->GetTxPower());

    TEST_EQ(zWireless::ConfigData::ConfigCenterFrequency1Default, MyInterface->GetCenterFrequency1());
//    TEST_EQ(zWireless::ConfigData::ConfigCenterFrequency2Default, MyInterface->GetCenterFrequency2());

    // Refresh
    TEST_TRUE(MyInterface->Refresh());

    // Validate
    TEST_EQ(iface.first, MyInterface->GetIfIndex());
    TEST_EQ(iface.second, MyInterface->GetIfName());
    switch (MyInterface->GetOpMode())
    {
    case zWireless::ConfigData::OPMODE_AP:
    {
      TEST_EQ(zInterface::ConfigData::IFTYPE_IEEE8023, MyInterface->GetIfType());
      break;
    }
    case zWireless::ConfigData::OPMODE_STA:
    {
      TEST_EQ(zInterface::ConfigData::IFTYPE_IEEE8023, MyInterface->GetIfType());
      break;
    }
    case zWireless::ConfigData::OPMODE_MONITOR:
    {
      TEST_EQ(zInterface::ConfigData::IFTYPE_IEEE80211, MyInterface->GetIfType());
      break;
    }
    default:
    {
      TEST_EQ(zInterface::ConfigData::IFTYPE_IEEE8023, MyInterface->GetIfType());
      break;
    }
    }
    TEST_NEQ(zWireless::ConfigData::ConfigHwAddressDefault, MyInterface->GetHwAddress());
//    TEST_EQ(zWireless::ConfigData::ConfigMtuDefault, MyInterface->GetMtu());
    TEST_EQ(zWireless::ConfigData::ConfigIpAddressDefault, MyInterface->GetIpAddress());
    TEST_EQ(zWireless::ConfigData::ConfigNetmaskDefault, MyInterface->GetNetmask());
    TEST_NEQ(zWireless::ConfigData::STATE_ERR, MyInterface->GetAdminState());
    TEST_NEQ(zWireless::ConfigData::ConfigPhyIndexDefault, MyInterface->GetPhyIndex());
    TEST_NEQ(zWireless::ConfigData::ConfigPhyNameDefault, MyInterface->GetPhyName());
    TEST_NEQ(zWireless::ConfigData::HWMODE_ERR, MyInterface->GetHwMode());
    TEST_EQ(zWireless::ConfigData::HWMODE_DEF, MyInterface->GetHwMode()); // TODO: Updated when implemented
    TEST_NEQ(zWireless::ConfigData::HWMODE_LAST, MyInterface->GetHwMode());
    TEST_NEQ(zWireless::ConfigData::HTMODE_ERR, MyInterface->GetHtMode());
    TEST_NEQ(zWireless::ConfigData::HTMODE_DEF, MyInterface->GetHtMode());
    TEST_NEQ(zWireless::ConfigData::HTMODE_LAST, MyInterface->GetHtMode());
    TEST_NEQ(zWireless::ConfigData::OPMODE_ERR, MyInterface->GetOpMode());
    TEST_NEQ(zWireless::ConfigData::OPMODE_NONE, MyInterface->GetOpMode());
    TEST_NEQ(zWireless::ConfigData::OPMODE_LAST, MyInterface->GetOpMode());
	TEST_EQ(zWireless::ConfigData::ConfigChannelDefault, MyInterface->GetChannel());
//    TEST_EQ(zWireless::ConfigData::ConfigTxPowerDefault, MyInterface->GetTxPower());
//    TEST_EQ(zWireless::ConfigData::ConfigCenterFrequency1Default, MyInterface->GetCenterFrequency1());

    // Cleanup
    delete (MyInterface);

  }

  // Return success
  UTEST_RETURN;

}

int
zWirelessTest_MonitorInterface(void* arg)
{

  ZLOG_DEBUG("#############################################################");
  ZLOG_DEBUG("# zWirelessTest_MonitorInterface()");
  ZLOG_DEBUG("#############################################################");

  // First step is to determine if we were executed with root privileges, if not, bypass test
  uid_t uid = getuid(), euid = geteuid();
  if (uid != 0 || uid != euid)
  {
    ZLOG_DEBUG("Test bypassed");
    UTEST_BYPASS;
  }

  // Second step is to find an radio we can test with, if none exist, bypass test
  nl80211::ListPhysCommand physcmd;
  TEST_TRUE(physcmd.Exec());
  std::map<int, std::string> phys = physcmd();
  if (phys.empty())
  {
    ZLOG_DEBUG("Test bypassed");
    UTEST_BYPASS;
  }

  zWireless::Interface *MyInterface = NULL;

  FOREACH(auto& phy, phys)
  {
    std::string ifname = std::string("mon") + zToStr(phy.first);
    MyInterface = new zWireless::Interface(ifname);
    TEST_ISNOT_NULL(MyInterface);

    if (zInterface::ConfigData::ConfigIfIndexDefault == MyInterface->GetIfIndex())
    {
      TEST_EQ(zInterface::ConfigData::ConfigIfIndexDefault, MyInterface->GetIfIndex());
      TEST_NEQ(zInterface::ConfigData::ConfigIfNameDefault, MyInterface->GetIfName());
      TEST_EQ(ifname, MyInterface->GetIfName());
      TEST_EQ(zInterface::ConfigData::IFTYPE_IEEE80211, MyInterface->GetIfType());
      TEST_EQ(zInterface::ConfigData::ConfigHwAddressDefault, MyInterface->GetHwAddress());
      TEST_EQ(zInterface::ConfigData::ConfigMtuDefault, MyInterface->GetMtu());
      TEST_EQ(zInterface::ConfigData::ConfigIpAddressDefault, MyInterface->GetIpAddress());
      TEST_EQ(zInterface::ConfigData::ConfigNetmaskDefault, MyInterface->GetNetmask());
      TEST_EQ(zInterface::ConfigData::STATE_DEF, MyInterface->GetAdminState());
      TEST_EQ(zWireless::ConfigData::ConfigPhyIndexDefault, MyInterface->GetPhyIndex());
      TEST_EQ(zWireless::ConfigData::ConfigPhyNameDefault, MyInterface->GetPhyName());
      TEST_EQ(zWireless::ConfigData::HWMODE_DEF, MyInterface->GetHwMode());
      TEST_EQ(zWireless::ConfigData::HTMODE_DEF, MyInterface->GetHtMode());
      TEST_EQ(zWireless::ConfigData::OPMODE_DEF, MyInterface->GetOpMode());
      TEST_EQ(zWireless::ConfigData::ConfigChannelDefault, MyInterface->GetChannel());
//      TEST_EQ(zWireless::ConfigData::ConfigTxPowerDefault, MyInterface->GetTxPower());

//      TEST_EQ(zWireless::ConfigData::ConfigCenterFrequency1Default, MyInterface->GetCenterFrequency1());

      TEST_TRUE(MyInterface->SetPhyIndex(phy.first));
      TEST_TRUE(MyInterface->SetOpMode(zWireless::ConfigData::OPMODE_MONITOR));
      TEST_TRUE(MyInterface->Create());
    }

    // Verify
    TEST_NEQ(zInterface::ConfigData::ConfigIfIndexDefault, MyInterface->GetIfIndex());
    TEST_NEQ(zInterface::ConfigData::ConfigIfNameDefault, MyInterface->GetIfName());
    TEST_EQ(ifname, MyInterface->GetIfName());
//    TEST_EQ(zInterface::ConfigData::IFTYPE_IEEE80211, MyInterface->GetIfType());
    TEST_NEQ(zInterface::ConfigData::ConfigHwAddressDefault, MyInterface->GetHwAddress());
    TEST_NEQ(zInterface::ConfigData::ConfigMtuDefault, MyInterface->GetMtu());
    TEST_EQ(zInterface::ConfigData::ConfigIpAddressDefault, MyInterface->GetIpAddress());
    TEST_EQ(zInterface::ConfigData::ConfigNetmaskDefault, MyInterface->GetNetmask());
    TEST_EQ(zInterface::ConfigData::STATE_DOWN, MyInterface->GetAdminState());
    TEST_NEQ(zWireless::ConfigData::ConfigPhyIndexDefault, MyInterface->GetPhyIndex());
    TEST_EQ(phy.first, MyInterface->GetPhyIndex());
    TEST_NEQ(zWireless::ConfigData::ConfigPhyNameDefault, MyInterface->GetPhyName());
//    TEST_NEQ(zWireless::ConfigData::HWMODE_DEF, MyInterface->GetHwMode());
    TEST_NEQ(zWireless::ConfigData::HWMODE_ERR, MyInterface->GetHwMode());
    TEST_NEQ(zWireless::ConfigData::HTMODE_DEF, MyInterface->GetHtMode());
    TEST_NEQ(zWireless::ConfigData::HTMODE_ERR, MyInterface->GetHtMode());
    TEST_EQ(zWireless::ConfigData::OPMODE_MONITOR, MyInterface->GetOpMode());
//    TEST_NEQ(zWireless::ConfigData::ConfigChannelDefault, MyInterface->GetChannel());
//    TEST_NEQ(zWireless::ConfigData::ConfigCenterFrequency1Default, MyInterface->GetCenterFrequency1());
//    TEST_NEQ(zWireless::ConfigData::ConfigTxPowerDefault, MyInterface->GetTxPower());

    // Setup interface; commit each independently so we know which one failed, if any
    TEST_TRUE(MyInterface->SetAdminState(zWireless::ConfigData::STATE_UP));
    TEST_TRUE(MyInterface->Commit());
	TEST_TRUE(MyInterface->SetChannel(36));
	TEST_TRUE(MyInterface->SetHtMode(zWireless::ConfigData::HTMODE::HTMODE_HT20) );
    TEST_TRUE(MyInterface->SetCenterFrequency1(5210));
    TEST_TRUE(MyInterface->Commit());
	TEST_TRUE(MyInterface->SetChannel(36));
	TEST_TRUE(MyInterface->SetHtMode(zWireless::ConfigData::HTMODE::HTMODE_HT40PLUS) );
    TEST_TRUE(MyInterface->SetCenterFrequency1(5210));
    TEST_TRUE(MyInterface->Commit());
    TEST_TRUE(MyInterface->SetTxPower(2000));
    TEST_TRUE(MyInterface->Commit());

    // Verify
    TEST_NEQ(zInterface::ConfigData::ConfigIfIndexDefault, MyInterface->GetIfIndex());
    TEST_NEQ(zInterface::ConfigData::ConfigIfNameDefault, MyInterface->GetIfName());
    TEST_EQ(ifname, MyInterface->GetIfName());
    TEST_EQ(zInterface::ConfigData::IFTYPE_IEEE80211, MyInterface->GetIfType());
    TEST_NEQ(zInterface::ConfigData::ConfigHwAddressDefault, MyInterface->GetHwAddress());
    TEST_NEQ(zInterface::ConfigData::ConfigMtuDefault, MyInterface->GetMtu());
    TEST_EQ(zInterface::ConfigData::ConfigIpAddressDefault, MyInterface->GetIpAddress());
    TEST_EQ(zInterface::ConfigData::ConfigNetmaskDefault, MyInterface->GetNetmask());
    TEST_EQ(zInterface::ConfigData::STATE_UP, MyInterface->GetAdminState());
    TEST_NEQ(zWireless::ConfigData::ConfigPhyIndexDefault, MyInterface->GetPhyIndex());
    TEST_EQ(phy.first, MyInterface->GetPhyIndex());
    TEST_NEQ(zWireless::ConfigData::ConfigPhyNameDefault, MyInterface->GetPhyName());
    TEST_NEQ(zWireless::ConfigData::HWMODE_ERR, MyInterface->GetHwMode());
    TEST_NEQ(zWireless::ConfigData::HTMODE_ERR, MyInterface->GetHtMode());
    TEST_EQ(zWireless::ConfigData::OPMODE_MONITOR, MyInterface->GetOpMode());
//    TEST_EQ(zWireless::ConfigData::ConfigCenterFrequency1Default, MyInterface->GetCenterFrequency1());
//    TEST_EQ(2000, MyInterface->GetTxPower());

    // Cleanup
    TEST_TRUE(MyInterface->Destroy());
    delete (MyInterface);
  }

  // Return success
  UTEST_RETURN;
  //TODO:	Clean-up resource leaks
}

int
zWirelessTest_AccessPointInterface(void* arg)
{

  ZLOG_DEBUG("#############################################################");
  ZLOG_DEBUG("# zWirelessTest_AccessPointInterface()");
  ZLOG_DEBUG("#############################################################");

  // First step is to determine if we were executed with root privileges, if not, bypass test
  uid_t uid = getuid(), euid = geteuid();
  if (uid != 0 || uid != euid)
  {
    ZLOG_DEBUG("Test bypassed");
    UTEST_BYPASS;
  }

  // Second step is to find an radio we can test with, if none exist, bypass test
  nl80211::ListPhysCommand physcmd;
  TEST_TRUE(physcmd.Exec());
  std::map<int, std::string> phys = physcmd();
  if (phys.empty())
  {
    ZLOG_DEBUG("Test bypassed");
    UTEST_BYPASS;
  }

  zWireless::AccessPointInterface *MyInterface = NULL;

  FOREACH(auto& phy, phys)
  {
    std::string ifname = std::string("vap") + zToStr(phy.first);
    MyInterface = new zWireless::AccessPointInterface(ifname);
    TEST_ISNOT_NULL(MyInterface);

    if (zInterface::ConfigData::ConfigIfIndexDefault == MyInterface->GetIfIndex())
    {
      TEST_EQ(zInterface::ConfigData::ConfigIfIndexDefault, MyInterface->GetIfIndex());
      TEST_NEQ(zInterface::ConfigData::ConfigIfNameDefault, MyInterface->GetIfName());
      TEST_EQ(ifname, MyInterface->GetIfName());
      TEST_EQ(zInterface::ConfigData::IFTYPE_IEEE80211, MyInterface->GetIfType());
      TEST_EQ(zInterface::ConfigData::ConfigHwAddressDefault, MyInterface->GetHwAddress());
      TEST_EQ(zInterface::ConfigData::ConfigMtuDefault, MyInterface->GetMtu());
      TEST_EQ(zInterface::ConfigData::ConfigIpAddressDefault, MyInterface->GetIpAddress());
      TEST_EQ(zInterface::ConfigData::ConfigNetmaskDefault, MyInterface->GetNetmask());
      TEST_EQ(zInterface::ConfigData::STATE_DEF, MyInterface->GetAdminState());
      TEST_EQ(zWireless::ConfigData::ConfigPhyIndexDefault, MyInterface->GetPhyIndex());
      TEST_EQ(zWireless::ConfigData::ConfigPhyNameDefault, MyInterface->GetPhyName());
      TEST_NEQ(zWireless::ConfigData::HWMODE_ERR, MyInterface->GetHwMode());
      TEST_NEQ(zWireless::ConfigData::HTMODE_ERR, MyInterface->GetHtMode());
      TEST_NEQ(zWireless::ConfigData::OPMODE_ERR, MyInterface->GetOpMode());
//      TEST_EQ(zWireless::ConfigData::ConfigCenterFrequency1Default, MyInterface->GetCenterFrequency1());
//      TEST_EQ(zWireless::ConfigData::ConfigTxPowerDefault, MyInterface->GetTxPower());

      TEST_TRUE(MyInterface->SetPhyIndex(phy.first));
      TEST_TRUE(MyInterface->Create());
    }

    // Verify
    TEST_NEQ(zInterface::ConfigData::ConfigIfIndexDefault, MyInterface->GetIfIndex());
    TEST_NEQ(zInterface::ConfigData::ConfigIfNameDefault, MyInterface->GetIfName());
    TEST_EQ(ifname, MyInterface->GetIfName());
    TEST_EQ(zInterface::ConfigData::IFTYPE_IEEE8023, MyInterface->GetIfType());
    TEST_NEQ(zInterface::ConfigData::ConfigHwAddressDefault, MyInterface->GetHwAddress());
    TEST_NEQ(zInterface::ConfigData::ConfigMtuDefault, MyInterface->GetMtu());
    TEST_EQ(zInterface::ConfigData::ConfigIpAddressDefault, MyInterface->GetIpAddress());
    TEST_EQ(zInterface::ConfigData::ConfigNetmaskDefault, MyInterface->GetNetmask());
    TEST_EQ(zInterface::ConfigData::STATE_DOWN, MyInterface->GetAdminState());
    TEST_NEQ(zWireless::ConfigData::ConfigPhyIndexDefault, MyInterface->GetPhyIndex());
    TEST_EQ(phy.first, MyInterface->GetPhyIndex());
    TEST_NEQ(zWireless::ConfigData::ConfigPhyNameDefault, MyInterface->GetPhyName());
    TEST_NEQ(zWireless::ConfigData::HWMODE_ERR, MyInterface->GetHwMode());
    TEST_NEQ(zWireless::ConfigData::HTMODE_ERR, MyInterface->GetHtMode());
    TEST_NEQ(zWireless::ConfigData::OPMODE_ERR, MyInterface->GetOpMode());
//    TEST_NEQ(zWireless::ConfigData::ConfigChannelDefault, MyInterface->GetChannel());
//    TEST_NEQ(zWireless::ConfigData::ConfigTxPowerDefault, MyInterface->GetTxPower());

    // Setup and verify; commit each independently so we know which one failed, if any
    TEST_TRUE(MyInterface->SetIfName(ifname));
    TEST_TRUE(MyInterface->Commit());

    // Cleanup
    TEST_TRUE(MyInterface->Destroy());
    delete (MyInterface);
  }

  // Return success
  UTEST_RETURN;

}

