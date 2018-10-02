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
#include <zutils/ieee80211/ManagementFrame.h>

using namespace zWireless::ieee80211;


// Partial beacon frame used for validating assemble / disassemble of management frames
static uint8_t fbuf[] =
{
    0x80, 0x00, 0x34, 0x12, 0xff, 0xff, 0xff, 0xff,
    0xff, 0xff, 0x00, 0x11, 0x22, 0x33, 0x44, 0x55,
    0x00, 0x11, 0x22, 0x33, 0x44, 0x55, 0x34, 0x12
};

int
Ieee80211Test_ManagementFrameGetSet(void* arg_)
{

  ZLOG_DEBUG("#############################################################");
  ZLOG_DEBUG("# Ieee80211Test_ManagementFrameGetSet()");
  ZLOG_DEBUG("#############################################################");

  // Create frame and validate
  ManagementFrame frame;
  TEST_IS_ZERO(frame.Version());
  TEST_EQ(Frame::TYPE_MGMT, frame.Type());
  TEST_EQ(Frame::SUBTYPE_NONE, frame.Subtype());
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

  // Return success
  return (UTEST_PASS);

}

int
Ieee80211Test_ManagementFrameAssemble(void* arg_)
{

  ZLOG_DEBUG("#############################################################");
  ZLOG_DEBUG("# Ieee80211Test_ManagementFrameAssemble()");
  ZLOG_DEBUG("#############################################################");

  // Create frame and validate
  ManagementFrame frame;
  TEST_IS_ZERO(frame.Version());
  TEST_EQ(Frame::TYPE_MGMT, frame.Type());
  TEST_EQ(Frame::SUBTYPE_NONE, frame.Subtype());
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

  // Create socket buffer to assemble to
  zSocket::Buffer sb;

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

  // Assemble and verify
  TEST_TRUE(frame.Assemble(sb, false));
  TEST_EQ(sb.Size(), sizeof(fbuf));

  // Verify data matches expected
  uint8_t* p = sb.Head();
  for (int i = 0; i < sizeof(fbuf); i++)
  {
    TEST_EQ_MSG((int)fbuf[i], *p++, zLog::IntStr(i));
  }

  // Return success
  return (UTEST_PASS);

}

int
Ieee80211Test_ManagementFrameDisassemble(void* arg_)
{

  ZLOG_DEBUG("#############################################################");
  ZLOG_DEBUG("# Ieee80211Test_ManagementFrameDisassemble()");
  ZLOG_DEBUG("#############################################################");

  // Create frame and validate
  ManagementFrame frame;
  TEST_IS_ZERO(frame.Version());
  TEST_EQ(Frame::TYPE_MGMT, frame.Type());
  TEST_EQ(Frame::SUBTYPE_NONE, frame.Subtype());
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

  // Create socket buffer to disassemble from
  zSocket::Buffer sb;
  memcpy(sb.Head(), fbuf, sizeof(fbuf));
  sb.Put(sizeof(fbuf));
  TEST_EQ(sizeof(fbuf), sb.Length());

  // Disassemble Data frame
  TEST_TRUE(frame.Disassemble(sb, false));
  TEST_IS_ZERO(sb.Length());

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
  TEST_EQ(0x1234, frame.DurationId());
  TEST_EQ(std::string("ff:ff:ff:ff:ff:ff"), frame.ReceiverAddress());
  TEST_EQ(std::string("00:11:22:33:44:55"), frame.TransmitterAddress());
  TEST_EQ(std::string("00:11:22:33:44:55"), frame.Bssid());
  TEST_EQ(0x04, frame.FragmentNum());
  TEST_EQ(0x0123, frame.SequenceNum());

  // Return success
  return (UTEST_PASS);

}

