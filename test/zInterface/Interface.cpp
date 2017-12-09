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


using namespace zUtils;
using namespace zUtils::zInterface;

int
zInterfaceTest_InterfaceLoop(void* arg)
{

  ZLOG_DEBUG("#############################################################");
  ZLOG_DEBUG("# zInterfaceTest_InterfaceLoop()");
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

  // Setup configuration for loopback interface
  TEST_TRUE(MyConfig->SetIfName("lo"));
  TEST_TRUE(MyConfig->SetIfType(ConfigData::IFTYPE_LOOP));
  TEST_TRUE(MyConfig->SetAdminState(ConfigData::STATE_UP));

  Interface *MyInterface = new zInterface::Interface(*MyConfig);
  TEST_ISNOT_NULL(MyInterface);
  TEST_EQ(1, MyInterface->GetIfIndex());
  TEST_EQ(std::string("lo"), MyInterface->GetIfName());
  TEST_EQ(ConfigData::IFTYPE_LOOP, MyInterface->GetIfType());
  TEST_EQ(std::string("00:00:00:00:00:00"), MyInterface->GetHwAddress());
//  TEST_EQ(std::string("127.0.0.1"), MyInterface->IpAddress());
  TEST_EQ(ConfigData::STATE_UP, MyInterface->GetAdminState());

  // Cleanup
  delete (MyInterface);
  delete (MyConfig);

  // Return success
  return (0);
}
