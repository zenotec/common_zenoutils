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

#include <list>
#include <mutex>

#include <zutils/zLog.h>
#include <zutils/zQueue.h>
#include <zutils/zThread.h>
#include <zutils/zData.h>
#include <zutils/zEvent.h>
#include <zutils/zConfig.h>
#include <zutils/zSerial.h>

#include "UnitTest.h"
#include "zSerialTest.h"

using namespace Test;
using namespace zUtils;

int
zSerialTest_ConfigPathDefaults(void *arg_)
{

  ZLOG_DEBUG("#############################################################");
  ZLOG_DEBUG("# zSerialTest_ConfigPathDefaults()");
  ZLOG_DEBUG("#############################################################");

  zSerial::SerialConfigPath path;

  // Return success
  return (0);

}

int
zSerialTest_ConfigDataDefaults(void *arg_)
{

  ZLOG_DEBUG("#############################################################");
  ZLOG_DEBUG("# zSerialTest_ConfigDataDefaults()");
  ZLOG_DEBUG("#############################################################");

  // Create new config data and verify
  zSerial::SerialConfigData *MyConfig = new zSerial::SerialConfigData;
  TEST_ISNOT_NULL(MyConfig);
  TEST_EQ(zSerial::SerialConfigData::ConfigTypeDefault, MyConfig->GetType());

  // Cleanup
  delete (MyConfig);

  // Return success
  return (0);

}

int
zSerialTest_PortDefaults(void *arg_)
{

  ZLOG_DEBUG("#############################################################");
  ZLOG_DEBUG("# zComTest_PortDefaults()");
  ZLOG_DEBUG("#############################################################");

  TestPort *MyPort = new TestPort;
  TEST_ISNOT_NULL(MyPort);

  // Cleanup
  delete (MyPort);

  // Return success
  return (0);

}

int
zSerialTest_TtyPortDefaults(void *arg_)
{

  ZLOG_DEBUG("#############################################################");
  ZLOG_DEBUG("# zComTest_TtyPortDefaults()");
  ZLOG_DEBUG("#############################################################");

  zSerial::TtySerialPort *MyPort = new zSerial::TtySerialPort;
  TEST_ISNOT_NULL(MyPort);
  TEST_EQ(zSerial::SerialConfigData::ConfigTypeTty, MyPort->GetType());
  TEST_EQ(zSerial::TtySerialConfigData::ConfigDeviceDefault, MyPort->GetDevice());
  TEST_EQ(zSerial::TtySerialPort::BAUD_DEF, MyPort->GetBaud());
  TEST_EQ(zSerial::TtySerialPort::DATABITS_DEF, MyPort->GetDataBits());
  TEST_EQ(zSerial::TtySerialPort::STOPBITS_DEF, MyPort->GetStopBits());
  TEST_EQ(zSerial::TtySerialPort::PARITY_DEF, MyPort->GetParity());
  TEST_EQ(zSerial::TtySerialPort::FLOWCNTL_DEF, MyPort->GetFlowControl());

  // Cleanup
  delete (MyPort);

  // Return success
  return (0);

}
