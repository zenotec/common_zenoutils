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
  TEST_EQ(zWireless::ConfigData::ConfigPhyDevDefault, MyInterface->GetPhyDev());
  TEST_EQ(zWireless::ConfigData::ConfigPhyNameDefault, MyInterface->GetPhyName());
  TEST_EQ(zWireless::ConfigData::HTMODE_DEF, MyInterface->GetHtMode());
  TEST_EQ(zWireless::ConfigData::OPMODE_DEF, MyInterface->GetOpMode());
  TEST_EQ(zWireless::ConfigData::ConfigFrequencyDefault, MyInterface->GetFrequency());
  TEST_EQ(zWireless::ConfigData::ConfigTxPowerDefault, MyInterface->GetTxPower());
  TEST_EQ(zWireless::ConfigData::ConfigCenterFrequency1Default, MyInterface->GetCenterFrequency1());
  TEST_EQ(zWireless::ConfigData::ConfigCenterFrequency2Default, MyInterface->GetCenterFrequency2());
  delete (MyInterface);

  FOREACH(auto& iface, ifaces)
  {

    ZLOG_DEBUG("Testing interface: " + iface.second + "[" + ZLOG_INT(iface.first) + "]");

    MyInterface = new zWireless::Interface(iface.second);
    TEST_ISNOT_NULL(MyInterface);

    TEST_EQ(iface.first, MyInterface->GetIfIndex());
    TEST_EQ(iface.second, MyInterface->GetIfName());

    TEST_EQ_MSG(zInterface::ConfigData::IFTYPE_IEEE80211, MyInterface->GetIfType(), iface.second);
    TEST_EQ_MSG(zInterface::ConfigData::ConfigHwAddressDefault, MyInterface->GetHwAddress(), iface.second);
    TEST_EQ_MSG(zInterface::ConfigData::ConfigMtuDefault, MyInterface->GetMtu(), iface.second);
    TEST_EQ_MSG(zInterface::ConfigData::ConfigIpAddressDefault, MyInterface->GetIpAddress(), iface.second);
    TEST_EQ_MSG(zInterface::ConfigData::ConfigNetmaskDefault, MyInterface->GetNetmask(), iface.second);
    TEST_EQ_MSG(zInterface::ConfigData::STATE_DEF, MyInterface->GetAdminState(), iface.second);
    TEST_EQ_MSG(zWireless::ConfigData::ConfigPhyIndexDefault, MyInterface->GetPhyIndex(), iface.second);
    TEST_EQ_MSG(zWireless::ConfigData::ConfigPhyDevDefault, MyInterface->GetPhyDev(), iface.second);
    TEST_EQ_MSG(zWireless::ConfigData::ConfigPhyNameDefault, MyInterface->GetPhyName(), iface.second);
    TEST_EQ_MSG(zWireless::ConfigData::HTMODE_DEF, MyInterface->GetHtMode(), iface.second);
    TEST_EQ_MSG(zWireless::ConfigData::OPMODE_DEF, MyInterface->GetOpMode(), iface.second);
    TEST_EQ_MSG(zWireless::ConfigData::ConfigFrequencyDefault, MyInterface->GetFrequency(), iface.second);
    TEST_EQ_MSG(zWireless::ConfigData::ConfigCenterFrequency1Default, MyInterface->GetCenterFrequency1(), iface.second);
    TEST_EQ_MSG(zWireless::ConfigData::ConfigCenterFrequency2Default, MyInterface->GetCenterFrequency2(), iface.second);
    TEST_EQ(zWireless::ConfigData::ConfigTxPowerDefault, MyInterface->GetTxPower());

    // Refresh
    TEST_TRUE_MSG(MyInterface->Refresh(), iface.second);

    // Validate
    TEST_EQ(iface.first, MyInterface->GetIfIndex());
    TEST_EQ(iface.second, MyInterface->GetIfName());
    switch (MyInterface->GetOpMode())
    {
      case zWireless::ConfigData::OPMODE_AP:
      {
        TEST_EQ_MSG(zInterface::ConfigData::IFTYPE_IEEE8023, MyInterface->GetIfType(), iface.second);
        break;
      }
      case zWireless::ConfigData::OPMODE_STA:
      {
        TEST_EQ_MSG(zInterface::ConfigData::IFTYPE_IEEE8023, MyInterface->GetIfType(), iface.second);
        break;
      }
      case zWireless::ConfigData::OPMODE_MONITOR:
      {
        TEST_EQ_MSG(zInterface::ConfigData::IFTYPE_IEEE80211, MyInterface->GetIfType(), iface.second);
        break;
      }
      default:
      {
        TEST_EQ_MSG(zInterface::ConfigData::IFTYPE_IEEE8023, MyInterface->GetIfType(), iface.second);
        break;
      }
    }

    TEST_NEQ_MSG(zWireless::ConfigData::ConfigHwAddressDefault, MyInterface->GetHwAddress(), iface.second);
    TEST_NEQ_MSG(zWireless::ConfigData::ConfigMtuDefault, MyInterface->GetMtu(), iface.second);
    TEST_EQ_MSG(zWireless::ConfigData::ConfigIpAddressDefault, MyInterface->GetIpAddress(), iface.second);
    TEST_EQ_MSG(zWireless::ConfigData::ConfigNetmaskDefault, MyInterface->GetNetmask(), iface.second);
    TEST_NEQ_MSG(zWireless::ConfigData::STATE_ERR, MyInterface->GetAdminState(), iface.second);
    TEST_NEQ_MSG(zWireless::ConfigData::ConfigPhyIndexDefault, MyInterface->GetPhyIndex(), iface.second);
    TEST_NEQ_MSG(zWireless::ConfigData::ConfigPhyNameDefault, MyInterface->GetPhyName(), iface.second);
    TEST_NEQ_MSG(zWireless::ConfigData::HTMODE_ERR, MyInterface->GetHtMode(), iface.second);
    TEST_NEQ_MSG(zWireless::ConfigData::HTMODE_DEF, MyInterface->GetHtMode(), iface.second);
    TEST_NEQ_MSG(zWireless::ConfigData::HTMODE_LAST, MyInterface->GetHtMode(), iface.second);
    TEST_NEQ_MSG(zWireless::ConfigData::OPMODE_ERR, MyInterface->GetOpMode(), iface.second);
    TEST_NEQ_MSG(zWireless::ConfigData::OPMODE_NONE, MyInterface->GetOpMode(), iface.second);
    TEST_NEQ_MSG(zWireless::ConfigData::OPMODE_LAST, MyInterface->GetOpMode(), iface.second);

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
      TEST_EQ_MSG(zInterface::ConfigData::IFTYPE_IEEE80211, MyInterface->GetIfType(), ifname);
      TEST_EQ_MSG(zInterface::ConfigData::ConfigHwAddressDefault, MyInterface->GetHwAddress(), ifname);
      TEST_EQ_MSG(zInterface::ConfigData::ConfigMtuDefault, MyInterface->GetMtu(), ifname);
      TEST_EQ_MSG(zInterface::ConfigData::ConfigIpAddressDefault, MyInterface->GetIpAddress(), ifname);
      TEST_EQ_MSG(zInterface::ConfigData::ConfigNetmaskDefault, MyInterface->GetNetmask(), ifname);
      TEST_EQ_MSG(zInterface::ConfigData::STATE_DEF, MyInterface->GetAdminState(), ifname);
      TEST_EQ_MSG(zWireless::ConfigData::ConfigPhyIndexDefault, MyInterface->GetPhyIndex(), ifname);
      TEST_EQ_MSG(zWireless::ConfigData::ConfigPhyDevDefault, MyInterface->GetPhyDev(), ifname);
      TEST_EQ_MSG(zWireless::ConfigData::ConfigPhyNameDefault, MyInterface->GetPhyName(), ifname);
      TEST_EQ_MSG(zWireless::ConfigData::HTMODE_DEF, MyInterface->GetHtMode(), ifname);
      TEST_EQ_MSG(zWireless::ConfigData::OPMODE_DEF, MyInterface->GetOpMode(), ifname);
      TEST_EQ_MSG(zWireless::ConfigData::ConfigFrequencyDefault, MyInterface->GetFrequency(), ifname);
      TEST_EQ_MSG(zWireless::ConfigData::ConfigCenterFrequency1Default, MyInterface->GetCenterFrequency1(), ifname);
      TEST_EQ_MSG(zWireless::ConfigData::ConfigCenterFrequency2Default, MyInterface->GetCenterFrequency2(), ifname);
      TEST_EQ_MSG(zWireless::ConfigData::ConfigTxPowerDefault, MyInterface->GetTxPower(), ifname);


      TEST_TRUE_MSG(MyInterface->SetPhyIndex(phy.first), ifname);
      TEST_TRUE_MSG(MyInterface->SetOpMode(zWireless::ConfigData::OPMODE_MONITOR), ifname);
      TEST_TRUE_MSG(MyInterface->Create(), ifname);
    }

    // Verify
    TEST_NEQ_MSG(zInterface::ConfigData::ConfigIfIndexDefault, MyInterface->GetIfIndex(), ifname);
    TEST_NEQ_MSG(zInterface::ConfigData::ConfigIfNameDefault, MyInterface->GetIfName(), ifname);
    TEST_EQ_MSG(ifname, MyInterface->GetIfName(), ifname);
    TEST_EQ_MSG(zInterface::ConfigData::IFTYPE_IEEE80211, MyInterface->GetIfType(), ifname);
    TEST_NEQ_MSG(zInterface::ConfigData::ConfigHwAddressDefault, MyInterface->GetHwAddress(), ifname);
    TEST_NEQ_MSG(zInterface::ConfigData::ConfigMtuDefault, MyInterface->GetMtu(), ifname);
    TEST_EQ_MSG(zInterface::ConfigData::ConfigIpAddressDefault, MyInterface->GetIpAddress(), ifname);
    TEST_EQ_MSG(zInterface::ConfigData::ConfigNetmaskDefault, MyInterface->GetNetmask(), ifname);
    TEST_EQ_MSG(zInterface::ConfigData::STATE_DOWN, MyInterface->GetAdminState(), ifname);
    TEST_NEQ_MSG(zWireless::ConfigData::ConfigPhyIndexDefault, MyInterface->GetPhyIndex(), ifname);
    TEST_EQ_MSG(phy.first, MyInterface->GetPhyIndex(), ifname);
    TEST_NEQ_MSG(zWireless::ConfigData::ConfigPhyDevDefault, MyInterface->GetPhyDev(), ifname);
    TEST_NEQ_MSG(zWireless::ConfigData::ConfigPhyNameDefault, MyInterface->GetPhyName(), ifname);
    TEST_NEQ_MSG(zWireless::ConfigData::HTMODE_DEF, MyInterface->GetHtMode(), ifname);
    TEST_NEQ_MSG(zWireless::ConfigData::HTMODE_ERR, MyInterface->GetHtMode(), ifname);
    TEST_EQ_MSG(zWireless::ConfigData::OPMODE_MONITOR, MyInterface->GetOpMode(), ifname);
    TEST_EQ_MSG(zWireless::ConfigData::ConfigFrequencyDefault, MyInterface->GetFrequency(), ifname);
    TEST_EQ_MSG(zWireless::ConfigData::ConfigCenterFrequency1Default, MyInterface->GetCenterFrequency1(), ifname);
    TEST_EQ_MSG(zWireless::ConfigData::ConfigCenterFrequency2Default, MyInterface->GetCenterFrequency2(), ifname);
//    TEST_EQ_MSG(zWireless::ConfigData::ConfigTxPowerDefault, MyInterface->GetTxPower(), ifname);

    // Setup interface; commit each independently so we know which one failed, if any
    TEST_TRUE(MyInterface->SetAdminState(zWireless::ConfigData::STATE_UP));
    TEST_TRUE(MyInterface->Commit());

    TEST_TRUE(MyInterface->SetFrequency(2437));
	TEST_TRUE(MyInterface->SetHtMode(zWireless::ConfigData::HTMODE::HTMODE_HT20) );
    TEST_TRUE(MyInterface->Commit());

    TEST_TRUE(MyInterface->SetFrequency(5180));
	TEST_TRUE(MyInterface->SetHtMode(zWireless::ConfigData::HTMODE::HTMODE_HT40) );
    TEST_TRUE(MyInterface->Commit());

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
zWirelessTest_setFrequency(void* arg){

  ZLOG_DEBUG("#############################################################");
  ZLOG_DEBUG("# zWirelessTest_setFrequency()");
  ZLOG_DEBUG("#############################################################");
  unsigned int frequency = 149;
       
  std::string ifname = std::string("vap");
  zWireless::AccessPointInterface *MyInterface = new zWireless::AccessPointInterface(ifname);
  MyInterface->SetFrequency(frequency);
  TEST_EQ(frequency, MyInterface->GetFrequency());

  delete MyInterface;
  // Return success
  UTEST_RETURN;
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

