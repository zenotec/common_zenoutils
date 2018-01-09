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

#include <zutils/zLog.h>
using namespace zUtils;

#include "Ieee80211Test.h"
#include "UnitTest.h"

#include "RadioTap.h"
#include "Frame.h"
#include "ManagementFrame.h"
#include "Beacon.h"
#include "Probe.h"
#include "Association.h"
#include "Authentication.h"
using namespace zUtils::zWireless::ieee80211;

int
Ieee80211Test_RadiotapDefaults(void* arg_)
{

  ZLOG_DEBUG("#############################################################");
  ZLOG_DEBUG("# Ieee80211Test_RadiotapDefaults()");
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
Ieee80211Test_FrameDefaults(void* arg_)
{

  ZLOG_DEBUG("#############################################################");
  ZLOG_DEBUG("# Ieee80211Test_FrameDefaults()");
  ZLOG_DEBUG("#############################################################");

  Frame frame;
  TEST_IS_ZERO(frame.Version());
  TEST_EQ(Frame::TYPE_ERR, frame.Type());
  TEST_EQ(Frame::SUBTYPE_ERR, frame.Subtype());
  TEST_FALSE(frame.ToDS());
  TEST_FALSE(frame.FromDS());
  TEST_FALSE(frame.MoreFragments());
  TEST_FALSE(frame.Retry());
  TEST_FALSE(frame.PowerManagement());
  TEST_FALSE(frame.MoreData());
  TEST_FALSE(frame.Protected());
  TEST_FALSE(frame.Order());
  TEST_IS_ZERO(frame.DurationId());

  // Return success
  return (0);
}

int
Ieee80211Test_ManagementFrameDefaults(void* arg_)
{

  ZLOG_DEBUG("#############################################################");
  ZLOG_DEBUG("# Ieee80211Test_ManagementFrameDefaults()");
  ZLOG_DEBUG("#############################################################");

  ManagementFrame frame;
  TEST_IS_ZERO(frame.Version());
  TEST_EQ(Frame::TYPE_MGMT, frame.Type());
  TEST_EQ(Frame::SUBTYPE_ERR, frame.Subtype());
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

  // Return success
  return (0);
}

int
Ieee80211Test_BeaconDefaults(void* arg_)
{

  ZLOG_DEBUG("#############################################################");
  ZLOG_DEBUG("# Ieee80211Test_BeaconDefaults()");
  ZLOG_DEBUG("#############################################################");

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
  TEST_IS_ZERO(frame.SequenceControl());
  TEST_IS_ZERO(frame.Timestamp());
  TEST_IS_ZERO(frame.Interval());
  TEST_IS_ZERO(frame.Capabilities());

  // Return success
  return (0);
}

int
Ieee80211Test_ProbeRequestDefaults(void* arg_)
{

  ZLOG_DEBUG("#############################################################");
  ZLOG_DEBUG("# Ieee80211Test_ProbeRequestDefaults()");
  ZLOG_DEBUG("#############################################################");

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
  TEST_IS_ZERO(frame.SequenceControl());

  // Return success
  return (0);
}

int
Ieee80211Test_ProbeResponseDefaults(void* arg_)
{

  ZLOG_DEBUG("#############################################################");
  ZLOG_DEBUG("# Ieee80211Test_ProbeResponseDefaults()");
  ZLOG_DEBUG("#############################################################");

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
  TEST_IS_ZERO(frame.SequenceControl());
  TEST_IS_ZERO(frame.Timestamp());
  TEST_IS_ZERO(frame.Interval());
  TEST_IS_ZERO(frame.Capabilities());

  // Return success
  return (0);
}

int
Ieee80211Test_AssociationRequestDefaults(void* arg_)
{

  ZLOG_DEBUG("#############################################################");
  ZLOG_DEBUG("# Ieee80211Test_AssociationRequestDefaults()");
  ZLOG_DEBUG("#############################################################");

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
  TEST_IS_ZERO(frame.Capabilities());
  TEST_IS_ZERO(frame.Interval());

  // Return success
  return (0);
}

int
Ieee80211Test_AssociationResponseDefaults(void* arg_)
{

  ZLOG_DEBUG("#############################################################");
  ZLOG_DEBUG("# Ieee80211Test_AssociationResponseDefaults()");
  ZLOG_DEBUG("#############################################################");

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

  // Return success
  return (0);
}

int
Ieee80211Test_AuthenticationDefaults(void* arg_)
{

  ZLOG_DEBUG("#############################################################");
  ZLOG_DEBUG("# Ieee80211Test_AuthenticationDefaults()");
  ZLOG_DEBUG("#############################################################");

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

  // Return success
  return (0);
}


