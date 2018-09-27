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

#include <string.h>

#include <zutils/zLog.h>
using namespace zUtils;
ZLOG_MODULE_INIT(zLog::Log::MODULE_TEST);

#include "Ieee80211Test.h"
#include "UnitTest.h"
#include <zutils/ieee80211/RadioTap.h>
#include <zutils/ieee80211/Beacon.h>

using namespace zWireless::ieee80211;

const uint8_t beacon_frame[] =
{
    0x80, 0x00, 0x00, 0x00, 0xff, 0xff, 0xff, 0xff,
	0xff, 0xff, 0x00, 0x11, 0x22, 0x33, 0x44, 0x55,
	0x00, 0x11, 0x22, 0x33, 0x44, 0x55, 0xa0, 0xfd,
	0x3b, 0xf0, 0x5f, 0xfe, 0x38, 0x00, 0x00, 0x00,
	0x64, 0x00, 0x11, 0x00, 0x00, 0x08, 0x4c, 0x4c,
	0x57, 0x69, 0x66, 0x69, 0x35, 0x30, 0x01, 0x08,
	0x8c, 0x12, 0x98, 0x24, 0xb0, 0x48, 0x60, 0x6c,
	0x03, 0x01, 0x24, 0x05, 0x04, 0x01, 0x02, 0x00,
	0x00, 0x07, 0x0a, 0x44, 0x45, 0x20, 0x24, 0x08,
	0x14, 0x64, 0x0b, 0x1a, 0x00, 0x30, 0x14, 0x01,
	0x00, 0x00, 0x0f, 0xac, 0x04, 0x01, 0x00, 0x00,
	0x0f, 0xac, 0x04, 0x01, 0x00, 0x00, 0x0f, 0xac,
	0x02, 0x0c, 0x00, 0x3b, 0x02, 0x74, 0x00, 0x2d,
	0x1a, 0xce, 0x01, 0x1b, 0xff, 0xff, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x3d, 0x16, 0x24, 0x05, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x7f, 0x08, 0x04, 0x00, 0x00,
	0x02, 0x00, 0x00, 0x00, 0x40,
	// start of WMM/WME
	0xdd, 0x18, 0x00, 0x50, 0xf2, 0x02, 0x01, 0x01,
	0x80, 0x00, 0x03, 0xa4, 0x00, 0x00, 0x27, 0xa4,
	0x00, 0x00, 0x42, 0x43, 0x5e, 0x00, 0x62, 0x32,
	0x2f, 0x00
};

const size_t beacon_frame_len = sizeof(beacon_frame);

int
Ieee80211Test_BeaconGetSet(void* arg_)
{

  ZLOG_DEBUG("#############################################################");
  ZLOG_DEBUG("# Ieee80211Test_BeaconGetSet()");
  ZLOG_DEBUG("#############################################################");

  // Create frame and validate
  Beacon frame;
  TEST_IS_ZERO(frame.Version());
  TEST_EQ(Frame::TYPE_MGMT, frame.Type());
  TEST_EQ(Frame::SUBTYPE_BEACON, frame.Subtype());
  TEST_FALSE(frame.ToDS());
  TEST_FALSE(frame.FromDS());
  TEST_FALSE(frame.MoreFragments());
  TEST_FALSE(frame.Retry());
  TEST_FALSE(frame.PowerManagement());
  TEST_FALSE(frame.MoreData());
  TEST_FALSE(frame.Protected());
  TEST_FALSE(frame.Order());
  TEST_IS_ZERO(frame.DurationId());
  TEST_EQ(std::string("ff:ff:ff:ff:ff:ff"), frame.ReceiverAddress());
  TEST_EQ(std::string(""), frame.TransmitterAddress());
  TEST_EQ(std::string(""), frame.Bssid());
  TEST_IS_ZERO(frame.FragmentNum());
  TEST_IS_ZERO(frame.SequenceNum());
  TEST_IS_ZERO(frame.Timestamp());
  TEST_IS_ZERO(frame.Interval());
  TEST_IS_ZERO(frame.Capabilities());
  TEST_EQ(std::string(""), frame.Ssid());
  TEST_IS_ZERO(frame.Rates().size());
////  TEST_IS_ZERO(frame.PowerCaps().size());
  TEST_IS_ZERO(frame.ExtRates().size());
//  TEST_EQ(4, frame.WmmWme().size()); // all vendor specific tags start with a 3 byte OUI and 1 byte OUI_TYPE

  // Set non default values
  WmmWmeTag::wmm_wme wmmwme = {};
  wmmwme.wme_subtype = 1;
  wmmwme.wme_version = 2;
  wmmwme.wme_qos_info = 3;
  wmmwme.reserved = 4;
  wmmwme.aci0.aci_aifsn_field = 5;
  wmmwme.aci0.ecw = 6;
  wmmwme.aci0.txop_limit = 7;
  wmmwme.aci1.aci_aifsn_field = 8;
  wmmwme.aci1.ecw = 9;
  wmmwme.aci1.txop_limit = 10;
  wmmwme.aci2.aci_aifsn_field = 11;
  wmmwme.aci2.ecw = 12;
  wmmwme.aci2.txop_limit = 13;
  wmmwme.aci3.aci_aifsn_field = 14;
  wmmwme.aci3.ecw = 15;
  wmmwme.aci3.txop_limit = 16;

  TEST_TRUE(frame.Version(1));
  TEST_TRUE(frame.Type(Frame::TYPE_DATA));
  TEST_TRUE(frame.Subtype(Frame::SUBTYPE_BEACON));
  TEST_TRUE(frame.ToDS(true));
  TEST_TRUE(frame.FromDS(true));
  TEST_TRUE(frame.MoreFragments(true));
  TEST_TRUE(frame.Retry(true));
  TEST_TRUE(frame.PowerManagement(true));
  TEST_TRUE(frame.MoreData(true));
  TEST_TRUE(frame.Protected(true));
  TEST_TRUE(frame.Order(true));
  TEST_TRUE(frame.DurationId(8888));
  TEST_TRUE(frame.ReceiverAddress("ff:ff:ff:ff:ff:ff"));
  TEST_TRUE(frame.TransmitterAddress("00:11:22:33:44:55"));
  TEST_TRUE(frame.Bssid("00:11:22:33:44:55"));
  TEST_TRUE(frame.FragmentNum(0x04));
  TEST_TRUE(frame.SequenceNum(0x0123));
  TEST_TRUE(frame.Timestamp(0x01234567));
  TEST_TRUE(frame.Interval(100));
  TEST_TRUE(frame.Capabilities(0x1234));
  TEST_TRUE(frame.Ssid("TestSSID"));
  TEST_TRUE(frame.Rates(1));
  TEST_TRUE(frame.Rates(2));
  TEST_TRUE(frame.Rates(4));
  TEST_TRUE(frame.Rates(8));
////  TEST_TRUE(frame.PowerCaps(20,30));
  TEST_TRUE(frame.ExtRates(17));
  TEST_TRUE(frame.ExtRates(18));

  std::vector<uint8_t> rates_ = {0x82, 0x84, 0x8B, 0x96, 6*2, 9*2, 12*2, 18*2};
  std::vector<uint8_t> exrates_ = {24, 36, 48, 54};
  HtCapsTag::ht_caps htcaps_ = { 0xaaaa, 0xee, { { 10, 9, 8, 7, 6, 5, 4, 3, 2, 1 }, 0x0123, 0x44 }, 0x4444, 0x55555555L, 0x66 };
  //                         Chan s1    s2      s3      mcs mask                             hdr     mcs   padding - move to ht_caps
  HtInfoTag::ht_info info_ = { 6, 0x11, 0x2222, 0x3333, { { 10, 9, 8, 7, 6, 5, 4, 3, 2, 1 },  0x44, { 0xff, 0xfe, 0xfd } } };

  TEST_TRUE(frame.Dsss(6));
  TimTag:tim_tag tim_ = { 1, 2, 3, 4 };
  TEST_TRUE(frame.Tim(tim_));
  TEST_TRUE(frame.HtCaps(htcaps_));
  TEST_TRUE(frame.HtInfo(info_));
//  TEST_TRUE(frame.Country("US"));	//RKBNot Really a Beacon Parameter...

//  TEST_TRUE(frame.WmmWme(wmmwme));

  // Verify
  TEST_EQ(frame.Version(), 1);
  TEST_EQ(Frame::TYPE_DATA, frame.Type());
  TEST_EQ(Frame::SUBTYPE_BEACON, frame.Subtype());
  TEST_TRUE(frame.ToDS());
  TEST_TRUE(frame.FromDS());
  TEST_TRUE(frame.MoreFragments());
  TEST_TRUE(frame.Retry());
  TEST_TRUE(frame.PowerManagement());
  TEST_TRUE(frame.MoreData());
  TEST_TRUE(frame.Protected());
  TEST_TRUE(frame.Order());
  TEST_EQ(frame.DurationId(), 8888);
  TEST_EQ(std::string("ff:ff:ff:ff:ff:ff"), frame.ReceiverAddress());
  TEST_EQ(std::string("00:11:22:33:44:55"), frame.TransmitterAddress());
  TEST_EQ(std::string("00:11:22:33:44:55"), frame.Bssid());
  TEST_EQ(0x04, frame.FragmentNum());
  TEST_EQ(0x0123, frame.SequenceNum());
  TEST_EQ(0x01234567, frame.Timestamp());
  TEST_EQ(100, frame.Interval());
  TEST_EQ(0x1234, frame.Capabilities());
  TEST_EQ(std::string("TestSSID"), frame.Ssid());
  TEST_EQ(4, frame.Rates().size());
  TEST_EQ(1, frame.Rates()[0]);
  TEST_EQ(2, frame.Rates()[1]);
  TEST_EQ(4, frame.Rates()[2]);
  TEST_EQ(8, frame.Rates()[3]);
////  TEST_EQ(20, frame.PowerCaps()[0]);
////  TEST_EQ(30, frame.PowerCaps()[1]);
  TEST_EQ(17, frame.ExtRates()[0]);
  TEST_EQ(18, frame.ExtRates()[1]);

//  TEST_EQ(1, frame.WmmWme.WmmWme().wme_subtype);
//  TEST_EQ(2, frame.WmmWme.WmmWme().wme_version);
//  TEST_EQ(3, frame.WmmWme.WmmWme().wme_qos_info);
//  TEST_EQ(4, frame.WmmWme.WmmWme().reserved);
//  TEST_EQ(5, frame.WmmWme.WmmWme().aci0.aci_aifsn_field);
//  TEST_EQ(6, frame.WmmWme.WmmWme().aci0.ecw);
//  TEST_EQ(7, frame.WmmWme.WmmWme().aci0.txop_limit);
//  TEST_EQ(8, frame.WmmWme.WmmWme().aci1.aci_aifsn_field);
//  TEST_EQ(9, frame.WmmWme.WmmWme().aci1.ecw);
//  TEST_EQ(10, frame.WmmWme.WmmWme().aci1.txop_limit);
//  TEST_EQ(11, frame.WmmWme.WmmWme().aci2.aci_aifsn_field);
//  TEST_EQ(12, frame.WmmWme.WmmWme().aci2.ecw);
//  TEST_EQ(13, frame.WmmWme.WmmWme().aci2.txop_limit);
//  TEST_EQ(14, frame.WmmWme.WmmWme().aci3.aci_aifsn_field);
//  TEST_EQ(15, frame.WmmWme.WmmWme().aci3.ecw);
//  TEST_EQ(16, frame.WmmWme.WmmWme().aci3.txop_limit);

  // Return success
  return (0);
}

int
Ieee80211Test_BeaconAssemble(void* arg_)
{

  ZLOG_DEBUG("#############################################################");
  ZLOG_DEBUG("# Ieee80211Test_BeaconAssemble()");
  ZLOG_DEBUG("#############################################################");

  size_t len = 0;
  uint8_t frm_buf[512] = { 0 };
  uint8_t frm_beacon[] =
  {
      0x80, 0x00, 0x34, 0x12, 0xff, 0xff, 0xff, 0xff,
      0xff, 0xff, 0x00, 0x11, 0x22, 0x33, 0x44, 0x55,
      0x00, 0x11, 0x22, 0x33, 0x44, 0x55, 0x34, 0x12,
      0x77, 0x66, 0x55, 0x44, 0x33, 0x22, 0x11, 0x00,
      0x64, 0x00, 0x34, 0x12, 0x00, 0x08, 0x54, 0x65,
      0x73, 0x74, 0x53, 0x53, 0x49, 0x44, 0x01, 0x04,
      0x01, 0x02, 0x04, 0x08, 0x32, 0x02, 0x96, 0x97,
      0x2D, 0x1A, 0x34, 0x12, 0x56, 0x78, 0x79, 0x7A,
      0x7B, 0x7C, 0x7D, 0x7E, 0x7F, 0x80, 0x81, 0x83,
      0x82, 0x84, 0x00, 0x00, 0x00, 0x86, 0x85, 0x8A,
      0x89, 0x88, 0x87, 0x8B,
      // WMM/WME
      0xDD, 0x18, 0x00, 0x50, 0xF2, 0x02, 0x01, 0x01,
      0x80, 0x00, 0x03, 0xA4, 0x00, 0x00, 0x27, 0xA4,
      0x00, 0x00, 0x42, 0x43, 0x5E, 0x00, 0x62, 0x32,
      0x2F, 0x00,
  };

  // Create frame and validate
  Beacon frame;
  TEST_IS_ZERO(frame.Version());
  TEST_EQ(Frame::TYPE_MGMT, frame.Type());
  TEST_EQ(Frame::SUBTYPE_BEACON, frame.Subtype());
  TEST_FALSE(frame.ToDS());
  TEST_FALSE(frame.FromDS());
  TEST_FALSE(frame.MoreFragments());
  TEST_FALSE(frame.Retry());
  TEST_FALSE(frame.PowerManagement());
  TEST_FALSE(frame.MoreData());
  TEST_FALSE(frame.Protected());
  TEST_FALSE(frame.Order());
  TEST_IS_ZERO(frame.DurationId());
  TEST_EQ(std::string("ff:ff:ff:ff:ff:ff"), frame.ReceiverAddress());
  TEST_EQ(std::string(""), frame.TransmitterAddress());
  TEST_EQ(std::string(""), frame.Bssid());
  TEST_IS_ZERO(frame.FragmentNum());
  TEST_IS_ZERO(frame.SequenceNum());
  TEST_IS_ZERO(frame.Timestamp());
  TEST_IS_ZERO(frame.Interval());
  TEST_IS_ZERO(frame.Capabilities());
  TEST_EQ(std::string(""), frame.Ssid());
  TEST_IS_ZERO(frame.Rates().size());
  TEST_IS_ZERO(frame.ExtRates().size());
  TEST_EQ(4, frame.WmmWme().size()); // All Vendor Specific Tags start with a 3 byte OUI and 1 byte OUI_TYPE

  // Set values for management frame
  TEST_TRUE(frame.Version(0));
  TEST_TRUE(frame.Type(Frame::TYPE_MGMT));
  TEST_TRUE(frame.Subtype(Frame::SUBTYPE_BEACON));
  TEST_TRUE(frame.ToDS(false));
  TEST_TRUE(frame.FromDS(false));
  TEST_TRUE(frame.MoreFragments(false));
  TEST_TRUE(frame.Retry(false));
  TEST_TRUE(frame.PowerManagement(false));
  TEST_TRUE(frame.MoreData(false));
  TEST_TRUE(frame.Protected(false));
  TEST_TRUE(frame.Order(false));
  TEST_TRUE(frame.DurationId(0x1234));
  TEST_TRUE(frame.ReceiverAddress("ff:ff:ff:ff:ff:ff"));
  TEST_TRUE(frame.TransmitterAddress("00:11:22:33:44:55"));
  TEST_TRUE(frame.Bssid("00:11:22:33:44:55"));
  TEST_TRUE(frame.FragmentNum(0x04));
  TEST_TRUE(frame.SequenceNum(0x0123));
  TEST_TRUE(frame.Timestamp(0x0011223344556677));
  TEST_TRUE(frame.Interval(100));
  TEST_TRUE(frame.Capabilities(0x1234));
  TEST_TRUE(frame.Ssid("TestSSID"));
  TEST_TRUE(frame.Rates(1));
  TEST_TRUE(frame.Rates(2));
  TEST_TRUE(frame.Rates(4));
  TEST_TRUE(frame.Rates(8));

  HtCapsTag::ht_caps caps = {};
  caps.ht_cap_info = 0x1234; // LE in memory
  caps.ampdu_parms = 0x56;
  caps.supported_mcs_set.rx_mcs_bitmask = {0x78, 0x79, 0x7A, 0x7B, 0x7C, 0x7D, 0x7E, 0x7F, 0x80, 0x81};
  caps.supported_mcs_set.rx_highest_rate = 0x8283; // LE in memory
  caps.supported_mcs_set.tx_mcs_fields.tx_bits = 0x84;
  caps.ht_ext_cap = 0X8586; // LE in memory
  caps.trans_beam_cap = 0X8788898A; // LE in memory
  caps.asel_cap = 0X8B;
  TEST_TRUE(frame.HtCaps(caps));

  TEST_TRUE(frame.ExtRates(0x96));
  TEST_TRUE(frame.ExtRates(0x97));

  WmmWmeTag::wmm_wme wmmwme = {};
  wmmwme.wme_subtype = 0x01;
  wmmwme.wme_version = 0x01;
  wmmwme.wme_qos_info = 0x80;
  wmmwme.aci0.aci_aifsn_field = 0x03;
  wmmwme.aci0.ecw = 0xA4;
  wmmwme.aci0.txop_limit = 0;
  wmmwme.aci1.aci_aifsn_field = 0x27;
  wmmwme.aci1.ecw = 0xA4;
  wmmwme.aci1.txop_limit = 0;
  wmmwme.aci2.aci_aifsn_field = 0x42;
  wmmwme.aci2.ecw = 0x43;
  wmmwme.aci2.txop_limit = 0x005E;
  wmmwme.aci3.aci_aifsn_field = 0x62;
  wmmwme.aci3.ecw = 0x32;
  wmmwme.aci3.txop_limit = 0x002F;
  TEST_TRUE(frame.WmmWme(wmmwme));

  // Verify
  TEST_EQ(0, frame.Version());
  TEST_EQ(Frame::TYPE_MGMT, frame.Type());
  TEST_EQ(Frame::SUBTYPE_BEACON, frame.Subtype());
  TEST_FALSE(frame.ToDS());
  TEST_FALSE(frame.FromDS());
  TEST_FALSE(frame.MoreFragments());
  TEST_FALSE(frame.Retry());
  TEST_FALSE(frame.PowerManagement());
  TEST_FALSE(frame.MoreData());
  TEST_FALSE(frame.Protected());
  TEST_FALSE(frame.Order());
  TEST_EQ(0x1234, frame.DurationId());
  TEST_EQ(std::string("ff:ff:ff:ff:ff:ff"), frame.ReceiverAddress());
  TEST_EQ(std::string("00:11:22:33:44:55"), frame.TransmitterAddress());
  TEST_EQ(std::string("00:11:22:33:44:55"), frame.Bssid());
  TEST_EQ(0x04, frame.FragmentNum());
  TEST_EQ(0x0123, frame.SequenceNum());
  TEST_EQ(0x0011223344556677, frame.Timestamp());
  TEST_EQ(100, frame.Interval());
  TEST_EQ(0x1234, frame.Capabilities());
  TEST_EQ(std::string("TestSSID"), frame.Ssid());

  TEST_EQ(4, frame.Rates().size());
  TEST_EQ(1, frame.Rates()[0]);
  TEST_EQ(2, frame.Rates()[1]);
  TEST_EQ(4, frame.Rates()[2]);
  TEST_EQ(8, frame.Rates()[3]);

  caps = frame.HtCaps();
  TEST_EQ(caps.ht_cap_info, 0x1234);
  TEST_EQ(caps.ampdu_parms, 0x56);
  TEST_EQ(int(caps.supported_mcs_set.rx_mcs_bitmask.size()), 10);
  TEST_EQ(int(caps.supported_mcs_set.rx_mcs_bitmask[0]), 0x78);
  TEST_EQ(int(caps.supported_mcs_set.rx_mcs_bitmask[1]), 0x79);
  TEST_EQ(int(caps.supported_mcs_set.rx_mcs_bitmask[2]), 0x7A);
  TEST_EQ(int(caps.supported_mcs_set.rx_mcs_bitmask[3]), 0x7B);
  TEST_EQ(int(caps.supported_mcs_set.rx_mcs_bitmask[4]), 0x7C);
  TEST_EQ(int(caps.supported_mcs_set.rx_mcs_bitmask[5]), 0x7D);
  TEST_EQ(int(caps.supported_mcs_set.rx_mcs_bitmask[6]), 0x7E);
  TEST_EQ(int(caps.supported_mcs_set.rx_mcs_bitmask[7]), 0x7F);
  TEST_EQ(int(caps.supported_mcs_set.rx_mcs_bitmask[8]), 0x80);
  TEST_EQ(int(caps.supported_mcs_set.rx_mcs_bitmask[9]), 0x81);
  TEST_EQ(caps.supported_mcs_set.rx_highest_rate, 0x8283);
  TEST_EQ(caps.supported_mcs_set.tx_mcs_fields.tx_bits, 0x84);
  TEST_EQ(caps.ht_ext_cap, 0x8586);
  TEST_EQ(caps.trans_beam_cap, 0x8788898A);
  TEST_EQ(caps.asel_cap, 0x8B);

  TEST_EQ(2, frame.ExtRates().size());
  TEST_EQ(0x96, frame.ExtRates()[0]);
  TEST_EQ(0x97, frame.ExtRates()[1]);
  //frame.ExtRates.Display();

//  wmmwme = frame.WmmWme.WmmWme();
//  TEST_EQ(wmmwme.wme_subtype, 0x01);
//  TEST_EQ(wmmwme.wme_version, 0x01);
//  TEST_EQ(wmmwme.wme_qos_info, 0x80);
//  TEST_EQ(wmmwme.aci0.aci_aifsn_field, 0x03);
//  TEST_EQ(wmmwme.aci0.ecw, 0xA4);
//  TEST_EQ(wmmwme.aci0.txop_limit, 0);
//  TEST_EQ(wmmwme.aci1.aci_aifsn_field, 0x27);
//  TEST_EQ(wmmwme.aci1.ecw, 0xA4);
//  TEST_EQ(wmmwme.aci1.txop_limit, 0);
//  TEST_EQ(wmmwme.aci2.aci_aifsn_field, 0x42);
//  TEST_EQ(wmmwme.aci2.ecw, 0x43);
//  TEST_EQ(wmmwme.aci2.txop_limit, 0x005E);
//  TEST_EQ(wmmwme.aci3.aci_aifsn_field, 0x62);
//  TEST_EQ(wmmwme.aci3.ecw, 0x32);
//  TEST_EQ(wmmwme.aci3.txop_limit, 0x002F);

  zSocket::Buffer sb;

  // Assemble and verify
  TEST_TRUE(frame.Assemble(sb));

  TEST_EQ(sb.Size(), sizeof(frm_beacon));

  uint8_t* p = sb.Head();
  for (int i = 0; i < sizeof(frm_beacon); i++)
  {
    TEST_EQ_MSG((int)frm_beacon[i], *p++, zLog::IntStr(i));
  }

  // Return success
  return (0);
}

int
Ieee80211Test_BeaconDisassemble(void* arg_)
{

  ZLOG_DEBUG("#############################################################");
  ZLOG_DEBUG("# Ieee80211Test_BeaconDisassemble()");
  ZLOG_DEBUG("#############################################################");

  // Make copy of raw beacon frame
  zSocket::Buffer sb;
  sb.Write(beacon_frame, beacon_frame_len);
  sb.Push(beacon_frame_len); // reset data ptr to start of buffer

  // Create frame and validate
  Beacon frame;
  TEST_IS_ZERO(frame.Version());
  TEST_EQ(Frame::TYPE_MGMT, frame.Type());
  TEST_EQ(Frame::SUBTYPE_BEACON, frame.Subtype());
  TEST_FALSE(frame.ToDS());
  TEST_FALSE(frame.FromDS());
  TEST_FALSE(frame.MoreFragments());
  TEST_FALSE(frame.Retry());
  TEST_FALSE(frame.PowerManagement());
  TEST_FALSE(frame.MoreData());
  TEST_FALSE(frame.Protected());
  TEST_FALSE(frame.Order());
  TEST_IS_ZERO(frame.DurationId());
  TEST_EQ(std::string("ff:ff:ff:ff:ff:ff"), frame.ReceiverAddress());
  TEST_EQ(std::string(""), frame.TransmitterAddress());
  TEST_EQ(std::string(""), frame.Bssid());
  TEST_IS_ZERO(frame.FragmentNum());
  TEST_IS_ZERO(frame.SequenceNum());
  TEST_IS_ZERO(frame.Timestamp());
  TEST_IS_ZERO(frame.Interval());
  TEST_IS_ZERO(frame.Capabilities());
  TEST_EQ(std::string(""), frame.Ssid());
  TEST_IS_ZERO(frame.Rates().size());
////  TEST_IS_ZERO(frame.PowerCaps().size());
  TEST_IS_ZERO(frame.ExtRates().size());

  // Disassemble beacon
  TEST_TRUE(frame.Disassemble(sb));

  // Verify
  TEST_IS_ZERO(frame.Version());
  TEST_EQ(Frame::TYPE_MGMT, frame.Type());
  TEST_EQ(Frame::SUBTYPE_BEACON, frame.Subtype());
  TEST_FALSE(frame.ToDS());
  TEST_FALSE(frame.FromDS());
  TEST_FALSE(frame.MoreFragments());
  TEST_FALSE(frame.Retry());
  TEST_FALSE(frame.PowerManagement());
  TEST_FALSE(frame.MoreData());
  TEST_FALSE(frame.Protected());
  TEST_FALSE(frame.Order());
  TEST_EQ(0, frame.DurationId());
  TEST_EQ(std::string("ff:ff:ff:ff:ff:ff"), frame.ReceiverAddress());
  TEST_EQ(std::string("00:11:22:33:44:55"), frame.TransmitterAddress());
  TEST_EQ(std::string("00:11:22:33:44:55"), frame.Bssid());
  TEST_EQ(0, frame.FragmentNum());
  TEST_EQ(0x0fda, frame.SequenceNum());
  TEST_EQ((uint64_t)0x38fe5ff03b, frame.Timestamp());
  TEST_EQ(100, frame.Interval());
  TEST_EQ(0x0011, frame.Capabilities());
  TEST_EQ(std::string("LLWifi50"), frame.Ssid());
  TEST_EQ(8, frame.Rates().size());
  TEST_EQ(0x8c, frame.Rates()[0]); /* 6(B) */
  TEST_EQ(0x12, frame.Rates()[1]); /* 9 */
  TEST_EQ(0x98, frame.Rates()[2]); /* 12(B) */
  TEST_EQ(0x24, frame.Rates()[3]); /* 18 */
  TEST_EQ(0xb0, frame.Rates()[4]); /* 24(B) */
  TEST_EQ(0x48, frame.Rates()[5]); /* 36 */
  TEST_EQ(0x60, frame.Rates()[6]); /* 48 */
  TEST_EQ(0x6c, frame.Rates()[7]); /* 54 */
////  TEST_TRUE(frame.PowerCaps(20,30));
  TEST_TRUE(frame.ExtRates(17));
  TEST_TRUE(frame.ExtRates(18));

//  WmmWmeTag::wmm_wme wmmwme = frame.WmmWme.WmmWme();
//  TEST_EQ(0x01, wmmwme.wme_subtype);
//  TEST_EQ(0x01, wmmwme.wme_version);
//  TEST_EQ(0x80, wmmwme.wme_qos_info);
//  TEST_EQ(0x00, wmmwme.reserved);
//
//  TEST_EQ(0x03, wmmwme.aci0.aci_aifsn_field);
//  TEST_EQ(0xA4, wmmwme.aci0.ecw);
//  TEST_EQ(0x0000, wmmwme.aci0.txop_limit); // multi octet fields are little endian
//
//  TEST_EQ(0x27, wmmwme.aci1.aci_aifsn_field);
//  TEST_EQ(0xA4, wmmwme.aci1.ecw);
//  TEST_EQ(0x0000, wmmwme.aci1.txop_limit); //
//
//  TEST_EQ(0x42, wmmwme.aci2.aci_aifsn_field);
//  TEST_EQ(0x43, wmmwme.aci2.ecw);
//  TEST_EQ(0x005E, wmmwme.aci2.txop_limit); //
//
//  TEST_EQ(0x62, wmmwme.aci3.aci_aifsn_field);
//  TEST_EQ(0x32, wmmwme.aci3.ecw);
//  TEST_EQ(0x002f, wmmwme.aci3.txop_limit); //

  // Return success
  return (0);
}

