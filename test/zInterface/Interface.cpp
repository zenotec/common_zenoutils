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
using namespace zUtils::zConfig;
using namespace zUtils::zInterface;

int
zInterfaceTest_InterfaceCtor(void* arg)
{

  ZLOG_DEBUG("#############################################################");
  ZLOG_DEBUG("# zInterfaceTest_InterfaceCtor()");
  ZLOG_DEBUG("#############################################################");

  InterfaceConfigData *MyConfig = new InterfaceConfigData;
  TEST_ISNOT_NULL(MyConfig);
  TEST_EQ(InterfaceConfigData::ConfigNameDefault, MyConfig->GetName());
  TEST_EQ(InterfaceConfigData::TYPE_DEF, MyConfig->GetType());
  TEST_EQ(InterfaceConfigData::ConfigAddressDefault, MyConfig->GetAddress());
  TEST_EQ(InterfaceConfigData::STATE_DEF, MyConfig->GetState());

  // Set
  TEST_TRUE(MyConfig->SetName("eth0"));
  TEST_TRUE(MyConfig->SetType(InterfaceConfigData::TYPE_WIRED));
  TEST_TRUE(MyConfig->SetAddress("1.2.3.4"));
  TEST_TRUE(MyConfig->SetState(InterfaceConfigData::STATE_UP));

  Interface *MyInterface = new zInterface::Interface(*MyConfig);
  TEST_ISNOT_NULL(MyInterface);
  TEST_EQ(std::string("eth0"), MyInterface->GetName());
  TEST_EQ(InterfaceConfigData::TYPE_WIRED, MyInterface->GetType());
  TEST_EQ(std::string("1.2.3.4"), MyInterface->GetAddress());
  TEST_EQ(InterfaceConfigData::STATE_UP, MyInterface->GetState());

  // Cleanup
  delete (MyInterface);

  // Return success
  return (0);
}

int
zInterfaceTest_InterfaceRefresh(void* arg)
{

  ZLOG_DEBUG("#############################################################");
  ZLOG_DEBUG("# zInterfaceTest_InterfaceRefresh()");
  ZLOG_DEBUG("#############################################################");

  InterfaceConfigData *MyConfig = new InterfaceConfigData;
  TEST_ISNOT_NULL(MyConfig);
  TEST_EQ(InterfaceConfigData::ConfigNameDefault, MyConfig->GetName());
  TEST_EQ(InterfaceConfigData::TYPE_DEF, MyConfig->GetType());
  TEST_EQ(InterfaceConfigData::ConfigAddressDefault, MyConfig->GetAddress());
  TEST_EQ(InterfaceConfigData::STATE_DEF, MyConfig->GetState());

  // Set
  TEST_TRUE(MyConfig->SetName("lo"));
  TEST_TRUE(MyConfig->SetType(InterfaceConfigData::TYPE_LOOP));

  Interface *MyInterface = new zInterface::Interface(*MyConfig);
  TEST_ISNOT_NULL(MyInterface);
  TEST_EQ(std::string("lo"), MyInterface->GetName());
  TEST_EQ(InterfaceConfigData::TYPE_LOOP, MyInterface->GetType());
  TEST_EQ(InterfaceConfigData::ConfigAddressDefault, MyInterface->GetAddress());
  TEST_EQ(InterfaceConfigData::STATE_DEF, MyInterface->GetState());
  TEST_EQ(-1, MyInterface->Index());
  TEST_EQ(std::string(""), MyInterface->HwAddress());

  // Refresh and verify
  TEST_TRUE(MyInterface->Refresh());
  TEST_EQ(std::string("lo"), MyInterface->GetName());
  TEST_EQ(InterfaceConfigData::TYPE_LOOP, MyInterface->GetType());
  TEST_NEQ(-1, MyInterface->Index());
  TEST_EQ(std::string("127.0.0.1"), MyInterface->GetAddress());
  TEST_EQ(std::string("00:00:00:00:00:00"), MyInterface->HwAddress());

  // Cleanup
  delete (MyInterface);

  // Return success
  return (0);
}
