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
zInterfaceTest_InterfaceConfigurationCtor (void* arg)
{

  // Setup configuration object to mimic a InterfaceConfiguration configuration object
  InterfaceConfigPath path;
  ConfigData config;
  config.Put(path.Name(), std::string("eth0"));
  config.Put(path.Type(), InterfaceConfigData::ConfigTypeWired);
  config.Put(path.State(), InterfaceConfigData::ConfigStateUp);
  config.Put(path.Rate(), 10000000);

  // Construct and verify
  InterfaceConfigData *MyConfig = new InterfaceConfigData (config);
  TEST_ISNOT_NULL(MyConfig);
  TEST_EQ(std::string("eth0"), MyConfig->GetName());
  TEST_EQ(InterfaceConfigData::ConfigTypeWired, MyConfig->GetType());
  TEST_EQ(InterfaceConfigData::ConfigStateUp, MyConfig->GetState());
  TEST_EQ(10000000, MyConfig->GetRate());

  // Cleanup
  delete (MyConfig);

  // Return success
  return (0);
}

int
zInterfaceTest_InterfaceConfigurationGetSet (void* arg)
{
  std::vector<std::string> slaves;

  InterfaceConfigData *MyConfig = new InterfaceConfigData;
  TEST_ISNOT_NULL(MyConfig);
  TEST_EQ(InterfaceConfigData::ConfigNameDefault, MyConfig->GetName());
  TEST_EQ(InterfaceConfigData::ConfigTypeDefault, MyConfig->GetType());
  TEST_EQ(InterfaceConfigData::ConfigStateDefault, MyConfig->GetState());
  TEST_EQ(InterfaceConfigData::ConfigRateDefault, MyConfig->GetRate());

  // Set
  TEST_TRUE(MyConfig->SetName("eth0"));
  TEST_TRUE(MyConfig->SetType(InterfaceConfigData::ConfigTypeWired));
  TEST_TRUE(MyConfig->SetState(InterfaceConfigData::ConfigStateUp));
  TEST_TRUE(MyConfig->SetRate(1000000));

  // Get
  TEST_EQ(std::string("eth0"), MyConfig->GetName());
  TEST_EQ(InterfaceConfigData::ConfigTypeWired, MyConfig->GetType());
  TEST_EQ(InterfaceConfigData::ConfigStateUp, MyConfig->GetState());
  TEST_EQ(1000000, MyConfig->GetRate());

  // Cleanup
  delete (MyConfig);

  // Return success
  return (0);

}

