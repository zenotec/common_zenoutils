/*
 * Copyright (c) 2017 Cable Television Laboratories, Inc. ("CableLabs")
 *                    and others.  All rights reserved.
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

#ifndef __IEEE_EAPOLKEYFRAME_H__
#define __IEEE_EAPOLKEYFRAME_H__


#include <zutils/ieee8023/Frame.h>
#include <zutils/ieee8023/ieee8023.h>
#include <zutils/ieee80211/ieee80211.h>
#include <zutils/ieee80211/RsnTag.h>

using namespace zUtils;

namespace zUtils
{
namespace zSocket
{
namespace ieee8023
{

#define INSERT_VECTOR_LEN 16

/* Notes from IEEE-802.11-2016
 * Different EAPOL frames as defined in 802.1X-2010 however, WPA2 uses only the
 * EAPOL Key frame format.
 *
 * When priority processing of Data Frames is supported, the EAPOL-Key frame
 * shall be sent at the highest priority. Anytime an invalid field is found, the
 * packet is to be discarded.
 */

typedef struct eapol_hdr
{
  uint8_t proto_ver;
  uint8_t type;
  uint16_t length;
}__attribute__((packed)) eap_hdr;

typedef struct eapol_body
{
  uint8_t des_type;
  uint16_t key_info;
  uint16_t key_len;
  uint64_t key_replay_cntr;
  uint8_t key_nounce[NOUNCE_LEN];
  uint8_t key_iv[INSERT_VECTOR_LEN];
  uint64_t key_rsc;
  uint64_t key_id;
}__attribute__((packed)) eapol;

typedef struct gtk_kde_format
{
  uint16_t hdr;
  // The gtk is variable length and directly follows the hdr
}__attribute__((packed)) kde_gtk;

typedef struct mac_kde_format
{
  uint64_t mac_addr;
}__attribute__((packed)) kde_mac;

typedef struct pmkid_kde_format
{
  uint8_t pmkid[16];
}__attribute__((packed)) kde_pmkid;

typedef struct smk_kde_format
{
  uint8_t smk[32];
  uint8_t key_nounce[32];
}__attribute__((packed)) kde_smk;

typedef struct nounce_kde_format
{
  uint8_t key_nounce[32];
}__attribute__((packed)) kde_nounce;

typedef struct lifetime_kde_format
{
  uint32_t key_lifetime;
}__attribute__((packed)) kde_lifetime;

typedef struct error_kde_format
{
  uint16_t reserved;
  uint16_t error_type;
}__attribute__((packed)) kde_error;

typedef struct igtk_kde_format
{
  uint16_t key_id;
  uint64_t ipn;
  uint8_t* igtk;
}__attribute__((packed)) kde_igtk;

typedef struct key_id_kde_format
{
  // Standard only uses bits 0 and 1, 2-15 are reserved
  uint16_t key_id;
}__attribute__((packed)) kde_key_id;

typedef struct multiB_gtk_kde_format
{
  // Bits 0 and 1 are key Id
  // Bit 2 is Tx
  // Bit 3 to 7 are Reserved
  // Bit 8 to 15 are for Band Id
  uint16_t flags;
  uint8_t gtk[6];
}__attribute__((packed)) kde_multiB_gtk;

typedef struct multiB_key_id_format
{
  // Bits 0 and 1 are key Id
  // Bit 2 is Tx
  // Bit 3 to 7 are Reserved
  // Bit 8 to 15 are for Band Id
  uint16_t flags;
}__attribute__((packed)) kde_multiB_key_id;

typedef struct key_data_element_format
{
  uint8_t type;
  uint8_t length;
  struct rsn_oui_format oui;
  uint8_t d_type;
  union
  {
    kde_gtk gtk;
    kde_mac mac;
    kde_pmkid pmkid;
    kde_smk smk;
    kde_nounce nounce;
    kde_lifetime lifetime;
    kde_error error;
    kde_igtk igtk;
    kde_key_id key_id;
    kde_multiB_gtk multi_gtk;
    kde_multiB_key_id multi_key_id;
  } d;
}__attribute__((packed)) kde;

typedef struct eapol_key_data
{
  uint16_t len;
  uint8_t* data;
}__attribute__((packed)) key_data;

class EapolKeyFrame : public Frame
{
public:


  EapolKeyFrame(AKM_CIPHER_SUITES akm_ = AKM_CIPHER_SUITES::AKM_CIPHER_UNKNOWN);

  virtual
  ~EapolKeyFrame();

  virtual bool
  Assemble(zSocket::Buffer& sb_, bool fcs_);

  virtual bool
  Disassemble(zSocket::Buffer& sb_, bool fcs_);

  bool
  SetKeyDescVer(AKM_CIPHER_SUITES akm_,
                IEEE_80211_CIPHER_SUITES cipher_);

  eap_hdr
  GetHeader() const;

  bool
  SetHeader( eap_hdr& hdr_);

  bool
  SetProtoVer(uint8_t ver_);

  uint8_t
  GetProtoVer() const;

  bool
  SetHdrType(uint8_t type_);

  uint8_t
  GetHdrType() const;

  bool
  SetHdrLen(uint16_t len_);

  uint16_t
  GetHdrLen() const;

  bool
  SetBody(eapol& eapol_);

  eapol
  GetBody() const;

  bool
  SetNounce(const uint8_t *nounce_);

  uint8_t*
  GetNounce() const;

  uint16_t
  GetKeyDataLen() const;

  bool
  SetKeyDataLen(uint16_t len_);

  std::vector<uint8_t>
  GetEapolData() const;

  bool
  SetMIC( std::vector<uint8_t>& mic_);

  std::vector<uint8_t>
  GetMIC() const;

  bool
  SetKeyDataVals(std::vector<uint8_t>& kData_);

  std::vector<uint8_t>
  GetKeyDataVals() const;

private:

  eap_hdr _hdr;
  eapol _eapol;
  uint16_t _keyDataLen;
  std::vector<uint8_t> _eapol_data;
  std::vector<uint8_t> _mic;
  std::vector<uint8_t> _kData;
};


}
}
}



#endif //__IEEE_EAPOLKEYFRAME_H__
