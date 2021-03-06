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
#include <zutils/ieee80211/ieee80211.h>

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
