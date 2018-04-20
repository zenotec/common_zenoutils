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
#include <zutils/nl80211/ListInterfacesCommand.h>
#include <zutils/nl80211/ListPhysCommand.h>

using namespace Test;
using namespace zUtils;
using namespace zUtils::zWireless::ieee80211;

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
    TEST_TRUE(MyBss->SetFrequency(2412));
    TEST_TRUE(MyBss->Commit());
    TEST_TRUE(MyBss->SetTxPower(1000));
    TEST_TRUE(MyBss->Commit());

    std::vector<uint8_t> rates_ = {0x82, 0x84, 0x8B, 0x96, 6*2, 9*2, 12*2, 18*2};
    std::vector<uint8_t> exrates_ = {24, 36, 48, 54};
	HtCapsTag::ht_caps htcaps_ = { 0xaaaa, 0xee, { { 10, 9, 8, 7, 6, 5, 4, 3, 2, 1 }, 0x0123, 0x44 }, 0x4444, 0x55555555L, 0x66 };
    //                         Chan s1    s2      s3      mcs mask                             hdr     mcs   padding - move to ht_caps
    HtInfoTag::ht_info info_ = { 6, 0x11, 0x2222, 0x3333, { { 10, 9, 8, 7, 6, 5, 4, 3, 2, 1 }, 0x0123, 0x44, { 0xff, 0xfe, 0xfd } } };
  
#if 0	//TODO	Change for Tagss
    TEST_TRUE(MyBss->SetRates(rates_));
    TEST_TRUE(MyBss->SetDsss(MyBss->GetChannel()));
    TEST_TRUE(MyBss->SetPowerCaps(10, 17));
    TEST_TRUE(MyBss->SetHtCaps(htcaps_));
    TEST_TRUE(MyBss->SetExtRates(exrates_));
    TEST_TRUE(MyBss->SetHtInfo(info_));
#endif
    // Verify
    TEST_NEQ(zWireless::ConfigData::ConfigIfIndexDefault, MyBss->GetIfIndex());
    TEST_NEQ(zWireless::ConfigData::ConfigIfNameDefault, MyBss->GetIfName());
    TEST_EQ(ifname, MyBss->GetIfName());
    TEST_EQ(zWireless::ConfigData::IFTYPE_IEEE8023, MyBss->GetIfType());
    TEST_EQ(std::string("00:08:d0:d0:50:00"), MyBss->GetHwAddress());
    TEST_NEQ(zWireless::ConfigData::ConfigMtuDefault, MyBss->GetMtu());
//  TEST_NEQ(zWireless::ConfigData::ConfigIpAddressDefault, MyInterface->GetIpAddress());
//  TEST_NEQ(zWireless::ConfigData::ConfigNetmaskDefault, MyInterface->GetNetmask());
    TEST_EQ(zWireless::ConfigData::STATE_UP, MyBss->GetAdminState());
    TEST_NEQ(zWireless::ConfigData::ConfigPhyIndexDefault, MyBss->GetPhyIndex());
    TEST_EQ(phy.first, MyBss->GetPhyIndex());
    TEST_NEQ(zWireless::ConfigData::ConfigPhyNameDefault, MyBss->GetPhyName());
    TEST_NEQ(zWireless::ConfigData::HTMODE_ERR, MyBss->GetHtMode());
    TEST_EQ(zWireless::ConfigData::OPMODE_AP, MyBss->GetOpMode());
    TEST_EQ(2412, MyBss->GetFrequency());
    TEST_EQ(1000, MyBss->GetTxPower());

	// Verify Ht Capabilities Parameters
//  HtCapsTag::ht_caps htcaps = MyBss->GetHtCaps();
#if 0	//TODO	Change for Tagss
    TEST_EQ(0xaaaa, MyBss->GetHtCaps().ht_cap_info);
    TEST_EQ(0xee, MyBss->GetHtCaps().ampdu_parms);
    TEST_EQ(10, MyBss->GetHtCaps().supported_mcs_set.rx_mcs_bitmask[0]);
    TEST_EQ(1, MyBss->GetHtCaps().supported_mcs_set.rx_mcs_bitmask[9]);
	TEST_EQ(10, MyBss->GetHtCaps().supported_mcs_set.rx_highest_rate);
	TEST_EQ(0x44, MyBss->GetHtCaps().supported_mcs_set.tx_mcs_fields.tx_bits);
    TEST_EQ(0x5555, MyBss->GetHtCaps().ht_ext_cap);
    TEST_EQ(0x66666666L, MyBss->GetHtCaps().trans_beam_cap);
    TEST_EQ(0x77, MyBss->GetHtCaps().asel_cap);
#endif
#if 0	//TODO	Change for Tagss
	// Verify Ht Information Parameters
//  HtInfoTag::ht_info hti = MyBss->GetHtInfo();
    TEST_EQ(6, MyBss->GetHtInfo().ht_primary_channel);
    TEST_EQ(0x11, MyBss->GetHtInfo().ht_subset_1);
    TEST_EQ(0x2222, MyBss->GetHtInfo().ht_subset_2);
    TEST_EQ(0x3333, MyBss->GetHtInfo().ht_subset_3);
    TEST_EQ(10, MyBss->GetHtInfo().ht_rx_mcs.rx_mcs_bitmask[0]);
    TEST_EQ(1, MyBss->GetHtInfo().ht_rx_mcs.rx_mcs_bitmask[9]);
    TEST_EQ(0x0123, MyBss->GetHtInfo().ht_rx_mcs.rx_highest_data_rate);
    TEST_EQ(0x44, MyBss->GetHtInfo().ht_rx_mcs.rx_tx_mcs);
#endif	
    // Cleanup
    TEST_TRUE(MyBss->Destroy());
    delete (MyBss);
  }

  // Return success
  UTEST_RETURN;

}

