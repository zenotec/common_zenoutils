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
zSerialTest_SerialConfigDataCtor(void *arg_)
{

  ZLOG_DEBUG("#############################################################");
  ZLOG_DEBUG("# zSerialTest_SerialConfigDataCtor()");
  ZLOG_DEBUG("#############################################################");

  std::string obs, exp;

  // Create new config data and verify
  zSerial::SerialConfigData *MyConfig1 = new zSerial::SerialConfigData;
  TEST_ISNOT_NULL(MyConfig1);
  TEST_EQ(zSerial::SerialConfigData::ConfigTypeDefault, MyConfig1->GetType());

  // Create new serial config path and verify
  zSerial::SerialConfigPath path;

  // Create new serial config data and verify
  zConfig::ConfigData MyData(path);
  exp = zSerial::SerialConfigData::ConfigTypeEcho;
  TEST_TRUE(MyData.Put(path.Type(), exp));
  TEST_TRUE(MyData.Get(path.Type(), obs));
  TEST_EQ(obs, exp);

  zSerial::SerialConfigData *MyConfig2 = new zSerial::SerialConfigData(MyData.GetData());
  TEST_ISNOT_NULL(MyConfig2);
  TEST_EQ(zSerial::SerialConfigData::ConfigTypeEcho, MyConfig2->GetType());

  zSerial::SerialConfigData *MyConfig3 = new zSerial::SerialConfigData(MyData);
  TEST_ISNOT_NULL(MyConfig3);
  TEST_EQ(zSerial::SerialConfigData::ConfigTypeEcho, MyConfig3->GetType());

  TEST_TRUE(MyConfig3->SetType(zSerial::SerialConfigData::ConfigTypeTty));
  TEST_EQ(zSerial::SerialConfigData::ConfigTypeTty, MyConfig3->GetType());

  zSerial::SerialConfigData *MyConfig4 = new zSerial::SerialConfigData(*MyConfig3);
  TEST_ISNOT_NULL(MyConfig4);
  TEST_EQ(zSerial::SerialConfigData::ConfigTypeTty, MyConfig4->GetType());

  // Cleanup
  delete (MyConfig1);
  delete (MyConfig2);
  delete (MyConfig3);
  delete (MyConfig4);

  // Return success
  return (0);

}

int
zSerialTest_SerialConfigDataGetSet(void *arg_)
{

  ZLOG_DEBUG("#############################################################");
  ZLOG_DEBUG("# zSerialTest_SerialConfigDataGetSet()");
  ZLOG_DEBUG("#############################################################");

  std::string obs, exp;

  // Create new serial config path and verify
  zSerial::SerialConfigPath path;

  // Create new serial config data and verify
  zConfig::ConfigData MyData(path);
  exp = zSerial::SerialConfigData::ConfigTypeDefault;
  TEST_TRUE(MyData.Put(path.Type(), exp));
  TEST_TRUE(MyData.Get(path.Type(), obs));
  TEST_EQ(obs, exp);

  // Create new config data and verify
  zSerial::SerialConfigData *MyConfig = new zSerial::SerialConfigData(MyData);
  TEST_ISNOT_NULL(MyConfig);
  TEST_EQ(zSerial::SerialConfigData::ConfigTypeDefault, MyConfig->GetType());
  TEST_TRUE(MyConfig->SetType(zSerial::SerialConfigData::ConfigTypeEcho));
  TEST_EQ(zSerial::SerialConfigData::ConfigTypeEcho, MyConfig->GetType());

  delete (MyConfig);

  // Return success
  return (0);

}

int
zSerialTest_TtyConfigDataCtor(void *arg_)
{

  ZLOG_DEBUG("#############################################################");
  ZLOG_DEBUG("# zSerialTest_TtyConfigDataCtor()");
  ZLOG_DEBUG("#############################################################");

  std::string obs, exp;

  // Create new config data and verify
  zSerial::TtySerialConfigData *MyConfig1 = new zSerial::TtySerialConfigData;
  TEST_ISNOT_NULL(MyConfig1);
  TEST_EQ(zSerial::SerialConfigData::ConfigTypeTty, MyConfig1->GetType());
  TEST_EQ(zSerial::TtySerialConfigData::ConfigDeviceDefault, MyConfig1->GetDevice());
  TEST_EQ(zSerial::TtySerialConfigData::ConfigBaudDefault, MyConfig1->GetBaud());
  TEST_EQ(zSerial::TtySerialConfigData::ConfigDatabitsDefault, MyConfig1->GetDataBits());
  TEST_EQ(zSerial::TtySerialConfigData::ConfigStopbitsDefault, MyConfig1->GetStopBits());
  TEST_EQ(zSerial::TtySerialConfigData::ConfigParityDefault, MyConfig1->GetParity());
  TEST_EQ(zSerial::TtySerialConfigData::ConfigFlowCtrlDefault, MyConfig1->GetFlowControl());

  // Create new serial config path and verify
  zSerial::TtySerialConfigPath path;

  // Create new serial config data and verify
  zConfig::ConfigData MyData(path);
  exp = zSerial::SerialConfigData::ConfigTypeTty;
  TEST_TRUE(MyData.Put(path.Type(), exp));
  TEST_TRUE(MyData.Get(path.Type(), obs));
  TEST_EQ(obs, exp);

  exp = std::string("/dev/tty");
  TEST_TRUE(MyData.Put(path.Device(), exp));
  TEST_TRUE(MyData.Get(path.Device(), obs));
  TEST_EQ(obs, exp);

  exp = zSerial::TtySerialConfigData::ConfigBaud38400;
  TEST_TRUE(MyData.Put(path.Baud(), exp));
  TEST_TRUE(MyData.Get(path.Baud(), obs));
  TEST_EQ(obs, exp);

  exp = zSerial::TtySerialConfigData::ConfigDatabits6;
  TEST_TRUE(MyData.Put(path.Databits(), exp));
  TEST_TRUE(MyData.Get(path.Databits(), obs));
  TEST_EQ(obs, exp);

  exp = zSerial::TtySerialConfigData::ConfigStopbits2;
  TEST_TRUE(MyData.Put(path.Stopbits(), exp));
  TEST_TRUE(MyData.Get(path.Stopbits(), obs));
  TEST_EQ(obs, exp);

  exp = zSerial::TtySerialConfigData::ConfigParityOdd;
  TEST_TRUE(MyData.Put(path.Parity(), exp));
  TEST_TRUE(MyData.Get(path.Parity(), obs));
  TEST_EQ(obs, exp);

  exp = zSerial::TtySerialConfigData::ConfigFlowCtrlHard;
  TEST_TRUE(MyData.Put(path.FlowCtrl(), exp));
  TEST_TRUE(MyData.Get(path.FlowCtrl(), obs));
  TEST_EQ(obs, exp);

  zSerial::TtySerialConfigData *MyConfig2 = new zSerial::TtySerialConfigData(MyData.GetData());
  TEST_ISNOT_NULL(MyConfig2);
  TEST_EQ(zSerial::SerialConfigData::ConfigTypeTty, MyConfig2->GetType());
  TEST_EQ(std::string("/dev/tty"), MyConfig2->GetDevice());
  TEST_EQ(zSerial::TtySerialConfigData::ConfigBaud38400, MyConfig2->GetBaud());
  TEST_EQ(zSerial::TtySerialConfigData::ConfigDatabits6, MyConfig2->GetDataBits());
  TEST_EQ(zSerial::TtySerialConfigData::ConfigStopbits2, MyConfig2->GetStopBits());
  TEST_EQ(zSerial::TtySerialConfigData::ConfigParityOdd, MyConfig2->GetParity());
  TEST_EQ(zSerial::TtySerialConfigData::ConfigFlowCtrlHard, MyConfig2->GetFlowControl());

  zSerial::TtySerialConfigData *MyConfig3 = new zSerial::TtySerialConfigData(MyData);
  TEST_ISNOT_NULL(MyConfig3);
  TEST_EQ(zSerial::SerialConfigData::ConfigTypeTty, MyConfig3->GetType());
  TEST_EQ(std::string("/dev/tty"), MyConfig3->GetDevice());
  TEST_EQ(zSerial::TtySerialConfigData::ConfigBaud38400, MyConfig3->GetBaud());
  TEST_EQ(zSerial::TtySerialConfigData::ConfigDatabits6, MyConfig3->GetDataBits());
  TEST_EQ(zSerial::TtySerialConfigData::ConfigStopbits2, MyConfig3->GetStopBits());
  TEST_EQ(zSerial::TtySerialConfigData::ConfigParityOdd, MyConfig3->GetParity());
  TEST_EQ(zSerial::TtySerialConfigData::ConfigFlowCtrlHard, MyConfig3->GetFlowControl());

  TEST_TRUE(MyConfig3->SetBaud(zSerial::TtySerialConfigData::ConfigBaud19200));

  zSerial::TtySerialConfigData *MyConfig4 = new zSerial::TtySerialConfigData(*MyConfig3);
  TEST_ISNOT_NULL(MyConfig4);
  TEST_EQ(std::string("/dev/tty"), MyConfig4->GetDevice());
  TEST_EQ(zSerial::TtySerialConfigData::ConfigBaud19200, MyConfig4->GetBaud());
  TEST_EQ(zSerial::TtySerialConfigData::ConfigDatabits6, MyConfig4->GetDataBits());
  TEST_EQ(zSerial::TtySerialConfigData::ConfigStopbits2, MyConfig4->GetStopBits());
  TEST_EQ(zSerial::TtySerialConfigData::ConfigParityOdd, MyConfig4->GetParity());
  TEST_EQ(zSerial::TtySerialConfigData::ConfigFlowCtrlHard, MyConfig4->GetFlowControl());

  // Cleanup
  delete (MyConfig1);
  delete (MyConfig2);
  delete (MyConfig3);
  delete (MyConfig4);

  // Return success
  return (0);

}

int
zSerialTest_TtyConfigDataGetSet(void *arg_)
{

  ZLOG_DEBUG("#############################################################");
  ZLOG_DEBUG("# zSerialTest_TtyConfigDataGetSet()");
  ZLOG_DEBUG("#############################################################");

  // Create new config data and verify
  zSerial::TtySerialConfigData *MyConfig = new zSerial::TtySerialConfigData;
  TEST_ISNOT_NULL(MyConfig);
  TEST_EQ(zSerial::SerialConfigData::ConfigTypeTty, MyConfig->GetType());
  TEST_EQ(zSerial::TtySerialConfigData::ConfigDeviceDefault, MyConfig->GetDevice());
  TEST_EQ(zSerial::TtySerialConfigData::ConfigBaudDefault, MyConfig->GetBaud());
  TEST_EQ(zSerial::TtySerialConfigData::ConfigDatabitsDefault, MyConfig->GetDataBits());
  TEST_EQ(zSerial::TtySerialConfigData::ConfigStopbitsDefault, MyConfig->GetStopBits());
  TEST_EQ(zSerial::TtySerialConfigData::ConfigParityDefault, MyConfig->GetParity());
  TEST_EQ(zSerial::TtySerialConfigData::ConfigFlowCtrlDefault, MyConfig->GetFlowControl());

  TEST_TRUE(MyConfig->SetDevice(std::string("/dev/tty")));
  TEST_TRUE(MyConfig->SetBaud(zSerial::TtySerialConfigData::ConfigBaud38400));
  TEST_TRUE(MyConfig->SetDataBits(zSerial::TtySerialConfigData::ConfigDatabits6));
  TEST_TRUE(MyConfig->SetStopBits(zSerial::TtySerialConfigData::ConfigStopbits2));
  TEST_TRUE(MyConfig->SetParity(zSerial::TtySerialConfigData::ConfigParityOdd));
  TEST_TRUE(MyConfig->SetFlowControl(zSerial::TtySerialConfigData::ConfigFlowCtrlHard));

  TEST_EQ(std::string("/dev/tty"), MyConfig->GetDevice());
  TEST_EQ(zSerial::TtySerialConfigData::ConfigBaud38400, MyConfig->GetBaud());
  TEST_EQ(zSerial::TtySerialConfigData::ConfigDatabits6, MyConfig->GetDataBits());
  TEST_EQ(zSerial::TtySerialConfigData::ConfigStopbits2, MyConfig->GetStopBits());
  TEST_EQ(zSerial::TtySerialConfigData::ConfigParityOdd, MyConfig->GetParity());
  TEST_EQ(zSerial::TtySerialConfigData::ConfigFlowCtrlHard, MyConfig->GetFlowControl());

  delete (MyConfig);

  // Return success
  return (0);

}

int
zSerialTest_TtyConfigPortGetSet(void *arg_)
{

  ZLOG_DEBUG("#############################################################");
  ZLOG_DEBUG("# zSerialTest_TtyConfigPortGetSet()");
  ZLOG_DEBUG("#############################################################");

  // Create new config data and verify
  zSerial::TtySerialConfigData *MyConfig = new zSerial::TtySerialConfigData;
  TEST_ISNOT_NULL(MyConfig);
  ZLOG_DEBUG("#############################################################");
  TEST_EQ(zSerial::SerialConfigData::ConfigTypeTty, MyConfig->GetType());
  ZLOG_DEBUG("#############################################################");
  TEST_EQ(zSerial::TtySerialConfigData::ConfigDeviceDefault, MyConfig->GetDevice());
  ZLOG_DEBUG("#############################################################");
  TEST_EQ(zSerial::TtySerialConfigData::ConfigBaudDefault, MyConfig->GetBaud());
  ZLOG_DEBUG("#############################################################");
  TEST_EQ(zSerial::TtySerialConfigData::ConfigDatabitsDefault, MyConfig->GetDataBits());
  ZLOG_DEBUG("#############################################################");
  TEST_EQ(zSerial::TtySerialConfigData::ConfigStopbitsDefault, MyConfig->GetStopBits());
  ZLOG_DEBUG("#############################################################");
  TEST_EQ(zSerial::TtySerialConfigData::ConfigParityDefault, MyConfig->GetParity());
  ZLOG_DEBUG("#############################################################");
  TEST_EQ(zSerial::TtySerialConfigData::ConfigFlowCtrlDefault, MyConfig->GetFlowControl());
  ZLOG_DEBUG("#############################################################");

  TEST_TRUE(MyConfig->SetDevice(std::string("/dev/tty")));
  ZLOG_DEBUG("#############################################################");
  TEST_TRUE(MyConfig->SetBaud(zSerial::TtySerialConfigData::ConfigBaud38400));
  ZLOG_DEBUG("#############################################################");
  TEST_TRUE(MyConfig->SetDataBits(zSerial::TtySerialConfigData::ConfigDatabits6));
  ZLOG_DEBUG("#############################################################");
  TEST_TRUE(MyConfig->SetStopBits(zSerial::TtySerialConfigData::ConfigStopbits2));
  ZLOG_DEBUG("#############################################################");
  TEST_TRUE(MyConfig->SetParity(zSerial::TtySerialConfigData::ConfigParityOdd));
  ZLOG_DEBUG("#############################################################");
  TEST_TRUE(MyConfig->SetFlowControl(zSerial::TtySerialConfigData::ConfigFlowCtrlHard));
  ZLOG_DEBUG("#############################################################");

  TEST_EQ(std::string("/dev/tty"), MyConfig->GetDevice());
  ZLOG_DEBUG("#############################################################");
  TEST_EQ(zSerial::TtySerialConfigData::ConfigBaud38400, MyConfig->GetBaud());
  ZLOG_DEBUG("#############################################################");
  TEST_EQ(zSerial::TtySerialConfigData::ConfigDatabits6, MyConfig->GetDataBits());
  ZLOG_DEBUG("#############################################################");
  TEST_EQ(zSerial::TtySerialConfigData::ConfigStopbits2, MyConfig->GetStopBits());
  ZLOG_DEBUG("#############################################################");
  TEST_EQ(zSerial::TtySerialConfigData::ConfigParityOdd, MyConfig->GetParity());
  ZLOG_DEBUG("#############################################################");
  TEST_EQ(zSerial::TtySerialConfigData::ConfigFlowCtrlHard, MyConfig->GetFlowControl());
  ZLOG_DEBUG("#############################################################");

  zSerial::TtySerialPort *MyPort = new zSerial::TtySerialPort(*MyConfig);
  TEST_ISNOT_NULL(MyPort);
  ZLOG_DEBUG("#############################################################");
  TEST_EQ(std::string("/dev/tty"), MyPort->GetDevice());
  ZLOG_DEBUG("#############################################################");
  TEST_EQ(zSerial::TtySerialPort::BAUD_38400, MyPort->GetBaud());
  ZLOG_DEBUG("#############################################################");
  TEST_EQ(zSerial::TtySerialPort::DATABITS_6, MyPort->GetDataBits());
  ZLOG_DEBUG("#############################################################");
  TEST_EQ(zSerial::TtySerialPort::STOPBITS_2, MyPort->GetStopBits());
  ZLOG_DEBUG("#############################################################");
  TEST_EQ(zSerial::TtySerialPort::PARITY_ODD, MyPort->GetParity());
  ZLOG_DEBUG("#############################################################");
  TEST_EQ(zSerial::TtySerialPort::FLOWCNTL_HARD, MyPort->GetFlowControl());
  ZLOG_DEBUG("#############################################################");

  // Clean up
  delete (MyConfig);
  ZLOG_DEBUG("#############################################################");
  delete (MyPort);
  ZLOG_DEBUG("#############################################################");

  // Return success
  return (0);

}

