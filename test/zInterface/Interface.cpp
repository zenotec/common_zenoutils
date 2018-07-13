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
#include <zutils/zLog.h>
using namespace zUtils;
ZLOG_MODULE_INIT(zLog::Log::MODULE_TEST);
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

  Interface *MyInterface = new zInterface::Interface("lo");
  TEST_ISNOT_NULL(MyInterface);
  TEST_EQ(1, MyInterface->GetIfIndex());
  TEST_EQ(std::string("lo"), MyInterface->GetIfName());
  TEST_EQ(ConfigData::IFTYPE_DEF, MyInterface->GetIfType());
  TEST_EQ(ConfigData::ConfigHwAddressDefault, MyInterface->GetHwAddress());
  TEST_EQ(ConfigData::ConfigMtuDefault, MyInterface->GetMtu());
  TEST_EQ(ConfigData::ConfigIpAddressDefault, MyInterface->GetIpAddress());
  TEST_EQ(ConfigData::ConfigNetmaskDefault, MyInterface->GetNetmask());
  TEST_EQ(ConfigData::STATE_DEF, MyInterface->GetAdminState());
  TEST_EQ(1 , MyInterface->GetIfIndex()); 
  TEST_TRUE(MyInterface->SetIfIndex(3));
  //TEST_EQ(3, MyInterface->GetIfIndex()); 
  std::string ifname1 = "interface";
  const std::string& pass = ifname1;
  TEST_TRUE(MyInterface->SetIfName(pass)); 
  TEST_EQ(ConfigData::ConfigMasterIfIndexDefault, 0); 
  TEST_EQ(ConfigData::ConfigMasterIfIndexDefault, MyInterface->GetMasterIfIndex()); 
  TEST_TRUE(MyInterface->SetMasterIfIndex(1)); 
  TEST_EQ(1, MyInterface->GetMasterIfIndex()); 
  TEST_TRUE(MyInterface->SetMasterIfName("f")); 
  TEST_EQ(std::string("f"), MyInterface->GetMasterIfName()); 
  TEST_TRUE(MyInterface->SetMtu(1));
  std::string hwaddr1 = "00:00:00:00:00:00";
  const std::string& passadd = hwaddr1;
  TEST_TRUE(MyInterface->SetHwAddress(passadd));

  ConfigData* MyConfig = NULL;
  MyConfig = new ConfigData;
  MyInterface->GetConfig();
  //TEST_TRUE(MyInterface->SetConfig(MyConfig));
  
  // Refresh
  TEST_TRUE(MyInterface->Refresh());
  TEST_EQ(1, MyInterface->GetIfIndex());
  TEST_EQ(std::string("lo"), MyInterface->GetIfName());
  TEST_EQ(ConfigData::IFTYPE_LOOP, MyInterface->GetIfType());
  TEST_EQ(std::string("00:00:00:00:00:00"), MyInterface->GetHwAddress());
  TEST_EQ(65536, MyInterface->GetMtu());
  TEST_EQ(ConfigData::STATE_UP, MyInterface->GetAdminState());
  std::string ipadd = MyInterface->GetIpAddress();//
  TEST_TRUE(MyInterface->SetIpAddress(ipadd));

  std::string netmask1 = "255.255.255.0";
  const std::string& passnetmask = netmask1;
  TEST_TRUE(MyInterface->SetNetmask(passnetmask));
  TEST_EQ(std::string("255.255.255.0"), MyInterface->GetNetmask());//
  
  TEST_TRUE(MyInterface->SetAdminState(ConfigData::STATE_UP));
  TEST_EQ(ConfigData::STATE_UP, MyInterface->GetAdminState());
  MyInterface->Commit(); 
  TEST_TRUE(MyInterface->SetAdminState(ConfigData::STATE_DOWN));
  MyInterface->Commit();

 // Cleanup
  delete (MyConfig);
  delete (MyInterface);

  // Return success
  return (0);
}
