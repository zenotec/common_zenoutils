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

#include <string>

#include <zutils/zLog.h>
#include <zutils/ieee8023/EapolFrame.h>
#include <zutils/zSecurity.h>

using namespace zUtils;
using namespace zUtils::zWireless::ieee80211;
using namespace zUtils::zSecurity;
using namespace zUtils::zSocket;

#include "zSecurityTest.h"
#include "UnitTest.h"

ZLOG_MODULE_INIT(zLog::Log::MODULE_TEST);

const struct rsn_element def_version = { .version = 1 };
const struct rsn_element ver_unknown = { .version = 0 };
const struct rsn_oui_format def_oui = { .oui_b1 = 0x00, .oui_b2 = 0x0F, .oui_b3 = 0xAC };
const struct rsn_suite def_group = { .cipher_oui = def_oui, .cipher_suite_type = 4 };
const struct rsn_suite def_unknown = { .cipher_oui = def_oui, .cipher_suite_type = 255 };
const struct rsn_suite def_pair = { .cipher_oui = def_oui, .cipher_suite_type = 4 };
const struct rsn_suite def_akm = { .cipher_oui = def_oui, .cipher_suite_type = 2 };
const struct rsn_suite_count def_count = { .suite_count = 1 };

int 
zSecurityTest_Defaults(void* arg_)
{

  ZLOG_DEBUG("#############################################################");
  ZLOG_DEBUG("# zSecurityTest_Defaults()");
  ZLOG_DEBUG("#############################################################");
  zSecurity::SecurityWpa wpa;
  TEST_EQ(wpa.GetHandshakeState(), SecurityWpa::CUR_MSG_STATE::NONE);
  TEST_EQ(wpa.GetGroupCipher(), IEEE_80211_CIPHER_SUITES::CIPHER_UNKNOWN);
  TEST_EQ(wpa.GetPtkCipher(), IEEE_80211_CIPHER_SUITES::CIPHER_UNKNOWN);
  TEST_EQ(wpa.GetAkmCipher(), AKM_CIPHER_SUITES::AKM_CIPHER_UNKNOWN);
  TEST_EQ(wpa.GetReplayCntr(), 0);
  TEST_EQ(wpa.GetPmk()[0], 0);
  TEST_EQ(wpa.GetPmk()[PMK_LEN - 1], 0);



  return(UTEST_PASS);
}

int 
zSecurityTest_GetSet(void* arg_)
{
  RsnTag::rsn_default rsn_def = {
          .protocol = def_version,
          .group_data_cipher = def_group,
          .pairwise_count = def_count,
          .pairwise_cipher_suite = def_pair,
          .akm_count = def_count,
          .akm_suite_list = def_akm
  };
  RsnTag rsneTag;
  rsneTag(IEEE_80211_CIPHER_SUITES::CCMP_128, {IEEE_80211_CIPHER_SUITES::CCMP_128},
                             {AKM_CIPHER_SUITES::PSK});
  char psk[PMK_LEN] = {};
  psk[0] = 'w';
  psk[1] = 'p';
  psk[2] = 'a';
  psk[3] = 'T';
  psk[4] = 'e';
  psk[5] = 's';
  psk[6] = 't';
  psk[7] = 'i';
  psk[8] = 'n';
  psk[9] = 'g';
  ZLOG_DEBUG("#############################################################");
  ZLOG_DEBUG("# zSecurityTest_GetSet()");
  ZLOG_DEBUG("#############################################################");
  zSecurity::SecurityWpa wpa;
  TEST_EQ(wpa.GetHandshakeState(), SecurityWpa::CUR_MSG_STATE::NONE);
  TEST_EQ(wpa.GetGroupCipher(), IEEE_80211_CIPHER_SUITES::CIPHER_UNKNOWN);
  TEST_EQ(wpa.GetPtkCipher(), IEEE_80211_CIPHER_SUITES::CIPHER_UNKNOWN);
  TEST_EQ(wpa.GetAkmCipher(), AKM_CIPHER_SUITES::AKM_CIPHER_UNKNOWN);

  TEST_TRUE(wpa.SetGroupCipher(IEEE_80211_CIPHER_SUITES::CCMP_128));
  TEST_TRUE(wpa.SetPtkCipher(IEEE_80211_CIPHER_SUITES::CCMP_128));
  TEST_TRUE(wpa.SetAkmCipher(AKM_CIPHER_SUITES::PSK));
  TEST_TRUE(wpa.SetHandshakeState(SecurityWpa::CUR_MSG_STATE::MES_1));
  TEST_TRUE(wpa.SetReplayCntr(4));
  TEST_TRUE(wpa.SetPmk((uint8_t*)psk));
  TEST_TRUE(wpa.SetRsnTag(rsneTag));

  RsnTag::rsn_default rsnTag_def = wpa.GetRsnTag();

  TEST_EQ(wpa.GetHandshakeState(), SecurityWpa::CUR_MSG_STATE::MES_1);
  TEST_EQ(wpa.GetGroupCipher(), IEEE_80211_CIPHER_SUITES::CCMP_128);
  TEST_EQ(wpa.GetPtkCipher(), IEEE_80211_CIPHER_SUITES::CCMP_128);
  TEST_EQ(wpa.GetAkmCipher(), AKM_CIPHER_SUITES::PSK);
  TEST_EQ(wpa.GetReplayCntr(), 4);
  TEST_EQ(wpa.GetPmk()[0], (uint8_t)psk[0]);
  TEST_EQ(wpa.GetPmk()[1], (uint8_t)psk[1]);
  TEST_EQ(wpa.GetPmk()[2], (uint8_t)psk[2]);
  TEST_EQ(wpa.GetPmk()[3], (uint8_t)psk[3]);
  TEST_EQ(wpa.GetPmk()[4], (uint8_t)psk[4]);
  TEST_EQ(wpa.GetPmk()[5], (uint8_t)psk[5]);
  TEST_EQ(wpa.GetPmk()[6], (uint8_t)psk[6]);
  TEST_EQ(wpa.GetPmk()[7], (uint8_t)psk[7]);

  TEST_EQ(rsn_def.protocol.version, rsnTag_def.protocol.version);
  TEST_EQ(rsn_def.group_data_cipher.cipher_oui.oui_b1,
              rsnTag_def.group_data_cipher.cipher_oui.oui_b1);
  TEST_EQ(rsn_def.group_data_cipher.cipher_oui.oui_b2,
              rsnTag_def.group_data_cipher.cipher_oui.oui_b2);
  TEST_EQ(rsn_def.group_data_cipher.cipher_oui.oui_b3,
              rsnTag_def.group_data_cipher.cipher_oui.oui_b3);
  TEST_EQ(rsn_def.group_data_cipher.cipher_suite_type,
              rsnTag_def.group_data_cipher.cipher_suite_type);
  TEST_EQ(rsn_def.akm_count.suite_count, rsnTag_def.akm_count.suite_count);
  TEST_EQ(rsn_def.pairwise_count.suite_count, rsnTag_def.pairwise_count.suite_count);
  TEST_EQ(rsn_def.pairwise_cipher_suite.cipher_oui.oui_b1,
              rsnTag_def.pairwise_cipher_suite.cipher_oui.oui_b1);
  TEST_EQ(rsn_def.pairwise_cipher_suite.cipher_oui.oui_b2,
              rsnTag_def.pairwise_cipher_suite.cipher_oui.oui_b2);
  TEST_EQ(rsn_def.pairwise_cipher_suite.cipher_oui.oui_b3,
              rsnTag_def.pairwise_cipher_suite.cipher_oui.oui_b3);
  TEST_EQ(rsn_def.akm_suite_list.cipher_oui.oui_b1,
              rsnTag_def.akm_suite_list.cipher_oui.oui_b1);
  TEST_EQ(rsn_def.akm_suite_list.cipher_oui.oui_b2,
              rsnTag_def.akm_suite_list.cipher_oui.oui_b2);
  TEST_EQ(rsn_def.akm_suite_list.cipher_oui.oui_b3,
              rsnTag_def.akm_suite_list.cipher_oui.oui_b3);
  TEST_EQ(rsn_def.pairwise_cipher_suite.cipher_suite_type,
              rsnTag_def.pairwise_cipher_suite.cipher_suite_type);
  TEST_EQ(rsn_def.akm_suite_list.cipher_suite_type,
              rsnTag_def.akm_suite_list.cipher_suite_type);

  return(UTEST_PASS);
}

int
zSecurityTest_Handshake_Mes1(void* arg_)
{

  ZLOG_DEBUG("#############################################################");
  ZLOG_DEBUG("# zSecurityTest_Handshake_Mes1()");
  ZLOG_DEBUG("#############################################################");

  zSecurity::SecurityWpa wpa;
  ieee8023::EapolKeyFrame eapol;
  ieee8023::EapolKeyFrame eapolS;
  Buffer sb;
  TEST_EQ(wpa.GetHandshakeState(), SecurityWpa::CUR_MSG_STATE::NONE);
  TEST_EQ(wpa.GetGroupCipher(), IEEE_80211_CIPHER_SUITES::CIPHER_UNKNOWN);
  TEST_EQ(wpa.GetPtkCipher(), IEEE_80211_CIPHER_SUITES::CIPHER_UNKNOWN);
  TEST_EQ(wpa.GetAkmCipher(), AKM_CIPHER_SUITES::AKM_CIPHER_UNKNOWN);
  TEST_EQ(wpa.GetReplayCntr(), 0);
  TEST_TRUE(wpa.SetGroupCipher(IEEE_80211_CIPHER_SUITES::CCMP_128));
  TEST_TRUE(wpa.SetPtkCipher(IEEE_80211_CIPHER_SUITES::CCMP_128));
  TEST_TRUE(wpa.SetAkmCipher(AKM_CIPHER_SUITES::PSK));
  TEST_TRUE(wpa.SetHandshakeState(SecurityWpa::CUR_MSG_STATE::MES_1));
  TEST_TRUE(eapol.SetDestination(std::string("ff:ff:ff:ff:ff:ff")));
  TEST_TRUE(eapol.SetSource(std::string("00:01:02:03:04:05")));

  TEST_TRUE(wpa.CreateCurMes(eapol, SecurityWpa::CUR_MSG_STATE::MES_2));
  TEST_EQ(wpa.GetReplayCntr(), 0);
  TEST_EQ(wpa.GetHandshakeState(), SecurityWpa::CUR_MSG_STATE::MES_2);
  eap_hdr fHdr = eapol.GetHeader();
  eapol_body fBody = eapol.GetBody();
  key_data fkData;
  fkData.len = eapol.GetKeyDataLen();

  TEST_EQ(fHdr.type, EAP_KEY);
  TEST_EQ(fHdr.proto_ver, VER_2004);

  TEST_EQ(fBody.des_type, IEEE_80211_KEY_DESCRIPTOR);
  TEST_EQ(fBody.key_info, 0x008a);
  TEST_EQ(fBody.key_len, 16);
  TEST_EQ(fBody.key_replay_cntr, wpa.GetReplayCntr());
  TEST_EQ(fBody.key_nounce[0], wpa.GetAnounce()[0]);
  TEST_EQ(fBody.key_nounce[NOUNCE_LEN-1], wpa.GetAnounce()[NOUNCE_LEN-1]);
  TEST_EQ(fBody.key_iv[0], 0);
  TEST_EQ(fBody.key_iv[INSERT_VECTOR_LEN-1], 0);
  TEST_EQ(fBody.key_rsc, 0);
  TEST_EQ(fBody.key_id, 0);

  TEST_EQ(fkData.len, 0);

  TEST_TRUE(eapol.Assemble(sb, false));
  sb.Reset();
  sb.Put(sizeof(struct ieee8023_addr));
  sb.Put(sizeof(struct ieee8023_addr));
  sb.Put(sizeof(zSocket::ieee8023::eapol_hdr));
  sb.Put(sizeof(zSocket::ieee8023::eapol_body));
  std::vector<uint8_t> sMic = eapol.GetMIC();
  sb.Put(sMic.size());
  sb.Put(sizeof(fkData.len));
  TEST_TRUE(eapolS.Disassemble(sb, false));

  fHdr = eapolS.GetHeader();
  fBody = eapolS.GetBody();
  fkData.len = eapolS.GetKeyDataLen();

  TEST_EQ(fHdr.proto_ver, IEEE_8021X_PROTO_VER::VER_2004);
  TEST_EQ(fHdr.type, IEEE_8021X_PROTO_TYPE::EAP_KEY);

  TEST_EQ(fBody.des_type, IEEE_80211_KEY_DESCRIPTOR);
  TEST_EQ(fBody.key_info, 0x008a);
  TEST_EQ(fBody.key_len, 16);
  TEST_EQ(fBody.key_replay_cntr, wpa.GetReplayCntr());
  TEST_EQ(fBody.key_nounce[0], wpa.GetAnounce()[0]);
  TEST_EQ(fBody.key_nounce[NOUNCE_LEN-1], wpa.GetAnounce()[NOUNCE_LEN-1]);
  TEST_EQ(fBody.key_iv[0], 0);
  TEST_EQ(fBody.key_iv[INSERT_VECTOR_LEN-1], 0);
  TEST_EQ(fBody.key_rsc, 0);
  TEST_EQ(fBody.key_id, 0);


  return(UTEST_PASS);
}

int
zSecurityTest_Handshake_Mes2(void* arg_)
{
  RsnTag::rsn_default rsn_def = {
          .protocol = def_version,
          .group_data_cipher = def_group,
          .pairwise_count = def_count,
          .pairwise_cipher_suite = def_pair,
          .akm_count = def_count,
          .akm_suite_list = def_akm
  };

  RsnTag::rsn_default rsn_unknown = {
          .protocol = ver_unknown,
          .group_data_cipher = def_unknown,
          .pairwise_count = def_count,
          .pairwise_cipher_suite = def_unknown,
          .akm_count = def_count,
          .akm_suite_list = def_unknown
  };
  char psk[PMK_LEN] = {};
  psk[0] = 'w';
  psk[1] = 'p';
  psk[2] = 'a';
  psk[3] = 'T';
  psk[4] = 'e';
  psk[5] = 's';
  psk[6] = 't';
  psk[7] = 'i';
  psk[8] = 'n';
  psk[9] = 'g';
  ZLOG_DEBUG("#############################################################");
  ZLOG_DEBUG("# zSecurityTest_Handshake_Mes2()");
  ZLOG_DEBUG("#############################################################");
  zSecurity::SecurityWpa wpa;
  zSecurity::SecurityWpa wpaS;
  ieee8023::EapolKeyFrame eapol(AKM_CIPHER_SUITES::PSK);
  ieee8023::EapolKeyFrame eapolS(AKM_CIPHER_SUITES::PSK);
  ieee8023::EapolKeyFrame eapolS2(AKM_CIPHER_SUITES::PSK);
  ieee8023::EapolKeyFrame eapol2(AKM_CIPHER_SUITES::PSK);

  RsnTag rsneTag;
  rsneTag(IEEE_80211_CIPHER_SUITES::CCMP_128, {IEEE_80211_CIPHER_SUITES::CCMP_128},
                             {AKM_CIPHER_SUITES::PSK});
  Buffer sb;
  Buffer sb2;
  TEST_EQ(wpa.GetHandshakeState(), SecurityWpa::CUR_MSG_STATE::NONE);
  TEST_EQ(wpa.GetGroupCipher(), IEEE_80211_CIPHER_SUITES::CIPHER_UNKNOWN);
  TEST_EQ(wpa.GetPtkCipher(), IEEE_80211_CIPHER_SUITES::CIPHER_UNKNOWN);
  TEST_EQ(wpa.GetAkmCipher(), AKM_CIPHER_SUITES::AKM_CIPHER_UNKNOWN);
  TEST_EQ(wpa.GetReplayCntr(), 0);
  TEST_TRUE(wpa.SetGroupCipher(IEEE_80211_CIPHER_SUITES::CCMP_128));
  TEST_TRUE(wpa.SetPtkCipher(IEEE_80211_CIPHER_SUITES::CCMP_128));
  TEST_TRUE(wpa.SetAkmCipher(AKM_CIPHER_SUITES::PSK));
  TEST_TRUE(wpa.SetHandshakeState(SecurityWpa::CUR_MSG_STATE::MES_1));
  TEST_TRUE(wpa.SetPmk((uint8_t*)psk));
  TEST_TRUE(wpaS.SetPmk((uint8_t*)psk));
  TEST_TRUE(wpa.SetRsnTag(rsneTag));
  TEST_TRUE(wpaS.SetRsnTag(rsneTag));
  TEST_TRUE(eapol.SetDestination(std::string("ff:ff:ff:ff:ff:ff")));
  TEST_TRUE(eapol.SetSource(std::string("00:01:02:03:04:05")));
  TEST_TRUE(eapolS2.SetSource(std::string("ff:ff:ff:ff:ff:ff")));
  TEST_TRUE(eapolS2.SetDestination(std::string("00:01:02:03:04:05")));

  // Create Msg 1 wpa -> wpaS
  TEST_TRUE(wpa.CreateCurMes(eapol, SecurityWpa::CUR_MSG_STATE::MES_2));
  TEST_EQ(wpa.GetReplayCntr(), 0);
  TEST_EQ(wpa.GetHandshakeState(), SecurityWpa::CUR_MSG_STATE::MES_2);

  TEST_TRUE(eapol.Assemble(sb, false));
  size_t data_size = sb.Size();
  sb.Reset();
  sb.Put(data_size);
  TEST_TRUE(eapolS.Disassemble(sb, false));
  TEST_TRUE(wpaS.SetGroupCipher(IEEE_80211_CIPHER_SUITES::CCMP_128));
  TEST_TRUE(wpaS.SetPtkCipher(IEEE_80211_CIPHER_SUITES::CCMP_128));
  TEST_TRUE(wpaS.SetAkmCipher(AKM_CIPHER_SUITES::PSK));
  TEST_TRUE(wpaS.SetHandshakeState(SecurityWpa::CUR_MSG_STATE::MES_1));
  //Process Msg 1 from wpa
  TEST_TRUE(wpaS.ProcessCurMes(eapolS, SecurityWpa::CUR_MSG_STATE::MES_2));
  //Create msg 2 wpaS -> wpa
  TEST_TRUE(wpaS.CreateCurMes(eapolS2, SecurityWpa::CUR_MSG_STATE::MES_3));
  TEST_TRUE(eapolS2.Assemble(sb2, false));
  data_size = sb2.Size();

  sb2.Reset();
  sb2.Put(data_size);
  TEST_TRUE(eapol2.Disassemble(sb2, false));
  TEST_TRUE(wpa.ProcessCurMes(eapol2, SecurityWpa::CUR_MSG_STATE::MES_3));
  TEST_EQ(wpa.GetHandshakeState(), SecurityWpa::CUR_MSG_STATE::MES_3);
  uint8_t* ptk1 = wpa.GetPtk();
  uint8_t* ptk2 = wpaS.GetPtk();
  for(int i = 0; i < 16; ++i)
  {
    TEST_EQ(ptk1[i], ptk2[i]);
  }
  return(UTEST_PASS);
}

int
zSecurityTest_Handshake_Mes3(void* arg_)
{
  char psk[PMK_LEN] = {};
  psk[0] = 'w';
  psk[1] = 'p';
  psk[2] = 'a';
  psk[3] = 'T';
  psk[4] = 'e';
  psk[5] = 's';
  psk[6] = 't';
  psk[7] = 'i';
  psk[8] = 'n';
  psk[9] = 'g';
  ZLOG_DEBUG("#############################################################");
  ZLOG_DEBUG("# zSecurityTest_Handshake_Mes2()");
  ZLOG_DEBUG("#############################################################");
  zSecurity::SecurityWpa wpa;
  zSecurity::SecurityWpa wpaS;
  ieee8023::EapolKeyFrame eapol(AKM_CIPHER_SUITES::PSK);
  ieee8023::EapolKeyFrame eapolS(AKM_CIPHER_SUITES::PSK);
  ieee8023::EapolKeyFrame eapolS2(AKM_CIPHER_SUITES::PSK);
  ieee8023::EapolKeyFrame eapol2(AKM_CIPHER_SUITES::PSK);
  ieee8023::EapolKeyFrame eapolS3(AKM_CIPHER_SUITES::PSK);
  ieee8023::EapolKeyFrame eapol3(AKM_CIPHER_SUITES::PSK);

  RsnTag rsneTag;
  rsneTag(IEEE_80211_CIPHER_SUITES::CCMP_128, {IEEE_80211_CIPHER_SUITES::CCMP_128},
                             {AKM_CIPHER_SUITES::PSK});
  Buffer sb;
  Buffer sb2;
  Buffer sb3;
  TEST_TRUE(wpa.SetGroupCipher(IEEE_80211_CIPHER_SUITES::CCMP_128));
  TEST_TRUE(wpa.SetPtkCipher(IEEE_80211_CIPHER_SUITES::CCMP_128));
  TEST_TRUE(wpa.SetAkmCipher(AKM_CIPHER_SUITES::PSK));
  TEST_TRUE(wpa.SetHandshakeState(SecurityWpa::CUR_MSG_STATE::MES_1));
  TEST_TRUE(wpa.SetPmk((uint8_t*)psk));
  TEST_TRUE(wpaS.SetPmk((uint8_t*)psk));
  TEST_TRUE(wpa.SetRsnTag(rsneTag));
  TEST_TRUE(wpaS.SetRsnTag(rsneTag));
  TEST_TRUE(eapol.SetDestination(std::string("ff:ff:ff:ff:ff:ff")));
  TEST_TRUE(eapol.SetSource(std::string("00:01:02:03:04:05")));
  TEST_TRUE(eapolS2.SetSource(std::string("ff:ff:ff:ff:ff:ff")));
  TEST_TRUE(eapolS2.SetDestination(std::string("00:01:02:03:04:05")));
  TEST_TRUE(eapol3.SetDestination(std::string("ff:ff:ff:ff:ff:ff")));
  TEST_TRUE(eapol3.SetSource(std::string("00:01:02:03:04:05")));

  // Message 1
  TEST_TRUE(wpa.CreateCurMes(eapol, SecurityWpa::CUR_MSG_STATE::MES_2));
  TEST_EQ(wpa.GetReplayCntr(), 0);
  TEST_EQ(wpa.GetHandshakeState(), SecurityWpa::CUR_MSG_STATE::MES_2);
  TEST_TRUE(eapol.Assemble(sb, false));
  size_t data_size = sb.Size();
  sb.Reset();
  sb.Put(data_size);
  TEST_TRUE(eapolS.Disassemble(sb, false));
  TEST_TRUE(wpaS.SetGroupCipher(IEEE_80211_CIPHER_SUITES::CCMP_128));
  TEST_TRUE(wpaS.SetPtkCipher(IEEE_80211_CIPHER_SUITES::CCMP_128));
  TEST_TRUE(wpaS.SetAkmCipher(AKM_CIPHER_SUITES::PSK));
  TEST_TRUE(wpaS.SetHandshakeState(SecurityWpa::CUR_MSG_STATE::MES_1));
  //Process Msg 1 from wpa
  TEST_TRUE(wpaS.ProcessCurMes(eapolS, SecurityWpa::CUR_MSG_STATE::MES_2));
  //Create msg 2 wpaS -> wpa
  TEST_TRUE(wpaS.CreateCurMes(eapolS2, SecurityWpa::CUR_MSG_STATE::MES_3));
  TEST_TRUE(eapolS2.Assemble(sb2, false));
  data_size = sb2.Size();
  sb2.Reset();
  sb2.Put(data_size);
  TEST_TRUE(eapol2.Disassemble(sb2, false));
  TEST_TRUE(wpa.ProcessCurMes(eapol2, SecurityWpa::CUR_MSG_STATE::MES_3));
  //Create Mes - 3
  TEST_EQ(wpa.GetHandshakeState(), SecurityWpa::CUR_MSG_STATE::MES_3);
  TEST_TRUE(wpa.CreateCurMes(eapol3, SecurityWpa::CUR_MSG_STATE::MES_4));
  TEST_EQ(wpa.GetHandshakeState(), SecurityWpa::CUR_MSG_STATE::MES_4);
  TEST_TRUE(eapol3.Assemble(sb3, false));
  //Disassemble Mes -3
  data_size = sb3.Size();
  sb3.Reset();
  sb3.Put(data_size);

  TEST_TRUE(eapolS3.Disassemble(sb3, false));
  TEST_TRUE(wpaS.ProcessCurMes(eapolS3, SecurityWpa::CUR_MSG_STATE::MES_4));
  TEST_EQ(wpaS.GetHandshakeState(), SecurityWpa::CUR_MSG_STATE::MES_4);
  return(UTEST_PASS);
}

int
zSecurityTest_Handshake_Mes4(void* arg_)
{
  char psk[PMK_LEN] = {};
  psk[0] = 'w';
  psk[1] = 'p';
  psk[2] = 'a';
  psk[3] = 'T';
  psk[4] = 'e';
  psk[5] = 's';
  psk[6] = 't';
  psk[7] = 'i';
  psk[8] = 'n';
  psk[9] = 'g';
  ZLOG_DEBUG("#############################################################");
  ZLOG_DEBUG("# zSecurityTest_Handshake_Mes2()");
  ZLOG_DEBUG("#############################################################");
  zSecurity::SecurityWpa wpa;
  zSecurity::SecurityWpa wpaS;
  ieee8023::EapolKeyFrame eapol(AKM_CIPHER_SUITES::PSK);
  ieee8023::EapolKeyFrame eapolS(AKM_CIPHER_SUITES::PSK);
  ieee8023::EapolKeyFrame eapolS2(AKM_CIPHER_SUITES::PSK);
  ieee8023::EapolKeyFrame eapol2(AKM_CIPHER_SUITES::PSK);
  ieee8023::EapolKeyFrame eapolS3(AKM_CIPHER_SUITES::PSK);
  ieee8023::EapolKeyFrame eapol3(AKM_CIPHER_SUITES::PSK);
  ieee8023::EapolKeyFrame eapolS4(AKM_CIPHER_SUITES::PSK);
  ieee8023::EapolKeyFrame eapol4(AKM_CIPHER_SUITES::PSK);

  RsnTag rsneTag;
  rsneTag(IEEE_80211_CIPHER_SUITES::CCMP_128, {IEEE_80211_CIPHER_SUITES::CCMP_128},
                             {AKM_CIPHER_SUITES::PSK});
  Buffer sb;
  Buffer sb2;
  Buffer sb3;
  Buffer sb4;
  TEST_TRUE(wpa.SetGroupCipher(IEEE_80211_CIPHER_SUITES::CCMP_128));
  TEST_TRUE(wpa.SetPtkCipher(IEEE_80211_CIPHER_SUITES::CCMP_128));
  TEST_TRUE(wpa.SetAkmCipher(AKM_CIPHER_SUITES::PSK));
  TEST_TRUE(wpa.SetHandshakeState(SecurityWpa::CUR_MSG_STATE::MES_1));
  TEST_TRUE(wpa.SetPmk((uint8_t*)psk));
  TEST_TRUE(wpaS.SetPmk((uint8_t*)psk));
  TEST_TRUE(wpa.SetRsnTag(rsneTag));
  TEST_TRUE(wpaS.SetRsnTag(rsneTag));
  TEST_TRUE(eapol.SetDestination(std::string("ff:ff:ff:ff:ff:ff")));
  TEST_TRUE(eapol.SetSource(std::string("00:01:02:03:04:05")));
  TEST_TRUE(eapolS2.SetSource(std::string("ff:ff:ff:ff:ff:ff")));
  TEST_TRUE(eapolS2.SetDestination(std::string("00:01:02:03:04:05")));
  TEST_TRUE(eapol3.SetDestination(std::string("ff:ff:ff:ff:ff:ff")));
  TEST_TRUE(eapol3.SetSource(std::string("00:01:02:03:04:05")));
  TEST_TRUE(eapolS4.SetSource(std::string("ff:ff:ff:ff:ff:ff")));
  TEST_TRUE(eapolS4.SetDestination(std::string("00:01:02:03:04:05")));
  TEST_TRUE(wpa.SetGroupCipher(IEEE_80211_CIPHER_SUITES::CCMP_128));
  TEST_TRUE(wpa.SetPtkCipher(IEEE_80211_CIPHER_SUITES::CCMP_128));
  TEST_TRUE(wpa.SetAkmCipher(AKM_CIPHER_SUITES::PSK));
  TEST_TRUE(wpa.SetHandshakeState(SecurityWpa::CUR_MSG_STATE::MES_1));
  TEST_TRUE(wpa.SetPmk((uint8_t*)psk));
  TEST_TRUE(wpaS.SetPmk((uint8_t*)psk));
  TEST_TRUE(wpa.SetRsnTag(rsneTag));
  TEST_TRUE(wpaS.SetRsnTag(rsneTag));
  TEST_TRUE(eapol.SetDestination(std::string("ff:ff:ff:ff:ff:ff")));
  TEST_TRUE(eapol.SetSource(std::string("00:01:02:03:04:05")));
  TEST_TRUE(eapolS2.SetSource(std::string("ff:ff:ff:ff:ff:ff")));
  TEST_TRUE(eapolS2.SetDestination(std::string("00:01:02:03:04:05")));
  TEST_TRUE(eapol3.SetDestination(std::string("ff:ff:ff:ff:ff:ff")));
  TEST_TRUE(eapol3.SetSource(std::string("00:01:02:03:04:05")));

  // Message 1
  TEST_TRUE(wpa.CreateCurMes(eapol, SecurityWpa::CUR_MSG_STATE::MES_2));
  TEST_EQ(wpa.GetReplayCntr(), 0);
  TEST_EQ(wpa.GetHandshakeState(), SecurityWpa::CUR_MSG_STATE::MES_2);
  TEST_TRUE(eapol.Assemble(sb, false));
  size_t data_size = sb.Size();
  sb.Reset();
  sb.Put(data_size);
  TEST_TRUE(eapolS.Disassemble(sb, false));
  TEST_TRUE(wpaS.SetGroupCipher(IEEE_80211_CIPHER_SUITES::CCMP_128));
  TEST_TRUE(wpaS.SetPtkCipher(IEEE_80211_CIPHER_SUITES::CCMP_128));
  TEST_TRUE(wpaS.SetAkmCipher(AKM_CIPHER_SUITES::PSK));
  TEST_TRUE(wpaS.SetHandshakeState(SecurityWpa::CUR_MSG_STATE::MES_1));
  //Process Msg 1 from wpa
  TEST_TRUE(wpaS.ProcessCurMes(eapolS, SecurityWpa::CUR_MSG_STATE::MES_2));
  //Create msg 2 wpaS -> wpa
  TEST_TRUE(wpaS.CreateCurMes(eapolS2, SecurityWpa::CUR_MSG_STATE::MES_3));
  TEST_TRUE(eapolS2.Assemble(sb2, false));
  data_size = sb2.Size();
  sb2.Reset();
  sb2.Put(data_size);
  TEST_TRUE(eapol2.Disassemble(sb2, false));
  TEST_TRUE(wpa.ProcessCurMes(eapol2, SecurityWpa::CUR_MSG_STATE::MES_3));
  //Create Mes - 3
  TEST_EQ(wpa.GetHandshakeState(), SecurityWpa::CUR_MSG_STATE::MES_3);
  TEST_TRUE(wpa.CreateCurMes(eapol3, SecurityWpa::CUR_MSG_STATE::MES_4));
  TEST_EQ(wpa.GetHandshakeState(), SecurityWpa::CUR_MSG_STATE::MES_4);
  TEST_TRUE(eapol3.Assemble(sb3, false));
  //Disassemble Mes -3
  data_size = sb3.Size();
  sb3.Reset();
  sb3.Put(data_size);
  TEST_TRUE(eapolS3.Disassemble(sb3, false));
  TEST_TRUE(wpaS.ProcessCurMes(eapolS3, SecurityWpa::CUR_MSG_STATE::MES_4));
  TEST_EQ(wpaS.GetHandshakeState(), SecurityWpa::CUR_MSG_STATE::MES_4);
  TEST_TRUE(wpaS.CreateCurMes(eapolS4, SecurityWpa::COMPLETE));
  TEST_EQ(wpaS.GetHandshakeState(), SecurityWpa::COMPLETE);
  TEST_TRUE(eapolS4.Assemble(sb4, false));
  data_size = sb4.Size();
  sb4.Reset();
  sb4.Put(data_size);
  TEST_TRUE(eapol4.Disassemble(sb4,false));
  TEST_TRUE(wpa.ProcessCurMes(eapol4, SecurityWpa::COMPLETE));
  TEST_EQ(wpa.GetHandshakeState(), SecurityWpa::COMPLETE);



  return(UTEST_PASS);
}


