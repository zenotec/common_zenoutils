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
using namespace zUtils::zConfig;
using namespace zUtils::zInterface;

int
zInterfaceTest_InterfaceFactory(void* arg)
{

  ZLOG_DEBUG("#############################################################");
  ZLOG_DEBUG("# zInterfaceTest_InterfaceFactory()");
  ZLOG_DEBUG("#############################################################");

//  // Setup configuration object to mimic a InterfaceConfiguration configuration object
//  zConfig::ConfigData config;
//  ConfigData iface;
//
//  TEST_TRUE(iface.Name(std::string("lo")));
//  TEST_TRUE(iface.Type(ConfigData::TYPE_LOOP));
//
//  TEST_TRUE_MSG(config.AddChild(iface.GetData()), iface.GetJson());
//
//  TEST_TRUE(iface.Name(std::string("eth0")));
//  TEST_TRUE(iface.Type(ConfigData::TYPE_WIRED));
//
//  TEST_TRUE_MSG(config.AddChild(iface.GetData()), iface.GetJson());

//  InterfaceTable MyTable = InterfaceFactory::Create(config);
//  TEST_ISNOT_NULL(MyTable["lo"].get());
//  TEST_ISNOT_NULL(MyTable["eth0"].get());

  // Return success
  return (0);
}

