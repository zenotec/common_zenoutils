/*
 * Copyright (c) 2016 Cable Television Laboratories, Inc. ("CableLabs")
 *                    and others.  All rights reserved.
 *
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
#include <unistd.h>

#include "UnitTest.h"
#include "zWirelessTest.h"

#include <zutils/zCompatibility.h>
#include <zutils/zLog.h>
using namespace zUtils;
ZLOG_MODULE_INIT(zLog::Log::MODULE_TEST);
#include <zutils/zWireless.h>
#include <zutils/zAccessPointInterface.h>
#include <zutils/nl80211/ListInterfacesCommand.h>
#include <zutils/nl80211/ListPhysCommand.h>
#include "ieee80211.h"

using namespace Test;
using namespace zUtils;
//using namespace ieee80211;


class TestAccessPointInterface : public zWireless::AccessPointInterface
{
  bool _running;
  bool commit(){
    return 1;
    }
public:
  TestAccessPointInterface(const std::string& name_) :
    AccessPointInterface(name_), _running(false)
  {
    this->SetOpMode(zWireless::ConfigData::OPMODE_AP);
  }
  ~TestAccessPointInterface()
  {
  }
};


int
zWirelessTest_GetSsid(void* arg){
  ZLOG_DEBUG("#############################################################");
  ZLOG_DEBUG("# zWirelessTest_GetSsid()");
  ZLOG_DEBUG("#############################################################");

  
  std::string ssid;
  std::string ifname = std::string("vap");
  TestAccessPointInterface *MyInterface = new TestAccessPointInterface(ifname);
  ssid = MyInterface->GetSsid(); //
  TEST_EQ(ssid,MyInterface->GetSsid());
  
  delete MyInterface;
  UTEST_RETURN;
}

int
zWirelessTest_SetSsid(void* arg){
  ZLOG_DEBUG("#############################################################");
  ZLOG_DEBUG("# zWirelessTest_SetSsid()");
  ZLOG_DEBUG("#############################################################");

  std::string ifname = std::string("ifname1");
  TestAccessPointInterface *MyInterface = new TestAccessPointInterface(ifname);
  std::string ssid1 = "hello";
  const std::string& ssid_ = ssid1;
  MyInterface->SetSsid(ssid_);
  TEST_EQ(ssid_ ,MyInterface->GetSsid());
  
  delete MyInterface;
  UTEST_RETURN;
}

int
zWirelessTest_TestInterface(void* arg)
{
  std::string name1 = std::string("ap1");
  TestAccessPointInterface * MyInterface = new TestAccessPointInterface(name1);
  MyInterface->Create();
  MyInterface->Stop();

  delete MyInterface;
  UTEST_RETURN;
}


int
zWirelessTest_GetBssid(void* arg){
  ZLOG_DEBUG("#############################################################");
  ZLOG_DEBUG("# zWirelessTest_GetBssid()");
  ZLOG_DEBUG("#############################################################");

  
  std::string bssid;
  std::string ifname = std::string("a");
  TestAccessPointInterface *MyInterface = new TestAccessPointInterface(ifname);
  bssid = MyInterface->GetBssid();
  TEST_EQ(bssid, MyInterface->GetBssid());
  
  delete MyInterface;
  UTEST_RETURN;
}

int
zWirelessTest_SetBssid(void* arg){
  ZLOG_DEBUG("#############################################################");
  ZLOG_DEBUG("# zWirelessTest_SetBssid()");
  ZLOG_DEBUG("#############################################################");

  std::string ifname = std::string("ifname2");
  TestAccessPointInterface *MyInterface = new TestAccessPointInterface(ifname);
  std::string bssid1 = "hello2";
  const std::string& bssid_ = bssid1;
  MyInterface->SetBssid(bssid_);
  TEST_EQ(bssid_ ,MyInterface->GetBssid());
  
  delete MyInterface;
  UTEST_RETURN;
}

/*int
zWirelessTest_Start(void* arg){
  ZLOG_DEBUG("#############################################################");
  ZLOG_DEBUG("# zWirelessTest_start()");
  ZLOG_DEBUG("#############################################################");

  ProbeRequest *MyProbeRequest = new ProbeRequest();
  Beacon *MyBeacon = new Beacon();
  

  UTEST_RETURN;



}*/
