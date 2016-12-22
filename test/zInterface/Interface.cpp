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

  Interface *MyInterface1 = new zInterface::Interface;
  TEST_ISNOT_NULL(MyInterface1);
  TEST_EQ(InterfaceConfigData::ConfigNameDefault, MyInterface1->GetName());
  TEST_EQ(InterfaceConfigData::ConfigTypeDefault, MyInterface1->GetType());
  TEST_EQ(InterfaceConfigData::ConfigStateDefault, MyInterface1->GetState());
  TEST_EQ(InterfaceConfigData::ConfigRateDefault, MyInterface1->GetRate());

  // Setup configuration object to mimic a InterfaceConfiguration configuration object
  InterfaceConfigPath path;
  ConfigData config;
  config.Put(path.Name(), std::string("eth0"));
  config.Put(path.Type(), InterfaceConfigData::ConfigTypeWired);
  config.Put(path.State(), InterfaceConfigData::ConfigStateUp);
  config.Put(path.Rate(), 10000000);

  // Construct and verify
  InterfaceConfigData *MyConfig = new InterfaceConfigData(config);
  TEST_ISNOT_NULL(MyConfig);
  TEST_EQ(std::string("eth0"), MyConfig->GetName());
  TEST_EQ(InterfaceConfigData::ConfigTypeWired, MyConfig->GetType());
  TEST_EQ(InterfaceConfigData::ConfigStateUp, MyConfig->GetState());
  TEST_EQ(10000000, MyConfig->GetRate());

  Interface *MyInterface2 = new zInterface::Interface(*MyConfig);
  TEST_ISNOT_NULL(MyInterface2);
  TEST_EQ(std::string("eth0"), MyInterface2->GetName());
  TEST_EQ(InterfaceConfigData::ConfigTypeWired, MyInterface2->GetType());
  TEST_EQ(InterfaceConfigData::ConfigStateUp, MyInterface2->GetState());
  TEST_EQ(10000000, MyInterface2->GetRate());

  Interface *MyInterface3 = new zInterface::Interface(*MyInterface2);
  TEST_ISNOT_NULL(MyInterface3);
  TEST_EQ(std::string("eth0"), MyInterface3->GetName());
  TEST_EQ(InterfaceConfigData::ConfigTypeWired, MyInterface3->GetType());
  TEST_EQ(InterfaceConfigData::ConfigStateUp, MyInterface3->GetState());
  TEST_EQ(10000000, MyInterface3->GetRate());

  // Cleanup
  delete (MyInterface1);
  delete (MyInterface2);
  delete (MyInterface3);

  // Return success
  return (0);
}

int
zInterfaceTest_InterfaceRefresh(void* arg)
{

  ZLOG_DEBUG("#############################################################");
  ZLOG_DEBUG("# zInterfaceTest_InterfaceRefresh()");
  ZLOG_DEBUG("#############################################################");

  Interface *MyInterface = new zInterface::Interface;
  TEST_ISNOT_NULL(MyInterface);
  TEST_EQ(InterfaceConfigData::ConfigNameDefault, MyInterface->GetName());
  TEST_EQ(InterfaceConfigData::ConfigTypeDefault, MyInterface->GetType());
  TEST_EQ(InterfaceConfigData::ConfigStateDefault, MyInterface->GetState());
  TEST_EQ(InterfaceConfigData::ConfigRateDefault, MyInterface->GetRate());

  // Set up interface
  TEST_TRUE(MyInterface->SetName(std::string("lo")));
  TEST_TRUE(MyInterface->SetType(InterfaceConfigData::ConfigTypeLoop));

  // Get
  TEST_EQ(-1, MyInterface->Index());
  TEST_EQ(std::string(""), MyInterface->IpAddress());
  TEST_EQ(std::string(""), MyInterface->HwAddress());

  // Refresh and verify
  TEST_TRUE(MyInterface->Refresh());
  TEST_EQ(std::string("lo"), MyInterface->GetName());
  TEST_EQ(InterfaceConfigData::ConfigTypeLoop, MyInterface->GetType());
  TEST_NEQ(-1, MyInterface->Index());
  TEST_EQ(std::string("127.0.0.1"), MyInterface->IpAddress());
  TEST_EQ(std::string("00:00:00:00:00:00"), MyInterface->HwAddress());

  // Cleanup
  delete (MyInterface);

  // Return success
  return (0);
}
