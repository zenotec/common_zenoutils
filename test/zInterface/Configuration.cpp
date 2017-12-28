/*
 * Interface.cpp
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
#include <zutils/zInterface.h>

#include "UnitTest.h"
#include "zInterfaceTest.h"
using namespace zUtils::zInterface;

int
zInterfaceTest_InterfaceConfiguration (void* arg)
{
  std::vector<std::string> slaves;

  ZLOG_DEBUG("#############################################################");
  ZLOG_DEBUG("# zInterfaceTest_InterfaceConfiguration()");
  ZLOG_DEBUG("#############################################################");

  ConfigData *MyConfig = new ConfigData;
  TEST_ISNOT_NULL(MyConfig);
  TEST_EQ(ConfigData::ConfigIndexDefault, MyConfig->GetIfIndex());
  TEST_EQ(ConfigData::ConfigNameDefault, MyConfig->GetIfName());
  TEST_EQ(ConfigData::IFTYPE_DEF, MyConfig->GetIfType());
  TEST_EQ(ConfigData::ConfigHwAddressDefault, MyConfig->GetHwAddress());
  TEST_EQ(ConfigData::ConfigMtuDefault, MyConfig->GetMtu());
  TEST_EQ(ConfigData::ConfigIpAddressDefault, MyConfig->GetIpAddress());
  TEST_EQ(ConfigData::ConfigNetmaskDefault, MyConfig->GetNetmask());
  TEST_EQ(ConfigData::STATE_DEF, MyConfig->GetAdminState());

  // Set
  TEST_TRUE(MyConfig->SetIfIndex(3));
  TEST_TRUE(MyConfig->SetIfName("eth0"));
  TEST_TRUE(MyConfig->SetIfType(ConfigData::IFTYPE_IEEE8023));
  TEST_TRUE(MyConfig->SetHwAddress("00:11:22:33:44:55"));
  TEST_TRUE(MyConfig->SetMtu(1200));
  TEST_TRUE(MyConfig->SetIpAddress("1.2.3.4"));
  TEST_TRUE(MyConfig->SetNetmask("255.255.0.0"));
  TEST_TRUE(MyConfig->SetAdminState(ConfigData::STATE_UP));

  // Get
  TEST_NEQ(ConfigData::ConfigIndexDefault, MyConfig->GetIfIndex());
  TEST_EQ(3, MyConfig->GetIfIndex());
  TEST_NEQ(ConfigData::ConfigNameDefault, MyConfig->GetIfName());
  TEST_EQ(std::string("eth0"), MyConfig->GetIfName());
  TEST_NEQ(ConfigData::IFTYPE_DEF, MyConfig->GetIfType());
  TEST_EQ(ConfigData::IFTYPE_IEEE8023, MyConfig->GetIfType());
  TEST_NEQ(ConfigData::ConfigHwAddressDefault, MyConfig->GetHwAddress());
  TEST_EQ(std::string("00:11:22:33:44:55"), MyConfig->GetHwAddress());
  TEST_EQ(1200, MyConfig->GetMtu());
  TEST_NEQ(ConfigData::ConfigIpAddressDefault, MyConfig->GetIpAddress());
  TEST_EQ(std::string("1.2.3.4"), MyConfig->GetIpAddress());
  TEST_NEQ(ConfigData::ConfigNetmaskDefault, MyConfig->GetNetmask());
  TEST_EQ(std::string("255.255.0.0"), MyConfig->GetNetmask());
  TEST_NEQ(ConfigData::STATE_DEF, MyConfig->GetAdminState());
  TEST_EQ(ConfigData::STATE_UP, MyConfig->GetAdminState());

  // Cleanup
  delete (MyConfig);

  // Return success
  return (0);

}

