/*
 * Copyright (c) 2018 ZenoTec LLC (http://www.zenotec.net)
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

#include <zutils/zSocket.h>
using namespace zSocket;

#include <zutils/ieee80211/RadioTap.h>
#include <zutils/ieee80211/ControlFrame.h>
using namespace zWireless;

#include "Ieee80211Test.h"
#include "UnitTest.h"

ZLOG_MODULE_INIT(zLog::Log::MODULE_TEST);

int
Ieee80211Test_ControlFrameGetSet(void* arg_)
{

  ZLOG_DEBUG("#############################################################");
  ZLOG_DEBUG("# Ieee80211Test_ControlFrameGetSet()");
  ZLOG_DEBUG("#############################################################");

  // Create frame and validate
  ieee80211::ControlFrame frame;
  TEST_IS_ZERO(frame.Version());
  TEST_EQ(ieee80211::Frame::TYPE_CNTL, frame.Type());
  TEST_EQ(ieee80211::Frame::SUBTYPE_NONE, frame.Subtype());
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

  // Set non default values
  TEST_TRUE(frame.Version(1));
  TEST_TRUE(frame.Type(ieee80211::Frame::TYPE_CNTL));
  TEST_TRUE(frame.Subtype(ieee80211::Frame::SUBTYPE_RTS));
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

  // Verify
  TEST_EQ(frame.Version(), 1);
  TEST_EQ(ieee80211::Frame::TYPE_CNTL, frame.Type());
  TEST_EQ(ieee80211::Frame::SUBTYPE_RTS, frame.Subtype());
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

  // Return success
  return (0);
}

int
Ieee80211Test_ControlFrameAssemble(void* arg_)
{

  ZLOG_DEBUG("#############################################################");
  ZLOG_DEBUG("# Ieee80211Test_ControlFrameAssemble()");
  ZLOG_DEBUG("#############################################################");

  uint8_t fbuf[] =
  {
      0xb4, 0x00, 0x74, 0x00, 0x88, 0x99, 0xaa, 0xbb,
      0xcc, 0xdd, 0x00, 0x11, 0x22, 0x33, 0x44, 0x55,
  };

  // Create frame and validate
  ieee80211::ControlFrame frame;
  TEST_IS_ZERO(frame.Version());
  TEST_EQ(ieee80211::Frame::TYPE_CNTL, frame.Type());
  TEST_EQ(ieee80211::Frame::SUBTYPE_NONE, frame.Subtype());
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

  // Create socket buffer to assemble to
  zSocket::Buffer sb;

  // Set values for Control frame
  TEST_TRUE(frame.Version(0));
  TEST_TRUE(frame.Type(ieee80211::Frame::TYPE_CNTL));
  TEST_TRUE(frame.Subtype(ieee80211::Frame::SUBTYPE_RTS));
  TEST_TRUE(frame.ToDS(false));
  TEST_TRUE(frame.FromDS(false));
  TEST_TRUE(frame.MoreFragments(false));
  TEST_TRUE(frame.Retry(false));
  TEST_TRUE(frame.PowerManagement(false));
  TEST_TRUE(frame.MoreData(false));
  TEST_TRUE(frame.Protected(false));
  TEST_TRUE(frame.Order(false));
  TEST_TRUE(frame.DurationId(0x0074));
  TEST_TRUE(frame.ReceiverAddress("88:99:aa:bb:cc:dd"));
  TEST_TRUE(frame.TransmitterAddress("00:11:22:33:44:55"));

  // Verify
  TEST_EQ(0, frame.Version());
  TEST_EQ(ieee80211::Frame::TYPE_CNTL, frame.Type());
  TEST_EQ(ieee80211::Frame::SUBTYPE_RTS, frame.Subtype());
  TEST_FALSE(frame.ToDS());
  TEST_FALSE(frame.FromDS());
  TEST_FALSE(frame.MoreFragments());
  TEST_FALSE(frame.Retry());
  TEST_FALSE(frame.PowerManagement());
  TEST_FALSE(frame.MoreData());
  TEST_FALSE(frame.Protected());
  TEST_FALSE(frame.Order());
  TEST_EQ(0x0074, frame.DurationId());
  TEST_EQ(std::string("88:99:aa:bb:cc:dd"), frame.ReceiverAddress());
  TEST_EQ(std::string("00:11:22:33:44:55"), frame.TransmitterAddress());

  // Assemble and verify
  TEST_TRUE(frame.Assemble(sb));
  TEST_EQ(sb.Size(), sizeof(fbuf));

  // Verify data matches expected
  uint8_t* p = sb.Head();
  for (int i = 0; i < sizeof(fbuf); i++)
  {
    TEST_EQ_MSG((int)fbuf[i], *p++, zLog::IntStr(i));
  }

  // Return success
  return (0);
}

int
Ieee80211Test_ControlFrameDisassemble(void* arg_)
{

  ZLOG_DEBUG("#############################################################");
  ZLOG_DEBUG("# Ieee80211Test_ControlFrameDisassemble()");
  ZLOG_DEBUG("#############################################################");

  // RTS frame
  uint8_t fbuf[] =
  {
      0xb4, 0x00, 0x74, 0x00, 0x88, 0x99, 0xaa, 0xbb,
      0xcc, 0xdd, 0x00, 0x11, 0x22, 0x33, 0x44, 0x55,
  };

  // Create frame and validate
  ieee80211::ControlFrame frame;
  TEST_IS_ZERO(frame.Version());
  TEST_EQ(ieee80211::Frame::TYPE_CNTL, frame.Type());
  TEST_EQ(ieee80211::Frame::SUBTYPE_NONE, frame.Subtype());
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

  // Create socket buffer to disassemble from
  zSocket::Buffer sb;
  memcpy(sb.Head(), fbuf, sizeof(fbuf));
  sb.Put(sizeof(fbuf));
  TEST_EQ(sizeof(fbuf), sb.Length());

  // Disassemble Control frame
  TEST_TRUE(frame.Disassemble(sb));
  TEST_IS_ZERO(sb.Length());

  // Verify
  TEST_IS_ZERO(frame.Version());
  TEST_EQ(ieee80211::Frame::TYPE_CNTL, frame.Type());
  TEST_EQ(ieee80211::Frame::SUBTYPE_RTS, frame.Subtype());
  TEST_FALSE(frame.ToDS());
  TEST_FALSE(frame.FromDS());
  TEST_FALSE(frame.MoreFragments());
  TEST_FALSE(frame.Retry());
  TEST_FALSE(frame.PowerManagement());
  TEST_FALSE(frame.MoreData());
  TEST_FALSE(frame.Protected());
  TEST_FALSE(frame.Order());
  TEST_EQ(0x0074, frame.DurationId());
  TEST_EQ(std::string("88:99:aa:bb:cc:dd"), frame.ReceiverAddress());
  TEST_EQ(std::string("00:11:22:33:44:55"), frame.TransmitterAddress());

  // Return success
  return (0);
}

