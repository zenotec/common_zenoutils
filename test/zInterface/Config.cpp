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

//  // Setup configuration object to mimic a InterfaceConfiguration configuration object
//  ConfigPath path;
//  ConfigData config(path);
//  TEST_TRUE(config.PutValue(path.Name(), std::string("eth0")));
//  TEST_TRUE(config.PutValue(path.Type(), ConfigData::ConfigTypeWired));
//  TEST_TRUE(config.PutValue(path.IpAddress(), std::string("1.2.3.4")));
//  TEST_TRUE(config.PutValue(path.State(), ConfigData::ConfigStateUp));
//
//  // Construct and verify
//  ConfigData *MyConfig = new ConfigData (config);
//  TEST_ISNOT_NULL(MyConfig);
//  TEST_EQ(std::string("eth0"), MyConfig->Name());
//  TEST_EQ(ConfigData::TYPE_WIRED, MyConfig->Type());
//  TEST_EQ(ConfigData::STATE_UP, MyConfig->GetState());
//  TEST_EQ(std::string("1.2.3.4"), MyConfig->GetIpAddress());
//
//  // Cleanup
//  delete (MyConfig);
//
  // Return success
  return (0);
}

int
zInterfaceTest_InterfaceConfigurationGetSet (void* arg)
{
  std::vector<std::string> slaves;

//  ConfigData *MyConfig = new ConfigData;
//  TEST_ISNOT_NULL(MyConfig);
//  TEST_EQ(ConfigData::ConfigNameDefault, MyConfig->Name());
//  TEST_EQ(ConfigData::TYPE_DEF, MyConfig->Type());
//  TEST_EQ(ConfigData::ConfigIpAddressDefault, MyConfig->GetIpAddress());
//  TEST_EQ(ConfigData::STATE_DEF, MyConfig->GetState());
//
//  // Set
//  TEST_TRUE(MyConfig->Name("eth0"));
//  TEST_TRUE(MyConfig->Type(ConfigData::TYPE_WIRED));
//  TEST_TRUE(MyConfig->SetIpAddress("1.2.3.4"));
//  TEST_TRUE(MyConfig->SetState(ConfigData::STATE_UP));
//
//  // Get
//  TEST_EQ(std::string("eth0"), MyConfig->Name());
//  TEST_EQ(ConfigData::TYPE_WIRED, MyConfig->Type());
//  TEST_EQ(std::string("1.2.3.4"), MyConfig->GetIpAddress());
//  TEST_EQ(ConfigData::STATE_UP, MyConfig->GetState());
//
//  // Cleanup
//  delete (MyConfig);

  // Return success
  return (0);

}

