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
using namespace zUtils;
#include <zutils/ieee8023/Frame.h>

// local includes

#include "ieee8023.h"

ZLOG_MODULE_INIT(zUtils::zLog::Log::MODULE_SOCKET);

namespace zUtils
{
namespace zSocket
{
namespace ieee8023
{

//*****************************************************************************
// Class: Frame
//*****************************************************************************

Frame::Frame(const Frame::SUBTYPE subtype_, const Frame::PROTO proto_) :
    zSocket::Frame(zSocket::Frame::TYPE_8023),
    _subtype(subtype_),
    _proto(proto_),
    _fcs(0)
{
}

Frame::~Frame()
{
}

bool
Frame::Assemble(zSocket::Buffer& sb_, bool fcs_)
{

  // NOTE: Assumes caller's socket buffer data and tail are set to start of frame (empty)

  // Initialize frame pointer to start of data
  struct ieee8023_hdr* f = (struct ieee8023_hdr*)sb_.Data();

  // Write destination address
  if (sb_.Put(sizeof(f->dst)) && this->str2mac(this->GetDestination(), (uint8_t*)&f->dst))
  {
    sb_.Pull(sizeof(f->dst));
  }
  else
  {
    ZLOG_ERR("Error assembling frame");
    return (false);
  }

  // Write source address
  if (sb_.Put(sizeof(f->src)) && this->str2mac(this->GetSource(), (uint8_t*)&f->src))
  {
    sb_.Pull(sizeof(f->src));
  }
  else
  {
    ZLOG_ERR("Error assembling frame");
    return (false);
  }

  return (true);

}

bool
Frame::Disassemble(zSocket::Buffer& sb_, bool fcs_)
{

  // NOTE: Assumes caller's socket buffer data is set to start of frame and
  //   tail is set to end of frame (full)
  // NOTE: Returns socket buffer data pointer set to start of subtype header

  // Initialize frame pointer to start of data
  struct ieee8023_hdr* f = (struct ieee8023_hdr*)sb_.Data();

  // Read destination address
  std::string dst;
  if (sb_.Pull(sizeof(f->dst)) && this->mac2str((uint8_t*)&f->dst, dst))
  {
    this->SetDestination(dst);
  }
  else
  {
    ZLOG_ERR("Error disassembling frame");
    return (false);
  }

  // Read source address
  std::string src;
  if (sb_.Pull(sizeof(f->src)) && this->mac2str((uint8_t*)&f->src, src))
  {
    this->SetSource(src);
  }
  else
  {
    ZLOG_ERR("Error disassembling frame");
    return (false);
  }

  // Verify presence of type field
  if (sb_.Length() < sizeof(f->u.type))
  {
    ZLOG_ERR("Error disassembling frame");
    return (false);
  }

  // Determine frame type
  uint16_t type = be16toh(f->u.type);
  if (type < 1500)
  {
    if ((f->u.llc.dst_sap == 0xaa) && (f->u.llc.src_sap == 0xaa))
    {
      this->_subtype = Frame::SUBTYPE_LLC;
    }
    else
    {
      this->_subtype = Frame::SUBTYPE_ETHER;
    }
  }
  else if (type == PROTO_VLAN)
  {
    this->_subtype = Frame::SUBTYPE_VLAN;
  }
  else
  {
    this->_subtype = Frame::SUBTYPE_ETHER2;
  }

  return (true);

}

bool
Frame::Peek(const zSocket::Buffer& sb_, bool fcs_)
{

  // NOTE: Assumes caller's socket buffer data is set to start of frame and
  //   tail is set to end of frame (full)
  // NOTE: Does not alter caller's buffer

  // Create copy of caller's buffer to avoid modifying pointers
  Buffer tmp(sb_);

  // Disassemble frame using temporary local buffer
  return (this->Disassemble(tmp, fcs_));
}

uint8_t*
Frame::Assemble(uint8_t* p_, size_t& rem_, bool fcs_)
{

  struct ieee8023_hdr* f = (struct ieee8023_hdr*) p_;

  p_ = this->chklen(p_, (sizeof(f->dst) + sizeof(f->src)), rem_);
  if (!p_)
  {
    return (NULL);
  }

  if (!this->str2mac(this->_dst, f->dst.addr))
  {
    return (NULL);
  }

  if (!this->str2mac(this->_src, f->src.addr))
  {
    return (NULL);
  }

  return (p_);
}

uint8_t*
Frame::Disassemble(uint8_t* p_, size_t& rem_, bool fcs_)
{

  size_t len = 0;
  struct ieee8023_hdr* f = (struct ieee8023_hdr*) p_;

  if (p_ && rem_ && fcs_)
  {
    if (!(this->_fcs = this->chkfcs(p_, rem_)))
    {
//      ZLOG_WARN("FCS check failed"); // TODO:
    }
  }

  // Read destination address
  p_ = this->chklen(p_, sizeof(f->dst), rem_);
  if (!p_ || !mac2str(f->dst.addr, this->_dst))
  {
    ZLOG_WARN("Error disassembling frame: " + ZLOG_INT(rem_));
    return(NULL);
  }

  // Read source address
  p_ = this->chklen(p_, sizeof(f->src), rem_);
  if (!p_ || !mac2str(f->src.addr, this->_src))
  {
    ZLOG_WARN("Error disassembling frame: " + ZLOG_INT(rem_));
    return(NULL);
  }

  // Check for minimum frame size
  if (rem_ < (sizeof(f->u.type) + 46))
  {
    ZLOG_WARN("Error disassembling frame: " + ZLOG_INT(rem_));
    return(NULL);
  }

  // Determine frame type
  uint16_t type = be16toh(f->u.type);
  if (type < 1500)
  {
    this->_subtype = Frame::SUBTYPE_ETHER;
    if ((f->u.llc.dst_sap == 0xaa) && (f->u.llc.src_sap == 0xaa))
    {
      this->_subtype = Frame::SUBTYPE_LLC;
    }
  }
  else if (type == PROTO_VLAN)
  {
    this->_subtype = Frame::SUBTYPE_VLAN;
    p_ = this->chklen(p_, sizeof(f->u.vlan), rem_);
    if (!p_ || !this->SetProto(Frame::PROTO(be16toh(f->u.vlan.proto))))
    {
      ZLOG_WARN("Error disassembling frame: " + ZLOG_INT(rem_));
      return(NULL);
    }
    if (!this->PutPayload(f->u.vlan.data, rem_))
    {
      p_ = NULL;
    }
  }
  else
  {
    this->_subtype = Frame::SUBTYPE_ETHER2;
  }


  return (p_);
}

uint8_t*
Frame::Peek(uint8_t* p_, size_t len_, bool fcs_)
{
  size_t rem = len_;
  uint8_t* p = this->Disassemble(p_, rem);
  return((p == NULL) ? NULL : p_);
}

Frame::SUBTYPE
Frame::GetSubtype() const
{
  return (this->_subtype);
}

bool
Frame::SetSubtype(const Frame::SUBTYPE subtype_)
{
  bool status = false;
  switch (subtype_)
  {
  case Frame::SUBTYPE_NONE ... Frame::SUBTYPE_LAST:
    this->_subtype = subtype_;
    status = true;
    break;
  default:
    break;
  }
  return (status);
}

Frame::PROTO
Frame::GetProto() const
{
  return (this->_proto);
}

bool
Frame::SetProto(const Frame::PROTO proto_)
{
  bool status = false;
  switch (proto_)
  {
  case PROTO_IPv4 ... PROTO_LAST:
    this->_proto = proto_;
    status = true;
    break;
  default:
    break;
  }
  return (status);
}

std::string
Frame::GetDestination() const
{
  return (this->_dst);
}

bool
Frame::SetDestination(const std::string& dst_)
{
  this->_dst = dst_;
  return (true);
}

std::string
Frame::GetSource() const
{
  return (this->_src);
}

bool
Frame::SetSource(const std::string& src_)
{
  this->_src = src_;
  return (true);
}

size_t
Frame::GetPayload(uint8_t*& buf_, const size_t len_) const
{
  size_t cnt = std::min(this->_payload.size(), len_);
  memcpy(buf_, this->_payload.data(), cnt);
  return(cnt);
}

size_t
Frame::GetPayloadLength() const
{
  return(this->_payload.size());
}

bool
Frame::PutPayload(const uint8_t* buf_, const size_t len_)
{
  bool status = false;

  if (!len_)
  {
    ZLOG_WARN("Zero length payload");
  }

  if (buf_ && (len_ < 7952))
  {
    this->_payload.clear();
    this->_payload.resize(len_);
    status = (memcpy(this->_payload.data(), buf_, len_) == this->_payload.data());
  }
  return (status);
}

void
Frame::Display(const std::string& prefix_) const
{
  std::cout << prefix_ << "------------------------------------------" << std::endl;
  std::cout << prefix_ <<  "----- IEEE802.3 Header ------------------" << std::endl;
  std::cout << prefix_ <<  "\tDest:  \t" << this->GetDestination() << std::endl;
  std::cout << prefix_ <<  "\tSource:\t" << this->GetSource() << std::endl;
}

bool
Frame::str2mac(const std::string& addr_, uint8_t* hwaddr_)
{
  return (sscanf(addr_.c_str(), "%hhx:%hhx:%hhx:%hhx:%hhx:%hhx", &hwaddr_[0], &hwaddr_[1],
      &hwaddr_[2], &hwaddr_[3], &hwaddr_[4], &hwaddr_[5]) == ETH_ALEN);
}

bool
Frame::mac2str(const uint8_t* hwaddr_, std::string& addr_)
{
  bool status = false;
  char str[256] = { 0 };
  if (sprintf(str, "%02hhx:%02hhx:%02hhx:%02hhx:%02hhx:%02hhx", hwaddr_[0], hwaddr_[1], hwaddr_[2],
      hwaddr_[3], hwaddr_[4], hwaddr_[5]) > 0)
  {
    addr_ = std::string(str);
    status = true;
  }
  return (status);
}

uint8_t*
Frame::chklen(uint8_t* f_, size_t len_, size_t& rem_)
{
  if (!f_ || (len_ > rem_))
  {
    return(NULL);
  }
  rem_ -= len_;
  return(f_ + len_);
}

uint32_t
Frame::chkfcs(uint8_t* f_, size_t& rem_)
{
  rem_ -= sizeof(uint32_t);
  return(0); // TODO: implement FCS computation
}

}
}
}
