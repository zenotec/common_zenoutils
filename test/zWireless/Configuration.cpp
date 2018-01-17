/*
 * Wireless.cpp
 *
 *  Created on: Feb 24, 2016
 *      Author: kmahoney
 */

// libc includes
// libc++ includes
#include <string>
#include <fstream>
#include <iostream>

// libzutils includes

#include <zutils/zCompatibility.h>
#include <zutils/zLog.h>
using namespace zUtils;
ZLOG_MODULE_INIT(zLog::Log::MODULE_TEST);
#include <zutils/zWireless.h>

#include "UnitTest.h"
#include "zWirelessTest.h"
using namespace zUtils;

int
zWirelessTest_WirelessConfiguration (void* arg)
{
  std::vector<std::string> slaves;

  ZLOG_DEBUG("#############################################################");
  ZLOG_DEBUG("# zWirelessTest_WirelessConfiguration()");
  ZLOG_DEBUG("#############################################################");

  zWireless::ConfigData *MyConfig = new zWireless::ConfigData;
  TEST_ISNOT_NULL(MyConfig);
  TEST_EQ(zWireless::ConfigData::ConfigIndexDefault, MyConfig->GetIfIndex());
  TEST_EQ(zWireless::ConfigData::ConfigNameDefault, MyConfig->GetIfName());
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

  // Set
  TEST_TRUE(MyConfig->SetIfIndex(3));
  TEST_TRUE(MyConfig->SetIfName("wlan0"));
  TEST_TRUE(MyConfig->SetIfType(zInterface::ConfigData::IFTYPE_IEEE80211));
  TEST_TRUE(MyConfig->SetHwAddress("00:11:22:33:44:55"));
  TEST_TRUE(MyConfig->SetMtu(1200));
  TEST_TRUE(MyConfig->SetIpAddress("1.2.3.4"));
  TEST_TRUE(MyConfig->SetNetmask("255.255.0.0"));
  TEST_TRUE(MyConfig->SetAdminState(zInterface::ConfigData::STATE_UP));
  TEST_TRUE(MyConfig->SetPhyIndex(1));
  TEST_TRUE(MyConfig->SetPhyName("phy1"));
  TEST_TRUE(MyConfig->SetHwMode(zWireless::ConfigData::HWMODE_N));
  TEST_TRUE(MyConfig->SetHtMode(zWireless::ConfigData::HTMODE_HT40PLUS));
  TEST_TRUE(MyConfig->SetOpMode(zWireless::ConfigData::OPMODE_MONITOR));
  TEST_TRUE(MyConfig->SetSsid("TestSsid1"));
  TEST_TRUE(MyConfig->SetBssid("11:22:33:44:55:66"));
  TEST_TRUE(MyConfig->SetChannel(6));
  TEST_TRUE(MyConfig->SetTxPower(23));

  // Get
  TEST_NEQ(zWireless::ConfigData::ConfigIndexDefault, MyConfig->GetIfIndex());
  TEST_EQ(3, MyConfig->GetIfIndex());
  TEST_NEQ(zWireless::ConfigData::ConfigNameDefault, MyConfig->GetIfName());
  TEST_EQ(std::string("wlan0"), MyConfig->GetIfName());
  TEST_NEQ(zWireless::ConfigData::IFTYPE_DEF, MyConfig->GetIfType());
  TEST_EQ(zWireless::ConfigData::IFTYPE_IEEE80211, MyConfig->GetIfType());
  TEST_NEQ(zWireless::ConfigData::ConfigHwAddressDefault, MyConfig->GetHwAddress());
  TEST_EQ(std::string("00:11:22:33:44:55"), MyConfig->GetHwAddress());
  TEST_EQ(1200, MyConfig->GetMtu());
  TEST_NEQ(zWireless::ConfigData::ConfigIpAddressDefault, MyConfig->GetIpAddress());
  TEST_EQ(std::string("1.2.3.4"), MyConfig->GetIpAddress());
  TEST_NEQ(zWireless::ConfigData::ConfigNetmaskDefault, MyConfig->GetNetmask());
  TEST_EQ(std::string("255.255.0.0"), MyConfig->GetNetmask());
  TEST_NEQ(zWireless::ConfigData::STATE_DEF, MyConfig->GetAdminState());
  TEST_EQ(zWireless::ConfigData::STATE_UP, MyConfig->GetAdminState());
  TEST_NEQ(zWireless::ConfigData::ConfigPhyIndexDefault, MyConfig->GetPhyIndex());
  TEST_EQ(1, MyConfig->GetPhyIndex());
  TEST_NEQ(zWireless::ConfigData::ConfigPhyNameDefault, MyConfig->GetPhyName());
  TEST_EQ(std::string("phy1"), MyConfig->GetPhyName());
  TEST_NEQ(zWireless::ConfigData::HWMODE_DEF, MyConfig->GetHwMode());
  TEST_EQ(zWireless::ConfigData::HWMODE_N, MyConfig->GetHwMode());
  TEST_NEQ(zWireless::ConfigData::HTMODE_DEF, MyConfig->GetHtMode());
  TEST_EQ(zWireless::ConfigData::HTMODE_HT40PLUS, MyConfig->GetHtMode());
  TEST_NEQ(zWireless::ConfigData::OPMODE_DEF, MyConfig->GetOpMode());
  TEST_EQ(zWireless::ConfigData::OPMODE_MONITOR, MyConfig->GetOpMode());
  TEST_NEQ(zWireless::ConfigData::ConfigSsidDefault, MyConfig->GetSsid());
  TEST_EQ(std::string("TestSsid1"), MyConfig->GetSsid());
  TEST_NEQ(zWireless::ConfigData::ConfigBssidDefault, MyConfig->GetBssid());
  TEST_EQ(std::string("11:22:33:44:55:66"), MyConfig->GetBssid());
  TEST_NEQ(zWireless::ConfigData::ConfigChannelDefault, MyConfig->GetChannel());
  TEST_EQ(6, MyConfig->GetChannel());
  TEST_NEQ(zWireless::ConfigData::ConfigTxPowerDefault, MyConfig->GetTxPower());
  TEST_EQ(23, MyConfig->GetTxPower());

  // Cleanup
  delete (MyConfig);

  // Return success
  return (0);

}

