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
#include <zutils/ieee80211/Frame.h>
using namespace zUtils;

#include "Ieee80211Test.h"
#include "UnitTest.h"

using namespace zWireless::ieee80211;

ZLOG_MODULE_INIT(zLog::Log::MODULE_TEST);

int
Ieee80211Test_FrameGetSet(void* arg_)
{

  ZLOG_DEBUG("#############################################################");
  ZLOG_DEBUG("# Ieee80211Test_FrameGetSet()");
  ZLOG_DEBUG("#############################################################");

  // Create frame
  Frame frame(Frame::TYPE_MGMT);
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

  // Return success
  return (0);
}

int
Ieee80211Test_FrameAssemble(void* arg_)
{

  ZLOG_DEBUG("#############################################################");
  ZLOG_DEBUG("# Ieee80211Test_FrameAssemble()");
  ZLOG_DEBUG("#############################################################");

  size_t len = 0;
  uint8_t frm_type_mgmt[] = { 0x10, 0x11, 0x34, 0x12, 0x11, 0x12, 0x13, 0x14, 0x15, 0x16, 0x21, 0x22, 0x23, 0x24, 0x25, 0x26 };
  uint8_t frm_type_cntl[] = { 0x24, 0x22, 0x56, 0x34, 0x11, 0x12, 0x13, 0x14, 0x15, 0x16, 0x21, 0x22, 0x23, 0x24, 0x25, 0x26 };
  uint8_t frm_type_data[] = { 0x38, 0x44, 0x78, 0x56, 0x11, 0x12, 0x13, 0x14, 0x15, 0x16, 0x21, 0x22, 0x23, 0x24, 0x25, 0x26 };
  uint8_t frm_type_err[]  = { 0x4c, 0x88, 0x90, 0x78, 0x00, 0x00, 0x00 };

  // Create frame and validate
  Frame frame(Frame::TYPE_NONE);
  TEST_IS_ZERO(frame.Version());
  TEST_EQ(Frame::TYPE_NONE, frame.Type());
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

  // ========== MANAGEMENT FRAME

  // Set values for management frame
  TEST_TRUE(frame.Version(0));
  TEST_TRUE(frame.Type(Frame::TYPE_MGMT));
  TEST_TRUE(frame.Subtype(Frame::SUBTYPE_ASSRESP));
  TEST_TRUE(frame.ToDS(true));
  TEST_TRUE(frame.FromDS(false));
  TEST_TRUE(frame.MoreFragments(false));
  TEST_TRUE(frame.Retry(false));
  TEST_TRUE(frame.PowerManagement(true));
  TEST_TRUE(frame.MoreData(false));
  TEST_TRUE(frame.Protected(false));
  TEST_TRUE(frame.Order(false));
  TEST_TRUE(frame.DurationId(0x1234));
  TEST_TRUE(frame.SetDestination("11:12:13:14:15:16"));
  TEST_TRUE(frame.SetSource("21:22:23:24:25:26"));

  // Verify
  TEST_EQ(0, frame.Version());
  TEST_EQ(Frame::TYPE_MGMT, frame.Type());
  TEST_EQ(Frame::SUBTYPE_ASSRESP, frame.Subtype());
  TEST_TRUE(frame.ToDS());
  TEST_FALSE(frame.FromDS());
  TEST_FALSE(frame.MoreFragments());
  TEST_FALSE(frame.Retry());
  TEST_TRUE(frame.PowerManagement());
  TEST_FALSE(frame.MoreData());
  TEST_FALSE(frame.Protected());
  TEST_FALSE(frame.Order());
  TEST_EQ(0x1234, frame.DurationId());
  TEST_EQ(std::string("11:12:13:14:15:16"), frame.GetDestination());
  TEST_EQ(std::string("21:22:23:24:25:26"), frame.GetSource());
  zSocket::Buffer sb;

  // Assemble and verify
  TEST_TRUE(frame.Assemble(sb, false));
  sb.Reset();
  for (int i = 0; i < sizeof(frm_type_mgmt); i++)
  {
    TEST_EQ_MSG((int)frm_type_mgmt[i], sb.Data()[i], zLog::IntStr(i));
  }

  // ========== CONTROL FRAME

  // Set values for control frame
  TEST_TRUE(frame.Version(0));
  TEST_TRUE(frame.Type(Frame::TYPE_CNTL));
  TEST_TRUE(frame.Subtype(Frame::SUBTYPE_RASSREQ));
  TEST_TRUE(frame.ToDS(false));
  TEST_TRUE(frame.FromDS(true));
  TEST_TRUE(frame.MoreFragments(false));
  TEST_TRUE(frame.Retry(false));
  TEST_TRUE(frame.PowerManagement(false));
  TEST_TRUE(frame.MoreData(true));
  TEST_TRUE(frame.Protected(false));
  TEST_TRUE(frame.Order(false));
  TEST_TRUE(frame.DurationId(0x3456));

  // Verify
  TEST_EQ(0, frame.Version());
  TEST_EQ(Frame::TYPE_CNTL, frame.Type());
  TEST_EQ(Frame::SUBTYPE_RASSREQ, frame.Subtype());
  TEST_FALSE(frame.ToDS());
  TEST_TRUE(frame.FromDS());
  TEST_FALSE(frame.MoreFragments());
  TEST_FALSE(frame.Retry());
  TEST_FALSE(frame.PowerManagement());
  TEST_TRUE(frame.MoreData());
  TEST_FALSE(frame.Protected());
  TEST_FALSE(frame.Order());
  TEST_EQ(0x3456, frame.DurationId());

  // Assemble and verify
  sb.Reset();
  TEST_TRUE(frame.Assemble(sb, false));
  sb.Reset();
  for (int i = 0; i < sizeof(frm_type_cntl); i++)
  {
    TEST_EQ_MSG((int)frm_type_cntl[i], sb.Data()[i], zLog::IntStr(i));
  }

  // ========== DATA FRAME

  // Set values for data frame
  TEST_TRUE(frame.Version(0));
  TEST_TRUE(frame.Type(Frame::TYPE_DATA));
  TEST_TRUE(frame.Subtype(Frame::SUBTYPE_RASSRESP));
  TEST_TRUE(frame.ToDS(false));
  TEST_TRUE(frame.FromDS(false));
  TEST_TRUE(frame.MoreFragments(true));
  TEST_TRUE(frame.Retry(false));
  TEST_TRUE(frame.PowerManagement(false));
  TEST_TRUE(frame.MoreData(false));
  TEST_TRUE(frame.Protected(true));
  TEST_TRUE(frame.Order(false));
  TEST_TRUE(frame.DurationId(0x5678));

  // Verify
  TEST_EQ(0, frame.Version());
  TEST_EQ(Frame::TYPE_DATA, frame.Type());
  TEST_EQ(Frame::SUBTYPE_RASSRESP, frame.Subtype());
  TEST_FALSE(frame.ToDS());
  TEST_FALSE(frame.FromDS());
  TEST_TRUE(frame.MoreFragments());
  TEST_FALSE(frame.Retry());
  TEST_FALSE(frame.PowerManagement());
  TEST_FALSE(frame.MoreData());
  TEST_TRUE(frame.Protected());
  TEST_FALSE(frame.Order());
  TEST_EQ(0x5678, frame.DurationId());

  // Assemble and verify
  sb.Reset();
  TEST_TRUE(frame.Assemble(sb, false));
  sb.Reset();
  for (int i = 0; i < sizeof(frm_type_data); i++)
  {
    TEST_EQ_MSG((int)frm_type_data[i], sb.Data()[i], zLog::IntStr(i));
  }

  // Return success
  return (0);
}

int
Ieee80211Test_FrameDisassemble(void* arg_)
{

  ZLOG_DEBUG("#############################################################");
  ZLOG_DEBUG("# Ieee80211Test_FrameDisassemble()");
  ZLOG_DEBUG("#############################################################");

  size_t len = 0;
  uint8_t frm_type_mgmt[] = { 0x10, 0x11, 0x34, 0x12, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff };
  uint8_t frm_type_cntl[] = { 0x24, 0x22, 0x56, 0x34, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff };
  uint8_t frm_type_data[] = { 0x38, 0x44, 0x78, 0x56, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff };
  uint8_t frm_type_err[]  = { 0x4f, 0x88, 0x90, 0x78, 0x00, 0x00, 0x00 };
  uint8_t frm_short[]     = { 0x00, 0x00 };

  // Create frame
  Frame frame(Frame::TYPE_NONE);
  TEST_IS_ZERO(frame.Version());
  TEST_EQ(Frame::TYPE_NONE, frame.Type());
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

  // Disassemble short frame
  zSocket::Buffer shortSb;
  shortSb.Write(frm_short, sizeof(frm_short));
  shortSb.Push(sizeof(frm_short));
  TEST_TRUE(frame.Disassemble(shortSb, shortSb.Length()));

  // Disassemble management frame
  zSocket::Buffer mgmtSb;
  mgmtSb.Write(frm_type_mgmt, sizeof(frm_type_mgmt));
  mgmtSb.Push(sizeof(frm_type_mgmt));
  TEST_TRUE(frame.Disassemble(mgmtSb, false));

  // Verify
  TEST_IS_ZERO(frame.Version());
  TEST_EQ(Frame::TYPE_MGMT, frame.Type());
  TEST_EQ(Frame::SUBTYPE_ASSRESP, frame.Subtype());
  TEST_TRUE(frame.ToDS());
  TEST_FALSE(frame.FromDS());
  TEST_FALSE(frame.MoreFragments());
  TEST_FALSE(frame.Retry());
  TEST_TRUE(frame.PowerManagement());
  TEST_FALSE(frame.MoreData());
  TEST_FALSE(frame.Protected());
  TEST_FALSE(frame.Order());
  TEST_EQ(0x1234, frame.DurationId());

  // Disassemble control frame
  zSocket::Buffer cntlSb;
  cntlSb.Write(frm_type_cntl, sizeof(frm_type_cntl));
  cntlSb.Push(sizeof(frm_type_cntl));
  TEST_TRUE(frame.Disassemble(cntlSb, false));
  TEST_EQ(0, len);

  // Verify
  TEST_IS_ZERO(frame.Version());
  TEST_EQ(Frame::TYPE_CNTL, frame.Type());
  TEST_EQ(Frame::SUBTYPE_RASSREQ, frame.Subtype());
  TEST_FALSE(frame.ToDS());
  TEST_TRUE(frame.FromDS());
  TEST_FALSE(frame.MoreFragments());
  TEST_FALSE(frame.Retry());
  TEST_FALSE(frame.PowerManagement());
  TEST_TRUE(frame.MoreData());
  TEST_FALSE(frame.Protected());
  TEST_FALSE(frame.Order());
  TEST_EQ(0x3456, frame.DurationId());

  // Disassemble data frame
  zSocket::Buffer dataSb;
  dataSb.Write(frm_type_data, sizeof(frm_type_data));
  dataSb.Push(sizeof(frm_type_data));
  TEST_TRUE(frame.Disassemble(dataSb, false));

  // Verify
  TEST_IS_ZERO(frame.Version());
  TEST_EQ(Frame::TYPE_DATA, frame.Type());
  TEST_EQ(Frame::SUBTYPE_RASSRESP, frame.Subtype());
  TEST_FALSE(frame.ToDS());
  TEST_FALSE(frame.FromDS());
  TEST_TRUE(frame.MoreFragments());
  TEST_FALSE(frame.Retry());
  TEST_FALSE(frame.PowerManagement());
  TEST_FALSE(frame.MoreData());
  TEST_TRUE(frame.Protected());
  TEST_FALSE(frame.Order());
  TEST_EQ(0x5678, frame.DurationId());

  // Disassemble unknown type frame
  zSocket::Buffer errSb;
  errSb.Write(frm_type_err, sizeof(frm_type_err));
  errSb.Push(sizeof(frm_type_err));
  TEST_FALSE(frame.Disassemble(errSb, false));

  // Return success
  return (0);
}

