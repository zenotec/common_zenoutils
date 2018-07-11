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
#include <string.h>
#include <endian.h>

// libc++ includes
#include <iostream>

// libzutils includes
#include <zutils/zLog.h>
#include <zutils/ieee80211/DataFrame.h>
using namespace zUtils;

// local includes
#include "ieee80211.h"

ZLOG_MODULE_INIT(zLog::Log::MODULE_WIRELESS);

namespace zUtils
{
namespace zWireless
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
    Frame(Frame::TYPE_DATA)
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

  if (!(p_ = this->chklen(p_, sizeof(f->addr1), rem_)))
  {
    ZLOG_ERR("Buffer overrun");
    return (NULL);
  }

  if (!this->str2mac(this->Address(ADDRESS_1), f->addr1))
  {
    ZLOG_WARN("Missing or invalid address field: 1");
  }

  if (!(p_ = this->chklen(p_, sizeof(f->addr2), rem_)))
  {
    ZLOG_ERR("Buffer overrun");
    return (NULL);
  }

  if (!this->str2mac(this->Address(ADDRESS_2), f->addr2))
  {
    ZLOG_WARN("Missing or invalid address field: 2");
  }

  if (!(p_ = this->chklen(p_, sizeof(f->addr3), rem_)))
  {
    ZLOG_ERR("Buffer overrun");
    return (NULL);
  }

  if (!this->str2mac(this->Address(ADDRESS_3), f->addr3))
  {
    ZLOG_WARN("Missing or invalid address field: 3");
  }

  if (!(p_ = this->chklen(p_, sizeof(f->seqcntl), rem_)))
  {
    ZLOG_ERR("Buffer overrun");
    return (NULL);
  }
  f->seqcntl = htole16(this->SequenceControl());

  if (this->Subtype() & Frame::SUBTYPE_DATAQOS)
  {
    if (!(p_ = this->chklen(p_, sizeof(f->u.qosdata.qoscntl), rem_)))
    {
      ZLOG_ERR("Buffer overrun");
      return (NULL);
    }
    f->u.qosdata.qoscntl = htole16(this->QosControl());
  }

  if (this->ToDS() && this->FromDS())
  {
    if (!(p_ = this->chklen(p_, sizeof(f->u.data4addr.addr4), rem_)))
    {
      ZLOG_ERR("Buffer overrun");
      return (NULL);
    }
    if (!this->str2mac(this->Address(ADDRESS_4), f->u.data4addr.addr4))
    {
      ZLOG_WARN("Missing or invalid address field: 4");
    }
  }

  uint8_t* llc = p_;
  if (!(p_ = this->chklen(llc, sizeof(struct data_llc), rem_)))
  {
    ZLOG_ERR("Buffer overrun");
    return (NULL);
  }
  memcpy(llc, &this->_llc, sizeof(struct data_llc));

  uint8_t* pay = p_;
  size_t len = this->GetPayloadLength();

  if (!(p_ = this->chklen(pay, len, rem_)))
  {
    ZLOG_ERR("Buffer overrun");
    return (NULL);
  }
  if (this->GetPayload(pay, len) != len)
  {
    ZLOG_WARN("Missing or invalid payload");
  }
  return (p_);
}

uint8_t*
DataFrame::Disassemble(uint8_t* p_, size_t& rem_, bool fcs_)
{

  ieee80211_hdr* f = (ieee80211_hdr*) p_;
  p_ = Frame::Disassemble(p_, rem_, fcs_);

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

  p_ = this->chklen(p_, sizeof(f->u.data.addr3), rem_);
  if (!p_ || !this->Address(ADDRESS_3, f->u.data.addr3))
  {
    ZLOG_ERR("Error disassembling address field 3: " + ZLOG_P(p_));
    return (NULL);
  }

  p_ = this->chklen(p_, sizeof(f->u.data.seqcntl), rem_);
  if (!p_ || !this->SequenceControl(le16toh(f->u.data.seqcntl)))
  {
    ZLOG_ERR("Error disassembling sequence control field: " + ZLOG_P(p_));
    return(NULL);
  }

  if (this->Subtype() == Frame::SUBTYPE_DATAQOS)
  {
    p_ = this->chklen(p_, sizeof(f->u.data.u.qosdata.qoscntl), rem_);
    if (!p_ || !this->QosControl(le16toh(f->u.data.u.qosdata.qoscntl)))
    {
      ZLOG_ERR("Error disassembling QoS control field: " + ZLOG_P(p_));
      return(NULL);
    }
  }

  if (this->ToDS() && this->FromDS())
  {
    p_ = this->chklen(p_, sizeof(f->u.data.u.data4addr.addr4), rem_);
    if (!p_ || !this->Address(ADDRESS_4, f->u.data.u.data4addr.addr4))
    {
      ZLOG_ERR("Error disassembling address field 4: " + ZLOG_P(p_));
      return (NULL);
    }
  }

  uint8_t* llc = p_;
  p_ = this->chklen(llc, sizeof(struct data_llc), rem_);
  if (!p_)
  {
    return(llc);
  }
  memcpy(&this->_llc, llc, sizeof(struct data_llc));

  uint8_t* pay = p_;
  size_t len = rem_;
  p_ = this->chklen(pay, len, rem_);
  if (!p_ || !this->PutPayload(pay, len))
  {
    return(pay);
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

const data_llc&
DataFrame::Llc() const
{
  return (this->_llc);
}

bool
DataFrame::Llc(const data_llc& llc_)
{
  this->_llc = llc_;
  return (true);
}

void
DataFrame::Display() const
{
  Frame::Display();
  std::cout << "----- IEEE802.11 Data Header -------------" << std::endl;
  std::cout << "\tRA:       \t" << this->ReceiverAddress() << std::endl;
  std::cout << "\tDA:       \t" << this->DestinationAddress() << std::endl;
  std::cout << "\tTA:       \t" << this->TransmitterAddress() << std::endl;
  std::cout << "\tSA:       \t" << this->SourceAddress() << std::endl;
  std::cout << "\tBSSID:    \t" << this->Bssid() << std::endl;
  std::cout << "\tFrag:     \t" << (int) this->FragmentNum() << std::endl;
  std::cout << "\tSeq:      \t" << (int) this->SequenceNum() << std::endl;
}

}
}
}
