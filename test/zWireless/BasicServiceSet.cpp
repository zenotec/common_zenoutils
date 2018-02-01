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

  FOREACH(auto& phy, phys)
  {
    std::string ifname = std::string("vap") + zToStr(phy.first);
    std::string ssid = std::string("TestSsid") + zToStr(phy.first);

    // Create BSS
    zWireless::BasicServiceSet* MyBss = new zWireless::BasicServiceSet(ifname, ssid);
    TEST_ISNOT_NULL(MyBss);
    TEST_TRUE(MyBss->SetPhyIndex(phy.first));
    TEST_TRUE(MyBss->SetBssid("00:08:d0:d0:50:00"));
    TEST_TRUE(MyBss->Create());

    // Setup interface; commit each independently so we know which one failed, if any
    TEST_TRUE(MyBss->SetChannel(1));
    TEST_TRUE(MyBss->Commit());
    TEST_TRUE(MyBss->SetTxPower(1000));
    TEST_TRUE(MyBss->Commit());

    // Verify
    TEST_NEQ(zWireless::ConfigData::ConfigIfIndexDefault, MyBss->GetIfIndex());
    TEST_NEQ(zWireless::ConfigData::ConfigIfNameDefault, MyBss->GetIfName());
    TEST_EQ(ifname, MyBss->GetIfName());
    TEST_EQ(zWireless::ConfigData::IFTYPE_IEEE8023, MyBss->GetIfType());
    TEST_EQ(std::string("00:08:d0:d0:50:00"), MyBss->GetHwAddress());
    TEST_EQ(zWireless::ConfigData::ConfigMtuDefault, MyBss->GetMtu());
//    TEST_NEQ(zWireless::ConfigData::ConfigIpAddressDefault, MyInterface->GetIpAddress());
//    TEST_NEQ(zWireless::ConfigData::ConfigNetmaskDefault, MyInterface->GetNetmask());
    TEST_EQ(zWireless::ConfigData::STATE_UP, MyBss->GetAdminState());
    TEST_NEQ(zWireless::ConfigData::ConfigPhyIndexDefault, MyBss->GetPhyIndex());
    TEST_EQ(phy.first, MyBss->GetPhyIndex());
    TEST_NEQ(zWireless::ConfigData::ConfigPhyNameDefault, MyBss->GetPhyName());
    TEST_NEQ(zWireless::ConfigData::HWMODE_ERR, MyBss->GetHwMode());
    TEST_NEQ(zWireless::ConfigData::HTMODE_ERR, MyBss->GetHtMode());
    TEST_EQ(zWireless::ConfigData::OPMODE_AP, MyBss->GetOpMode());
    TEST_EQ(1, MyBss->GetChannel());
    TEST_EQ(1000, MyBss->GetTxPower());

    // Cleanup
    TEST_TRUE(MyBss->Destroy());
    delete (MyBss);
  }

  // Return success
  UTEST_RETURN;

}

