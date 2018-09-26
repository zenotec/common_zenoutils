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

#include <unistd.h>
#include <net/if.h>

#include <zutils/zLog.h>
using namespace zUtils;
ZLOG_MODULE_INIT(zLog::Log::MODULE_TEST);

#include "UnitTest.h"
#include "Nl80211Test.h"
using namespace Test;

#include <zutils/ieee80211/Frame.h>
#include <zutils/ieee80211/Beacon.h>
#include <zutils/ieee80211/Probe.h>
using namespace zWireless;

#include <zutils/netlink/GetLinkCommand.h>
#include <zutils/netlink/SetLinkCommand.h>

#include <zutils/nl80211/ListPhysCommand.h>
#include <zutils/nl80211/GetPhyCommand.h>
#include <zutils/nl80211/SetPhyCommand.h>
#include <zutils/nl80211/ListInterfacesCommand.h>
#include <zutils/nl80211/GetInterfaceCommand.h>
#include <zutils/nl80211/SetInterfaceCommand.h>
#include <zutils/nl80211/NewInterfaceCommand.h>
#include <zutils/nl80211/DelInterfaceCommand.h>
#include <zutils/nl80211/StartApCommand.h>
#include <zutils/nl80211/StopApCommand.h>
#include <zutils/nl80211/FrameEvent.h>
#include <zutils/nl80211/FrameCommand.h>

class TestCallback :
    public netlink::Callback
{

public:

  TestCallback(void* arg_ = NULL) :
    netlink::Callback(arg_),
    ValidCnt(0),
    FinishCnt(0),
    AckCnt(0),
    ErrCnt(0)
  {
  }

  virtual
  ~TestCallback()
  {
  }

  int ValidCnt;
  int FinishCnt;
  int AckCnt;
  int ErrCnt;

protected:

  virtual int
  valid_cb(struct nl_msg* msg_, void* arg_)
  {
    this->ValidCnt++;
    return (NL_OK);
  }

  virtual int
  finish_cb(struct nl_msg* msg_, void* arg_)
  {
    this->FinishCnt++;
    return (NL_OK);
  }

  virtual int
  ack_cb(struct nl_msg* msg_, void* arg_)
  {
    this->AckCnt++;
    return (NL_OK);
  }

  virtual int
  err_cb(struct sockaddr_nl* nla_, struct nlmsgerr* nlerr_, void* arg_)
  {
    this->ErrCnt++;
    return (NL_SKIP);
  }

private:

};

int
Nl80211Test_PhyGetCommands(void* arg_)
{

  ZLOG_DEBUG("#############################################################");
  ZLOG_DEBUG("# Nl80211Test_PhyGetCommands()");
  ZLOG_DEBUG("#############################################################");

  // First find a radio we can test with, if none exist, bypass test
  nl80211::ListPhysCommand listcmd;
  TEST_TRUE(listcmd.Exec());
  std::map<int, std::string> phys = listcmd();
  if (phys.empty())
  {
    ZLOG_WARN("No PHYs to test with");
    UTEST_BYPASS;
  }

  FOREACH(auto& phy, phys)
  {
    // Test get
    nl80211::GetPhyCommand getcmd(phy.first);
    TEST_TRUE(getcmd.Exec());
    TEST_EQ(getcmd.PhyIndex(), phy.first);
  }

  // Return success
  return (UTEST_PASS);
}

int
Nl80211Test_PhySetCommands(void* arg_)
{

  ZLOG_DEBUG("#############################################################");
  ZLOG_DEBUG("# Nl80211Test_PhySetCommands()");
  ZLOG_DEBUG("#############################################################");

  // First step is to determine if we were executed with root privileges, if not, bypass test
  uid_t uid = getuid(), euid = geteuid();
  if (uid != 0 || uid != euid)
  {
    UTEST_BYPASS;
  }

  // Next find an interface we can test with, if none exist, bypass test
  nl80211::ListInterfacesCommand ifacescmd;
  TEST_TRUE(ifacescmd.Exec());
  std::map<uint32_t, std::string> ifaces = ifacescmd();
  if (ifaces.empty())
  {
    ZLOG_WARN("No interfaces to test with");
    UTEST_BYPASS;
  }

  FOREACH(auto& iface, ifaces)
  {
    // Get phy index of interface
    nl80211::GetInterfaceCommand ifacecmd(iface.first);
    TEST_TRUE(ifacecmd.Exec());
    TEST_EQ(ifacecmd.IfIndex(), iface.first);

    // Get phy name of interface
    nl80211::GetPhyCommand getcmd(ifacecmd.PhyIndex());
    TEST_TRUE(getcmd.Exec());

    // Test set
    nl80211::SetPhyCommand setcmd(iface.first);
    TEST_TRUE(setcmd.PhyName("tphy"));
    TEST_TRUE(setcmd.Exec());
    TEST_EQ(std::string("tphy"), setcmd.PhyName());
    TEST_TRUE(setcmd.PhyName(getcmd.PhyName()));
    TEST_TRUE(setcmd.Exec());
  }

  // Return success
  return (UTEST_PASS);
}

int
Nl80211Test_InterfaceGetCommands(void* arg_)
{

  ZLOG_DEBUG("#############################################################");
  ZLOG_DEBUG("# Nl80211Test_InterfaceGetCommands()");
  ZLOG_DEBUG("#############################################################");

  // First find a radio we can test with, if none exist, bypass test
  nl80211::ListInterfacesCommand ifacescmd;
  TEST_TRUE(ifacescmd.Exec());
  std::map<uint32_t, std::string> ifaces = ifacescmd();
  if (ifaces.empty())
  {
    ZLOG_WARN("No interfaces to test with");
    UTEST_BYPASS;
  }

  FOREACH(auto& iface, ifaces)
  {
    nl80211::GetInterfaceCommand ifacecmd(iface.first);
    TEST_TRUE(ifacecmd.Exec());
    TEST_EQ(ifacecmd.IfIndex(), iface.first);
  }

  // Return success
  return (UTEST_PASS);
}

int
Nl80211Test_InterfaceSetCommands(void* arg_)
{

  ZLOG_DEBUG("#############################################################");
  ZLOG_DEBUG("# Nl80211Test_InterfaceSetCommands()");
  ZLOG_DEBUG("#############################################################");

  // First step is to determine if we were executed with root privileges, if not, bypass test
  uid_t uid = getuid(), euid = geteuid();
  if (uid != 0 || uid != euid)
  {
    UTEST_BYPASS;
  }

  // Next find a radio we can test with, if none exist, bypass test
  nl80211::ListInterfacesCommand ifacescmd;
  TEST_TRUE(ifacescmd.Exec());
  std::map<uint32_t, std::string> ifaces = ifacescmd();
  if (ifaces.empty())
  {
    ZLOG_WARN("No interfaces to test with");
    UTEST_BYPASS;
  }

  FOREACH(auto& iface, ifaces)
  {
    nl80211::SetInterfaceCommand ifacecmd(iface.first);
    TEST_TRUE(ifacecmd.Exec());
    TEST_EQ(ifacecmd.IfIndex(), iface.first);
  }

  // Return success
  return (UTEST_PASS);
}

int
Nl80211Test_InterfaceNewDelCommand(void* arg_)
{

  ZLOG_DEBUG("#############################################################");
  ZLOG_DEBUG("# Nl80211Test_InterfaceNewDelCommand()");
  ZLOG_DEBUG("#############################################################");

  // First step is to determine if we were executed with root privileges, if not, bypass test
  uid_t uid = getuid(), euid = geteuid();
  if (uid != 0 || uid != euid)
  {
    UTEST_BYPASS;
  }

  // Next find a radio we can test with, if none exist, bypass test
  nl80211::ListPhysCommand listcmd;
  TEST_TRUE(listcmd.Exec());
  std::map<int, std::string> phys = listcmd();
  if (phys.empty())
  {
    ZLOG_WARN("No PHYs to test with");
    UTEST_BYPASS;
  }

  // Create new interface
  nl80211::NewInterfaceCommand newcmd(std::string("vap"), phys.begin()->first);
  TEST_TRUE(newcmd.IfType(NL80211_IFTYPE_AP));
  TEST_TRUE(newcmd.Exec());
  TEST_ISNOT_ZERO(newcmd.IfIndex());
  TEST_EQ(std::string("vap"), newcmd.IfName());

  // Delete interface
  nl80211::DelInterfaceCommand delcmd(newcmd.IfIndex());
  TEST_TRUE(delcmd.Exec());

  // Return success
  return (UTEST_PASS);
}

int
Nl80211Test_ApCommands(void* arg_)
{

  ZLOG_DEBUG("#############################################################");
  ZLOG_DEBUG("# Nl80211Test_ApCommands()");
  ZLOG_DEBUG("#############################################################");

  // First step is to determine if we were executed with root privileges, if not, bypass test
  uid_t uid = getuid(), euid = geteuid();
  if (uid != 0 || uid != euid)
  {
    UTEST_BYPASS;
  }

  // Next find a radio we can test with, if none exist, bypass test
  nl80211::ListPhysCommand listcmd;
  TEST_TRUE(listcmd.Exec());
  std::map<int, std::string> phys = listcmd();
  if (phys.empty())
  {
    ZLOG_WARN("No PHYs to test with");
    UTEST_BYPASS;
  }

  // Create new interface
  nl80211::NewInterfaceCommand newcmd(std::string("vap"), phys.begin()->first);
  TEST_TRUE(newcmd.IfType(NL80211_IFTYPE_AP));
  TEST_TRUE(newcmd.Exec());
  TEST_ISNOT_ZERO(newcmd.IfIndex());
  TEST_EQ(std::string("vap"), newcmd.IfName());

  // Set interface up
  netlink::SetLinkCommand setcmd(newcmd.IfIndex());
  TEST_TRUE(setcmd.SetFlags((IFF_UP | IFF_RUNNING)));
  TEST_TRUE(setcmd.Exec());

  // Create new beacon
  uint8_t bbuf[512] = {0};
  size_t blen = sizeof(bbuf);
  ieee80211::Beacon beacon;
  beacon.Assemble(bbuf, blen, false);

  // Start AP
  nl80211::StartApCommand startcmd(newcmd.IfIndex());
  TEST_TRUE(startcmd.Ssid(std::string("vap-ssid")));
  TEST_TRUE(startcmd.Channel(2437));
  TEST_TRUE(startcmd.ChannelWidth(NL80211_CHAN_WIDTH_20_NOHT));
  TEST_TRUE(startcmd.CenterFrequency1(2437));
  TEST_TRUE(startcmd.BeaconInterval(100));
  TEST_TRUE(startcmd.BeaconHead.Set(beacon.Head(), beacon.HeadSize()));
  TEST_TRUE(startcmd.BeaconTail.Set(beacon.Tail(), beacon.TailSize()));
  TEST_TRUE(startcmd.DtimPeriod(1));
  TEST_TRUE(startcmd.Exec());

  // Stop AP
  nl80211::StopApCommand stopcmd(newcmd.IfIndex());
  TEST_TRUE(stopcmd.Exec());

  // Delete interface
  nl80211::DelInterfaceCommand delcmd(newcmd.IfIndex());
  TEST_TRUE(delcmd.Exec());

  // Return success
  return (UTEST_PASS);
}

int
Nl80211Test_FrameEvent(void* arg_)
{

  ZLOG_DEBUG("#############################################################");
  ZLOG_DEBUG("# Nl80211Test_FrameEvent()");
  ZLOG_DEBUG("#############################################################");

  // First step is to determine if we were executed with root privileges, if not, bypass test
  uid_t uid = getuid(), euid = geteuid();
  if (uid != 0 || uid != euid)
  {
    UTEST_BYPASS;
  }

  // Next find a radio we can test with, if none exist, bypass test
  nl80211::ListPhysCommand listcmd;
  TEST_TRUE(listcmd.Exec());
  std::map<int, std::string> phys = listcmd();
  if (phys.empty())
  {
    ZLOG_WARN("No PHYs to test with");
    UTEST_BYPASS;
  }

  // Create new interface
  nl80211::NewInterfaceCommand newcmd(std::string("vap"), phys.begin()->first);
  TEST_TRUE(newcmd.IfType(NL80211_IFTYPE_AP));
  TEST_TRUE(newcmd.Exec());
  TEST_ISNOT_ZERO(newcmd.IfIndex());
  TEST_EQ(std::string("vap"), newcmd.IfName());

  // Set interface up
  netlink::SetLinkCommand setcmd(newcmd.IfIndex());
  TEST_TRUE(setcmd.SetFlags((IFF_UP | IFF_RUNNING)));
  TEST_TRUE(setcmd.Exec());

  // Create new beacon
  uint8_t bbuf[512] = {0};
  size_t blen = sizeof(bbuf);
  ieee80211::Beacon beacon;
  beacon.Assemble(bbuf, blen, false);

  // Start AP
  nl80211::StartApCommand startcmd(newcmd.IfIndex());
  TEST_TRUE(startcmd.Ssid(std::string("vap-ssid")));
  TEST_TRUE(startcmd.Channel(2437));
  TEST_TRUE(startcmd.ChannelWidth(NL80211_CHAN_WIDTH_20_NOHT));
  TEST_TRUE(startcmd.CenterFrequency1(2437));
  TEST_TRUE(startcmd.BeaconInterval(100));
  TEST_TRUE(startcmd.BeaconHead.Set(beacon.Head(), beacon.HeadSize()));
  TEST_TRUE(startcmd.BeaconTail.Set(beacon.Tail(), beacon.TailSize()));
  TEST_TRUE(startcmd.DtimPeriod(1));
  TEST_TRUE(startcmd.Exec());

  // Create test callback for receiving management frames
  TestCallback tcb;

  // Create frame event for monitoring for management frames
  nl80211::FrameEvent fevent(newcmd.IfIndex(), tcb);
  uint16_t type = IEEE80211_TYPE_SUBTYPE(ieee80211::Frame::TYPE_MGMT, ieee80211::Frame::SUBTYPE_PROBEREQ);
  TEST_TRUE(fevent.FrameType(type));
  TEST_TRUE(fevent.FrameMatch.Set(NULL, 0));
  TEST_TRUE(fevent.Exec());
  TEST_TRUE(fevent.Listen());

  sleep(5);

  // Stop AP
  nl80211::StopApCommand stopcmd(newcmd.IfIndex());
  TEST_TRUE(stopcmd.Exec());

  // Delete interface
  nl80211::DelInterfaceCommand delcmd(newcmd.IfIndex());
  TEST_TRUE(delcmd.Exec());

  TEST_IS_ZERO(tcb.FinishCnt);
  TEST_IS_ZERO(tcb.AckCnt);
  TEST_IS_ZERO(tcb.ErrCnt);
  TEST_ISNOT_ZERO(tcb.ValidCnt);

  // Return success
  return (UTEST_PASS);
}

int
Nl80211Test_FrameCommand(void* arg_)
{

  ZLOG_DEBUG("#############################################################");
  ZLOG_DEBUG("# Nl80211Test_FrameCommand()");
  ZLOG_DEBUG("#############################################################");

  // First step is to determine if we were executed with root privileges, if not, bypass test
  uid_t uid = getuid(), euid = geteuid();
  if (uid != 0 || uid != euid)
  {
    UTEST_BYPASS;
  }

  // Next find a radio we can test with, if none exist, bypass test
  nl80211::ListPhysCommand listcmd;
  TEST_TRUE(listcmd.Exec());
  std::map<int, std::string> phys = listcmd();
  if (phys.empty())
  {
    ZLOG_WARN("No PHYs to test with");
    UTEST_BYPASS;
  }

  // Create new interface
  nl80211::NewInterfaceCommand newcmd(std::string("vap"), phys.begin()->first);
  TEST_TRUE(newcmd.IfType(NL80211_IFTYPE_AP));
  TEST_TRUE(newcmd.Exec());
  TEST_ISNOT_ZERO(newcmd.IfIndex());
  TEST_EQ(std::string("vap"), newcmd.IfName());

  // Set interface up
  netlink::SetLinkCommand setcmd(newcmd.IfIndex());
  TEST_TRUE(setcmd.SetFlags((IFF_UP | IFF_RUNNING)));
  TEST_TRUE(setcmd.Exec());

  // Get interface info
  nl80211::GetInterfaceCommand getcmd(newcmd.IfIndex());
  TEST_TRUE(getcmd.Exec());

  // Create new beacon frame for starting AP interface
  uint8_t bbuf[512] = {0};
  size_t blen = sizeof(bbuf);
  ieee80211::Beacon beacon;
  beacon.Assemble(bbuf, blen, false);

  // Start AP
  nl80211::StartApCommand startcmd(newcmd.IfIndex());
  TEST_TRUE(startcmd.Ssid(std::string("vap-ssid")));
  TEST_TRUE(startcmd.Channel(2437));
  TEST_TRUE(startcmd.ChannelWidth(NL80211_CHAN_WIDTH_20_NOHT));
  TEST_TRUE(startcmd.CenterFrequency1(2437));
  TEST_TRUE(startcmd.BeaconInterval(100));
  TEST_TRUE(startcmd.BeaconHead.Set(beacon.Head(), beacon.HeadSize()));
  TEST_TRUE(startcmd.BeaconTail.Set(beacon.Tail(), beacon.TailSize()));
  TEST_TRUE(startcmd.DtimPeriod(1));
  TEST_TRUE(startcmd.Exec());

  // Create new probe response to send from AP interface
  uint8_t pbuf[512] = {0};
  size_t plen = sizeof(pbuf);
  ieee80211::ProbeResponse probe_resp;
  TEST_TRUE(probe_resp.ReceiverAddress(std::string("00:00:00:00:00:00")));
  TEST_TRUE(probe_resp.TransmitterAddress(getcmd.Mac()));
  probe_resp.Assemble(pbuf, plen, false);

  // Create new frame command for sending probe respose
  nl80211::FrameCommand framecmd(newcmd.IfIndex());
  TEST_TRUE(framecmd.Frame.Set(pbuf, (sizeof(pbuf) - plen)));
  TEST_TRUE(framecmd.Exec());

  // Stop AP
  nl80211::StopApCommand stopcmd(newcmd.IfIndex());
  TEST_TRUE(stopcmd.Exec());

  // Delete interface
  nl80211::DelInterfaceCommand delcmd(newcmd.IfIndex());
  TEST_TRUE(delcmd.Exec());

  // Return success
  return (UTEST_PASS);
}


