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
#include <zutils/ieee8023/EapolFrame.h>

using namespace zUtils;
using namespace zSocket::ieee8023;

#include "Ieee8023Test.h"
#include "UnitTest.h"

ZLOG_MODULE_INIT(zLog::Log::MODULE_TEST);

int
EapolTest_GetSet(void* arg_)
{
  zSocket::ieee8023::eap_hdr fHdr;
  zSocket::ieee8023::eap_hdr sHdr = {
      IEEE_8021X_PROTO_VER::VER_2001,
      IEEE_8021X_PROTO_TYPE::EAP_KEY,
      sizeof(zSocket::ieee8023::eapol_body) + 16
  };

  uint8_t nounce[NOUNCE_LEN] = {};
  uint8_t k_iv[INSERT_VECTOR_LEN] = {};
  uint8_t k_replay[8] = {};
  zSocket::ieee8023::eapol_body fBody;

  zSocket::ieee8023::eapol_body sBody;

  sBody.des_type = IEEE_8021X_KEY_DESC::IEEE_80211_KEY_DESCRIPTOR;
  sBody.key_info = (HMAC_SHA_1 | KEY_ACK);
  sBody.key_len = 16;
  //sBody.key_replay_cntr = 0;
  memcpy(sBody.key_nounce, nounce, NOUNCE_LEN);
  memcpy(sBody.key_iv, k_iv, INSERT_VECTOR_LEN);

  //memcpy(sBody.key_replay_cntr, k_replay, 8);
  sBody.key_replay_cntr = (uint64_t)k_replay;
  sBody.key_rsc = 0;
  sBody.key_id = 0;

  std::vector<uint8_t> fMic;
  std::vector<uint8_t> sMic(16, 0);

  key_data fkData;
  key_data kData = {16, NULL};


  ZLOG_DEBUG("#############################################################");
  ZLOG_DEBUG("# EapolTest_GetSet()");
  ZLOG_DEBUG("#############################################################");

  zSocket::ieee8023::Frame f;
  TEST_EQ(f.GetType(), zSocket::ieee8023::Frame::TYPE_8023);
  TEST_EQ(f.GetSubtype(), zSocket::ieee8023::Frame::SUBTYPE_NONE);
  TEST_EQ(f.GetProto(), zSocket::ieee8023::Frame::PROTO_NONE);
  TEST_IS_ZERO(f.GetPayloadLength());

  zSocket::ieee8023::EapolKeyFrame eapol(AKM_CIPHER_SUITES::PSK);
  TEST_EQ(eapol.GetSubtype(), zSocket::ieee8023::Frame::SUBTYPE_EAPOL);
  TEST_TRUE(eapol.SetHeader(sHdr));
  TEST_TRUE(eapol.SetBody(sBody));
  TEST_TRUE(eapol.SetKeyDataLen(kData.len));
  TEST_TRUE(eapol.SetMIC(sMic));

  fHdr = eapol.GetHeader();
  TEST_EQ(fHdr.type, sHdr.type);
  TEST_EQ(fHdr.length, sHdr.length);
  TEST_EQ(fHdr.proto_ver, sHdr.proto_ver);

  fBody = eapol.GetBody();
  TEST_EQ(fBody.des_type, sBody.des_type);
  TEST_EQ(fBody.key_info, sBody.key_info);
  TEST_EQ(fBody.key_len, sBody.key_len);
  TEST_EQ(fBody.key_replay_cntr, sBody.key_replay_cntr);
  TEST_EQ(fBody.key_nounce[0], sBody.key_nounce[0]);
  TEST_EQ(fBody.key_nounce[NOUNCE_LEN-1], sBody.key_nounce[NOUNCE_LEN-1]);
  TEST_EQ(fBody.key_iv[0], sBody.key_iv[0]);
  TEST_EQ(fBody.key_iv[INSERT_VECTOR_LEN-1], sBody.key_iv[INSERT_VECTOR_LEN-1]);
  TEST_EQ(fBody.key_rsc, sBody.key_rsc);
  TEST_EQ(fBody.key_id, sBody.key_id);

  fkData.len = eapol.GetKeyDataLen();
  TEST_EQ(fkData.len, kData.len);
  TEST_EQ(fkData.data, NULL);

  // Return success
  return (UTEST_PASS);
}

int
EapolTest_Assemble(void* arg_)
{
  zSocket::ieee8023::eap_hdr fHdr;
  zSocket::ieee8023::eapol_body fBody;
  key_data fkData;

  size_t reqSize = sizeof(zSocket::ieee8023::eapol_body) + 16 + 2;
  zSocket::ieee8023::eap_hdr sHdr = {
      IEEE_8021X_PROTO_VER::VER_2001,
      IEEE_8021X_PROTO_TYPE::EAP_KEY,
      (uint16_t)reqSize
  };
  zSocket::ieee8023::eapol_body sBody;
  uint8_t nounce[NOUNCE_LEN] = {};
  uint8_t k_iv[INSERT_VECTOR_LEN] = {};
  uint8_t k_replay[8] = {};
  sBody.des_type = IEEE_8021X_KEY_DESC::IEEE_80211_KEY_DESCRIPTOR;
  sBody.key_info = (HMAC_SHA_1 | KEY_ACK);
  sBody.key_len = 16;
  //sBody.key_replay_cntr = 0;
  sBody.key_replay_cntr = 1;
 // memcpy(sBody.key_replay_cntr, k_replay, 8);
  memcpy(sBody.key_nounce, nounce, NOUNCE_LEN);
  memcpy(sBody.key_iv, k_iv, INSERT_VECTOR_LEN);
  sBody.key_rsc = 0;
  sBody.key_id = 0;
  key_data kData = {0, NULL};
  std::vector<uint8_t> sMic(16, 0);
  ZLOG_DEBUG("#############################################################");
  ZLOG_DEBUG("# Ieee8023Test_Assemble()");
  ZLOG_DEBUG("#############################################################");

  zSocket::ieee8023::Frame f;
  TEST_EQ(f.GetType(), zSocket::ieee8023::Frame::TYPE_8023);
  TEST_EQ(f.GetSubtype(), zSocket::ieee8023::Frame::SUBTYPE_NONE);
  TEST_EQ(f.GetProto(), zSocket::ieee8023::Frame::PROTO_NONE);
  TEST_IS_ZERO(f.GetPayloadLength());

  zSocket::ieee8023::EapolKeyFrame eapol(AKM_CIPHER_SUITES::PSK);
  zSocket::ieee8023::EapolKeyFrame eapolS(AKM_CIPHER_SUITES::PSK);
  TEST_EQ(eapol.GetSubtype(), zSocket::ieee8023::Frame::SUBTYPE_EAPOL);
  TEST_TRUE(eapol.SetHeader(sHdr));
  TEST_TRUE(eapol.SetBody(sBody));
  TEST_TRUE(eapol.SetKeyDataLen(kData.len));
  TEST_TRUE(eapol.SetMIC(sMic));
  TEST_TRUE(eapol.SetDestination(std::string("ff:ff:ff:ff:ff:ff")));
  TEST_TRUE(eapol.SetSource(std::string("00:01:02:03:04:05")));
  //TEST_TRUE(eapol.SetSubtype(zSocket::ieee8023::Frame::SUBTYPE_EAPOL));
  //TEST_TRUE(eapol.SetProto(zSocket::ieee8023::Frame::PROTO_EAPOL));
  TEST_EQ(eapol.GetSubtype(), zSocket::ieee8023::Frame::SUBTYPE_EAPOL);
  TEST_EQ(eapol.GetProto(), zSocket::ieee8023::Frame::PROTO_EAPOL);

  // Create empty buffer for assembling ARP frame
  zSocket::Buffer sb;

  // Assemble frame
  TEST_TRUE(eapol.Assemble(sb, false));
  size_t dataSize = sb.Size();
  sb.Reset();
  sb.Put(dataSize);
  /*
  sb.Put(sizeof(struct ieee8023_addr));
  sb.Put(sizeof(struct ieee8023_addr));
  sb.Put(sizeof(zSocket::ieee8023::eapol_hdr));
  sb.Put(sizeof(zSocket::ieee8023::eapol_body));
  sb.Put(sMic.size());
  sb.Put(sizeof(kData.len));
*/
  TEST_TRUE(eapolS.Disassemble(sb, false));
  fHdr = eapolS.GetHeader();
  fBody = eapolS.GetBody();
  fkData.len = eapolS.GetKeyDataLen();

  TEST_EQ(fHdr.type, sHdr.type);
  TEST_EQ(fHdr.length, sHdr.length);
  TEST_EQ(fHdr.proto_ver, sHdr.proto_ver);

  TEST_EQ(fBody.des_type, sBody.des_type);
  TEST_EQ(fBody.key_info, sBody.key_info);
  TEST_EQ(fBody.key_len, sBody.key_len);
  TEST_EQ(fBody.key_replay_cntr, sBody.key_replay_cntr);
  TEST_EQ(fBody.key_nounce[0], sBody.key_nounce[0]);
  TEST_EQ(fBody.key_nounce[NOUNCE_LEN-1], sBody.key_nounce[NOUNCE_LEN-1]);
  TEST_EQ(fBody.key_iv[0], sBody.key_iv[0]);
  TEST_EQ(fBody.key_iv[INSERT_VECTOR_LEN-1], sBody.key_iv[INSERT_VECTOR_LEN-1]);
  TEST_EQ(fBody.key_rsc, sBody.key_rsc);
  TEST_EQ(fBody.key_id, sBody.key_id);

  TEST_EQ(fkData.len, kData.len);
  TEST_EQ(fkData.data, NULL);

//  // Validate frame
//  for (int i = 0; i < arp_pkt_len; i++)
//  {
//    TEST_EQ_MSG((int)arp_pkt[i], buf[i], zLog::IntStr(i));
//  }

  // Return success
  return (UTEST_PASS);
}

int
EapolTest_Disassemble(void* arg_)
{

  ZLOG_DEBUG("#############################################################");
  ZLOG_DEBUG("# Ieee8023Test_Disassemble()");
  ZLOG_DEBUG("#############################################################");


  zSocket::ieee8023::eap_hdr fHdr;
  zSocket::ieee8023::eapol_body fBody;
  key_data fkData;

  zSocket::ieee8023::Ether2Frame f;
  TEST_EQ(f.GetType(), zSocket::ieee8023::Frame::TYPE_8023);
  TEST_EQ(f.GetSubtype(), zSocket::ieee8023::Frame::SUBTYPE_ETHER2);
  TEST_EQ(f.GetProto(), zSocket::ieee8023::Frame::PROTO_NONE);
  TEST_IS_ZERO(f.GetPayloadLength());

  // Make copy of raw ARP frame
  zSocket::Buffer sb;
  size_t buflen = eapol_pkt_1_len;
  memcpy(sb.Head(), eapol_pkt_1, eapol_pkt_1_len);
  sb.Put(eapol_pkt_1_len);

  zSocket::ieee8023::EapolKeyFrame eapol(AKM_CIPHER_SUITES::PSK);
  TEST_TRUE(eapol.Disassemble(sb, false));
  TEST_EQ(std::string("ff:ff:ff:ff:ff:ff"), eapol.GetDestination());
  TEST_EQ(std::string("00:01:02:03:04:05"), eapol.GetSource());
  TEST_EQ(zSocket::ieee8023::Frame::SUBTYPE_EAPOL, eapol.GetSubtype());
  TEST_EQ(zSocket::ieee8023::Frame::PROTO_EAPOL, eapol.GetProto());
  fHdr = eapol.GetHeader();
  fBody = eapol.GetBody();
  fkData.len = eapol.GetKeyDataLen();

  TEST_EQ(fHdr.proto_ver, IEEE_8021X_PROTO_VER::VER_2004);
  TEST_EQ(fHdr.type, IEEE_8021X_PROTO_TYPE::EAP_KEY);
  TEST_EQ(fHdr.length, 0x5f);

  TEST_EQ(fBody.des_type, IEEE_8021X_KEY_DESC::IEEE_80211_KEY_DESCRIPTOR);
  TEST_EQ(fBody.key_info, 0x008a);
  TEST_EQ(fBody.key_len, 0x0010);
 // TEST_EQ(fBody.key_replay_cntr[0], 0);
  //TEST_EQ(fBody.key_replay_cntr[7], 0);
  TEST_EQ(fBody.key_nounce[0], 0xc2);
  TEST_EQ(fBody.key_nounce[NOUNCE_LEN-1], 0xa5);
  TEST_EQ(fBody.key_iv[0], 0);
  TEST_EQ(fBody.key_iv[INSERT_VECTOR_LEN-1], 0);
  TEST_EQ(fBody.key_rsc, 0);
  TEST_EQ(fBody.key_id, 0);
  std::vector<uint8_t> mic;
  mic = eapol.GetMIC();
  
  TEST_EQ(fkData.len, 0);
  TEST_EQ(mic[0], 0);
  TEST_EQ(16, mic.size());

  zSocket::Buffer sb2;
  size_t buflen2 = eapol_pkt_2_len;
  memcpy(sb2.Head(), eapol_pkt_2, eapol_pkt_2_len);
  sb2.Put(eapol_pkt_2_len);
  zSocket::ieee8023::EapolKeyFrame eapol2(AKM_CIPHER_SUITES::PSK);
  TEST_TRUE(eapol2.Disassemble(sb2, false));
  mic = eapol2.GetMIC();
  TEST_EQ(mic[0], 0xa6);
  TEST_EQ(mic[15], 0xff);
  TEST_EQ(22, eapol2.GetKeyDataLen());
  // Return success
  return (UTEST_PASS);
}
