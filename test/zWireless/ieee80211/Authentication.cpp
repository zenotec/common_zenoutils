/*
 * Copyright (c) 2017 Cable Television Laboratories, Inc. ("CableLabs")
 *                    and others.  All rights reserved.
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
#include "Authentication.h"
using namespace zWireless::ieee80211;


int
Ieee80211Test_AuthenticationGetSet(void* arg_)
{
  ZLOG_DEBUG("#############################################################");
  ZLOG_DEBUG("# Ieee80211Test_AuthenticationGetSet()");
  ZLOG_DEBUG("#############################################################");

  // Create frame and validate
  Authentication frame;
  TEST_IS_ZERO(frame.Version());
  TEST_EQ(Frame::TYPE_MGMT, frame.Type());
  TEST_EQ(Frame::SUBTYPE_AUTHENTICATE, frame.Subtype());
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
  TEST_IS_ZERO(frame.Algorithm());
  TEST_IS_ZERO(frame.SequenceNumber());
  TEST_IS_ZERO(frame.StatusCode());

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
  TEST_TRUE(frame.Algorithm(Authentication::TYPE_SHARED_KEY));
  TEST_TRUE(frame.SequenceNumber(23));
  TEST_TRUE(frame.StatusCode(10));

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
  TEST_EQ(uint16_t(Authentication::TYPE_SHARED_KEY), frame.Algorithm());
  TEST_EQ(23, frame.SequenceNumber());
  TEST_EQ(10, frame.StatusCode());

  // Return success
  return (0);
}


int
Ieee80211Test_AuthenticationAssemble(void* arg_)
{
  ZLOG_DEBUG("#############################################################");
  ZLOG_DEBUG("# Ieee80211Test_AuthenticationAssemble()");
  ZLOG_DEBUG("#############################################################");

  size_t len = 0;
  uint8_t frm_buf[64] = { 0 };
  uint8_t frm_assreq[] =
  {
      0xb0, 0x00, 0x34, 0x12, 0xff, 0xff, 0xff, 0xff,
      0xff, 0xff, 0x00, 0x11, 0x22, 0x33, 0x44, 0x55,
      0x00, 0x11, 0x22, 0x33, 0x44, 0x55, 0x23, 0x01,
      0x01, 0x00, 0x17, 0x00, 0x0a, 0x00
  };

  // Create frame and validate
  Authentication frame;
  TEST_IS_ZERO(frame.Version());
  TEST_EQ(Frame::TYPE_MGMT, frame.Type());
  TEST_EQ(Frame::SUBTYPE_AUTHENTICATE, frame.Subtype());
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
  TEST_IS_ZERO(frame.Algorithm());
  TEST_IS_ZERO(frame.SequenceNumber());
  TEST_IS_ZERO(frame.StatusCode());

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
  TEST_TRUE(frame.Subtype(Frame::SUBTYPE_AUTHENTICATE));
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
  TEST_TRUE(frame.Algorithm(Authentication::TYPE_SHARED_KEY));
  TEST_TRUE(frame.SequenceNumber(23));
  TEST_TRUE(frame.StatusCode(10));

  // Verify
  TEST_EQ(0, frame.Version());
  TEST_EQ(Frame::TYPE_MGMT, frame.Type());
  TEST_EQ(Frame::SUBTYPE_AUTHENTICATE, frame.Subtype());
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
  TEST_EQ(uint16_t(Authentication::TYPE_SHARED_KEY), frame.Algorithm());
  TEST_EQ(23, frame.SequenceNumber());
  TEST_EQ(10, frame.StatusCode());

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
Ieee80211Test_AuthenticationDisassemble(void* arg_)
{
  ZLOG_DEBUG("#############################################################");
  ZLOG_DEBUG("# Ieee80211Test_AuthenticationDisassemble()");
  ZLOG_DEBUG("#############################################################");

  // Make copy of raw association request frame
  uint8_t buf[4096] = { 0 };
  size_t buflen = auth_len;
  memcpy(buf, auth, auth_len);
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
  TEST_EQ(26, rtaphdr.GetLength());
  TEST_EQ(0x0000482F, rtaphdr.GetPresent(0)); // bit 31 not set, so this is the only 'present' word

  // Create frame and validate
  Authentication frame;
  TEST_IS_ZERO(frame.Version());
  TEST_EQ(Frame::TYPE_MGMT, frame.Type());
  TEST_EQ(Frame::SUBTYPE_AUTHENTICATE, frame.Subtype());
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
  TEST_IS_ZERO(frame.Algorithm());
  TEST_IS_ZERO(frame.SequenceNumber());
  TEST_IS_ZERO(frame.StatusCode());

  // Disassemble association request
  TEST_ISNOT_NULL(frame.Disassemble(p, buflen, false));

  // Verify
  TEST_IS_ZERO(frame.Version());
  TEST_EQ(Frame::TYPE_MGMT, frame.Type());
  TEST_EQ(Frame::SUBTYPE_AUTHENTICATE, frame.Subtype());
  TEST_FALSE(frame.ToDS());
  TEST_FALSE(frame.FromDS());
  TEST_FALSE(frame.MoreFragments());
  TEST_FALSE(frame.Retry());
  TEST_FALSE(frame.PowerManagement());
  TEST_FALSE(frame.MoreData());
  TEST_FALSE(frame.Protected());
  TEST_FALSE(frame.Order());
  TEST_EQ(0x013a, frame.DurationId());
  TEST_EQ(std::string("04:f0:21:37:ec:b2"), frame.ReceiverAddress());
  TEST_EQ(std::string("4c:66:41:52:22:a5"), frame.TransmitterAddress());
  TEST_EQ(std::string("04:f0:21:37:ec:b2"), frame.Bssid());
  TEST_EQ(int(Authentication::TYPE_OPEN_SYSTEM), frame.Algorithm());
  TEST_EQ(1, frame.SequenceNumber());
  TEST_EQ(0, frame.StatusCode());


  // Return success
  return (0);
}




