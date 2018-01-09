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

#include "Ieee80211Test.h"
#include "UnitTest.h"

#include "RadioTap.h"
#include "Association.h"
using namespace zWireless::ieee80211;

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
  TEST_IS_ZERO(frame.SequenceControl());
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
  TEST_TRUE(frame.SequenceControl(0x0123));
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
  TEST_EQ(0x0123, frame.SequenceControl());
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
      0x00, 0x11, 0x22, 0x33, 0x44, 0x55, 0x23, 0x01,
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
  TEST_IS_ZERO(frame.SequenceControl());
  TEST_IS_ZERO(frame.Interval());
  TEST_IS_ZERO(frame.Capabilities());
  TEST_EQ(std::string(""), frame.Ssid());
  TEST_IS_ZERO(frame.Rates().size());

  // Assemble short frame and verify
  len = 2;
  TEST_IS_NULL(frame.Assemble(frm_buf, len, true));
  TEST_EQ(2, len);
  TEST_IS_ZERO(frm_buf[0]);
  TEST_IS_ZERO(frm_buf[1]);
  TEST_IS_ZERO(frm_buf[2]);
  TEST_IS_ZERO(frm_buf[3]);

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
  TEST_TRUE(frame.SequenceControl(0x0123));
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
  TEST_EQ(0x0123, frame.SequenceControl());
  TEST_EQ(100, frame.Interval());
  TEST_EQ(0x1234, frame.Capabilities());
  TEST_EQ(std::string("TestSSID"), frame.Ssid());
  TEST_EQ(4, frame.Rates().size());
  TEST_EQ(1, frame.Rates()[0]);
  TEST_EQ(2, frame.Rates()[1]);
  TEST_EQ(4, frame.Rates()[2]);
  TEST_EQ(8, frame.Rates()[3]);

  // Assemble and verify
  memset(frm_buf, 0, sizeof(frm_buf));
  len = sizeof(frm_buf);
  TEST_ISNOT_NULL(frame.Assemble(frm_buf, len));
  for (int i = 0; i < sizeof(frm_assreq); i++)
  {
    TEST_EQ_MSG((int)frm_assreq[i], frm_buf[i], zLog::IntStr(i));
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
  uint8_t buf[4096] = { 0 };
  size_t buflen = ass_req_len;
  memcpy(buf, ass_req, ass_req_len);
  uint8_t* p = buf;

  // Create radiotap header and validate
  RadioTap rtaphdr;
  TEST_EQ(0, rtaphdr.GetVersion());
  TEST_EQ(0, rtaphdr.GetLength());
  TEST_EQ(0, rtaphdr.GetPresent());

  // Disassemble and verify
  p = rtaphdr.Disassemble(p, buflen);
  TEST_ISNOT_NULL(p);
  TEST_EQ(0, rtaphdr.GetVersion());
  TEST_EQ(38, rtaphdr.GetLength());
  TEST_EQ(0xA000402F, rtaphdr.GetPresent(0));
  TEST_EQ(0xA0000820, rtaphdr.GetPresent(1));
  TEST_EQ(0x00000820, rtaphdr.GetPresent(2));

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
  TEST_IS_ZERO(frame.SequenceControl());
  TEST_IS_ZERO(frame.Interval());
  TEST_IS_ZERO(frame.Capabilities());
  TEST_EQ(std::string(""), frame.Ssid());
  TEST_IS_ZERO(frame.Rates().size());

  // Disassemble association request
  TEST_ISNOT_NULL(frame.Disassemble(p, buflen, true));

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
  TEST_EQ(0x7730, frame.SequenceControl());
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
  TEST_IS_ZERO(frame.SequenceControl());
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
  TEST_TRUE(frame.SequenceControl(0x0123));
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
  TEST_EQ(0x0123, frame.SequenceControl());
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
      0x21, 0x22, 0x23, 0x24, 0x25, 0x26, 0x23, 0x01,
      0x34, 0x12, 0x00, 0x00, 0x01, 0x00, 0x01, 0x04,
      0x01, 0x02, 0x04, 0x08
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
  TEST_IS_ZERO(frame.SequenceControl());
  TEST_IS_ZERO(frame.Capabilities());
  TEST_IS_ZERO(frame.Status());
  TEST_IS_ZERO(frame.AssociationIdentifier());
  TEST_IS_ZERO(frame.Rates().size());

  // Assemble short frame and verify
  len = 2;
  TEST_IS_NULL(frame.Assemble(frm_buf, len, true));
  TEST_EQ(2, len);
  TEST_IS_ZERO(frm_buf[0]);
  TEST_IS_ZERO(frm_buf[1]);
  TEST_IS_ZERO(frm_buf[2]);
  TEST_IS_ZERO(frm_buf[3]);

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
  TEST_TRUE(frame.SequenceControl(0x0123));
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
  TEST_EQ(0x0123, frame.SequenceControl());
  TEST_EQ(0x1234, frame.Capabilities());
  TEST_EQ(0, frame.Status());
  TEST_EQ(1, frame.AssociationIdentifier());
  TEST_EQ(4, frame.Rates().size());
  TEST_EQ(1, frame.Rates()[0]);
  TEST_EQ(2, frame.Rates()[1]);
  TEST_EQ(4, frame.Rates()[2]);
  TEST_EQ(8, frame.Rates()[3]);

  // Assemble and verify
  memset(frm_buf, 0, sizeof(frm_buf));
  len = sizeof(frm_buf);
  TEST_ISNOT_NULL(frame.Assemble(frm_buf, len));
  for (int i = 0; i < sizeof(frm_assresp); i++)
  {
    TEST_EQ_MSG((int)frm_assresp[i], frm_buf[i], zLog::IntStr(i));
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

  // Make copy of raw beacon frame
  uint8_t buf[4096] = { 0 };
  size_t buflen = ass_resp_len;
  memcpy(buf, ass_resp, ass_resp_len);
  uint8_t* p = buf;

  // Create radiotap header and validate
  RadioTap rtaphdr;
  TEST_EQ(0, rtaphdr.GetVersion());
  TEST_EQ(0, rtaphdr.GetLength());
  TEST_EQ(0, rtaphdr.GetPresent());

  // Disassemble and verify
  p = rtaphdr.Disassemble(p, buflen);
  TEST_ISNOT_NULL(p);
  TEST_EQ(0, rtaphdr.GetVersion());
  TEST_EQ(38, rtaphdr.GetLength());
  TEST_EQ(0xA000402F, rtaphdr.GetPresent(0));
  TEST_EQ(0xA0000820, rtaphdr.GetPresent(1));
  TEST_EQ(0x00000820, rtaphdr.GetPresent(2));

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
  TEST_IS_ZERO(frame.SequenceControl());
  TEST_IS_ZERO(frame.Capabilities());
  TEST_IS_ZERO(frame.Status());
  TEST_IS_ZERO(frame.AssociationIdentifier());
  TEST_IS_ZERO(frame.Rates().size());

  // Disassemble beacon
  TEST_ISNOT_NULL(frame.Disassemble(p, buflen, true));

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
  TEST_EQ(0x5350, frame.SequenceControl());
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

