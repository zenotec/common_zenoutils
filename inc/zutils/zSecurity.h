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

#ifndef __Z_SECURITY_H__
#define __Z_SECURITY_H__

#include <string>

#include <zutils/ieee8023/ieee8023.h>
#include <zutils/ieee80211/ieee80211.h>
#include <zutils/ieee8023/EapolFrame.h>
#include <zutils/ieee80211/RsnTag.h>

using namespace zSocket::ieee8023;
using namespace zUtils::zWireless::ieee80211;

namespace zUtils
{
namespace zSecurity
{

#define CCMP_PTK_LEN 48
#define GMK_LEN 16

uint8_t
MicLengthBytes(AKM_CIPHER_SUITES akm);

uint16_t
KeyLengthBytes(IEEE_80211_CIPHER_SUITES ptk);

uint8_t
KCKLengthBytes(AKM_CIPHER_SUITES akm);

uint8_t
KEKLengthBytes(AKM_CIPHER_SUITES akm);

INTEGRITY_ALGO
IntegrityAlgo(AKM_CIPHER_SUITES akm);



class Crypto
{
public:
  static const std::string PtkLabel;
  static const std::string GtkLabel;
  static const std::string GtkInitLabel;

  Crypto();

  ~Crypto();

  void
  CreateNounce(uint8_t *nounce);

  bool
  Sha_1_Prf(uint8_t *key, uint8_t *data, uint8_t *len, uint8_t *retHash);

  bool
  PairwiseExp(uint8_t *key, uint8_t *anounce, uint8_t *snounce,
              uint8_t *amac, uint8_t *smac, uint8_t *retKey);

  bool
  MicGenerateSha1(uint8_t *kck, uint8_t *data, uint8_t *len, uint8_t *mic);

  bool
  AesWrap(uint8_t *kek, uint8_t *NonEnData, uint8_t *EnData, uint8_t data_len);

  bool
  AesUnwrap(uint8_t *kek, uint8_t *NonEnData, uint8_t *EnData, uint8_t data_len);

  bool
  MakeGtk(uint8_t *amac, uint8_t *gtk);

protected:
  uint8_t _gtkId;

private:
  static const std::string GMK;
  size_t _pmk_exp_data;
  size_t _gmk_exp_data;
};


class SecurityWpa : public Crypto
{
public:
  enum CUR_MSG_STATE
  {
    NONE = 0,
    MES_1 = 1,
    MES_2 = 2,
    MES_3 = 3,
    MES_4 = 4,
    COMPLETE = 5,
    ERROR
  };

  SecurityWpa(CUR_MSG_STATE state_ = CUR_MSG_STATE::NONE,
              IEEE_80211_CIPHER_SUITES group_ = IEEE_80211_CIPHER_SUITES::CIPHER_UNKNOWN,
              IEEE_80211_CIPHER_SUITES ptk_ = IEEE_80211_CIPHER_SUITES::CIPHER_UNKNOWN,
              AKM_CIPHER_SUITES akm_ = AKM_CIPHER_SUITES::AKM_CIPHER_UNKNOWN);

  ~SecurityWpa();

  SecurityWpa::CUR_MSG_STATE
  GetHandshakeState() const;

  bool
  SetHandshakeState(CUR_MSG_STATE state_);

  bool
  ProcessCurMes(EapolKeyFrame& eapol_key_, CUR_MSG_STATE next_state_);

  bool
  CreateCurMes(EapolKeyFrame& eapol_key_, CUR_MSG_STATE next_state_);

  bool
  SetKeyDescVer(AKM_CIPHER_SUITES akm_,
                IEEE_80211_CIPHER_SUITES cipher_);

  bool
  SetGroupCipher(IEEE_80211_CIPHER_SUITES group_);

  IEEE_80211_CIPHER_SUITES
  GetGroupCipher() const;

  bool
  SetPtkCipher(IEEE_80211_CIPHER_SUITES ptk_);

  IEEE_80211_CIPHER_SUITES
  GetPtkCipher() const;

  bool
  SetAkmCipher(AKM_CIPHER_SUITES akm_);

  AKM_CIPHER_SUITES
  GetAkmCipher() const;

  bool
  SetReplayCntr(uint64_t replay_);

  uint64_t
  GetReplayCntr() const;

  bool
  SetPmk(uint8_t *pmk_);

  uint8_t*
  GetPmk();

  uint8_t*
  GetPtk();

  uint8_t*
  GetAnounce();

  uint8_t*
  GetSnounce();

  bool
  HandleMIC(uint8_t *data, uint8_t len, std::vector<uint8_t>& mic, bool verify);

  bool
  SetRsnTag(RsnTag& spprtdTag_);

  RsnTag::rsn_default
  GetRsnTag();

  bool
  CreateGtkKde(std::vector<uint8_t>& kde_);

  bool
  AddRsnTag(zWireless::ieee80211::RsnTag& rsnTag_);

  std::vector<zWireless::ieee80211::RsnTag>
  GetRsnTags() const;

  bool
  Install() const;

private:
  // Private Methods
  bool
  ProcessKeyData(std::vector<uint8_t>& keyData_, bool encypted_);

  bool
  DerivePairwiseKeys(void);

  bool
  _AssembleMesOne(EapolKeyFrame& eapol_key_);

  bool
  _AssembleMesTwo(EapolKeyFrame& eapol_key_);

  bool
  _AssembleMesThree(EapolKeyFrame& eapol_key_);

  bool
  _AssembleMesFour(EapolKeyFrame& eapol_key_);

  bool
  _DisassembleMesOne(EapolKeyFrame& eapol_key_);

  bool
  _DisassembleMesTwo(EapolKeyFrame& eapol_key_);

  bool
  _DisassembleMesThree(EapolKeyFrame& eapol_key_);

  bool
  _DisassembleMesFour(EapolKeyFrame& eapol_key_);

  void
  SetInstall(bool install_);

  SecurityWpa::CUR_MSG_STATE _state;
  IEEE_80211_CIPHER_SUITES _group_cipher;
  IEEE_80211_CIPHER_SUITES _ptk_cipher;
  AKM_CIPHER_SUITES _akm_cipher;
  bool _installed;
  uint8_t _kck_size;
  uint8_t _mic_size;
  uint8_t _kek_size;
  uint8_t _Anounce[NOUNCE_LEN] = {};
  uint8_t _Snounce[NOUNCE_LEN] = {};
  uint8_t _Amac[ETH_ALEN] = {};
  uint8_t _Smac[ETH_ALEN] = {};
  uint8_t _pmk[PMK_LEN] = {};
  uint8_t _ptkExp[CCMP_PTK_LEN] = {};
  std::vector<uint8_t> _mic;
  uint8_t* _kck;
  uint8_t* _kek;
  uint8_t* _ptk;
  uint8_t _gtk[GMK_LEN] = {};
  uint64_t _replayCtr;
  std::vector<kde> _parsedKde;
  std::vector<zWireless::ieee80211::RsnTag> _parsedRsnTag;
  RsnTag _spprtdTag;
};

}
}



#endif //__Z_SECURITY_H__
