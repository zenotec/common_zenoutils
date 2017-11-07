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
zInterfaceTest_InterfaceCtor(void* arg)
{

  ZLOG_DEBUG("#############################################################");
  ZLOG_DEBUG("# zInterfaceTest_InterfaceCtor()");
  ZLOG_DEBUG("#############################################################");

  ConfigData *MyConfig = new ConfigData;
  TEST_ISNOT_NULL(MyConfig);
  TEST_EQ(ConfigData::ConfigNameDefault, MyConfig->Name());
  TEST_EQ(ConfigData::ConfigTypeDefault, MyConfig->Type());
  TEST_EQ(ConfigData::ConfigHwAddressDefault, MyConfig->HwAddress());
  TEST_EQ(ConfigData::ConfigMtuDefault, MyConfig->Mtu());
  TEST_EQ(ConfigData::ConfigIpAddressDefault, MyConfig->IpAddress());
  TEST_EQ(ConfigData::ConfigNetmaskDefault, MyConfig->Netmask());
  TEST_EQ(ConfigData::ConfigAdminStateDefault, MyConfig->AdminState());

//  // Set
//  TEST_TRUE(MyConfig->Name("lo"));
//  TEST_TRUE(MyConfig->Type(ConfigData::ConfigTypeLoop));
//  TEST_TRUE(MyConfig->SetIpAddress("1.2.3.4"));
//  TEST_TRUE(MyConfig->SetState(ConfigData::STATE_DOWN));

  Interface *MyInterface = new zInterface::Interface(*MyConfig);
  TEST_ISNOT_NULL(MyInterface);
//  TEST_TRUE(MyInterface->Refresh());
//  TEST_TRUE(MyInterface->IsRefreshed());
//  TEST_EQ(1, MyInterface->GetIndex());
//  TEST_EQ(std::string("lo"), MyInterface->Name());
//  TEST_EQ(ConfigData::TYPE_LOOP, MyInterface->Type());
//  TEST_EQ(std::string("127.0.0.1"), MyInterface->IpAddress());
//  TEST_EQ(ConfigData::STATE_UP, MyInterface->AdminState());

  // Cleanup
  delete (MyInterface);
  delete (MyConfig);

  // Return success
  return (0);
}

int
zInterfaceTest_InterfaceRefresh(void* arg)
{

  ZLOG_DEBUG("#############################################################");
  ZLOG_DEBUG("# zInterfaceTest_InterfaceRefresh()");
  ZLOG_DEBUG("#############################################################");

  ConfigData *MyConfig = new ConfigData;
  TEST_ISNOT_NULL(MyConfig);
  TEST_EQ(ConfigData::ConfigNameDefault, MyConfig->Name());
  TEST_EQ(ConfigData::ConfigTypeDefault, MyConfig->Type());
  TEST_EQ(ConfigData::ConfigHwAddressDefault, MyConfig->HwAddress());
  TEST_EQ(ConfigData::ConfigMtuDefault, MyConfig->Mtu());
  TEST_EQ(ConfigData::ConfigIpAddressDefault, MyConfig->IpAddress());
  TEST_EQ(ConfigData::ConfigNetmaskDefault, MyConfig->Netmask());
  TEST_EQ(ConfigData::ConfigAdminStateDefault, MyConfig->AdminState());

//  // Set
//  TEST_TRUE(MyConfig->Name("lo"));
//  TEST_TRUE(MyConfig->Type(ConfigData::TYPE_LOOP));

  Interface *MyInterface = new zInterface::Interface(*MyConfig);
  TEST_ISNOT_NULL(MyInterface);
//  TEST_EQ(std::string("lo"), MyInterface->Name());
//  TEST_EQ(ConfigData::TYPE_LOOP, MyInterface->Type());
////  TEST_EQ(InterfaceConfigData::ConfigIpAddressDefault, MyInterface->GetIpAddress());
////  TEST_EQ(InterfaceConfigData::STATE_DEF, MyInterface->GetState());
////  TEST_EQ(-1, MyInterface->GetIndex());
////  TEST_EQ(std::string(""), MyInterface->GetHwAddress());
//
//  // Refresh and verify
//  TEST_TRUE(MyInterface->Refresh());
//  TEST_EQ(std::string("lo"), MyInterface->Name());
//  TEST_EQ(ConfigData::TYPE_LOOP, MyInterface->Type());
//  TEST_NEQ(-1, MyInterface->GetIndex());
//  TEST_EQ(std::string("127.0.0.1"), MyInterface->IpAddress());
//  TEST_EQ(std::string("00:00:00:00:00:00"), MyInterface->HwAddress());

  // Cleanup
  delete (MyInterface);
  delete (MyConfig);

  // Return success
  return (0);
}
