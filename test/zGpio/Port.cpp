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
zGpioTest_PortGetSet(void* arg)
{

  ZLOG_DEBUG("#############################################################");
  ZLOG_DEBUG("# zGpioTest_PortGetSet()");
  ZLOG_DEBUG("#############################################################");

  // Create new GPIO Port test configuration
  TestPortConfig *MyConfig = new TestPortConfig(1);
  TEST_ISNOT_NULL(MyConfig);
//  MyConfig->DisplayJson();

  // Create new GPIO port and verify
  zGpio::GpioPort *MyPort = new zGpio::GpioPort(*MyConfig);
  TEST_ISNOT_NULL(MyPort);

  // Open port
  TEST_TRUE(MyPort->Open() >= 0);

  // Verify configured state and current state match
  TEST_EQ(zGpio::GpioPort::STATE_DEF, MyPort->GetState());
  TEST_EQ(zGpio::GpioPort::STATE_DEF, MyPort->Get());
  TEST_EQ(MyPort->GetState(), MyPort->Get());

  // Modify current state and verify
  TEST_TRUE(MyPort->Set(zGpio::GpioPort::STATE_ACTIVE));
  TEST_EQ(zGpio::GpioPort::STATE_DEF, MyPort->GetState());
  TEST_EQ(zGpio::GpioPort::STATE_ACTIVE, MyPort->Get());
  TEST_NEQ(MyPort->GetState(), MyPort->Get());

  // Modify current state back and verify
  TEST_TRUE(MyPort->Set(zGpio::GpioPort::STATE_INACTIVE));
  TEST_EQ(zGpio::GpioPort::STATE_DEF, MyPort->GetState());
  TEST_EQ(zGpio::GpioPort::STATE_INACTIVE, MyPort->Get());
  TEST_EQ(MyPort->GetState(), MyPort->Get());

  // Close port
  TEST_TRUE(MyPort->Close());

  // Clean up
  delete (MyPort);
  delete (MyConfig);
  return (0);
}
