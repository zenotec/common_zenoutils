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
#include <zutils/zInterface.h>
#include <zutils/zMacVlanInterface.h>

#include <zutils/netlink/ListLinksCommand.h>

#include "UnitTest.h"
#include "zInterfaceTest.h"


using namespace zUtils;
using namespace zUtils::zInterface;

ZLOG_MODULE_INIT(zLog::Log::MODULE_TEST);

int
zInterfaceTest_InterfaceMacVlan(void* arg)
{

  ZLOG_DEBUG("#############################################################");
  ZLOG_DEBUG("# zInterfaceTest_InterfaceMacVlan()");
  ZLOG_DEBUG("#############################################################");

  // First step is to determine if we were executed with root privileges, if not, bypass test
  uid_t uid = getuid(), euid = geteuid();
  if (uid != 0 || uid != euid)
  {
    ZLOG_DEBUG("Test bypassed");
    UTEST_BYPASS;
  }

  // Second step is to find an interface we can test with, if none exist, bypass test
  netlink::ListLinksCommand ifacescmd;
  TEST_TRUE(ifacescmd.Exec());
  std::map<int, netlink::RouteLink> links = ifacescmd.LinkMap;
  if (links.empty())
  {
    UTEST_BYPASS;
  }

  Interface *Master = NULL;
  MacVlan *MyInterface = NULL;

  FOREACH(auto& link, links)
  {

    Master = new Interface(link.second.IfName());
    TEST_ISNOT_NULL(Master);
    TEST_TRUE(Master->Refresh());
    TEST_NEQ(0, Master->GetIfIndex());
    if (Master->GetIfType() != zInterface::ConfigData::IFTYPE_IEEE8023)
    {
      continue;
    }

    MyInterface = new MacVlan("uTestVif-" + link.second.IfName());
    TEST_ISNOT_NULL(MyInterface);

    // Cleanup from any previous failed tests
    if (MyInterface->Refresh())
    {
      TEST_TRUE(MyInterface->Destroy());
    }

    TEST_FALSE(MyInterface->Refresh());

    TEST_EQ(0, MyInterface->GetIfIndex());
    TEST_EQ(ConfigData::IFTYPE_DEF, MyInterface->GetIfType());
    TEST_EQ(ConfigData::ConfigHwAddressDefault, MyInterface->GetHwAddress());
    TEST_EQ(ConfigData::ConfigMtuDefault, MyInterface->GetMtu());
    TEST_EQ(ConfigData::ConfigIpAddressDefault, MyInterface->GetIpAddress());
    TEST_EQ(ConfigData::ConfigNetmaskDefault, MyInterface->GetNetmask());
    TEST_EQ(ConfigData::STATE_DEF, MyInterface->GetAdminState());

    // Create the VIF and validate
    TEST_TRUE(MyInterface->SetMasterIfIndex(Master->GetIfIndex()));
    TEST_TRUE(MyInterface->SetHwAddress("46:67:3d:e8:f2:7a"));
    MyInterface->Create();
    //TEST_EQ(ConfigData::IFTYPE_IEEE8023, MyInterface->GetIfType());
    TEST_EQ(std::string("46:67:3d:e8:f2:7a"), MyInterface->GetHwAddress());
    //TEST_NEQ(ConfigData::ConfigMtuDefault, MyInterface->GetMtu());
    TEST_EQ(ConfigData::ConfigIpAddressDefault, MyInterface->GetIpAddress());
    TEST_EQ(ConfigData::ConfigNetmaskDefault, MyInterface->GetNetmask());
    //TEST_EQ(ConfigData::STATE_DOWN, MyInterface->GetAdminState());

    MyInterface->Destroy();

    delete (MyInterface);
    delete (Master);
  }

  // Return success
  UTEST_RETURN;


  TEST_EQ(1, MyInterface->GetIfIndex());
  TEST_EQ(std::string("lo"), MyInterface->GetIfName());
  TEST_EQ(ConfigData::IFTYPE_DEF, MyInterface->GetIfType());
  TEST_EQ(ConfigData::ConfigHwAddressDefault, MyInterface->GetHwAddress());
  TEST_EQ(ConfigData::ConfigMtuDefault, MyInterface->GetMtu());
  TEST_EQ(ConfigData::ConfigIpAddressDefault, MyInterface->GetIpAddress());
  TEST_EQ(ConfigData::ConfigNetmaskDefault, MyInterface->GetNetmask());
  TEST_EQ(ConfigData::STATE_DEF, MyInterface->GetAdminState());

  // Refresh
  TEST_TRUE(MyInterface->Refresh());
  TEST_EQ(1, MyInterface->GetIfIndex());
  TEST_EQ(std::string("lo"), MyInterface->GetIfName());
  TEST_EQ(ConfigData::IFTYPE_LOOP, MyInterface->GetIfType());
  TEST_EQ(std::string("00:00:00:00:00:00"), MyInterface->GetHwAddress());
  TEST_EQ(65536, MyInterface->GetMtu());
  TEST_EQ(ConfigData::STATE_UP, MyInterface->GetAdminState());
  //  TEST_EQ(std::string("127.0.0.1"), MyInterface->GetIpAddress());
  //  TEST_EQ(std::string("255.255.255.0"), MyInterface->GetNetmask());

  // Cleanup
  delete (MyInterface);


  // Return success
  UTEST_RETURN;
}
