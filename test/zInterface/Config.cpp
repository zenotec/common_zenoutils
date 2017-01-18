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
  ConfigData config(path);
  TEST_TRUE(config.PutValue(path.Name(), std::string("eth0")));
  TEST_TRUE(config.PutValue(path.Type(), InterfaceConfigData::ConfigTypeWired));
  TEST_TRUE(config.PutValue(path.Address(), std::string("1.2.3.4")));
  TEST_TRUE(config.PutValue(path.State(), InterfaceConfigData::ConfigStateUp));

  // Construct and verify
  InterfaceConfigData *MyConfig = new InterfaceConfigData (config);
  TEST_ISNOT_NULL(MyConfig);
  TEST_EQ(std::string("eth0"), MyConfig->GetName());
  TEST_EQ(InterfaceConfigData::TYPE_WIRED, MyConfig->GetType());
  TEST_EQ(InterfaceConfigData::STATE_UP, MyConfig->GetState());
  TEST_EQ(std::string("1.2.3.4"), MyConfig->GetAddress());

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
  TEST_EQ(InterfaceConfigData::TYPE_DEF, MyConfig->GetType());
  TEST_EQ(InterfaceConfigData::ConfigAddressDefault, MyConfig->GetAddress());
  TEST_EQ(InterfaceConfigData::STATE_DEF, MyConfig->GetState());

  // Set
  TEST_TRUE(MyConfig->SetName("eth0"));
  TEST_TRUE(MyConfig->SetType(InterfaceConfigData::TYPE_WIRED));
  TEST_TRUE(MyConfig->SetAddress("1.2.3.4"));
  TEST_TRUE(MyConfig->SetState(InterfaceConfigData::STATE_UP));

  // Get
  TEST_EQ(std::string("eth0"), MyConfig->GetName());
  TEST_EQ(InterfaceConfigData::TYPE_WIRED, MyConfig->GetType());
  TEST_EQ(std::string("1.2.3.4"), MyConfig->GetAddress());
  TEST_EQ(InterfaceConfigData::STATE_UP, MyConfig->GetState());

  // Cleanup
  delete (MyConfig);

  // Return success
  return (0);

}

