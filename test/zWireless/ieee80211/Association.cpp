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
#include <zutils/ieee80211/Association.h>

using namespace zWireless::ieee80211;

const uint8_t ass_req_frame[] =
{
    0x00, 0x00, /* ........ */
    0x3a, 0x01, 0x00, 0x11, 0x22, 0x33, 0x44, 0x55, /* :...!7.. */
    0x01, 0x12, 0x23, 0x34, 0x45, 0x56, 0x00, 0x11, /* LfAR"... */
    0x22, 0x33, 0x44, 0x55, 0x30, 0x77, 0x31, 0x04, /* !7..0w1. */
    0x0a, 0x00, 0x00, 0x04, 0x47, 0x57, 0x30, 0x32, /* ....GW02 */
    0x01, 0x08, 0x82, 0x84, 0x8b, 0x96, 0x24, 0x30, /* ......$0 */
    0x48, 0x6c, 0x32, 0x04, 0x0c, 0x12, 0x18, 0x60, /* Hl2....` */
    0x21, 0x02, 0x02, 0x14, 0x24, 0x02, 0x01, 0x0b, /* !...$... */
    0x30, 0x14, 0x01, 0x00, 0x00, 0x0f, 0xac, 0x04, /* 0....... */
    0x01, 0x00, 0x00, 0x0f, 0xac, 0x04, 0x01, 0x00, /* ........ */
    0x00, 0x0f, 0xac, 0x02, 0x80, 0x00, 0x2d, 0x1a, /* ......-. */
    0xad, 0x01, 0x17, 0xff, 0xff, 0x00, 0x00, 0x00, /* ........ */
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, /* ........ */
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, /* ........ */
    0x00, 0x00, 0xdd, 0x09, 0x00, 0x10, 0x18, 0x02, /* ........ */
    0x00, 0x00, 0x10, 0x00, 0x00, 0xdd, 0x07, 0x00, /* ........ */
    0x50, 0xf2, 0x02, 0x00, 0x01, 0x00, 0x00, 0x41, /* P......A */
    0x2d, 0x60                                      /* -` */
};
const size_t ass_req_frame_len = sizeof(ass_req_frame);

const uint8_t ass_req_frame_wpa2[] =
{
    0x00, 0x00, /* ........ */
    0x3a, 0x01, 0x00, 0x11, 0x22, 0x33, 0x44, 0x55, /* :...!7.. */
    0x01, 0x12, 0x23, 0x34, 0x45, 0x56, 0x00, 0x11, /* LfAR"... */
    0x22, 0x33, 0x44, 0x55, 0x30, 0x77, 0x31, 0x04, /* !7..0w1. */
    0x0a, 0x00, 0x00, 0x04, 0x47, 0x57, 0x30, 0x32, /* ....GW02 */
    0x01, 0x08, 0x82, 0x84, 0x8b, 0x96, 0x24, 0x30, /* ......$0 */
    0x48, 0x6c, 0x32, 0x04, 0x0c, 0x12, 0x18, 0x60, /* Hl2....` */
    0x21, 0x02, 0x02, 0x14, 0x24, 0x02, 0x01, 0x0b, /* !...$... */
    0x30, 0x14, 0x01, 0x00, 0x00, 0x0f, 0xac, 0x04, /* 0....... */
    0x01, 0x00, 0x00, 0x0f, 0xac, 0x04, 0x01, 0x00, /* ........ */
    0x00, 0x0f, 0xac, 0x02, 0x80, 0x00, 0x2d, 0x1a, /* ......-. */
    0xad, 0x01, 0x17, 0xff, 0xff, 0x00, 0x00, 0x00, /* ........ */
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, /* ........ */
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, /* ........ */
    0x00, 0x00, 0xdd, 0x09, 0x00, 0x10, 0x18, 0x02, /* ........ */
    0x00, 0x00, 0x10, 0x00, 0x00, 0xdd, 0x07, 0x00, /* ........ */
    0x50, 0xf2, 0x02, 0x00, 0x01, 0x00, 0x00, 0x41, /* P......A */
    0x2d, 0x60                                      /* -` */
};
const size_t ass_req_frame_len_wpa2 = sizeof(ass_req_frame_wpa2);

const struct rsn_element def_version = { .version = 1 };
const struct rsn_element ver_unknown = { .version = 0 };
const struct rsn_oui_format def_oui = { .oui_b1 = 0x00, .oui_b2 = 0x0F, .oui_b3 = 0xAC };
const struct rsn_suite def_group = { .cipher_oui = def_oui, .cipher_suite_type = 4 };
const struct rsn_suite def_unknown = { .cipher_oui = def_oui, .cipher_suite_type = 255 };
const struct rsn_suite def_pair = { .cipher_oui = def_oui, .cipher_suite_type = 4 };
const struct rsn_suite def_akm = { .cipher_oui = def_oui, .cipher_suite_type = 2 };
const struct rsn_suite_count def_count = { .suite_count = 1 };

const uint8_t ass_resp_frame[] =
{
	0x10, 0x00,
    0x3a, 0x01, 0x01, 0x02, 0x03, 0x04, 0x05, 0x06,
    0x11, 0x12, 0x13, 0x14, 0x15, 0x16, 0x21, 0x22,
    0x23, 0x24, 0x25, 0x26, 0x50, 0x53, 0x31, 0x04,
    0x00, 0x00, 0x01, 0xc0, 0x01, 0x08, 0x82, 0x84,
    0x8b, 0x96, 0x0c, 0x12, 0x18, 0x24, 0x32, 0x04,
    0x30, 0x48, 0x60, 0x6c, 0x2d, 0x1a, 0xed, 0x19,
    0x1b, 0xff, 0xff, 0xff, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x01, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x3d, 0x16, 0x01, 0x00, 0x15, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x7f, 0x08, 0x04, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x40, 0xdd, 0x18, 0x00, 0x50, 0xf2, 0x02,
    0x01, 0x01, 0x80, 0x00, 0x03, 0xa4, 0x00, 0x00,
    0x27, 0xa4, 0x00, 0x00, 0x42, 0x43, 0x5e, 0x00,
    0x62, 0x32, 0x2f, 0x00, 0x05, 0xab, 0x23, 0x71
};
const size_t ass_resp_frame_len = sizeof(ass_resp_frame);


int
Ieee80211Test_AssociationRequestGetSet(void* arg_)
{

  ZLOG_DEBUG("#############################################################");
  ZLOG_DEBUG("# Ieee80211Test_AssociationRequestGetSet()");
  ZLOG_DEBUG("#############################################################");

  // Create frame and validate
  AssociationRequest frame;
  TEST_IS_ZERO(frame.Version());
  TEST_EQ(Frame::TYPE_MGMT, frame.Type());
  TEST_EQ(Frame::SUBTYPE_ASSREQ, frame.Subtype());
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
  TEST_IS_ZERO(frame.Interval());
  TEST_IS_ZERO(frame.Capabilities());
  TEST_EQ(std::string(""), frame.Ssid());
  TEST_IS_ZERO(frame.Rates().size());
  TEST_IS_ZERO(frame.RsnElement.Length());

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
  TEST_TRUE(frame.Interval(100));
  TEST_TRUE(frame.Capabilities(0x1234));
  TEST_TRUE(frame.Ssid("TestSSID"));
  TEST_TRUE(frame.Rates(1));
  TEST_TRUE(frame.Rates(2));
  TEST_TRUE(frame.Rates(4));
  TEST_TRUE(frame.Rates(8));
  TEST_TRUE(frame.RsnElement(RsnTag::CCMP_128, {RsnTag::CCMP_128}, {RsnTag::PSK}));


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
  TEST_EQ(100, frame.Interval());
  TEST_EQ(0x1234, frame.Capabilities());
  TEST_EQ(std::string("TestSSID"), frame.Ssid());
  TEST_EQ(4, frame.Rates().size());
  TEST_EQ(1, frame.Rates()[0]);
  TEST_EQ(2, frame.Rates()[1]);
  TEST_EQ(4, frame.Rates()[2]);
  TEST_EQ(8, frame.Rates()[3]);

  TEST_EQ(18, frame.RsnElement.Length());
  TEST_EQ(RsnTag::VER_80211_2016, frame.RsnElement.GetVersion());
  TEST_EQ(RsnTag::CCMP_128, frame.RsnElement.GetGroupDataCipher());
  TEST_EQ(RsnTag::CCMP_128, frame.RsnElement.GetPairwiseCiphers()[0]);
  TEST_EQ(RsnTag::PSK, frame.RsnElement.GetAkmSuites()[0]);


  // Return success
  return (0);
}

int
Ieee80211Test_AssociationRequestAssemble(void* arg_)
{

  ZLOG_DEBUG("#############################################################");
  ZLOG_DEBUG("# Ieee80211Test_AssociationRequestAssemble()");
  ZLOG_DEBUG("#############################################################");

  size_t len = 0;
  uint8_t frm_buf[64] = { 0 };
  uint8_t frm_assreq[] =
  {
      0x00, 0x00, 0x34, 0x12, 0xff, 0xff, 0xff, 0xff,
      0xff, 0xff, 0x00, 0x11, 0x22, 0x33, 0x44, 0x55,
      0x00, 0x11, 0x22, 0x33, 0x44, 0x55, 0x34, 0x12,
      0x34, 0x12, 0x64, 0x00, 0x00, 0x08, 0x54, 0x65,
      0x73, 0x74, 0x53, 0x53, 0x49, 0x44, 0x01, 0x04,
      0x01, 0x02, 0x04, 0x08
  };

  // Create frame and validate
  AssociationRequest frame;
  TEST_IS_ZERO(frame.Version());
  TEST_EQ(Frame::TYPE_MGMT, frame.Type());
  TEST_EQ(Frame::SUBTYPE_ASSREQ, frame.Subtype());
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
  TEST_IS_ZERO(frame.Interval());
  TEST_IS_ZERO(frame.Capabilities());
  TEST_EQ(std::string(""), frame.Ssid());
  TEST_IS_ZERO(frame.Rates().size());

  // Set values for management frame
  TEST_TRUE(frame.Version(0));
  TEST_TRUE(frame.Type(Frame::TYPE_MGMT));
  TEST_TRUE(frame.Subtype(Frame::SUBTYPE_ASSREQ));
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
  TEST_EQ(Frame::SUBTYPE_ASSREQ, frame.Subtype());
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

  TEST_EQ(sb.Size(), sizeof(frm_assreq));

  uint8_t* p = sb.Head();
  for (int i = 0; i < sizeof(frm_assreq); i++)
  {
    TEST_EQ_MSG((int)frm_assreq[i], *p++, zLog::IntStr(i));
  }

  // Return success
  return (0);
}

int
Ieee80211Test_AssociationRequestAssembleWpa2(void* arg_)
{

  ZLOG_DEBUG("#############################################################");
  ZLOG_DEBUG("# Ieee80211Test_AssociationRequestAssemble()");
  ZLOG_DEBUG("#############################################################");

  size_t len = 0;
  uint8_t frm_buf[64] = { 0 };
  uint8_t frm_assreq[] =
  {
      0x00, 0x00, 0x34, 0x12, 0xff, 0xff, 0xff, 0xff,
      0xff, 0xff, 0x00, 0x11, 0x22, 0x33, 0x44, 0x55,
      0x00, 0x11, 0x22, 0x33, 0x44, 0x55, 0x34, 0x12,
      0x34, 0x12, 0x64, 0x00, 0x00, 0x08, 0x54, 0x65,
      0x73, 0x74, 0x53, 0x53, 0x49, 0x44, 0x01, 0x04,
      0x01, 0x02, 0x04, 0x08, 0x30, 0x12, 0x01, 0x00,
      0x00, 0x0f, 0xac, 0x04, 0x01, 0x00, 0x00, 0x0f,
      0xac, 0x04, 0x01, 0x00, 0x00, 0x0f, 0xac, 0x02,
  };

  // Create frame and validate
  AssociationRequest frame;
  TEST_IS_ZERO(frame.Version());
  TEST_EQ(Frame::TYPE_MGMT, frame.Type());
  TEST_EQ(Frame::SUBTYPE_ASSREQ, frame.Subtype());
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
  TEST_IS_ZERO(frame.Interval());
  TEST_IS_ZERO(frame.Capabilities());
  TEST_EQ(std::string(""), frame.Ssid());
  TEST_IS_ZERO(frame.Rates().size());
  TEST_IS_ZERO(frame.RsnElement.Length());


  // Set values for management frame
  TEST_TRUE(frame.Version(0));
  TEST_TRUE(frame.Type(Frame::TYPE_MGMT));
  TEST_TRUE(frame.Subtype(Frame::SUBTYPE_ASSREQ));
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
  TEST_TRUE(frame.Interval(100));
  TEST_TRUE(frame.Capabilities(0x1234));
  TEST_TRUE(frame.Ssid("TestSSID"));
  TEST_TRUE(frame.Rates(1));
  TEST_TRUE(frame.Rates(2));
  TEST_TRUE(frame.Rates(4));
  TEST_TRUE(frame.Rates(8));
  TEST_TRUE(frame.RsnElement(RsnTag::CCMP_128, {RsnTag::CCMP_128}, {RsnTag::PSK}));


  // Verify
  TEST_EQ(0, frame.Version());
  TEST_EQ(Frame::TYPE_MGMT, frame.Type());
  TEST_EQ(Frame::SUBTYPE_ASSREQ, frame.Subtype());
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
  TEST_EQ(100, frame.Interval());
  TEST_EQ(0x1234, frame.Capabilities());
  TEST_EQ(std::string("TestSSID"), frame.Ssid());
  TEST_EQ(4, frame.Rates().size());
  TEST_EQ(1, frame.Rates()[0]);
  TEST_EQ(2, frame.Rates()[1]);
  TEST_EQ(4, frame.Rates()[2]);
  TEST_EQ(8, frame.Rates()[3]);
  TEST_EQ(18, frame.RsnElement.Length());
  TEST_EQ(RsnTag::VER_80211_2016, frame.RsnElement.GetVersion());
  TEST_EQ(RsnTag::CCMP_128, frame.RsnElement.GetGroupDataCipher());
  TEST_EQ(RsnTag::CCMP_128, frame.RsnElement.GetPairwiseCiphers()[0]);
  TEST_EQ(RsnTag::PSK, frame.RsnElement.GetAkmSuites()[0]);


  zSocket::Buffer sb;

  // Assemble and verify
  TEST_TRUE(frame.Assemble(sb, false));

  TEST_EQ(sb.Size(), sizeof(frm_assreq));

  uint8_t* p = sb.Head();
  for (int i = 0; i < sizeof(frm_assreq); i++)
  {
    TEST_EQ_MSG((int)frm_assreq[i], *p++, zLog::IntStr(i));
  }

  // Return success
  return (0);
}

int
Ieee80211Test_AssociationRequestDisassemble(void* arg_)
{

  ZLOG_DEBUG("#############################################################");
  ZLOG_DEBUG("# Ieee80211Test_AssociationRequestDisassemble()");
  ZLOG_DEBUG("#############################################################");

  // Make copy of raw association request frame
  zSocket::Buffer sb;
  sb.Write(ass_req_frame, ass_req_frame_len);
  sb.Push(ass_req_frame_len); // reset data ptr to start of buffer

  // Create frame and validate
  AssociationRequest frame;
  TEST_IS_ZERO(frame.Version());
  TEST_EQ(Frame::TYPE_MGMT, frame.Type());
  TEST_EQ(Frame::SUBTYPE_ASSREQ, frame.Subtype());
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
  TEST_IS_ZERO(frame.Interval());
  TEST_IS_ZERO(frame.Capabilities());
  TEST_EQ(std::string(""), frame.Ssid());
  TEST_IS_ZERO(frame.Rates().size());

  // Disassemble association request
  TEST_TRUE(frame.Disassemble(sb, false));

  // Verify
  TEST_IS_ZERO(frame.Version());
  TEST_EQ(Frame::TYPE_MGMT, frame.Type());
  TEST_EQ(Frame::SUBTYPE_ASSREQ, frame.Subtype());
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
  TEST_EQ(0x0773, frame.SequenceNum());
  TEST_EQ(0x0431, frame.Capabilities());
  TEST_EQ(0x000a, frame.Interval());
  TEST_EQ(std::string("GW02"), frame.Ssid());
  TEST_EQ(8, frame.Rates().size());
  TEST_EQ(0x82, frame.Rates()[0]); /* 1(B) */
  TEST_EQ(0x84, frame.Rates()[1]); /* 2(B) */
  TEST_EQ(0x8b, frame.Rates()[2]); /* 5.5(B) */
  TEST_EQ(0x96, frame.Rates()[3]); /* 11(B) */
  TEST_EQ(0x24, frame.Rates()[4]); /* 18 */
  TEST_EQ(0x30, frame.Rates()[5]); /* 24 */
  TEST_EQ(0x48, frame.Rates()[6]); /* 36 */
  TEST_EQ(0x6c, frame.Rates()[7]); /* 54 */

  // Return success
  return (0);
}


int
Ieee80211Test_AssociationRequestDisassembleWpa2(void* arg_)
{

  ZLOG_DEBUG("#############################################################");
  ZLOG_DEBUG("# Ieee80211Test_AssociationRequestDisassemble()");
  ZLOG_DEBUG("#############################################################");

  RsnTag::rsn_default rsn_def = {
          .protocol = def_version,
          .group_data_cipher = def_group,
          .pairwise_count = def_count,
          .pairwise_cipher_suite = def_pair,
          .akm_count = def_count,
          .akm_suite_list = def_akm
  };

  RsnTag::rsn_default rsn_unknown = {
          .protocol = ver_unknown,
          .group_data_cipher = def_unknown,
          .pairwise_count = def_count,
          .pairwise_cipher_suite = def_unknown,
          .akm_count = def_count,
          .akm_suite_list = def_unknown
  };

  // Make copy of raw association request frame
  zSocket::Buffer sb;
  sb.Write(ass_req_frame_wpa2, ass_req_frame_len_wpa2);
  sb.Push(ass_req_frame_len); // reset data ptr to start of buffer

  // Create frame and validate
  AssociationRequest frame;
  RsnTag::rsn_default rsnTag_def = frame.RsnElement();

  TEST_IS_ZERO(frame.Version());
  TEST_EQ(Frame::TYPE_MGMT, frame.Type());
  TEST_EQ(Frame::SUBTYPE_ASSREQ, frame.Subtype());
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
  TEST_IS_ZERO(frame.Interval());
  TEST_IS_ZERO(frame.Capabilities());
  TEST_EQ(std::string(""), frame.Ssid());
  TEST_IS_ZERO(frame.Rates().size());

  // Disassemble association request
  TEST_TRUE(frame.Disassemble(sb, false));

  // Verify
  rsnTag_def = frame.RsnElement();
  TEST_IS_ZERO(frame.Version());
  TEST_EQ(Frame::TYPE_MGMT, frame.Type());
  TEST_EQ(Frame::SUBTYPE_ASSREQ, frame.Subtype());
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
  TEST_EQ(0x0773, frame.SequenceNum());
  TEST_EQ(0x0431, frame.Capabilities());
  TEST_EQ(0x000a, frame.Interval());
  TEST_EQ(std::string("GW02"), frame.Ssid());
  TEST_EQ(8, frame.Rates().size());
  TEST_EQ(0x82, frame.Rates()[0]); /* 1(B) */
  TEST_EQ(0x84, frame.Rates()[1]); /* 2(B) */
  TEST_EQ(0x8b, frame.Rates()[2]); /* 5.5(B) */
  TEST_EQ(0x96, frame.Rates()[3]); /* 11(B) */
  TEST_EQ(0x24, frame.Rates()[4]); /* 18 */
  TEST_EQ(0x30, frame.Rates()[5]); /* 24 */
  TEST_EQ(0x48, frame.Rates()[6]); /* 36 */
  TEST_EQ(0x6c, frame.Rates()[7]); /* 54 */
  TEST_EQ(20, frame.RsnElement.Length());
  TEST_EQ(RsnTag::VER_80211_2016, frame.RsnElement.GetVersion());
  TEST_EQ(RsnTag::CCMP_128, frame.RsnElement.GetGroupDataCipher());
  TEST_EQ(RsnTag::CCMP_128, frame.RsnElement.GetPairwiseCiphers()[0]);
  TEST_EQ(RsnTag::PSK, frame.RsnElement.GetAkmSuites()[0]);
  TEST_EQ(rsn_def.protocol.version, rsnTag_def.protocol.version);
  TEST_EQ(rsn_def.group_data_cipher.cipher_oui.oui_b1,
              rsnTag_def.group_data_cipher.cipher_oui.oui_b1);
  TEST_EQ(rsn_def.group_data_cipher.cipher_oui.oui_b2,
              rsnTag_def.group_data_cipher.cipher_oui.oui_b2);
  TEST_EQ(rsn_def.group_data_cipher.cipher_oui.oui_b3,
              rsnTag_def.group_data_cipher.cipher_oui.oui_b3);
  TEST_EQ(rsn_def.group_data_cipher.cipher_suite_type,
              rsnTag_def.group_data_cipher.cipher_suite_type);
  TEST_EQ(rsn_def.akm_count.suite_count, rsnTag_def.akm_count.suite_count);
  TEST_EQ(rsn_def.pairwise_count.suite_count, rsnTag_def.pairwise_count.suite_count);
  TEST_EQ(rsn_def.pairwise_cipher_suite.cipher_oui.oui_b1,
              rsnTag_def.pairwise_cipher_suite.cipher_oui.oui_b1);
  TEST_EQ(rsn_def.pairwise_cipher_suite.cipher_oui.oui_b2,
              rsnTag_def.pairwise_cipher_suite.cipher_oui.oui_b2);
  TEST_EQ(rsn_def.pairwise_cipher_suite.cipher_oui.oui_b3,
              rsnTag_def.pairwise_cipher_suite.cipher_oui.oui_b3);
  TEST_EQ(rsn_def.pairwise_cipher_suite.cipher_suite_type,
              rsnTag_def.pairwise_cipher_suite.cipher_suite_type);
  TEST_EQ(rsn_def.akm_suite_list.cipher_oui.oui_b1,
              rsnTag_def.akm_suite_list.cipher_oui.oui_b1);
  TEST_EQ(rsn_def.akm_suite_list.cipher_oui.oui_b2,
              rsnTag_def.akm_suite_list.cipher_oui.oui_b2);
  TEST_EQ(rsn_def.akm_suite_list.cipher_oui.oui_b3,
              rsnTag_def.akm_suite_list.cipher_oui.oui_b3);
  TEST_EQ(rsn_def.akm_suite_list.cipher_suite_type,
              rsnTag_def.akm_suite_list.cipher_suite_type);
  // Return success
  return (0);
}

int
Ieee80211Test_AssociationResponseGetSet(void* arg_)
{

  ZLOG_DEBUG("#############################################################");
  ZLOG_DEBUG("# Ieee80211Test_AssociationResponseGetSet()");
  ZLOG_DEBUG("#############################################################");

  // Create frame and validate
  AssociationResponse frame;
  TEST_IS_ZERO(frame.Version());
  TEST_EQ(Frame::TYPE_MGMT, frame.Type());
  TEST_EQ(Frame::SUBTYPE_ASSRESP, frame.Subtype());
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
  TEST_IS_ZERO(frame.Capabilities());
  TEST_IS_ZERO(frame.Status());
  TEST_IS_ZERO(frame.AssociationIdentifier());
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
  TEST_TRUE(frame.Capabilities(0x1234));
  TEST_TRUE(frame.Status(100));
  TEST_TRUE(frame.AssociationIdentifier(1));
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
  TEST_EQ(0x1234, frame.Capabilities());
  TEST_EQ(100, frame.Status());
  TEST_EQ(1, frame.AssociationIdentifier());
  TEST_EQ(4, frame.Rates().size());
  TEST_EQ(1, frame.Rates()[0]);
  TEST_EQ(2, frame.Rates()[1]);
  TEST_EQ(4, frame.Rates()[2]);
  TEST_EQ(8, frame.Rates()[3]);

  // Return success
  return (0);
}

int
Ieee80211Test_AssociationResponseAssemble(void* arg_)
{

  ZLOG_DEBUG("#############################################################");
  ZLOG_DEBUG("# Ieee80211Test_AssociationResponseAssemble()");
  ZLOG_DEBUG("#############################################################");

  size_t len = 0;
  uint8_t frm_buf[64] = { 0 };
  uint8_t frm_assresp[] =
  {
      0x10, 0x00, 0x34, 0x12, 0x01, 0x02, 0x03, 0x04,
      0x05, 0x06, 0x11, 0x12, 0x13, 0x14, 0x15, 0x16,
      0x21, 0x22, 0x23, 0x24, 0x25, 0x26, 0x34, 0x12,
      0x34, 0x12, 0x00, 0x00, 0x01, 0x00, 0x01, 0x04,
      0x01, 0x02, 0x04, 0x08, 0xdd, 0x04, 0x00, 0x50,
      0xf2, 0x02
  };

  // Create frame and validate
  AssociationResponse frame;
  TEST_IS_ZERO(frame.Version());
  TEST_EQ(Frame::TYPE_MGMT, frame.Type());
  TEST_EQ(Frame::SUBTYPE_ASSRESP, frame.Subtype());
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
  TEST_IS_ZERO(frame.Capabilities());
  TEST_IS_ZERO(frame.Status());
  TEST_IS_ZERO(frame.AssociationIdentifier());
  TEST_IS_ZERO(frame.Rates().size());

  // Set values for management frame
  TEST_TRUE(frame.Version(0));
  TEST_TRUE(frame.Type(Frame::TYPE_MGMT));
  TEST_TRUE(frame.Subtype(Frame::SUBTYPE_ASSRESP));
  TEST_TRUE(frame.ToDS(false));
  TEST_TRUE(frame.FromDS(false));
  TEST_TRUE(frame.MoreFragments(false));
  TEST_TRUE(frame.Retry(false));
  TEST_TRUE(frame.PowerManagement(false));
  TEST_TRUE(frame.MoreData(false));
  TEST_TRUE(frame.Protected(false));
  TEST_TRUE(frame.Order(false));
  TEST_TRUE(frame.DurationId(0x1234));
  TEST_TRUE(frame.ReceiverAddress("01:02:03:04:05:06"));
  TEST_TRUE(frame.TransmitterAddress("11:12:13:14:15:16"));
  TEST_TRUE(frame.Bssid("21:22:23:24:25:26"));
  TEST_TRUE(frame.FragmentNum(0x04));
  TEST_TRUE(frame.SequenceNum(0x0123));
  TEST_TRUE(frame.Capabilities(0x1234));
  TEST_TRUE(frame.Status(0));
  TEST_TRUE(frame.AssociationIdentifier(1));
  TEST_TRUE(frame.Rates(1));
  TEST_TRUE(frame.Rates(2));
  TEST_TRUE(frame.Rates(4));
  TEST_TRUE(frame.Rates(8));

  // Verify
  TEST_EQ(0, frame.Version());
  TEST_EQ(Frame::TYPE_MGMT, frame.Type());
  TEST_EQ(Frame::SUBTYPE_ASSRESP, frame.Subtype());
  TEST_FALSE(frame.ToDS());
  TEST_FALSE(frame.FromDS());
  TEST_FALSE(frame.MoreFragments());
  TEST_FALSE(frame.Retry());
  TEST_FALSE(frame.PowerManagement());
  TEST_FALSE(frame.MoreData());
  TEST_FALSE(frame.Protected());
  TEST_FALSE(frame.Order());
  TEST_EQ(0x1234, frame.DurationId());
  TEST_EQ(std::string("01:02:03:04:05:06"), frame.ReceiverAddress());
  TEST_EQ(std::string("11:12:13:14:15:16"), frame.TransmitterAddress());
  TEST_EQ(std::string("21:22:23:24:25:26"), frame.Bssid());
  TEST_EQ(0x04, frame.FragmentNum());
  TEST_EQ(0x0123, frame.SequenceNum());
  TEST_EQ(0x1234, frame.Capabilities());
  TEST_EQ(0, frame.Status());
  TEST_EQ(1, frame.AssociationIdentifier());
  TEST_EQ(4, frame.Rates().size());
  TEST_EQ(1, frame.Rates()[0]);
  TEST_EQ(2, frame.Rates()[1]);
  TEST_EQ(4, frame.Rates()[2]);
  TEST_EQ(8, frame.Rates()[3]);

  zSocket::Buffer sb;

  // Assemble and verify
  TEST_TRUE(frame.Assemble(sb, false));

  TEST_EQ(sb.Size(), sizeof(frm_assresp));

  uint8_t* p = sb.Head();
  for (int i = 0; i < sizeof(frm_assresp); i++)
  {
    TEST_EQ_MSG((int)frm_assresp[i], *p++, zLog::IntStr(i));
  }

  // Return success
  return (0);
}

int
Ieee80211Test_AssociationResponseDisassemble(void* arg_)
{

  ZLOG_DEBUG("#############################################################");
  ZLOG_DEBUG("# Ieee80211Test_AssociationResponseDisassemble()");
  ZLOG_DEBUG("#############################################################");

  // Make copy of raw AssociationResponse frame
  zSocket::Buffer sb;
  sb.Write(ass_resp_frame, ass_resp_frame_len);
  sb.Push(ass_resp_frame_len); // reset data ptr to start of buffer

  // Create frame and validate
  AssociationResponse frame;
  TEST_IS_ZERO(frame.Version());
  TEST_EQ(Frame::TYPE_MGMT, frame.Type());
  TEST_EQ(Frame::SUBTYPE_ASSRESP, frame.Subtype());
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
  TEST_IS_ZERO(frame.Capabilities());
  TEST_IS_ZERO(frame.Status());
  TEST_IS_ZERO(frame.AssociationIdentifier());
  TEST_IS_ZERO(frame.Rates().size());

  // Disassemble beacon
  TEST_TRUE(frame.Disassemble(sb, false));

  // Verify
  TEST_IS_ZERO(frame.Version());
  TEST_EQ(Frame::TYPE_MGMT, frame.Type());
  TEST_EQ(Frame::SUBTYPE_ASSRESP, frame.Subtype());
  TEST_FALSE(frame.ToDS());
  TEST_FALSE(frame.FromDS());
  TEST_FALSE(frame.MoreFragments());
  TEST_FALSE(frame.Retry());
  TEST_FALSE(frame.PowerManagement());
  TEST_FALSE(frame.MoreData());
  TEST_FALSE(frame.Protected());
  TEST_FALSE(frame.Order());
  TEST_EQ(0x013a, frame.DurationId());
  TEST_EQ(std::string("01:02:03:04:05:06"), frame.ReceiverAddress());
  TEST_EQ(std::string("11:12:13:14:15:16"), frame.TransmitterAddress());
  TEST_EQ(std::string("21:22:23:24:25:26"), frame.Bssid());
  TEST_EQ(0x00, frame.FragmentNum());
  TEST_EQ(0x0535, frame.SequenceNum());
  TEST_EQ(0x0431, frame.Capabilities());
  TEST_EQ(0, frame.Status());
  TEST_EQ(0xc001, frame.AssociationIdentifier());
  TEST_EQ(8, frame.Rates().size());
  TEST_EQ(0x82, frame.Rates()[0]); /* 1(B) */
  TEST_EQ(0x84, frame.Rates()[1]); /* 2(B) */
  TEST_EQ(0x8b, frame.Rates()[2]); /* 5.5(B) */
  TEST_EQ(0x96, frame.Rates()[3]); /* 11(B) */
  TEST_EQ(0x0c, frame.Rates()[4]); /* 6 */
  TEST_EQ(0x12, frame.Rates()[5]); /* 9 */
  TEST_EQ(0x18, frame.Rates()[6]); /* 12 */
  TEST_EQ(0x24, frame.Rates()[7]); /* 16 */

  // Return success
  return (0);
}

