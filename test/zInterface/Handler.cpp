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
  // Setup configuration object to mimic a InterfaceConfiguration configuration object
  InterfaceConfigPath path;
  ConfigData config(path);
  ConfigData iface(path);

  TEST_TRUE(iface.Put(path.Name(), std::string("lo")));
  TEST_TRUE(iface.Put(path.Type(), InterfaceConfigData::ConfigTypeLoop));
  TEST_TRUE(iface.Put(path.State(), InterfaceConfigData::ConfigStateUp));
  TEST_TRUE(iface.Put(path.Rate(), 10000000));

  TEST_TRUE(config.Add(iface.GetData()));
  TEST_TRUE(config.Add(iface.GetData()));

  config.DisplayJson();

  InterfaceHandler *MyHandler = new InterfaceHandler(config);
  TEST_ISNOT_NULL(MyHandler);
  MyHandler->Display();

  // Cleanup
  delete (MyHandler);

  // Return success
  return (0);
}

