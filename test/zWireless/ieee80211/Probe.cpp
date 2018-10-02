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
using namespace Test;
#include <zutils/ieee80211/RadioTap.h>
#include <zutils/ieee80211/Probe.h>

using namespace zWireless::ieee80211;

const uint8_t probe_req_frame[] =
{
	0x40, 0x00, /* ......@. */
    0x00, 0x00, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, /* ........ */
    0x00, 0x11, 0x22, 0x33, 0x44, 0x55, 0xff, 0xff, /* ..;..... */
    0xff, 0xff, 0xff, 0xff, 0xd0, 0xba, 0x00, 0x00, /* ........ */
    0x01, 0x08, 0x02, 0x04, 0x0b, 0x0c, 0x12, 0x16, /* ........ */
    0x18, 0x24, 0x2d, 0x1a, 0x62, 0x01, 0x03, 0x00, /* .$-.b... */
    0x00, 0x00, 0x00, 0x01, 0x00, 0x00, 0x00, 0x00, /* ........ */
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, /* ........ */
    0x00, 0x08, 0xe0, 0xe1, 0x09, 0x00, 0x32, 0x04, /* ......2. */
    0x30, 0x48, 0x60, 0x6c, 0x7f, 0x09, 0x00, 0x00, /* 0H`l.... */
    0x00, 0x00, 0x01, 0x00, 0x00, 0x40, 0x00, 0xbf, /* .....@.. */
    0x0c, 0xb0, 0x71, 0xc0, 0x33, 0xf0, 0xff, 0x49, /* ..q.3..I */
    0x02, 0xf0, 0xff, 0x49, 0x02, 0xe4, 0x19, 0x58, /* ...I...X */
    0x68                                            /* h */
};
const size_t probe_req_frame_len = sizeof(probe_req_frame);

const uint8_t probe_resp_frame[] =
{
	0x50, 0x00, /* ......P. */
    0x3a, 0x01, 0x00, 0x11, 0x22, 0x33, 0x44, 0x55, /* :...;... */
    0x01, 0x12, 0x23, 0x34, 0x45, 0x56, 0x00, 0x11, /* ..C.9F.. */
    0x22, 0x33, 0x44, 0x55, 0xa0, 0x3f, 0x30, 0x98, /* C.9F.?0. */
    0x66, 0xc3, 0x88, 0x09, 0x00, 0x00, 0x64, 0x00, /* f.....d. */
    0x11, 0x14, 0x00, 0x08, 0x4c, 0x4c, 0x57, 0x69, /* ....LLWi */
    0x66, 0x69, 0x35, 0x30, 0x01, 0x08, 0x82, 0x84, /* fi50.... */
    0x0b, 0x16, 0x24, 0x30, 0x48, 0x6c, 0x03, 0x01, /* ..$0Hl.. */
    0x01, 0x2a, 0x01, 0x00, 0x32, 0x04, 0x0c, 0x12, /* .*..2... */
    0x18, 0x60, 0x30, 0x14, 0x01, 0x00, 0x00, 0x0f, /* .`0..... */
    0xac, 0x04, 0x01, 0x00, 0x00, 0x0f, 0xac, 0x04, /* ........ */
    0x01, 0x00, 0x00, 0x0f, 0xac, 0x02, 0x0c, 0x00, /* ........ */
    0x0b, 0x05, 0x00, 0x00, 0x2c, 0x00, 0x00, 0x46, /* ....,..F */
    0x05, 0x32, 0x08, 0x01, 0x00, 0x00, 0x2d, 0x1a, /* .2....-. */
    0xad, 0x09, 0x17, 0xff, 0xff, 0xff, 0x00, 0x00, /* ........ */
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, /* ........ */
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, /* ........ */
    0x00, 0x00, 0x3d, 0x16, 0x01, 0x08, 0x11, 0x00, /* ..=..... */
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, /* ........ */
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, /* ........ */
    0x00, 0x00, 0x4a, 0x0e, 0x14, 0x00, 0x0a, 0x00, /* ..J..... */
    0x2c, 0x01, 0xc8, 0x00, 0x14, 0x00, 0x05, 0x00, /* ,....... */
    0x19, 0x00, 0x7f, 0x08, 0x05, 0x00, 0x08, 0x00, /* ........ */
    0x00, 0x00, 0x00, 0x40, 0xdd, 0x06, 0x00, 0x14, /* ...@.... */
    0x6c, 0x00, 0x00, 0x00, 0xdd, 0x7e, 0x00, 0x50, /* l....~.P */
    0xf2, 0x04, 0x10, 0x4a, 0x00, 0x01, 0x10, 0x10, /* ...J.... */
    0x44, 0x00, 0x01, 0x02, 0x10, 0x3b, 0x00, 0x01, /* D....;.. */
    0x03, 0x10, 0x47, 0x00, 0x10, 0xfc, 0xff, 0x06, /* ..G..... */
    0xf4, 0xd2, 0xe3, 0x1c, 0x6e, 0x73, 0x72, 0xd7, /* ....nsr. */
    0xb3, 0x00, 0x6f, 0x08, 0xde, 0x10, 0x21, 0x00, /* ..o...!. */
    0x0d, 0x4e, 0x45, 0x54, 0x47, 0x45, 0x41, 0x52, /* .NETGEAR */
    0x2c, 0x20, 0x49, 0x6e, 0x63, 0x2e, 0x10, 0x23, /* , Inc..# */
    0x00, 0x05, 0x52, 0x37, 0x30, 0x30, 0x30, 0x10, /* ..R7000. */
    0x24, 0x00, 0x05, 0x52, 0x37, 0x30, 0x30, 0x30, /* $..R7000 */
    0x10, 0x42, 0x00, 0x02, 0x33, 0x32, 0x10, 0x54, /* .B..32.T */
    0x00, 0x08, 0x00, 0x06, 0x00, 0x50, 0xf2, 0x04, /* .....P.. */
    0x00, 0x01, 0x10, 0x11, 0x00, 0x09, 0x6d, 0x69, /* ......mi */
    0x63, 0x72, 0x6f, 0x6e, 0x65, 0x74, 0x31, 0x10, /* cronet1. */
    0x08, 0x00, 0x02, 0x20, 0x08, 0x10, 0x3c, 0x00, /* ... ..<. */
    0x01, 0x03, 0x10, 0x49, 0x00, 0x06, 0x00, 0x37, /* ...I...7 */
    0x2a, 0x00, 0x01, 0x20, 0xdd, 0x1e, 0x00, 0x90, /* *.. .... */
    0x4c, 0x04, 0x08, 0xbf, 0x0c, 0xb2, 0x59, 0x82, /* L.....Y. */
    0x0f, 0xea, 0xff, 0x00, 0x00, 0xea, 0xff, 0x00, /* ........ */
    0x00, 0xc0, 0x05, 0x00, 0x01, 0x00, 0x00, 0x00, /* ........ */
    0xc3, 0x02, 0x00, 0x02, 0xdd, 0x09, 0x00, 0x10, /* ........ */
    0x18, 0x02, 0x00, 0x00, 0x1c, 0x00, 0x00, 0xdd, /* ........ */
    0x18, 0x00, 0x50, 0xf2, 0x02, 0x01, 0x01, 0x88, /* ..P..... */
    0x00, 0x03, 0xa4, 0x00, 0x00, 0x27, 0xa4, 0x00, /* .....'.. */
    0x00, 0x42, 0x43, 0xbc, 0x00, 0x62, 0x32, 0x66, /* .BC..b2f */
    0x00, 0xb1, 0x37, 0xb0, 0xc2                    /* ..7.. */
};
const size_t probe_resp_frame_len = sizeof(probe_resp_frame);

int
Ieee80211Test_ProbeRequestGetSet(void* arg_)
{

  ZLOG_DEBUG("#############################################################");
  ZLOG_DEBUG("# Ieee80211Test_ProbeRequestGetSet()");
  ZLOG_DEBUG("#############################################################");

  // Create frame and validate
  ProbeRequest frame;
  TEST_IS_ZERO(frame.Version());
  TEST_EQ(Frame::TYPE_MGMT, frame.Type());
  TEST_EQ(Frame::SUBTYPE_PROBEREQ, frame.Subtype());
  TEST_FALSE(frame.ToDS());
  TEST_FALSE(frame.FromDS());
  TEST_FALSE(frame.MoreFragments());
  TEST_FALSE(frame.Retry());
  TEST_FALSE(frame.PowerManagement());
  TEST_FALSE(frame.MoreData());
  TEST_FALSE(frame.Protected());
  TEST_FALSE(frame.Order());
  TEST_IS_ZERO(frame.DurationId());
  TEST_EQ(std::string(""), frame.ReceiverAddress());
  TEST_EQ(std::string(""), frame.TransmitterAddress());
  TEST_EQ(std::string(""), frame.Bssid());
  TEST_IS_ZERO(frame.FragmentNum());
  TEST_IS_ZERO(frame.SequenceNum());
  TEST_EQ(std::string(""), frame.Ssid());
  TEST_IS_ZERO(frame.Rates().size());

  // Set non default values
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
  TEST_TRUE(frame.Ssid("TestSSID"));
  TEST_TRUE(frame.Rates(1));
  TEST_TRUE(frame.Rates(2));
  TEST_TRUE(frame.Rates(4));
  TEST_TRUE(frame.Rates(8));

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
  TEST_EQ(std::string("TestSSID"), frame.Ssid());
  TEST_EQ(4, frame.Rates().size());
  TEST_EQ(1, frame.Rates()[0]);
  TEST_EQ(2, frame.Rates()[1]);
  TEST_EQ(4, frame.Rates()[2]);
  TEST_EQ(8, frame.Rates()[3]);

  // Return success
  return (0);
}

int
Ieee80211Test_ProbeRequestAssemble(void* arg_)
{

  ZLOG_DEBUG("#############################################################");
  ZLOG_DEBUG("# Ieee80211Test_ProbeRequestAssemble()");
  ZLOG_DEBUG("#############################################################");

  size_t len = 0;
  uint8_t frm_buf[64] = { 0 };
  uint8_t frm_probe_req[] =
  {
      0x40, 0x00, 0x34, 0x12, 0xff, 0xff, 0xff, 0xff,
      0xff, 0xff, 0x00, 0x11, 0x22, 0x33, 0x44, 0x55,
      0x00, 0x11, 0x22, 0x33, 0x44, 0x55, 0x34, 0x12,
      0x00, 0x08, 0x54, 0x65, 0x73, 0x74, 0x53, 0x53,
      0x49, 0x44, 0x01, 0x04, 0x01, 0x02, 0x04, 0x08
  };


  // Create frame and validate
  ProbeRequest frame;
  TEST_IS_ZERO(frame.Version());
  TEST_EQ(Frame::TYPE_MGMT, frame.Type());
  TEST_EQ(Frame::SUBTYPE_PROBEREQ, frame.Subtype());
  TEST_FALSE(frame.ToDS());
  TEST_FALSE(frame.FromDS());
  TEST_FALSE(frame.MoreFragments());
  TEST_FALSE(frame.Retry());
  TEST_FALSE(frame.PowerManagement());
  TEST_FALSE(frame.MoreData());
  TEST_FALSE(frame.Protected());
  TEST_FALSE(frame.Order());
  TEST_IS_ZERO(frame.DurationId());
  TEST_EQ(std::string(""), frame.ReceiverAddress());
  TEST_EQ(std::string(""), frame.TransmitterAddress());
  TEST_EQ(std::string(""), frame.Bssid());
  TEST_IS_ZERO(frame.FragmentNum());
  TEST_IS_ZERO(frame.SequenceNum());
  TEST_EQ(std::string(""), frame.Ssid());
  TEST_IS_ZERO(frame.Rates().size());

  // Set values for management frame
  TEST_TRUE(frame.Version(0));
  TEST_TRUE(frame.Type(Frame::TYPE_MGMT));
  TEST_TRUE(frame.Subtype(Frame::SUBTYPE_PROBEREQ));
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
  TEST_TRUE(frame.Ssid("TestSSID"));
  TEST_TRUE(frame.Rates(1));
  TEST_TRUE(frame.Rates(2));
  TEST_TRUE(frame.Rates(4));
  TEST_TRUE(frame.Rates(8));

  // Verify
  TEST_EQ(0, frame.Version());
  TEST_EQ(Frame::TYPE_MGMT, frame.Type());
  TEST_EQ(Frame::SUBTYPE_PROBEREQ, frame.Subtype());
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
  TEST_EQ(std::string("TestSSID"), frame.Ssid());
  TEST_EQ(4, frame.Rates().size());
  TEST_EQ(1, frame.Rates()[0]);
  TEST_EQ(2, frame.Rates()[1]);
  TEST_EQ(4, frame.Rates()[2]);
  TEST_EQ(8, frame.Rates()[3]);

  zSocket::Buffer sb;

  // Assemble and verify
  TEST_TRUE(frame.Assemble(sb, false));

  TEST_EQ(sb.Size(), sizeof(frm_probe_req));

  uint8_t* p = sb.Head();
  for (int i = 0; i < sizeof(frm_probe_req); i++)
  {
    TEST_EQ_MSG((int)frm_probe_req[i], *p++, zLog::IntStr(i));
  }

  // Return success
  return (0);
}

int
Ieee80211Test_ProbeRequestDisassemble(void* arg_)
{

  ZLOG_DEBUG("#############################################################");
  ZLOG_DEBUG("# Ieee80211Test_ProbeRequestDisassemble()");
  ZLOG_DEBUG("#############################################################");

  // Make copy of raw Probe request frame
  zSocket::Buffer sb;
  sb.Write(probe_req_frame, probe_req_frame_len);
  sb.Push(probe_req_frame_len); // reset data ptr to start of buffer

  // Create frame and validate
  ProbeRequest frame;
  TEST_IS_ZERO(frame.Version());
  TEST_EQ(Frame::TYPE_MGMT, frame.Type());
  TEST_EQ(Frame::SUBTYPE_PROBEREQ, frame.Subtype());
  TEST_FALSE(frame.ToDS());
  TEST_FALSE(frame.FromDS());
  TEST_FALSE(frame.MoreFragments());
  TEST_FALSE(frame.Retry());
  TEST_FALSE(frame.PowerManagement());
  TEST_FALSE(frame.MoreData());
  TEST_FALSE(frame.Protected());
  TEST_FALSE(frame.Order());
  TEST_IS_ZERO(frame.DurationId());
  TEST_EQ(std::string(""), frame.ReceiverAddress());
  TEST_EQ(std::string(""), frame.TransmitterAddress());
  TEST_EQ(std::string(""), frame.Bssid());
  TEST_IS_ZERO(frame.FragmentNum());
  TEST_IS_ZERO(frame.SequenceNum());
  TEST_EQ(std::string(""), frame.Ssid());
  TEST_IS_ZERO(frame.Rates().size());

  // Disassemble beacon
  TEST_TRUE(frame.Disassemble(sb, false));

  // Verify
  TEST_IS_ZERO(frame.Version());
  TEST_EQ(Frame::TYPE_MGMT, frame.Type());
  TEST_EQ(Frame::SUBTYPE_PROBEREQ, frame.Subtype());
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
  TEST_EQ(std::string("ff:ff:ff:ff:ff:ff"), frame.Bssid());
  TEST_EQ(0x00, frame.FragmentNum());
  TEST_EQ(0x0bad, frame.SequenceNum());
  TEST_EQ(std::string(""), frame.Ssid());
  TEST_EQ(8, frame.Rates().size());
  TEST_EQ(0x02, frame.Rates()[0]); /* 1 */
  TEST_EQ(0x04, frame.Rates()[1]); /* 2 */
  TEST_EQ(0x0b, frame.Rates()[2]); /* 5.5 */
  TEST_EQ(0x0c, frame.Rates()[3]); /* 6 */
  TEST_EQ(0x12, frame.Rates()[4]); /* 9 */
  TEST_EQ(0x16, frame.Rates()[5]); /* 11 */
  TEST_EQ(0x18, frame.Rates()[6]); /* 12 */
  TEST_EQ(0x24, frame.Rates()[7]); /* 18 */

  // Return success
  return (0);
}


int
Ieee80211Test_ProbeResponseGetSet(void* arg_)
{

  ZLOG_DEBUG("#############################################################");
  ZLOG_DEBUG("# Ieee80211Test_ProbeResponseGetSet()");
  ZLOG_DEBUG("#############################################################");

  // Create frame and validate
  ProbeResponse frame;
  TEST_IS_ZERO(frame.Version());
  TEST_EQ(Frame::TYPE_MGMT, frame.Type());
  TEST_EQ(Frame::SUBTYPE_PROBERESP, frame.Subtype());
  TEST_FALSE(frame.ToDS());
  TEST_FALSE(frame.FromDS());
  TEST_FALSE(frame.MoreFragments());
  TEST_FALSE(frame.Retry());
  TEST_FALSE(frame.PowerManagement());
  TEST_FALSE(frame.MoreData());
  TEST_FALSE(frame.Protected());
  TEST_FALSE(frame.Order());
  TEST_IS_ZERO(frame.DurationId());
  TEST_EQ(std::string(""), frame.ReceiverAddress());
  TEST_EQ(std::string(""), frame.TransmitterAddress());
  TEST_EQ(std::string(""), frame.Bssid());
  TEST_IS_ZERO(frame.FragmentNum());
  TEST_IS_ZERO(frame.SequenceNum());
  TEST_IS_ZERO(frame.Timestamp());
  TEST_IS_ZERO(frame.Interval());
  TEST_IS_ZERO(frame.Capabilities());
  TEST_EQ(std::string(""), frame.Ssid());
  TEST_IS_ZERO(frame.Rates().size());

  // Set non default values
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

  // Return success
  return (0);
}

int
Ieee80211Test_ProbeResponseAssemble(void* arg_)
{

  ZLOG_DEBUG("#############################################################");
  ZLOG_DEBUG("# Ieee80211Test_ProbeResponseAssemble()");
  ZLOG_DEBUG("#############################################################");

  size_t len = 0;
  uint8_t frm_buf[64] = { 0 };
  uint8_t frm_probe_resp[] =
  {
      0x50, 0x00, 0x34, 0x12, 0xff, 0xff, 0xff, 0xff,
      0xff, 0xff, 0x00, 0x11, 0x22, 0x33, 0x44, 0x55,
      0x00, 0x11, 0x22, 0x33, 0x44, 0x55, 0x34, 0x12,
      0x77, 0x66, 0x55, 0x44, 0x33, 0x22, 0x11, 0x00,
      0x64, 0x00, 0x34, 0x12, 0x00, 0x08, 0x54, 0x65,
      0x73, 0x74, 0x53, 0x53, 0x49, 0x44, 0x01, 0x04,
      0x01, 0x02, 0x04, 0x08
  };

  // Create frame and validate
  ProbeResponse frame;
  TEST_IS_ZERO(frame.Version());
  TEST_EQ(Frame::TYPE_MGMT, frame.Type());
  TEST_EQ(Frame::SUBTYPE_PROBERESP, frame.Subtype());
  TEST_FALSE(frame.ToDS());
  TEST_FALSE(frame.FromDS());
  TEST_FALSE(frame.MoreFragments());
  TEST_FALSE(frame.Retry());
  TEST_FALSE(frame.PowerManagement());
  TEST_FALSE(frame.MoreData());
  TEST_FALSE(frame.Protected());
  TEST_FALSE(frame.Order());
  TEST_IS_ZERO(frame.DurationId());
  TEST_EQ(std::string(""), frame.ReceiverAddress());
  TEST_EQ(std::string(""), frame.TransmitterAddress());
  TEST_EQ(std::string(""), frame.Bssid());
  TEST_IS_ZERO(frame.FragmentNum());
  TEST_IS_ZERO(frame.SequenceNum());
  TEST_IS_ZERO(frame.Timestamp());
  TEST_IS_ZERO(frame.Interval());
  TEST_IS_ZERO(frame.Capabilities());
  TEST_EQ(std::string(""), frame.Ssid());
  TEST_IS_ZERO(frame.Rates().size());

  // Set values for management frame
  TEST_TRUE(frame.Version(0));
  TEST_TRUE(frame.Type(Frame::TYPE_MGMT));
  TEST_TRUE(frame.Subtype(Frame::SUBTYPE_PROBERESP));
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

  // Verify
  TEST_EQ(0, frame.Version());
  TEST_EQ(Frame::TYPE_MGMT, frame.Type());
  TEST_EQ(Frame::SUBTYPE_PROBERESP, frame.Subtype());
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

  zSocket::Buffer sb;

  // Assemble and verify
  TEST_TRUE(frame.Assemble(sb, false));

  TEST_EQ(sb.Size(), sizeof(frm_probe_resp));

  uint8_t* p = sb.Head();
  for (int i = 0; i < sizeof(frm_probe_resp); i++)
  {
    TEST_EQ_MSG((int)frm_probe_resp[i], *p++, zLog::IntStr(i));
  }

  // Return success
  return (0);
}

int
Ieee80211Test_ProbeResponseDisassemble(void* arg_)
{

  ZLOG_DEBUG("#############################################################");
  ZLOG_DEBUG("# Ieee80211Test_ProbeResponseDisassemble()");
  ZLOG_DEBUG("#############################################################");

  // Make copy of raw Probe response frame
  zSocket::Buffer sb;
  sb.Write(probe_resp_frame, probe_resp_frame_len);
  sb.Push(probe_resp_frame_len); // reset data ptr to start of buffer

  // Create frame and validate
  ProbeResponse frame;
  TEST_IS_ZERO(frame.Version());
  TEST_EQ(Frame::TYPE_MGMT, frame.Type());
  TEST_EQ(Frame::SUBTYPE_PROBERESP, frame.Subtype());
  TEST_FALSE(frame.ToDS());
  TEST_FALSE(frame.FromDS());
  TEST_FALSE(frame.MoreFragments());
  TEST_FALSE(frame.Retry());
  TEST_FALSE(frame.PowerManagement());
  TEST_FALSE(frame.MoreData());
  TEST_FALSE(frame.Protected());
  TEST_FALSE(frame.Order());
  TEST_IS_ZERO(frame.DurationId());
  TEST_EQ(std::string(""), frame.ReceiverAddress());
  TEST_EQ(std::string(""), frame.TransmitterAddress());
  TEST_EQ(std::string(""), frame.Bssid());
  TEST_IS_ZERO(frame.FragmentNum());
  TEST_IS_ZERO(frame.SequenceNum());
  TEST_IS_ZERO(frame.Timestamp());
  TEST_IS_ZERO(frame.Interval());
  TEST_IS_ZERO(frame.Capabilities());
  TEST_EQ(std::string(""), frame.Ssid());
  TEST_IS_ZERO(frame.Rates().size());

  // Disassemble beacon
  TEST_TRUE(frame.Disassemble(sb, false));

  // Verify
  TEST_IS_ZERO(frame.Version());
  TEST_EQ(Frame::TYPE_MGMT, frame.Type());
  TEST_EQ(Frame::SUBTYPE_PROBERESP, frame.Subtype());
  TEST_FALSE(frame.ToDS());
  TEST_FALSE(frame.FromDS());
  TEST_FALSE(frame.MoreFragments());
  TEST_FALSE(frame.Retry());
  TEST_FALSE(frame.PowerManagement());
  TEST_FALSE(frame.MoreData());
  TEST_FALSE(frame.Protected());
  TEST_FALSE(frame.Order());
  TEST_EQ(0x013a, frame.DurationId());
  TEST_EQ(std::string("00:11:22:33:44:55"), frame.ReceiverAddress());
  TEST_EQ(std::string("01:12:23:34:45:56"), frame.TransmitterAddress());
  TEST_EQ(std::string("00:11:22:33:44:55"), frame.Bssid());
  TEST_EQ(0x00, frame.FragmentNum());
  TEST_EQ(0x03fa, frame.SequenceNum());
  TEST_EQ((uint64_t)0x000988c3669830, frame.Timestamp());
  TEST_EQ(100, frame.Interval());
  TEST_EQ(0x1411, frame.Capabilities());
  TEST_EQ(std::string("LLWifi50"), frame.Ssid());
  TEST_EQ(8, frame.Rates().size());
  TEST_EQ(0x82, frame.Rates()[0]); /* 1(B) */
  TEST_EQ(0x84, frame.Rates()[1]); /* 2(B) */
  TEST_EQ(0x0b, frame.Rates()[2]); /* 5.5 */
  TEST_EQ(0x16, frame.Rates()[3]); /* 11 */
  TEST_EQ(0x24, frame.Rates()[4]); /* 18 */
  TEST_EQ(0x30, frame.Rates()[5]); /* 24 */
  TEST_EQ(0x48, frame.Rates()[6]); /* 36 */
  TEST_EQ(0x6c, frame.Rates()[7]); /* 54 */

  // Return success
  return (0);
}

