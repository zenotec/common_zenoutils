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
#include <zutils/ieee8023/Frame.h>
#include <zutils/ieee8023/EtherFrame.h>
#include <zutils/ieee8023/LlcFrame.h>
#include <zutils/ieee8023/Ether2Frame.h>
#include <zutils/ieee8023/VlanFrame.h>
using namespace zUtils;

#include "Ieee8023Test.h"
#include "UnitTest.h"

ZLOG_MODULE_INIT(zLog::Log::MODULE_TEST);

int
Ieee8023Test_Defaults(void* arg_)
{

  ZLOG_DEBUG("#############################################################");
  ZLOG_DEBUG("# Ieee8023Test_Defaults()");
  ZLOG_DEBUG("#############################################################");

  zSocket::ieee8023::Frame f;
  TEST_EQ(f.GetType(), zSocket::ieee8023::Frame::TYPE_8023);
  TEST_EQ(f.GetSubtype(), zSocket::ieee8023::Frame::SUBTYPE_NONE);
  TEST_EQ(f.GetProto(), zSocket::ieee8023::Frame::PROTO_NONE);
  TEST_IS_ZERO(f.GetPayloadLength());

  zSocket::ieee8023::EtherFrame ether_f;
  TEST_EQ(f.GetType(), zSocket::ieee8023::Frame::TYPE_8023);
  TEST_EQ(ether_f.GetSubtype(), zSocket::ieee8023::Frame::SUBTYPE_ETHER);
  TEST_EQ(ether_f.GetProto(), zSocket::ieee8023::Frame::PROTO_NONE);
  TEST_IS_ZERO(ether_f.GetPayloadLength());

  zSocket::ieee8023::LlcFrame llc_f;
  TEST_EQ(f.GetType(), zSocket::ieee8023::Frame::TYPE_8023);
  TEST_EQ(llc_f.GetSubtype(), zSocket::ieee8023::Frame::SUBTYPE_LLC);
  TEST_EQ(llc_f.GetProto(), zSocket::ieee8023::Frame::PROTO_NONE);
  TEST_IS_ZERO(llc_f.GetPayloadLength());

  zSocket::ieee8023::Ether2Frame ether2_f;
  TEST_EQ(f.GetType(), zSocket::ieee8023::Frame::TYPE_8023);
  TEST_EQ(ether2_f.GetSubtype(), zSocket::ieee8023::Frame::SUBTYPE_ETHER2);
  TEST_EQ(ether2_f.GetProto(), zSocket::ieee8023::Frame::PROTO_NONE);
  TEST_IS_ZERO(ether2_f.GetPayloadLength());

  zSocket::ieee8023::VlanFrame vlan_f;
  TEST_EQ(f.GetType(), zSocket::ieee8023::Frame::TYPE_8023);
  TEST_EQ(vlan_f.GetSubtype(), zSocket::ieee8023::Frame::SUBTYPE_VLAN);
  TEST_EQ(vlan_f.GetProto(), zSocket::ieee8023::Frame::PROTO_NONE);
  TEST_IS_ZERO(vlan_f.GetPayloadLength());

  // Return success
  return (UTEST_PASS);
}

int
Ieee8023Test_GetSet(void* arg_)
{

  ZLOG_DEBUG("#############################################################");
  ZLOG_DEBUG("# Ieee8023Test_GetSet()");
  ZLOG_DEBUG("#############################################################");

  zSocket::ieee8023::Frame f;
  TEST_EQ(f.GetType(), zSocket::ieee8023::Frame::TYPE_8023);
  TEST_EQ(f.GetSubtype(), zSocket::ieee8023::Frame::SUBTYPE_NONE);
  TEST_EQ(f.GetProto(), zSocket::ieee8023::Frame::PROTO_NONE);
  TEST_IS_ZERO(f.GetPayloadLength());

  // Return success
  return (UTEST_PASS);
}

int
Ieee8023Test_Assemble(void* arg_)
{

  ZLOG_DEBUG("#############################################################");
  ZLOG_DEBUG("# Ieee8023Test_Assemble()");
  ZLOG_DEBUG("#############################################################");

  zSocket::ieee8023::Frame f;
  TEST_EQ(f.GetType(), zSocket::ieee8023::Frame::TYPE_8023);
  TEST_EQ(f.GetSubtype(), zSocket::ieee8023::Frame::SUBTYPE_NONE);
  TEST_EQ(f.GetProto(), zSocket::ieee8023::Frame::PROTO_NONE);
  TEST_IS_ZERO(f.GetPayloadLength());

  // Set up ARP frame contents
  TEST_TRUE(f.SetDestination(std::string("ff:ff:ff:ff:ff:ff")));
  TEST_TRUE(f.SetSource(std::string("00:01:02:03:04:05")));
  TEST_TRUE(f.SetSubtype(zSocket::ieee8023::Frame::SUBTYPE_ETHER2));
  TEST_TRUE(f.SetProto(zSocket::ieee8023::Frame::PROTO_ARP));

  // Create empty buffer for assembling ARP frame
  zSocket::Buffer sb;

  // Assemble frame
  TEST_TRUE(f.Assemble(sb));

//  // Validate frame
//  for (int i = 0; i < arp_pkt_len; i++)
//  {
//    TEST_EQ_MSG((int)arp_pkt[i], buf[i], zLog::IntStr(i));
//  }

  // Return success
  return (UTEST_PASS);
}

int
Ieee8023Test_Disassemble(void* arg_)
{

  ZLOG_DEBUG("#############################################################");
  ZLOG_DEBUG("# Ieee8023Test_Disassemble()");
  ZLOG_DEBUG("#############################################################");

  zSocket::ieee8023::Ether2Frame f;
  TEST_EQ(f.GetType(), zSocket::ieee8023::Frame::TYPE_8023);
  TEST_EQ(f.GetSubtype(), zSocket::ieee8023::Frame::SUBTYPE_ETHER2);
  TEST_EQ(f.GetProto(), zSocket::ieee8023::Frame::PROTO_NONE);
  TEST_IS_ZERO(f.GetPayloadLength());

  // Make copy of raw ARP frame
  zSocket::Buffer sb;
  size_t buflen = arp_pkt_len;
  memcpy(sb.Head(), arp_pkt, arp_pkt_len);
  sb.Put(arp_pkt_len);

  TEST_TRUE(f.Disassemble(sb));
  TEST_EQ(std::string("ff:ff:ff:ff:ff:ff"), f.GetDestination());
  TEST_EQ(std::string("00:01:02:03:04:05"), f.GetSource());
  TEST_EQ(zSocket::ieee8023::Frame::SUBTYPE_ETHER2, f.GetSubtype());
  TEST_EQ(zSocket::ieee8023::Frame::PROTO_ARP, f.GetProto());

  // Return success
  return (UTEST_PASS);
}
