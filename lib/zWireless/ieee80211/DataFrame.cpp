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
#include <netinet/in.h>

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
  return (id);
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
  return (id);
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
  return (id);
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

bool
DataFrame::Assemble(zSocket::Buffer& sb_, bool fcs_)
{

  // NOTE: Assumes caller's socket buffer data and tail are set to start of frame (empty)

  // Initialize frame pointer to start of data
  struct ieee80211_hdr* f = (struct ieee80211_hdr*) sb_.Data();

  // Assemble lower level frame and validate
  if (!Frame::Assemble(sb_, fcs_) || (this->Type() != Frame::TYPE_DATA))
  {
    ZLOG_ERR("Error assembling frame");
    return (false);
  }

  // Write address 3 (BSSID)
  if (!this->GetAddress(Frame::ADDRESS_3).empty())
  {
    if (sb_.Put(sizeof(f->u.data.addr3))
        && this->str2mac(this->GetAddress(Frame::ADDRESS_3), f->u.data.addr3))
    {
      sb_.Pull(sizeof(f->u.data.addr3));
    }
    else
    {
      ZLOG_ERR("Error assembling frame");
      return (false);
    }
  }
  else
  {
    ZLOG_ERR("Missing address field: 3");
    return (false);
  }

  // Write sequence control field
  if (sb_.Put(sizeof(f->u.data.seqcntl)))
  {
    f->u.data.seqcntl = htole16(this->SequenceControl());
    sb_.Pull(sizeof(f->u.data.seqcntl));
  }
  else
  {
    ZLOG_ERR("Error assembling frame");
    return (false);
  }

  // Conditionally write QoS control field
  if (this->Subtype() & Frame::SUBTYPE_DATAQOS)
  {
    if (sb_.Put(sizeof(f->u.data.u.qosdata.qoscntl)))
    {
      f->u.data.u.qosdata.qoscntl = htole16(this->QosControl());
      sb_.Pull(sizeof(f->u.data.u.qosdata.qoscntl));
    }
    else
    {
      ZLOG_ERR("Error assembling frame");
      return (false);
    }
  }

  // Conditionally write address 4
  if (this->ToDS() && this->FromDS())
  {
    if (sb_.Put(sizeof(f->u.data.u.data4addr.addr4))
        && this->str2mac(this->GetAddress(Frame::ADDRESS_4), f->u.data.u.data4addr.addr4))
    {
      sb_.Pull(sizeof(f->u.data.u.data4addr.addr4));
    }
    else
    {
      ZLOG_ERR("Error assembling frame");
      return (false);
    }
  }

  // Get payload size
  size_t len = this->GetPayloadLength();
  if (len)
  {
    // Write LLC header
    if (sb_.Put(sizeof(struct data_llc)))
    {
      memcpy(sb_.Data(), &this->_llc, sizeof(struct data_llc));
      sb_.Pull(sizeof(struct data_llc));
    }
    else
    {
      ZLOG_ERR("Error assembling frame");
      return (false);
    }

    // Write payload (catch-all for remaining unsupported fields)
    if (sb_.Put(len) && (this->GetPayload(sb_.Data(), len) == len))
    {
      sb_.Pull(len);
    }
    else
    {
      ZLOG_ERR("Error assembling frame");
      return (false);
    }
  }

  return (true);

}

bool
DataFrame::Disassemble(zSocket::Buffer& sb_, bool fcs_)
{

  // NOTE: Assumes caller's socket buffer data is set to start of frame and
  //   tail is set to end of frame (full)

  // Initialize frame pointer to start of data
  struct ieee80211_hdr* f = (struct ieee80211_hdr*) sb_.Data();

  // Disassemble lower level frame and validate
  if (!Frame::Disassemble(sb_, fcs_) || (this->Type() != Frame::TYPE_DATA))
  {
    ZLOG_ERR("Error disassembling frame: " + ZLOG_INT(this->Type()));
    return (false);
  }

  // Read address 3 (BSSID) field
  if (sb_.Pull(sizeof(f->u.data.addr3)))
  {
    this->SetAddress(ADDRESS_3, f->u.data.addr3);
  }
  else
  {
    ZLOG_ERR("Missing address field: 3");
    return (false);
  }

  // Read sequence control field
  if (sb_.Pull(sizeof(f->u.data.seqcntl)))
  {
    this->SequenceControl(le16toh(f->u.data.seqcntl));
  }
  else
  {
    ZLOG_ERR("Error disassembling frame");
    return (false);
  }

  // Conditionally read QoS control field
  if (this->Subtype() & Frame::SUBTYPE_DATAQOS)
  {
    if (sb_.Pull(sizeof(f->u.data.u.qosdata.qoscntl)))
    {
      this->QosControl(le16toh(f->u.data.u.qosdata.qoscntl));
    }
    else
    {
      ZLOG_ERR("Error disassembling frame");
      return (false);
    }
  }

  // Conditionally read address 4
  if (this->ToDS() && this->FromDS())
  {
    if (sb_.Pull(sizeof(f->u.data.u.data4addr.addr4)))
    {
      this->SetAddress(ADDRESS_4, f->u.data.u.data4addr.addr4);
    }
    else
    {
      ZLOG_ERR("Missing address field: 4");
      return (false);
    }
  }

  // Read out LLC header
  if (sb_.Length())
  {
    uint8_t* p = sb_.Data();
    if (sb_.Pull(sizeof(struct data_llc)))
    {
      memcpy(&this->_llc, p, sizeof(struct data_llc));
    }
    else
    {
      ZLOG_ERR("Missing LLC header");
      return (false);
    }
  }

  // Read out the frame payload (catch-all for remaining unsupported fields)
  size_t len = sb_.Length();
  if (this->PutPayload(sb_.Data(), len))
  {
    sb_.Pull(len);
  }
  else
  {
    ZLOG_ERR("Error disassembling frame");
    return (false);
  }

  return (true);

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

  if (!this->str2mac(this->GetAddress(ADDRESS_1), f->addr1))
  {
    ZLOG_WARN("Missing or invalid address field: 1");
  }

  if (!(p_ = this->chklen(p_, sizeof(f->addr2), rem_)))
  {
    ZLOG_ERR("Buffer overrun");
    return (NULL);
  }

  if (!this->str2mac(this->GetAddress(ADDRESS_2), f->addr2))
  {
    ZLOG_WARN("Missing or invalid address field: 2");
  }

  if (!(p_ = this->chklen(p_, sizeof(f->addr3), rem_)))
  {
    ZLOG_ERR("Buffer overrun");
    return (NULL);
  }

  if (!this->str2mac(this->GetAddress(ADDRESS_3), f->addr3))
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
    if (!this->str2mac(this->GetAddress(ADDRESS_4), f->u.data4addr.addr4))
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
  if (!p_ || !this->SetAddress(ADDRESS_3, f->u.data.addr3))
  {
    ZLOG_ERR("Error disassembling address field 3: " + ZLOG_P(p_));
    return (NULL);
  }

  p_ = this->chklen(p_, sizeof(f->u.data.seqcntl), rem_);
  if (!p_ || !this->SequenceControl(le16toh(f->u.data.seqcntl)))
  {
    ZLOG_ERR("Error disassembling sequence control field: " + ZLOG_P(p_));
    return (NULL);
  }

  if (this->Subtype() == Frame::SUBTYPE_DATAQOS)
  {
    p_ = this->chklen(p_, sizeof(f->u.data.u.qosdata.qoscntl), rem_);
    if (!p_ || !this->QosControl(le16toh(f->u.data.u.qosdata.qoscntl)))
    {
      ZLOG_ERR("Error disassembling QoS control field: " + ZLOG_P(p_));
      return (NULL);
    }
  }

  if (this->ToDS() && this->FromDS())
  {
    p_ = this->chklen(p_, sizeof(f->u.data.u.data4addr.addr4), rem_);
    if (!p_ || !this->SetAddress(ADDRESS_4, f->u.data.u.data4addr.addr4))
    {
      ZLOG_ERR("Error disassembling address field 4: " + ZLOG_P(p_));
      return (NULL);
    }
  }

  uint8_t* llc = p_;
  p_ = this->chklen(llc, sizeof(struct data_llc), rem_);
  if (!p_)
  {
    return (llc);
  }
  memcpy(&this->_llc, llc, sizeof(struct data_llc));

  uint8_t* pay = p_;
  size_t len = rem_;
  p_ = this->chklen(pay, len, rem_);
  if (!p_ || !this->PutPayload(pay, len))
  {
    return (pay);
  }

  return (p_);
}

std::string
DataFrame::ReceiverAddress() const
{
  return (this->GetAddress(Frame::ADDRESS_1));
}

bool
DataFrame::ReceiverAddress(const std::string& address_)
{
  return (this->SetAddress(Frame::ADDRESS_1, address_));
}

std::string
DataFrame::TransmitterAddress() const
{
  return (this->GetAddress(Frame::ADDRESS_2));
}

bool
DataFrame::TransmitterAddress(const std::string& address_)
{
  return (this->SetAddress(Frame::ADDRESS_2, address_));
}

std::string
DataFrame::DestinationAddress() const
{
  Frame::ADDRESS_ID id = _da_addrid(this->ToDS(), this->FromDS());
  return (this->GetAddress(id));
}

bool
DataFrame::DestinationAddress(const std::string& address_)
{
  Frame::ADDRESS_ID id = _da_addrid(this->ToDS(), this->FromDS());
  return (this->SetAddress(id, address_));
}

std::string
DataFrame::SourceAddress() const
{
  Frame::ADDRESS_ID id = _sa_addrid(this->ToDS(), this->FromDS());
  return (this->GetAddress(id));
}

bool
DataFrame::SourceAddress(const std::string& address_)
{
  Frame::ADDRESS_ID id = _sa_addrid(this->ToDS(), this->FromDS());
  return (this->SetAddress(id, address_));
}

std::string
DataFrame::Bssid() const
{
  Frame::ADDRESS_ID id = _bssid_addrid(this->ToDS(), this->FromDS());
  return (this->GetAddress(id));
}

bool
DataFrame::Bssid(const std::string& address_)
{
  Frame::ADDRESS_ID id = _bssid_addrid(this->ToDS(), this->FromDS());
  return (this->SetAddress(id, address_));
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
  std::cout << "\tFrag:     \t" << int(this->FragmentNum()) << std::endl;
  std::cout << "\tSeq:      \t" << int(this->SequenceNum()) << std::endl;
  std::cout << "----- IEEE802.11 LLC ---------------------" << std::endl;
  std::cout << "\tProto:    \t" << std::hex << int(ntohs(this->Llc().proto)) << std::dec << std::endl;
}

}
}
}
