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
  return (0);
}
