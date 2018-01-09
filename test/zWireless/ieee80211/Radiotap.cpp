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
using namespace zWireless::ieee80211;

int
Ieee80211Test_RadiotapGetSet(void* arg_)
{

  ZLOG_DEBUG("#############################################################");
  ZLOG_DEBUG("# Ieee80211Test_RadiotapGetSet()");
  ZLOG_DEBUG("#############################################################");

  RadioTapFieldTsft tsft;
  RadioTapFieldFlags flags;
  RadioTapFieldRate rate;
  RadioTapFieldChannel channel;
  RadioTapFieldSignalLevel level;
  RadioTapFieldSignalNoise noise;
  RadioTapFieldAntenna antenna;
  RadioTapFieldRxFlags rxflags;
  RadioTapFieldTxFlags txflags;

  // Create radiotap header and validate
  RadioTap rtaphdr;
  TEST_EQ(0, rtaphdr.GetVersion());
  TEST_EQ(0, rtaphdr.GetLength());
  TEST_EQ(0, rtaphdr.GetPresent());
  TEST_FALSE(rtaphdr.GetField(tsft));
  TEST_EQ(0, tsft());
  TEST_FALSE(rtaphdr.GetField(flags));
  TEST_EQ(0, flags());
  TEST_FALSE(flags.FCS());
  TEST_FALSE(rtaphdr.GetField(rate));
  TEST_EQ(0, rate());
  TEST_FALSE(rtaphdr.GetField(channel));
  TEST_EQ(0, channel());
  TEST_FALSE(rtaphdr.GetField(level));
  TEST_EQ(0, level());
  TEST_FALSE(rtaphdr.GetField(noise));
  TEST_EQ(0, noise());
  TEST_FALSE(rtaphdr.GetField(antenna));
  TEST_EQ(0, antenna());
  TEST_FALSE(rtaphdr.GetField(rxflags));
  TEST_EQ(0, rxflags());
  TEST_FALSE(rtaphdr.GetField(txflags));
  TEST_EQ(0, txflags());

  // Return success
  return (0);
}

int
Ieee80211Test_RadiotapAssemble(void* arg_)
{

  ZLOG_DEBUG("#############################################################");
  ZLOG_DEBUG("# Ieee80211Test_RadiotapAssemble()");
  ZLOG_DEBUG("#############################################################");

  RadioTapFieldTsft tsft;
  RadioTapFieldFlags flags;
  RadioTapFieldRate rate;
  RadioTapFieldChannel channel;
  RadioTapFieldSignalLevel level[3];
  RadioTapFieldSignalNoise noise;
  RadioTapFieldAntenna antenna[3];
  RadioTapFieldRxFlags rxflags;
  RadioTapFieldTxFlags txflags;

  // Create radiotap header and validate
  RadioTap rtaphdr;
  TEST_EQ(0, rtaphdr.GetVersion());
  TEST_EQ(0, rtaphdr.GetLength());
  TEST_EQ(0, rtaphdr.GetPresent());
  TEST_FALSE(rtaphdr.GetField(tsft));
  TEST_EQ(0, tsft());
  TEST_FALSE(rtaphdr.GetField(flags));
  TEST_EQ(0, flags());
  TEST_FALSE(flags.FCS());
  TEST_FALSE(rtaphdr.GetField(rate));
  TEST_EQ(0, rate());
  TEST_FALSE(rtaphdr.GetField(channel));
  TEST_EQ(0, channel());
  TEST_FALSE(rtaphdr.GetField(level[0], 0));
  TEST_EQ(0, level[0]());
  TEST_FALSE(rtaphdr.GetField(level[1], 1));
  TEST_EQ(0, level[1]());
  TEST_FALSE(rtaphdr.GetField(level[2], 2));
  TEST_EQ(0, level[2]());
  TEST_FALSE(rtaphdr.GetField(noise));
  TEST_EQ(0, noise());
  TEST_FALSE(rtaphdr.GetField(antenna[0], 0));
  TEST_EQ(0, antenna[0]());
  TEST_FALSE(rtaphdr.GetField(antenna[1], 1));
  TEST_EQ(0, antenna[1]());
  TEST_FALSE(rtaphdr.GetField(antenna[2], 2));
  TEST_EQ(0, antenna[2]());
  TEST_FALSE(rtaphdr.GetField(rxflags));
  TEST_EQ(0, rxflags());
  TEST_FALSE(rtaphdr.GetField(txflags));
  TEST_EQ(0, txflags());

  // Set fields
  TEST_TRUE(tsft(0x31966655));
  TEST_EQ(0x31966655, tsft());
  TEST_TRUE(rtaphdr.PutField(tsft));

  TEST_TRUE(flags(0x10));
  TEST_EQ(0x10, flags());
  TEST_TRUE(flags.FCS());
  TEST_TRUE(rtaphdr.PutField(flags));

  TEST_TRUE(rate(0x0c));
  TEST_EQ(0x0c, rate());
  TEST_EQ(6000000, rate.RateBps());
  TEST_EQ(6000, rate.RateKbps());
  TEST_EQ(6, rate.RateMbps());
  TEST_TRUE(rtaphdr.PutField(rate));

  TEST_TRUE(channel(0x0140143c));
  TEST_EQ(0x0140143c, channel());
  TEST_EQ(5180, channel.Frequency());
  TEST_EQ(36, channel.Channel());
  TEST_EQ(0x0140, channel.Flags());
  TEST_TRUE(rtaphdr.PutField(channel));

  TEST_TRUE(level[0](-30));
  TEST_EQ(-30, level[0]());
  TEST_TRUE(rtaphdr.PutField(level[0], 0));

  TEST_TRUE(level[1](-30));
  TEST_EQ(-30, level[1]());
  TEST_TRUE(rtaphdr.PutField(level[1], 1));

  TEST_TRUE(level[2](-63));
  TEST_EQ(-63, level[2]());
  TEST_TRUE(rtaphdr.PutField(level[2], 2));

  TEST_TRUE(antenna[1](0));
  TEST_EQ(0, antenna[1]());
  TEST_TRUE(rtaphdr.PutField(antenna[1], 1));

  TEST_TRUE(antenna[2](1));
  TEST_EQ(1, antenna[2]());
  TEST_TRUE(rtaphdr.PutField(antenna[2], 2));

  TEST_TRUE(rxflags(0));
  TEST_EQ(0, rxflags());
  TEST_TRUE(rtaphdr.PutField(rxflags));

//  rtaphdr.Display();

  // Assemble and verify
  uint8_t buf[1024] = { 0 };
  size_t buflen = sizeof(buf);
  uint8_t* p = rtaphdr.Assemble(buf, buflen);
  TEST_ISNOT_NULL(p);
  TEST_EQ(radiotap_len, (sizeof(buf) - buflen));
  TEST_EQ(0, rtaphdr.GetVersion());
  TEST_EQ(38, rtaphdr.GetLength());
  TEST_EQ(0xA000402F, rtaphdr.GetPresent(0));
  TEST_EQ(0xA0000820, rtaphdr.GetPresent(1));
  TEST_EQ(0x00000820, rtaphdr.GetPresent(2));
  TEST_IS_ZERO(memcmp(buf, radiotap, radiotap_len));

  // Return success
  return (0);
}

int
Ieee80211Test_RadiotapDisassemble(void* arg_)
{

  ZLOG_DEBUG("#############################################################");
  ZLOG_DEBUG("# Ieee80211Test_RadiotapDisassemble()");
  ZLOG_DEBUG("#############################################################");

  RadioTapFieldTsft tsft;
  RadioTapFieldFlags flags;
  RadioTapFieldRate rate;
  RadioTapFieldChannel channel;
  RadioTapFieldSignalLevel level[3];
  RadioTapFieldSignalNoise noise;
  RadioTapFieldAntenna antenna[3];
  RadioTapFieldRxFlags rxflags;
  RadioTapFieldTxFlags txflags;

  // Create radiotap header and validate
  RadioTap rtaphdr;
  TEST_EQ(0, rtaphdr.GetVersion());
  TEST_EQ(0, rtaphdr.GetLength());
  TEST_EQ(0, rtaphdr.GetPresent());
  TEST_FALSE(rtaphdr.GetField(tsft));
  TEST_EQ(0, tsft());
  TEST_FALSE(rtaphdr.GetField(flags));
  TEST_EQ(0, flags());
  TEST_FALSE(flags.FCS());
  TEST_FALSE(rtaphdr.GetField(rate));
  TEST_EQ(0, rate());
  TEST_FALSE(rtaphdr.GetField(channel));
  TEST_EQ(0, channel());
  TEST_FALSE(rtaphdr.GetField(level[0], 0));
  TEST_EQ(0, level[0]());
  TEST_FALSE(rtaphdr.GetField(level[1], 1));
  TEST_EQ(0, level[1]());
  TEST_FALSE(rtaphdr.GetField(level[2], 2));
  TEST_EQ(0, level[2]());
  TEST_FALSE(rtaphdr.GetField(noise));
  TEST_EQ(0, noise());
  TEST_FALSE(rtaphdr.GetField(antenna[0], 0));
  TEST_EQ(0, antenna[0]());
  TEST_FALSE(rtaphdr.GetField(antenna[1], 1));
  TEST_EQ(0, antenna[1]());
  TEST_FALSE(rtaphdr.GetField(antenna[2], 2));
  TEST_EQ(0, antenna[2]());
  TEST_FALSE(rtaphdr.GetField(rxflags));
  TEST_EQ(0, rxflags());
  TEST_FALSE(rtaphdr.GetField(txflags));
  TEST_EQ(0, txflags());

  // Make copy of raw radiotap header
  uint8_t buf[1024] = { 0 };
  size_t buflen = sizeof(buf);
  memcpy(buf, radiotap, radiotap_len);

  // Disassemble and verify
  uint8_t* p = rtaphdr.Disassemble(buf, buflen);
//  rtaphdr.Display();
  TEST_ISNOT_NULL(p);
  TEST_EQ(0, rtaphdr.GetVersion());
  TEST_EQ(38, rtaphdr.GetLength());
  TEST_EQ(0xA000402F, rtaphdr.GetPresent(0));
  TEST_EQ(0xA0000820, rtaphdr.GetPresent(1));
  TEST_EQ(0x00000820, rtaphdr.GetPresent(2));
  TEST_TRUE(rtaphdr.GetField(tsft));
  TEST_EQ(0x31966655, tsft());
  TEST_TRUE(rtaphdr.GetField(flags));
  TEST_EQ(0x10, flags());
  TEST_TRUE(flags.FCS());
  TEST_TRUE(rtaphdr.GetField(rate));
  TEST_EQ(0x0c, rate());
  TEST_EQ(6000000, rate.RateBps());
  TEST_EQ(6000, rate.RateKbps());
  TEST_EQ(6, rate.RateMbps());
  TEST_TRUE(rtaphdr.GetField(channel));
  TEST_EQ(0x0140143c, channel());
  TEST_EQ(5180, channel.Frequency());
  TEST_EQ(36, channel.Channel());
  TEST_EQ(0x0140, channel.Flags());
  TEST_TRUE(rtaphdr.GetField(level[0], 0));
  TEST_EQ(-30, level[0]());
  TEST_TRUE(rtaphdr.GetField(level[1], 1));
  TEST_EQ(-30, level[1]());
  TEST_TRUE(rtaphdr.GetField(level[2], 2));
  TEST_EQ(-63, level[2]());
  TEST_FALSE(rtaphdr.GetField(noise));
  TEST_EQ(0, noise());
  TEST_FALSE(rtaphdr.GetField(antenna[0], 0));
  TEST_EQ(0, antenna[0]());
  TEST_TRUE(rtaphdr.GetField(antenna[1], 1));
  TEST_EQ(0, antenna[1]());
  TEST_TRUE(rtaphdr.GetField(antenna[2], 2));
  TEST_EQ(1, antenna[2]());
  TEST_TRUE(rtaphdr.GetField(rxflags));
  TEST_EQ(0, rxflags());
  TEST_FALSE(rtaphdr.GetField(txflags));
  TEST_EQ(0, txflags());

  // Return success
  return (0);
}

