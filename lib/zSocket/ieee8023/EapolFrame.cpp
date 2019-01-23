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

#include <stdlib.h>
#include <string.h>
#include <algorithm>
#include <iostream>
#include <fstream>
#include <iomanip>

// libzutils includes
#include <zutils/zLog.h>
#include <zutils/ieee8023/EapolFrame.h>
#include <zutils/ieee80211/ieee80211.h>
#include <zutils/zSecurity.h>
using namespace zUtils;



ZLOG_MODULE_INIT(zLog::Log::MODULE_WIRELESS);

namespace zUtils
{
namespace zSocket
{
namespace ieee8023
{



EapolKeyFrame::EapolKeyFrame(AKM_CIPHER_SUITES akm_) :
    Frame(Frame::SUBTYPE_EAPOL, Frame::PROTO_EAPOL), _keyDataLen(0)
{
  this->_mic.resize(zSecurity::MicLengthBytes(akm_));
}

EapolKeyFrame::~EapolKeyFrame()
{
}

bool
EapolKeyFrame::SetProtoVer(uint8_t ver_)
{
  this->_hdr.proto_ver = ver_;
  return(this->_hdr.proto_ver == ver_);
}

uint8_t
EapolKeyFrame::GetProtoVer() const
{
  return(this->_hdr.proto_ver);
}

bool
EapolKeyFrame::SetHdrType(uint8_t type_)
{
  this->_hdr.type = type_;
  return(this->_hdr.type == type_);
}

uint8_t
EapolKeyFrame::GetHdrType() const
{
  return(this->_hdr.type);
}

bool
EapolKeyFrame::SetHdrLen(uint16_t len_)
{
  this->_hdr.length = len_;
  return(this->_hdr.length == len_);
}

uint16_t
EapolKeyFrame::GetHdrLen() const
{
  return(this->_hdr.length);
}

bool
EapolKeyFrame::SetHeader(eap_hdr& hdr_)
{
  this->_hdr = hdr_;
  return(this->_hdr.proto_ver == hdr_.proto_ver &&
          this->_hdr.length == hdr_.length);
}

eap_hdr
EapolKeyFrame::GetHeader() const
{
  return(this->_hdr);
};

eapol
EapolKeyFrame::GetBody() const
{
  return(this->_eapol);
}

bool
EapolKeyFrame::SetBody(eapol& eapol_)
{
  this->_eapol = eapol_;
  return(this->_eapol.des_type == eapol_.des_type &&
          this->_eapol.key_nounce[0] == eapol_.key_nounce[0] &&
          this->_eapol.key_nounce[NOUNCE_LEN] == eapol_.key_nounce[NOUNCE_LEN]);
}


bool
EapolKeyFrame::SetNounce(const uint8_t *nounce_)
{
  memcpy(this->_eapol.key_nounce, nounce_, 32);
  return(this->_eapol.key_nounce[0] == nounce_[0] &&
          this->_eapol.key_nounce[31] == nounce_[31]);
}


bool
EapolKeyFrame::SetKeyDataLen(uint16_t len_)
{
  this->_keyDataLen = len_;
  return(this->_keyDataLen == len_);
}

uint16_t
EapolKeyFrame::GetKeyDataLen() const
{
  return(this->_keyDataLen);
}

std::vector<uint8_t>
EapolKeyFrame::GetEapolData() const
{
  return(this->_eapol_data);
}

bool
EapolKeyFrame::SetMIC(std::vector<uint8_t>& mic_)
{
  this->_mic.resize(mic_.size());
  this->_mic.assign(mic_.begin(), mic_.end());
  return(this->_mic == mic_);
}

std::vector<uint8_t>
EapolKeyFrame::GetMIC() const
{
  return(this->_mic);
}

bool
EapolKeyFrame::SetKeyDataVals(std::vector<uint8_t>& kData_)
{
  this->_kData.resize(kData_.size());
  this->_kData.assign(kData_.begin(), kData_.end());
  return(this->SetKeyDataLen(this->_kData.size()));
}

std::vector<uint8_t>
EapolKeyFrame::GetKeyDataVals() const
{
  return(this->_kData);
}

bool
EapolKeyFrame::Assemble(zSocket::Buffer& sb_, bool fcs_)
{
  bool status = true;

  struct ieee8023_hdr* f = (struct ieee8023_hdr*)sb_.Data();

  if (!Frame::Assemble(sb_,fcs_) ||
      this->GetSubtype() != Frame::SUBTYPE_EAPOL)
  {
    ZLOG_ERR("Error assembling EAPOL Frame");
    return(false);
  }

  // Write eapol proto
  if (sb_.Put(sizeof(f->u.eapol.proto)))
  {
    f->u.eapol.proto = htobe16(this->GetProto());
    sb_.Pull(sizeof(f->u.eapol.proto));
  }
  else
  {
    ZLOG_ERR("Failed to write protocol number in EAPOL Header");
    return(false);
  }
  eap_hdr *hdr = (eap_hdr*)f->u.eapol.data;
  uint8_t *bufPtr = (uint8_t*)f->u.eapol.data;
  if (sb_.Put(sizeof(this->_hdr)))
  {
    hdr->proto_ver = this->_hdr.proto_ver;
    hdr->type = this->_hdr.type;
    hdr->length = htobe16(this->_hdr.length);
    sb_.Pull(sizeof(this->_hdr));
  }
  else
  {
    ZLOG_ERR("Failed to put header into buffer");
    return(false);
  }
  bufPtr += sizeof(eapol_hdr);
  if(sb_.Put(sizeof(eapol_body)))
  {
    *bufPtr = this->_eapol.des_type;
    bufPtr += sizeof(this->_eapol.des_type);
    *(uint16_t*)bufPtr = htobe16(this->_eapol.key_info);
    bufPtr += sizeof(this->_eapol.key_info);
    *(uint16_t*)bufPtr = htobe16(this->_eapol.key_len);
    bufPtr += sizeof(this->_eapol.key_len);
    *(uint64_t*)bufPtr = htobe64(this->_eapol.key_replay_cntr);
    bufPtr += sizeof(this->_eapol.key_replay_cntr);
    memcpy(bufPtr, this->_eapol.key_nounce, NOUNCE_LEN);
    bufPtr += NOUNCE_LEN;
    memcpy(bufPtr, this->_eapol.key_iv, INSERT_VECTOR_LEN);
    bufPtr += INSERT_VECTOR_LEN;
    *(uint64_t*)bufPtr = htobe64(this->_eapol.key_rsc);
    bufPtr += sizeof(this->_eapol.key_rsc);
    *(uint64_t*)bufPtr = htobe64(this->_eapol.key_id);
    bufPtr += sizeof(this->_eapol.key_id);
    sb_.Pull(sizeof(eapol_body));
  }
  else
  {
    ZLOG_ERR("Failed to place body onto buffer");
    return(false);
  }

  uint8_t* mic = sb_.Data();
  if(sb_.Put(this->_mic.size() * sizeof(uint8_t)))
  {
    memcpy(mic, this->_mic.data(), this->_mic.size());
    sb_.Pull((this->_mic.size() * sizeof(uint8_t)));
  }
  else
  {
    ZLOG_ERR("Failed to place mic onto buffer");
    return(false);
  }

  size_t reqData = sizeof(this->_keyDataLen) + (sizeof(uint8_t) * this->_keyDataLen);
  if(!sb_.Put(reqData))
  {
    ZLOG_ERR("Failed to add EAPOL Key Data");
    return(false);
  }
  bufPtr = sb_.Data();
  *(uint16_t*)bufPtr = htobe16(this->_keyDataLen);
  bufPtr += 2;

  // Since Key data could be encrypted, just add the data as seen.
  memcpy(bufPtr, this->_kData.data(), this->_keyDataLen);

  status &= sb_.Pull(reqData);

  return(status);
}

bool
EapolKeyFrame::Disassemble(zSocket::Buffer& sb_, bool fcs_)
{
  bool status = true;
  struct ieee8023_hdr* f = (struct ieee8023_hdr*)sb_.Data();

  if (!Frame::Disassemble(sb_, fcs_) || (this->GetSubtype() != Frame::SUBTYPE_EAPOL))
  {
    ZLOG_ERR("Error disassembling frame");
    return(false);
  }

  if(!sb_.Pull(sizeof(f->u.eapol.proto)))
  {
    ZLOG_ERR("Failed to pull header proto");
    return(false);
  }
  uint8_t* bufPtr = f->u.eapol.data;
  this->_eapol_data.clear();
  this->_hdr.proto_ver = *bufPtr;
  bufPtr += 1;
  this->_hdr.type = *bufPtr;
  bufPtr += 1;
  this->_hdr.length = be16toh(*(uint16_t*)bufPtr);
  bufPtr += 2;

  // We need a full copy of the data in order to do decryption on
  // Data also needs to be in same structure as when performed on
  size_t dataLen = this->_hdr.length + sizeof(this->_hdr);
  this->_eapol_data.resize(dataLen);
  memcpy(this->_eapol_data.data(), (uint8_t*)&this->_hdr, sizeof(this->_hdr));
  size_t copy_body = sizeof(this->_hdr);
  if(!sb_.Pull(sizeof(eapol_hdr)))
  {
    ZLOG_ERR("Failed to pull EAPOL Header from buffer");
    return(false);
  }

  int cntr = 0;
  this->_eapol.des_type = *bufPtr;
  cntr += 1;
  this->_eapol.key_info = be16toh(*(uint16_t*)(bufPtr + cntr));
  cntr+= 2;
  this->_eapol.key_len = be16toh(*(uint16_t*)(bufPtr + cntr));
  cntr += 2;
  this->_eapol.key_replay_cntr = be64toh(*(uint64_t*)(bufPtr+cntr));
  cntr += 8;

  memcpy(this->_eapol.key_nounce, bufPtr + cntr, NOUNCE_LEN);
  cntr += NOUNCE_LEN;

  memcpy(this->_eapol.key_iv, bufPtr + cntr , INSERT_VECTOR_LEN);
  cntr += INSERT_VECTOR_LEN;

  this->_eapol.key_rsc = be64toh(*(uint64_t*)(bufPtr + cntr));
  cntr += 8;

  this->_eapol.key_id = be64toh(*(uint64_t*)(bufPtr + cntr));
  cntr+= 8;
  memcpy(this->_eapol_data.data() + copy_body,
          (uint8_t*)&this->_eapol, sizeof(this->_eapol));
  if(!status || !sb_.Pull(sizeof(eapol_body)))
  {
    ZLOG_ERR("Failed to pull EAPOL Body");
    return(false);
  }

  bufPtr = sb_.Data();
  memcpy(this->_mic.data(), bufPtr, this->_mic.size());

  copy_body += sizeof(this->_eapol);
  memcpy(this->_eapol_data.data() + copy_body, this->_mic.data(), this->_mic.size());

  bufPtr += this->_mic.size();
  copy_body += this->_mic.size();

  this->_keyDataLen = be16toh(*(uint16_t*)bufPtr);
  *(uint16_t*)(this->_eapol_data.data() + copy_body) =  this->_keyDataLen;
  copy_body += sizeof(this->_keyDataLen);

  if(!sb_.Pull(((this->_mic.size()*sizeof(uint8_t)) + sizeof(uint16_t))))
  {
    ZLOG_ERR("Failed to pull MIC and Key data");
    return(false);
  }
  bufPtr = sb_.Data();
  // Data might be encrypted, so just store all the key data info
  if(this->GetKeyDataLen() > 0)
  {
    this->_kData.resize(this->GetKeyDataLen());
    memcpy(this->_kData.data(), bufPtr, this->GetKeyDataLen());
    memcpy(this->_eapol_data.data() + copy_body, bufPtr, this->GetKeyDataLen());
  }

  status &= sb_.Pull((sizeof(uint8_t) * this->GetKeyDataLen()));

  return(status);
}



}

}

}
