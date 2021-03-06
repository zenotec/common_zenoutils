/*
 * Copyright (c) 2014-2016 ZenoTec LLC (http://www.zenotec.net)
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at:
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#include <string>
#include <list>
#include <mutex>

#include <zutils/zLog.h>
#include <zutils/zSem.h>
#include <zutils/zThread.h>
#include <zutils/zData.h>
#include <zutils/zEvent.h>
#include <zutils/zConfig.h>
#include <zutils/zSwitch.h>

#include <zutils/zGpio.h>

#include "zGpioTest.h"

using namespace zUtils;
using namespace Test;

int
zGpioTest_HandlerAddPort(void* arg)
{

  ZLOG_DEBUG("#############################################################");
  ZLOG_DEBUG("# zGpioTest_HandlerAddPort()");
  ZLOG_DEBUG("#############################################################");

  // Create new GPIO port test configuration and verify
  TestPortConfig* MyConf = new TestPortConfig(1);
  TEST_ISNOT_NULL(MyConf);

  // Create new GPIO port and verify
  zGpio::GpioPort* MyPort = new zGpio::GpioPort(*MyConf);
  TEST_ISNOT_NULL(MyPort);

  // Create new GPIO handler and verify
  zGpio::GpioHandler* MyHandler = new zGpio::GpioHandler;
  TEST_ISNOT_NULL(MyHandler);

  // Add GpioPort and verify
  TEST_TRUE(MyHandler->Add(MyPort));

  // Clean up
  delete (MyHandler);
  delete (MyPort);
  delete (MyConf);
  return (0);
}

int
zGpioTest_HandlerOnOff(void* arg)
{

  ZLOG_DEBUG("#############################################################");
  ZLOG_DEBUG("# zGpioTest_HandlerOnOff()");
  ZLOG_DEBUG("#############################################################");

  // Create new GPIO port test configuration and verify
  TestPortConfig* MyConfig1 = new TestPortConfig(1);
  TEST_ISNOT_NULL(MyConfig1);
  TEST_EQ(1, MyConfig1->Identifier());

  // Create new GPIO port test configuration and verify
  TestPortConfig* MyConfig2 = new TestPortConfig(2);
  TEST_ISNOT_NULL(MyConfig2);
  TEST_EQ(2, MyConfig2->Identifier());

  // Configure both ports to be outputs
  TEST_TRUE(MyConfig1->Direction(zGpio::GpioConfigData::ConfigDirectionValueOut));
  TEST_TRUE(MyConfig2->Direction(zGpio::GpioConfigData::ConfigDirectionValueOut));

  // Create new GPIO port and verify
  zGpio::GpioPort* MyPort1 = new zGpio::GpioPort(*MyConfig1);
  TEST_ISNOT_NULL(MyPort1);
  TEST_EQ(zGpio::GpioPort::DIR_OUT, MyPort1->GetDirection());
  TEST_EQ(zGpio::GpioPort::STATE_INACTIVE, MyPort1->GetState());
  TEST_EQ(zGpio::GpioPort::STATE_INACTIVE, MyPort1->Get());

  // Create new GPIO port and verify
  zGpio::GpioPort* MyPort2 = new zGpio::GpioPort(*MyConfig2);
  TEST_ISNOT_NULL(MyPort2);
  TEST_EQ(zGpio::GpioPort::DIR_OUT, MyPort2->GetDirection());
  TEST_EQ(zGpio::GpioPort::STATE_INACTIVE, MyPort2->GetState());
  TEST_EQ(zGpio::GpioPort::STATE_INACTIVE, MyPort2->Get());

  // Create new GPIO handler and verify
  zGpio::GpioHandler* MyHandler = new zGpio::GpioHandler;
  TEST_ISNOT_NULL(MyHandler);

  // Create new GPIO event observer
  TestObserver* MyObserver = new TestObserver;
  TEST_ISNOT_NULL(MyObserver);

  // Register observer with handler
  TEST_TRUE(MyHandler->RegisterObserver(MyObserver));

  // Add GPIO ports and verify
  TEST_TRUE(MyHandler->Add(MyPort1));
  TEST_TRUE(MyHandler->Add(MyPort2));

  // Set state to inactive
  TEST_TRUE(MyHandler->Set(zGpio::GpioPort::STATE_INACTIVE));
  TEST_EQ(zGpio::GpioPort::STATE_INACTIVE, MyHandler->Get());
  TEST_EQ(zGpio::GpioPort::STATE_INACTIVE, MyPort1->Get());
  TEST_EQ(zGpio::GpioPort::STATE_INACTIVE, MyPort2->Get());

  // Set state to active
  TEST_TRUE(MyHandler->Set(zGpio::GpioPort::STATE_ACTIVE));
  TEST_EQ(zGpio::GpioPort::STATE_ACTIVE, MyHandler->Get());
  TEST_EQ(zGpio::GpioPort::STATE_ACTIVE, MyPort1->Get());
  TEST_EQ(zGpio::GpioPort::STATE_ACTIVE, MyPort2->Get());

  // Set state to inactive
  TEST_TRUE(MyHandler->Set(zGpio::GpioPort::STATE_INACTIVE));
  TEST_EQ(zGpio::GpioPort::STATE_INACTIVE, MyHandler->Get());
  TEST_EQ(zGpio::GpioPort::STATE_INACTIVE, MyPort1->Get());
  TEST_EQ(zGpio::GpioPort::STATE_INACTIVE, MyPort2->Get());

  // Unregister observer
  TEST_TRUE(MyHandler->UnregisterObserver(MyObserver));

  // Clean up
  delete (MyObserver);
  delete (MyHandler);
  delete (MyPort2);
  delete (MyPort1);
  delete (MyConfig2);
  delete (MyConfig1);

  return (0);
}

