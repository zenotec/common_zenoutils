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

// libc includes
#include <stdlib.h>
#include <endian.h>

// libc++ includes
#include <iostream>

// libzutils includes
#include <zutils/zLog.h>

// local includes
#include "DataFrame.h"

namespace zUtils
{
namespace ieee80211
{

static Frame::ADDRESS_ID
_da_addrid(bool tods_, bool fromds_)
{
  Frame::ADDRESS_ID id = Frame::ADDRESS_ERR;
  if (!tods_ && !fromds_)
  {
    id = Frame::ADDRESS_1;
  }
  else if (!tods_ && fromds_)
  {
    id = Frame::ADDRESS_1;
  }
  else if (tods_ && !fromds_)
  {
    id = Frame::ADDRESS_3;
  }
  else
  {
    id = Frame::ADDRESS_3;
  }
  return(id);
}

static Frame::ADDRESS_ID
_sa_addrid(bool tods_, bool fromds_)
{
  Frame::ADDRESS_ID id = Frame::ADDRESS_ERR;
  if (!tods_ && !fromds_)
  {
    id = Frame::ADDRESS_2;
  }
  else if (!tods_ && fromds_)
  {
    id = Frame::ADDRESS_3;
  }
  else if (tods_ && !fromds_)
  {
    id = Frame::ADDRESS_2;
  }
  else
  {
    id = Frame::ADDRESS_4;
  }
  return(id);
}

static Frame::ADDRESS_ID
_bssid_addrid(bool tods_, bool fromds_)
{
  Frame::ADDRESS_ID id = Frame::ADDRESS_ERR;
  if (!tods_ && !fromds_)
  {
    id = Frame::ADDRESS_3;
  }
  else if (!tods_ && fromds_)
  {
    id = Frame::ADDRESS_2;
  }
  else if (tods_ && !fromds_)
  {
    id = Frame::ADDRESS_1;
  }
  else
  {
    id = Frame::ADDRESS_3;
  }
  return(id);
}

//*****************************************************************************
// Class: DataFrame
//*****************************************************************************

DataFrame::DataFrame(const Frame::SUBTYPE subtype_) :
    Frame(Frame::TYPE_CNTL)
{
  this->Subtype(subtype_);
}

DataFrame::~DataFrame()
{
}

uint8_t*
DataFrame::Assemble(uint8_t* p_, size_t& rem_, bool fcs_)
{

  p_ = Frame::Assemble(p_, rem_, fcs_);
  ieee80211_datahdr* f = (ieee80211_datahdr*) p_;

  if (f == NULL)
  {
    ZLOG_WARN("Error assembling data frame: " + ZLOG_INT(rem_));
    return (NULL);
  }

  if (this->Type() != Frame::TYPE_DATA)
  {
    ZLOG_WARN("Frame type not data");
    return (NULL);
  }

  p_ = this->chklen(p_, sizeof(f->addr1), rem_);
  if (!p_ || this->Address(ADDRESS_1).empty() || !this->str2mac(this->Address(ADDRESS_1), f->addr1))
  {
    return (NULL);
  }

  p_ = this->chklen(p_, sizeof(f->addr2), rem_);
  if (!p_ || this->Address(ADDRESS_2).empty() || !this->str2mac(this->Address(ADDRESS_2), f->addr2))
  {
    return (NULL);
  }

  p_ = this->chklen(p_, sizeof(f->addr3), rem_);
  if (!p_ || this->Address(ADDRESS_3).empty() || !this->str2mac(this->Address(ADDRESS_3), f->addr3))
  {
    return (NULL);
  }

  p_ = this->chklen(p_, sizeof(f->seqcntl), rem_);
  if (p_)
  {
    f->seqcntl = htole16(this->SequenceControl());
  }

  if (this->Subtype() & Frame::SUBTYPE_DATAQOS)
  {
    p_ = this->chklen(p_, sizeof(f->u.qosdata.qoscntl), rem_);
    if (p_)
    {
      f->u.qosdata.qoscntl = htole16(this->QosControl());
    }
  }

  uint8_t* pay = p_;
  size_t len = this->GetPayloadLength();
  p_ = this->chklen(pay, len, rem_);
  if (!p_ || (this->GetPayload(pay, len) != len))
  {
    return (NULL);
  }

  return (p_);
}

uint8_t*
DataFrame::Disassemble(uint8_t* p_, size_t& rem_, bool fcs_)
{

  p_ = Frame::Disassemble(p_, rem_, fcs_);
  ieee80211_datahdr* f = (ieee80211_datahdr*) p_;

  if (f == NULL)
  {
    ZLOG_WARN("Error disassembling data frame: " + ZLOG_INT(rem_));
    return (NULL);
  }

  if (this->Type() != Frame::TYPE_DATA)
  {
    ZLOG_WARN("Frame type not data");
    return (NULL);
  }

  p_ = this->chklen(p_, sizeof(f->addr1), rem_);
  if (!p_ || !this->Address(ADDRESS_1, f->addr1))
  {
    return (NULL);
  }

  p_ = this->chklen(p_, sizeof(f->addr2), rem_);
  if (!p_ || !this->Address(ADDRESS_2, f->addr2))
  {
    return (NULL);
  }

  p_ = this->chklen(p_, sizeof(f->addr3), rem_);
  if (!p_ || !this->Address(ADDRESS_3, f->addr3))
  {
    return (NULL);
  }

  p_ = this->chklen(p_, sizeof(f->seqcntl), rem_);
  if (!p_ || !this->SequenceControl(le16toh(f->seqcntl)))
  {
    return(NULL);
  }

  if (this->Subtype() & Frame::SUBTYPE_DATAQOS)
  {
    p_ = this->chklen(p_, sizeof(f->u.qosdata.qoscntl), rem_);
    if (!p_ || !this->QosControl(le16toh(f->u.qosdata.qoscntl)))
    {
      return(NULL);
    }
  }

  uint8_t* pay = p_;
  size_t len = rem_;
  p_ = this->chklen(pay, len, rem_);
  if (!p_ || !this->PutPayload(pay, len))
  {
    return(NULL);
  }

  return (p_);
}

std::string
DataFrame::ReceiverAddress() const
{
  return(this->Address(Frame::ADDRESS_1));
}

bool
DataFrame::ReceiverAddress(const std::string& address_)
{
  return(this->Address(Frame::ADDRESS_1, address_));
}

std::string
DataFrame::TransmitterAddress() const
{
  return(this->Address(Frame::ADDRESS_2));
}

bool
DataFrame::TransmitterAddress(const std::string& address_)
{
  return(this->Address(Frame::ADDRESS_2, address_));
}

std::string
DataFrame::DestinationAddress() const
{
  Frame::ADDRESS_ID id = _da_addrid(this->ToDS(), this->FromDS());
  return (this->Address(id));
}

bool
DataFrame::DestinationAddress(const std::string& address_)
{
  Frame::ADDRESS_ID id = _da_addrid(this->ToDS(), this->FromDS());
  return (this->Address(id, address_));
}

std::string
DataFrame::SourceAddress() const
{
  Frame::ADDRESS_ID id = _sa_addrid(this->ToDS(), this->FromDS());
  return (this->Address(id));
}

bool
DataFrame::SourceAddress(const std::string& address_)
{
  Frame::ADDRESS_ID id = _sa_addrid(this->ToDS(), this->FromDS());
  return (this->Address(id, address_));
}

std::string
DataFrame::Bssid() const
{
  Frame::ADDRESS_ID id = _bssid_addrid(this->ToDS(), this->FromDS());
  return (this->Address(id));
}

bool
DataFrame::Bssid(const std::string& address_)
{
  Frame::ADDRESS_ID id = _bssid_addrid(this->ToDS(), this->FromDS());
  return (this->Address(id, address_));
}

void
DataFrame::Display() const
{
#ifdef DISPLAY_DATA_FRAMES
  Frame::Display();
  std::cout << "----- IEEE802.11 Data Header -------------" << std::endl;
  std::cout << "\tRA:       \t" << this->ReceiverAddress() << std::endl;
  std::cout << "\tDA:       \t" << this->DestinationAddress() << std::endl;
  std::cout << "\tTA:       \t" << this->TransmitterAddress() << std::endl;
  std::cout << "\tSA:       \t" << this->SourceAddress() << std::endl;
  std::cout << "\tBSSID:    \t" << this->Bssid() << std::endl;
  std::cout << "\tSeq:      \t" << (int) this->SequenceControl() << std::endl;
#endif
}

}
}