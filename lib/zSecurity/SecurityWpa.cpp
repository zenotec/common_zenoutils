/*
 * Copyright (c) 2014-2019 ZenoTec LLC (http://www.zenotec.net)
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
#include <algorithm>
#include <fstream>
#include <iostream>

#include <zutils/zLog.h>
#include <zutils/zSecurity.h>
#include <zutils/ieee80211/RsnTag.h>
using namespace zUtils;
using namespace zSocket::ieee8023;
using namespace zWireless::ieee80211;

ZLOG_MODULE_INIT(zLog::Log::MODULE_SECURITY);

namespace zUtils
{
namespace zSecurity
{

uint8_t
MicLengthBytes(AKM_CIPHER_SUITES akm)
{
  switch(akm)
  {
    case IEEE_STD_PMKSA_CACHING:
    case PSK:
    case FT_AUTH_8021X_SHA256:
    case FT_AUTH_PSK_SHA256:
    case IEEE_STD_PMKSA_CACHING_SHA256:
    case PSK_SHA256:
    case TDLS_SHA256:
    case SAE_SHA256_PMSKA_CACHING:
    case FT_AUTH_SAE:
    case AP_PEERKEY_AUTH:
    case SUITE_B_EAP_SHA256:{return(16);};
    case SUITE_B_EAP_SHA384:
    case FT_AUTH_SHA384:{return(24);};
    case AKM_CIPHER_UNKNOWN:
    case AKM_CIPHER_LAST:
    default:{return(0);};
  }
}

uint16_t
KeyLengthBytes(IEEE_80211_CIPHER_SUITES ptk)
{
  switch(ptk)
  {
    case WEP_40:{ return(5); };
    case TKIP:{ return(32); };
    //Specs state default is CCMP-128
    case USE_GROUP_CIPHER:
    case CCMP_128:{ return(16); };
    case WEP_104:{ return(13); };
    case BIP_CMAC_128:{ return(16); };
    case BIP_CMAC_256:{ return(32); };
    case BIP_GMAC_256:{ return(32); };
    case BIP_GMAC_128:{ return(16); };
    case GCMP_128:{ return(16); };
    case GCMP_256:{ return(32); };
    case CCMP_256:{ return(32); };
    case CIPHER_UNKNOWN:
    case CIPHER_SUITE_LAST:
    default:{ return(0); };
  }
}

uint8_t
KEKLengthBytes(AKM_CIPHER_SUITES akm)
{
  switch(akm)
  {
    case IEEE_STD_PMKSA_CACHING:
    case PSK:
    case FT_AUTH_8021X_SHA256:
    case FT_AUTH_PSK_SHA256:
    case IEEE_STD_PMKSA_CACHING_SHA256:
    case PSK_SHA256:
    case TDLS_SHA256:
    case SAE_SHA256_PMSKA_CACHING:
    case FT_AUTH_SAE:
    case AP_PEERKEY_AUTH:
    case SUITE_B_EAP_SHA256:{return(16);};
    case SUITE_B_EAP_SHA384:
    case FT_AUTH_SHA384:{return(32);};
    case AKM_CIPHER_UNKNOWN:
    case AKM_CIPHER_LAST:
    default:{return(0);};
  }
}

uint8_t
KCKLengthBytes(AKM_CIPHER_SUITES akm)
{
  switch(akm)
  {
    case IEEE_STD_PMKSA_CACHING:
    case PSK:
    case FT_AUTH_8021X_SHA256:
    case FT_AUTH_PSK_SHA256:
    case IEEE_STD_PMKSA_CACHING_SHA256:
    case PSK_SHA256:
    case TDLS_SHA256:
    case SAE_SHA256_PMSKA_CACHING:
    case FT_AUTH_SAE:
    case AP_PEERKEY_AUTH:
    case SUITE_B_EAP_SHA256:{return(16);};
    case SUITE_B_EAP_SHA384:
    case FT_AUTH_SHA384:{return(32);};
    case AKM_CIPHER_UNKNOWN:
    case AKM_CIPHER_LAST:
    default:{return(0);};
  }
}

INTEGRITY_ALGO
IntegrityAlgo(AKM_CIPHER_SUITES akm)
{
  switch(akm)
  {
    case IEEE_STD_PMKSA_CACHING:
    case PSK: {return(INTEGRITY_ALGO::HMAC_SHA_1_128);};
    case FT_AUTH_8021X_SHA256:
    case FT_AUTH_PSK_SHA256:
    case IEEE_STD_PMKSA_CACHING_SHA256:
    case PSK_SHA256:
    case TDLS_SHA256:
    case SAE_SHA256_PMSKA_CACHING:
    case FT_AUTH_SAE: {return(INTEGRITY_ALGO::AES_128_CMAC);};
    case AP_PEERKEY_AUTH:
    case SUITE_B_EAP_SHA256:{return(INTEGRITY_ALGO::HMAC_SHA_256);};
    case SUITE_B_EAP_SHA384:
    case FT_AUTH_SHA384:{return(INTEGRITY_ALGO::HMAC_SHA_385);};
    case AKM_CIPHER_UNKNOWN:
    case AKM_CIPHER_LAST:
    default:{return(INTEGRITY_ALGO::ALGO_UNKNOWN);};
  }
}

SecurityWpa::SecurityWpa(CUR_MSG_STATE state_,
                        IEEE_80211_CIPHER_SUITES group_,
                        IEEE_80211_CIPHER_SUITES ptk_,
                        AKM_CIPHER_SUITES akm_) :
      _state(state_), _group_cipher(group_), _ptk_cipher(ptk_),
      _kck_size(KCKLengthBytes(akm_)), _mic_size(MicLengthBytes(akm_)),
      _kek_size(KEKLengthBytes(akm_)), _akm_cipher(akm_), _replayCtr(0), _installed(false)
{
  this->_kck = NULL;
  this->_kek = NULL;
  this->_ptk = NULL;
}

SecurityWpa::~SecurityWpa()
{

}

bool
SecurityWpa::SetHandshakeState(SecurityWpa::CUR_MSG_STATE state_)
{
  this->_state = state_;
  return(this->_state == state_);
}

SecurityWpa::CUR_MSG_STATE
SecurityWpa::GetHandshakeState() const
{
  return(this->_state);
}

static
void _createStndrdHdr(eap_hdr& hdr)
{
  hdr.proto_ver = VER_2004;
  hdr.type = EAP_KEY;
}

bool
SecurityWpa::SetReplayCntr(uint64_t replay_)
{
  this->_replayCtr = replay_;
  return(this->_replayCtr == replay_);
}

uint64_t
SecurityWpa::GetReplayCntr() const
{
  return(this->_replayCtr);
}

bool
SecurityWpa::SetGroupCipher(IEEE_80211_CIPHER_SUITES group_)
{
  this->_group_cipher = group_;
  return(this->_group_cipher == group_);
}

IEEE_80211_CIPHER_SUITES
SecurityWpa::GetGroupCipher() const
{
  return(this->_group_cipher);
}

bool
SecurityWpa::SetPtkCipher(IEEE_80211_CIPHER_SUITES ptk_)
{
  bool status = true;
  if(ptk_ == IEEE_80211_CIPHER_SUITES::USE_GROUP_CIPHER)
    this->_ptk_cipher = this->_group_cipher;
  else
    this->_ptk_cipher = ptk_;
  return(status);
}

IEEE_80211_CIPHER_SUITES
SecurityWpa::GetPtkCipher() const
{
  return(this->_ptk_cipher);
}

uint8_t*
SecurityWpa::GetAnounce()
{
  return(this->_Anounce);
}

uint8_t*
SecurityWpa::GetSnounce()
{
  return(this->_Snounce);
}

bool
SecurityWpa::SetAkmCipher(AKM_CIPHER_SUITES akm_)
{
  this->_akm_cipher = akm_;
  this->_mic_size = MicLengthBytes(this->_akm_cipher);
  this->_kck_size = KCKLengthBytes(this->_akm_cipher);
  this->_kek_size = KEKLengthBytes(this->_akm_cipher);
  return(this->_akm_cipher == akm_);
}

AKM_CIPHER_SUITES
SecurityWpa::GetAkmCipher() const
{
  return(this->_akm_cipher);
}

bool
SecurityWpa::SetPmk(uint8_t *pmk_)
{
  memcpy(&this->_pmk, pmk_, PMK_LEN);
  return(this->_pmk[0] == pmk_[0] &&
          this->_pmk[PMK_LEN - 1] == pmk_[PMK_LEN - 1]);
}

uint8_t*
SecurityWpa::GetPmk()
{
  return(this->_pmk);
}

bool
SecurityWpa::SetRsnTag(RsnTag& spprtdTag_)
{
  this->_spprtdTag(spprtdTag_);
  return(true);
}

RsnTag::rsn_default
SecurityWpa::GetRsnTag()
{
  return(this->_spprtdTag());
}

bool
SecurityWpa::AddRsnTag(zWireless::ieee80211::RsnTag& rsnTag_)
{
  this->_parsedRsnTag.push_back(rsnTag_);
  return(true);
}

std::vector<zWireless::ieee80211::RsnTag>
SecurityWpa::GetRsnTags() const
{
  return(this->_parsedRsnTag);
}

bool
SecurityWpa::ProcessKeyData(std::vector<uint8_t>& keyData_, bool encrypted_)
{
  bool status = true;
  uint8_t cntr = 0;
  uint8_t* ptr = keyData_.data();
  kde* tmpKde;

  // Elements in key data are either RsnTags, or KDE
  // Currently we only support GTK KDE and a RSN tag
  while(cntr < (keyData_.size()-1))
  {
    switch(*(ptr+cntr))
    {
      // If type is 0xdd then it's a KDE
      case 0xdd:
      {
        // This means we are at a padding
        if(*(ptr+cntr+1) == 0x00)
        {
          cntr = keyData_.size();
          break;
        }
        tmpKde = (kde*)(ptr+cntr);
        if(tmpKde->d_type == KDE_DATA_TYPES::KDE_GTK &&
            true == encrypted_)
        {
          uint8_t* tPtr = (uint8_t*)&tmpKde->d.gtk.hdr + sizeof(uint16_t);
          memcpy(this->_gtk, tPtr, GMK_LEN);
        }
        // Currently we don't care about any other KDE's
        cntr += tmpKde->length + 2;
        break;
      }
       // If type is 0x30 then it's a RsnTag
      case 0x30:
      {
        RsnTag tmpTag;
        std::vector<uint8_t>::iterator it;
        std::vector<uint8_t> rsne;
        uint8_t len = *(ptr+cntr+1);
        it = keyData_.begin() + cntr + 2;
        rsne.assign(it, it+len);
        tmpTag.ParseTag(rsne);
        this->AddRsnTag(tmpTag);
        cntr += len + 2;
        break;
      }
      default:
        ZLOG_ERR("Unknown type in key data field");
        status = false;
        cntr = keyData_.size();
        break;
    }
  }
  return(status);
}

bool
SecurityWpa::CreateGtkKde(std::vector<uint8_t>& kde_)
{
  bool status = true;

  kde key_data_element;

  if(true == (status = MakeGtk(this->_Amac, this->_gtk)))
  {
    // Build KDE Element to IEEE-802.11-2016 standards
    key_data_element.type = 0xdd;
    key_data_element.oui.oui_b1 = 0x00;
    key_data_element.oui.oui_b2 = 0x0f;
    key_data_element.oui.oui_b3 = 0xac;
    key_data_element.d_type = (uint8_t)KDE_DATA_TYPES::KDE_GTK;
    uint16_t gtkHdr = this->_gtkId & 0x0040;
    key_data_element.length = 6 + GMK_LEN;
    kde_.push_back(key_data_element.type);
    kde_.push_back(key_data_element.length);
    kde_.push_back(key_data_element.oui.oui_b1);
    kde_.push_back(key_data_element.oui.oui_b2);
    kde_.push_back(key_data_element.oui.oui_b3);
    kde_.push_back(key_data_element.d_type);
    kde_.push_back(gtkHdr);
    kde_.push_back(gtkHdr >> 8);
    for(int i = 0; i < GMK_LEN; ++i)
    {
      kde_.push_back(this->_gtk[i]);
    }
  }
  else
  {
    ZLOG_ERR("Failed to create GTK");
  }

  return(status);
}

bool
SecurityWpa::DerivePairwiseKeys()
{
  bool status = PairwiseExp(this->_pmk, this->_Anounce, this->_Snounce,
                            this->_Amac, this->_Smac, this->_ptkExp);
  if(status)
  {
    this->_kck = this->_ptkExp;
    this->_kek = &this->_ptkExp[this->_kck_size];
    this->_ptk = &this->_ptkExp[this->_kck_size + this->_kek_size];
  }

  return(status);
}

uint8_t*
SecurityWpa::GetPtk()
{
  return(this->_ptk);
}

bool
SecurityWpa::HandleMIC(uint8_t *data, uint8_t len, std::vector<uint8_t>& mic, bool verify)
{
  bool status = true;
  uint8_t lenAr[] = {this->_kck_size, len, this->_mic_size};
  //Handle the verification of a MIC
  if(verify)
  {
    std::vector<uint8_t> newMic(this->_mic_size, 0);
    // In data, MIC will have to be zeroed out
    size_t micPlace = sizeof(eap_hdr) + sizeof(eapol);
    memcpy((data+micPlace), newMic.data(), newMic.size());
    if(true == (status &= MicGenerateSha1(this->_kck, data, lenAr, newMic.data())))
    {
      if(newMic != mic)
      {
        ZLOG_ERR("Failed to verify correct MIC");
        status = false;
      }
    }
  }
  // Else create a MIC to place into message
  else
  {
    status &= MicGenerateSha1(this->_kck, data, lenAr, mic.data());
  }

  return(status);
}

bool
SecurityWpa::ProcessCurMes(EapolKeyFrame& eapol_key_, CUR_MSG_STATE next_state_)
{
  bool status = true;
  switch(this->_state)
  {
    case MES_1:
      status &= _DisassembleMesOne(eapol_key_);
      break;
    case MES_2:
      status &= _DisassembleMesTwo(eapol_key_);
      break;
    case MES_3:
      status &= _DisassembleMesThree(eapol_key_);
      break;
    case MES_4:
      status &= _DisassembleMesFour(eapol_key_);
      break;
    case COMPLETE:
      ZLOG_ERR("Finished with EAPOL Handshake, no message to create");
      status &= false;
      break;
      // Currently unhandled cases
    case NONE:
    case ERROR:
    default:
      ZLOG_ERR("In an un-handled state for processing EAPOL Key message");
      status &= false;
  }
  if(status)
  {
    this->SetHandshakeState(next_state_);
  }

  return(status);
}

bool
SecurityWpa::CreateCurMes(EapolKeyFrame& eapol_key_, CUR_MSG_STATE next_state_)
{
  bool status = true;

  switch(this->_state)
  {
    case MES_1:
      status &= _AssembleMesOne(eapol_key_);
      break;
    case MES_2:
      status &= _AssembleMesTwo(eapol_key_);
      break;
    case MES_3:
      status &= _AssembleMesThree(eapol_key_);
      break;
    case MES_4:
      status &= _AssembleMesFour(eapol_key_);
      break;
    case COMPLETE:
      ZLOG_ERR("Finished with EAPOL Handshake, no message to create");
      status &= false;
      break;
    case NONE:
    case ERROR:
    default:
      ZLOG_ERR("Attempting to create a message in an un-handled state");
      status &= false;
  }
  if(status)
  {
    this->SetHandshakeState(next_state_);
  }
  return(status);
}


bool
SecurityWpa::_AssembleMesOne(EapolKeyFrame& eapol_key_)
{
  bool status = true;

  // Get Source and Dest address to check on return
  eapol_key_.str2mac(eapol_key_.GetSource(), this->_Amac);
  eapol_key_.str2mac(eapol_key_.GetDestination(), this->_Smac);

  // First Prepare Header
  //Will set length at end
  eap_hdr eHdr = {};
  eapol_body eBody = {};

  _createStndrdHdr(eHdr);

  // Prepare EapolBody
  eBody.des_type = IEEE_80211_KEY_DESCRIPTOR;
  eBody.key_info |= ( HMAC_SHA_1 | PTK_DERIVATION | KEY_ACK );
  eBody.key_len = KeyLengthBytes(this->_ptk_cipher);
  this->_mic.resize(this->_mic_size, 0);
  CreateNounce(this->_Anounce);
  memcpy(eBody.key_nounce, this->_Anounce, NOUNCE_LEN);

  eBody.key_replay_cntr = this->GetReplayCntr();

  eHdr.length = sizeof(eapol_body) + (sizeof(uint8_t) * this->_mic_size) + sizeof(uint16_t);

  status &= eapol_key_.SetHeader(eHdr);
  status &= eapol_key_.SetBody(eBody);
  status &= eapol_key_.SetMIC(this->_mic);
  status &= eapol_key_.SetKeyDataLen(0);

  return(status);
}

bool
SecurityWpa::_AssembleMesTwo(EapolKeyFrame& eapol_key_)
{
  bool status = true;
  eap_hdr eHdr = {};
  eapol_body eBody = {};
  key_data ekBody = {0, NULL};
  std::vector<uint8_t> buff;
  _createStndrdHdr(eHdr);
  this->_mic.resize(this->_mic_size, 0);

  eBody.des_type = IEEE_80211_KEY_DESCRIPTOR;
  eBody.key_info |= ( HMAC_SHA_1 | PTK_DERIVATION | KEY_MIC );
  eBody.key_len = 0;
  eBody.key_replay_cntr = this->GetReplayCntr();

  CreateNounce(this->_Snounce);
  memcpy(eBody.key_nounce, this->_Snounce, NOUNCE_LEN);

  // Derive Keys now that Nounce has been set
  // Will be needed in order to calculate MIC

  if(!this->DerivePairwiseKeys())
  {
    ZLOG_ERR("Failed to expand PTK");
    return(false);
  }
  std::vector<uint8_t> keyD;
  std::vector<uint8_t> rsn;
  RsnTag::rsn_default rsne = this->GetRsnTag();
  uint8_t tag_num = 48;
  rsn = this->_spprtdTag.GetPayload();
  keyD.push_back(tag_num);
  keyD.push_back((uint8_t)rsn.size());
  keyD.insert(keyD.end(), rsn.begin(), rsn.end());

  ekBody.len = keyD.size();
  eHdr.length = sizeof(eapol_body) + (sizeof(uint8_t) * this->_mic_size) +
                  sizeof(ekBody.len) + ekBody.len;
  buff.resize(eHdr.length + 4);
  size_t copy_body = sizeof(eHdr);
  memcpy(buff.data(), (uint8_t*)&eHdr, sizeof(eHdr));

  memcpy(buff.data() + copy_body, (uint8_t*)&eBody, sizeof(eBody));
  copy_body += sizeof(eBody);

  memcpy(buff.data() + copy_body, this->_mic.data(), this->_mic.size());
  copy_body += this->_mic.size();

  *(uint16_t*)(buff.data() + copy_body) = ekBody.len;
  copy_body += sizeof(ekBody.len);

  memcpy(buff.data() + copy_body, keyD.data(), keyD.size());
  if(!this->HandleMIC(buff.data(), buff.size(), this->_mic, false))
  {
    ZLOG_ERR("Failed to create mic");
    return(false);
  }
  if(!(status &= eapol_key_.SetHeader(eHdr) ) )
  {
    ZLOG_ERR("Setting Header");
    return(status);
  }
  if(!(status &= eapol_key_.SetBody(eBody) ))
  {
    ZLOG_ERR("Setting Body");
    return(status);
  }
  if(!(status &= eapol_key_.SetMIC(this->_mic)))
  {
    ZLOG_ERR("Setting Mic");
    return(status);
  }
  if(!(status &= eapol_key_.SetKeyDataVals(keyD)))
  {
    ZLOG_ERR("Setting data");
  }
  return(status);
}

bool
SecurityWpa::_AssembleMesThree(EapolKeyFrame& eapol_key_)
{
  bool status = true;
  eap_hdr eHdr = {};
  eapol_body eBody = {};
  key_data ekBody = {0, NULL};
  std::vector<uint8_t> buff;
  _createStndrdHdr(eHdr);
  this->_mic.clear();
  this->_mic.resize(this->_mic_size, 0);
  eBody.key_info |=(HMAC_SHA_1|PTK_DERIVATION|KEY_MIC|INSTALL|KEY_ACK|ENCRYPTED);
  eBody.key_len = KeyLengthBytes(this->_ptk_cipher);
  eBody.des_type = IEEE_80211_KEY_DESCRIPTOR;
  memcpy(eBody.key_nounce, this->_Anounce, NOUNCE_LEN);
  eBody.key_replay_cntr = this->GetReplayCntr();

  std::vector<uint8_t> keyD;
  std::vector<uint8_t> rsn;
  uint8_t tag_num = 48;
  rsn = this->_spprtdTag.GetPayload();
  keyD.push_back(tag_num);
  keyD.push_back((uint8_t)rsn.size());
  keyD.insert(keyD.end(), rsn.begin(), rsn.end());

  status &= this->CreateGtkKde(keyD);

  if(0 != keyD.size() % 8)
  {
    //Need to pad before encrypting
    keyD.push_back(0xdd);
    while(keyD.size() % 8 != 0)
      keyD.push_back(0x00);
  }

  std::vector<uint8_t> cipherText(keyD.size() + 8);
  if(!this->AesWrap(this->_kek, keyD.data(), cipherText.data(), keyD.size()))
  {
    ZLOG_ERR("Failed to encrypt key data");
    return(false);
  }

  ekBody.len = cipherText.size();
  eHdr.length = sizeof(eapol_body) + (sizeof(uint8_t) * this->_mic_size) +
                  sizeof(ekBody.len) + ekBody.len;
  buff.resize(eHdr.length + 4);
  size_t copy_body = sizeof(eHdr);
  memcpy(buff.data(), (uint8_t*)&eHdr, sizeof(eHdr));

  memcpy(buff.data() + copy_body, (uint8_t*)&eBody, sizeof(eBody));
  copy_body += sizeof(eBody);

  memcpy(buff.data() + copy_body, this->_mic.data(), this->_mic.size());
  copy_body += this->_mic.size();

  *(uint16_t*)(buff.data() + copy_body) = ekBody.len;
  copy_body += sizeof(ekBody.len);

  memcpy(buff.data() + copy_body, cipherText.data(), cipherText.size());
  if(!this->HandleMIC(buff.data(), buff.size(), this->_mic, false))
  {
    ZLOG_ERR("Failed to create mic");
    return(false);
  }

  if(!(status &= eapol_key_.SetHeader(eHdr) ) )
  {
    ZLOG_ERR("Setting Header");
    return(status);
  }
  if(!(status &= eapol_key_.SetBody(eBody) ))
  {
    ZLOG_ERR("Setting Body");
    return(status);
  }
  if(!(status &= eapol_key_.SetMIC(this->_mic)))
  {
    ZLOG_ERR("Setting Mic");
    return(status);
  }
  if(!(status &= eapol_key_.SetKeyDataVals(cipherText)))
  {
    ZLOG_ERR("Setting data");
  }
  return(status);
}

bool
SecurityWpa::_AssembleMesFour(EapolKeyFrame& eapol_key_)
{
  bool status = true;

  // First Prepare Header
  //Will set length at end
  eap_hdr eHdr = {};
  eapol_body eBody = {};

  _createStndrdHdr(eHdr);
  // Prepare EapolBody
  eBody.des_type = IEEE_80211_KEY_DESCRIPTOR;
  eBody.key_info |= (HMAC_SHA_1|PTK_DERIVATION|KEY_MIC|SECURE);
  eBody.key_len = 0;
  eBody.key_replay_cntr = this->GetReplayCntr();
  this->_mic.clear();
  this->_mic.resize(this->_mic_size, 0);

  eHdr.length = sizeof(eapol_body) + (sizeof(uint8_t) * this->_mic_size) +
                sizeof(uint16_t);
  size_t reqbuf = eHdr.length + sizeof(eap_hdr);
  std::vector<uint8_t> buff(reqbuf);
  uint8_t cntr = 0;

  memcpy(buff.data(), (uint8_t*)&eHdr, sizeof(eap_hdr));
  cntr += sizeof(eap_hdr);
  memcpy(buff.data() + cntr, (uint8_t*)&eBody, sizeof(eapol_body));
  cntr+=sizeof(eapol_body);
  memcpy(buff.data() + cntr, this->_mic.data(), this->_mic.size());
  cntr+=this->_mic.size();

  // Set Key length
  *(uint16_t*)(buff.data()+cntr) = 0;

  if(!this->HandleMIC(buff.data(), buff.size(), this->_mic, false))
  {
    ZLOG_ERR("Failed to create mic");
    return(false);
  }
  if(!(status &= eapol_key_.SetHeader(eHdr) ) )
  {
    ZLOG_ERR("Setting Header");
    return(status);
  }
  if(!(status &= eapol_key_.SetBody(eBody) ))
  {
    ZLOG_ERR("Setting Body");
    return(status);
  }
  if(!(status &= eapol_key_.SetMIC(this->_mic)))
  {
    ZLOG_ERR("Setting Mic");
    return(status);
  }
  status &= eapol_key_.SetKeyDataLen(0);

  return(status);
}

bool
SecurityWpa::_DisassembleMesOne(EapolKeyFrame& eapol_key_)
{
  bool status = true;
  eapol_key_.str2mac(eapol_key_.GetSource(), this->_Amac);
  eapol_key_.str2mac(eapol_key_.GetDestination(), this->_Smac);
  // Get header from frame
  eap_hdr eHdr = eapol_key_.GetHeader();
  eapol_body eBody = eapol_key_.GetBody();
  this->_mic.resize(this->_mic_size);
  this->_mic = eapol_key_.GetMIC();

  //key_data ekBody = eapol_key_.GetKeyData();
  uint16_t keyDataLen = eapol_key_.GetKeyDataLen();

  // Message one important info
  // Protocol Version is either 2001 or 2004
  if(eHdr.proto_ver != VER_2001 && eHdr.proto_ver != VER_2004)
  {
    ZLOG_ERR("Unsupported Protocol Version in EAPOL Header");
    status &= false;
  }

  // Type in Header is EAPOL_KEY
  if(eHdr.type != EAP_KEY)
  {
    ZLOG_ERR("Unsupported EAPOL Type in Header");
    status &= false;
  }

  // Verify flags are set in the key info field
  if(!(eBody.key_info & (HMAC_SHA_1 | PTK_DERIVATION | KEY_ACK)))
  {
    ZLOG_ERR("Key Information is in-correct");
    status &= false;
  }

  // Verify Key Length matches what is to be expected
  if(KeyLengthBytes(this->_ptk_cipher) != eBody.key_len)
  {
    ZLOG_ERR("Key length mismatch");
    status &= false;
  }

  // Get the Anounce value
  memcpy(this->_Anounce, eBody.key_nounce, NOUNCE_LEN);



  return(status);
}

bool
SecurityWpa::_DisassembleMesTwo(EapolKeyFrame& eapol_key_)
{
  bool status = true;
  bool spprtedRsne = false;
  eap_hdr eHdr = eapol_key_.GetHeader();
  eapol_body eBody = eapol_key_.GetBody();
  this->_mic.resize(this->_mic_size);
  this->_mic = eapol_key_.GetMIC();
  std::vector<uint8_t> tmpData = eapol_key_.GetEapolData();
  std::vector<uint8_t> tmpKeyData = eapol_key_.GetKeyDataVals();
  //key_data ekBody = eapol_key_.GetKeyData();
  RsnTag::rsn_default localRsne = this->_spprtdTag();

  if(eHdr.proto_ver != VER_2001 && eHdr.proto_ver != VER_2004)
  {
    ZLOG_ERR("Unsupported Protocol Version in EAPOL Header");
    status &= false;
  }

  // Verify flags are set in the key info field
  if(!(eBody.key_info & (HMAC_SHA_1 | PTK_DERIVATION)))
  {
    ZLOG_ERR("Key Information is in-correct");
    status &= false;
  }

  if(0 != eBody.key_len)
  {
    ZLOG_ERR("Key length mismatch");
    status &= false;
  }

  //Get Nounce value, to derive keys
  memcpy(this->_Snounce, eBody.key_nounce, NOUNCE_LEN);

  //Derive Keys
  if(!this->DerivePairwiseKeys())
  {
    ZLOG_ERR("Failed to derive pairwise keys");
    return(false);
  }

  //Verify Correct Mic
  if(!(eBody.key_info & KEY_MIC) ||
      !this->HandleMIC(tmpData.data(), tmpData.size(), this->_mic, true))
  {
    ZLOG_ERR("Failed handling MIC");
    return(false);
  }

  if(!this->ProcessKeyData(tmpKeyData, (eBody.key_info & ENCRYPTED)))
  {
    ZLOG_ERR("Failed processing key data");
    return(false);
  }

  // Verify RSNE Tag sent in message matches our Rsne
  FOREACH(auto&l, this->_parsedRsnTag)
  {
    RsnTag::rsn_default tmpRsne = l();
    if(localRsne == tmpRsne)
    {
      spprtedRsne = true;
      break;
    }
  }

  if(!spprtedRsne)
  {
    ZLOG_ERR("Station does not support same RSNE, quitting handshake");
    return(false);
  }

  if(!this->SetReplayCntr(1))
  {
    ZLOG_ERR("Failed to set replay counter");
    return(false);
  }

  return(status);
}

bool
SecurityWpa::_DisassembleMesThree(EapolKeyFrame& eapol_key_)
{
  bool status = true;
  bool spprtedRsne = false;
  eap_hdr eHdr = eapol_key_.GetHeader();
  eapol_body eBody = eapol_key_.GetBody();
  this->_mic.resize(this->_mic_size);
  this->_mic = eapol_key_.GetMIC();
  std::vector<uint8_t> tmpData = eapol_key_.GetEapolData();
  std::vector<uint8_t> tmpKeyData = eapol_key_.GetKeyDataVals();
  std::vector<uint8_t> unEncryptedKeyData((tmpKeyData.size() - 8));
  //key_data ekBody = eapol_key_.GetKeyData();
  RsnTag::rsn_default localRsne = this->_spprtdTag();

  if(eHdr.proto_ver != VER_2001 && eHdr.proto_ver != VER_2004)
  {
    ZLOG_ERR("Unsupported Protocol Version in EAPOL Header");
    return(false);
  }

  // Verify flags are set in the key info field
  if(!(eBody.key_info & (HMAC_SHA_1|PTK_DERIVATION|INSTALL|KEY_ACK)))
  {
    ZLOG_ERR("Key Information is in-correct");
    return(false);
  }

  if(KeyLengthBytes(this->_ptk_cipher) != eBody.key_len)
  {
    ZLOG_ERR("Key length mismatch");
    return(false);
  }
  //Verify Nounce is the same as the ANOUNCE from message 1
  for(int i = 0; i < NOUNCE_LEN; ++i)
  {
    if(this->_Anounce[i] != eBody.key_nounce[i])
    {
      ZLOG_ERR("Anounce from Mes 3 does not equal Anounce Mes 1");
      return(false);
    }
  }

  this->SetReplayCntr(eBody.key_replay_cntr);

  //Verify Correct Mic
  if(!(eBody.key_info & KEY_MIC) ||
      !this->HandleMIC(tmpData.data(), tmpData.size(), this->_mic, true))
  {
    ZLOG_ERR("Failed handling MIC");
    return(false);
  }

  if(eBody.key_info & INSTALL)
  {
    this->SetInstall(true);
  }

  if(eBody.key_info & ENCRYPTED)
  {
    if(!this->AesUnwrap(this->_kek, unEncryptedKeyData.data(),
        tmpKeyData.data(), tmpKeyData.size()))
    {
      ZLOG_ERR("Failed to encrypt key data");
      return(false);
    }
  }

  if(!this->ProcessKeyData(unEncryptedKeyData, (eBody.key_info & ENCRYPTED)))
  {
    ZLOG_ERR("Failed processing key data");
    return(false);
  }

  // Verify RSNE Tag sent in message matches our Rsne
  FOREACH(auto&l, this->_parsedRsnTag)
  {
    RsnTag::rsn_default tmpRsne = l();
    if(localRsne == tmpRsne)
    {
      spprtedRsne = true;
      break;
    }
  }

  if(!spprtedRsne)
  {
    ZLOG_ERR("Station does not support same RSNE, quitting handshake");
    return(false);
  }

  return(status);
}

bool
SecurityWpa::_DisassembleMesFour(EapolKeyFrame& eapol_key_)
{
  bool status = true;
  eap_hdr eHdr = eapol_key_.GetHeader();
  eapol_body eBody = eapol_key_.GetBody();
  this->_mic.clear();
  this->_mic.resize(this->_mic_size);
  this->_mic = eapol_key_.GetMIC();
  std::vector<uint8_t> tmpData = eapol_key_.GetEapolData();

  //key_data ekBody = eapol_key_.GetKeyData();
  uint16_t keyDataLen = eapol_key_.GetKeyDataLen();

  // Message one important info
  // Protocol Version is either 2001 or 2004
  if(eHdr.proto_ver != VER_2001 && eHdr.proto_ver != VER_2004)
  {
    ZLOG_ERR("Unsupported Protocol Version in EAPOL Header");
    status &= false;
  }

  // Type in Header is EAPOL_KEY
  if(eHdr.type != EAP_KEY)
  {
    ZLOG_ERR("Unsupported EAPOL Type in Header");
    status &= false;
  }

  // Verify flags are set in the key info field
  if(!(eBody.key_info & (HMAC_SHA_1|KEY_MIC|SECURE|PTK_DERIVATION)))
  {
    ZLOG_ERR("Key Information is in-correct");
    status &= false;
  }

  // Verify Key Length matches what is to be expected
  if(0 != eBody.key_len)
  {
    ZLOG_ERR("Key length mismatch");
    status &= false;
  }

  if(eBody.key_replay_cntr != this->GetReplayCntr())
  {
    ZLOG_ERR("Replay counter was incorrect on final message");
    return(false);
  }

  //Verify Correct Mic
  if(!this->HandleMIC(tmpData.data(), tmpData.size(), this->_mic, true))
  {
    ZLOG_ERR("Failed handling MIC");
    return(false);
  }

  this->SetInstall(true);

  return(status);
}

void
SecurityWpa::SetInstall(bool install_)
{
  this->_installed = install_;
}

bool
SecurityWpa::Install() const
{
  return(this->_installed);
}

}
}
