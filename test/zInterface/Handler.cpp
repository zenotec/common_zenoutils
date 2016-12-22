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
zInterfaceTest_InterfaceHandlerCtor(void* arg)
{

  ZLOG_DEBUG("#############################################################");
  ZLOG_DEBUG("# zInterfaceTest_InterfaceHandlerCtor()");
  ZLOG_DEBUG("#############################################################");

  // Setup configuration object to mimic a InterfaceConfiguration configuration object
  zConfig::ConfigData config;
  TEST_TRUE(config.Append(InterfaceConfigPath::ConfigRoot));
  InterfaceConfigData iface;

  TEST_TRUE(iface.SetName(std::string("lo")));
  TEST_TRUE(iface.SetType(InterfaceConfigData::ConfigTypeLoop));
  TEST_TRUE(iface.SetState(InterfaceConfigData::ConfigStateUp));
  TEST_TRUE(iface.SetRate(10000000));

  TEST_TRUE_MSG(config.Add(iface.GetData()), iface.GetJson());

  TEST_TRUE(iface.SetName(std::string("eth0")));
  TEST_TRUE(iface.SetType(InterfaceConfigData::ConfigTypeWired));
  TEST_TRUE(iface.SetState(InterfaceConfigData::ConfigStateUp));
  TEST_TRUE(iface.SetRate(1000000000));

  TEST_TRUE_MSG(config.Add(iface.GetData()), iface.GetJson());

  InterfaceHandler *MyHandler = new InterfaceHandler(config);
  TEST_ISNOT_NULL(MyHandler);
//  MyHandler->Display();

  // Cleanup
  delete (MyHandler);

  // Return success
  return (0);
}

