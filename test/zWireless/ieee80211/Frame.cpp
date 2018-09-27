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
  uint8_t frm_buf[32] = { 0 };
  uint8_t frm_type_mgmt[] = { 0x10, 0x11, 0x34, 0x12 };
  uint8_t frm_type_cntl[] = { 0x24, 0x22, 0x56, 0x34, 0x00 };
  uint8_t frm_type_data[] = { 0x38, 0x44, 0x78, 0x56, 0x00, 0x00 };
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

  // Assemble short frame and verify
  len = 2;
  TEST_IS_NULL(frame.Assemble(frm_buf, len));
  TEST_EQ(2, len);
  TEST_IS_ZERO(frm_buf[0]);
  TEST_IS_ZERO(frm_buf[1]);
  TEST_IS_ZERO(frm_buf[2]);
  TEST_IS_ZERO(frm_buf[3]);

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

  // Assemble and verify
  memset(frm_buf, 0, sizeof(frm_buf));
  len = sizeof(frm_buf);
  TEST_ISNOT_NULL(frame.Assemble(frm_buf, len));
  TEST_EQ((sizeof(frm_buf) - 4), len);
  for (int i = 0; i < sizeof(frm_type_mgmt); i++)
  {
    TEST_EQ_MSG((int)frm_type_mgmt[i], frm_buf[i], zLog::IntStr(i));
  }

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
  memset(frm_buf, 0, sizeof(frm_buf));
  len = sizeof(frm_buf);
  TEST_ISNOT_NULL(frame.Assemble(frm_buf, len));
  TEST_EQ((sizeof(frm_buf) - 4), len);
  for (int i = 0; i < sizeof(frm_type_cntl); i++)
  {
    TEST_EQ_MSG((int)frm_type_cntl[i], frm_buf[i], zLog::IntStr(i));
  }

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
  memset(frm_buf, 0, sizeof(frm_buf));
  len = sizeof(frm_buf);
  TEST_ISNOT_NULL(frame.Assemble(frm_buf, len));
  TEST_EQ((sizeof(frm_buf) - 4), len);
  for (int i = 0; i < sizeof(frm_type_data); i++)
  {
    TEST_EQ_MSG((int)frm_type_data[i], frm_buf[i], zLog::IntStr(i));
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
  len = sizeof(frm_short);
  TEST_IS_NULL(frame.Disassemble(frm_short, len));
  TEST_EQ(sizeof(frm_short), len);

  // Disassemble management frame
  len = sizeof(frm_type_mgmt);
  TEST_ISNOT_NULL(frame.Disassemble(frm_type_mgmt, len));
  TEST_IS_ZERO(len);

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
  len = sizeof(frm_type_cntl);
  TEST_ISNOT_NULL(frame.Disassemble(frm_type_cntl, len));
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
  len = sizeof(frm_type_data);
  TEST_ISNOT_NULL(frame.Disassemble(frm_type_data, len));
  TEST_EQ(0, len);

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
  len = sizeof(frm_type_err);
  TEST_IS_NULL(frame.Disassemble(frm_type_err, len));
  TEST_LT(len, sizeof(frm_type_err));

  // Return success
  return (0);
}

