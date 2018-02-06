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

ZLOG_MODULE_INIT(zUtils::zLog::Log::MODULE_WIRELESS);

namespace zUtils
{
namespace zWireless
{
namespace ieee8023
{

//*****************************************************************************
// Class: Frame
//*****************************************************************************

Frame::Frame(const TYPE type_) :
    _type(type_), _proto(PROTO_NONE), _fcs(0)
{
}

Frame::~Frame()
{
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

  p_ = this->chklen(p_, sizeof(f->dst), rem_);
  if (!p_ || !mac2str(f->dst.addr, this->_dst))
  {
    ZLOG_WARN("Error disassembling frame: " + ZLOG_INT(rem_));
    return(NULL);
  }

  p_ = this->chklen(p_, sizeof(f->src), rem_);
  if (!p_ || !mac2str(f->src.addr, this->_src))
  {
    ZLOG_WARN("Error disassembling frame: " + ZLOG_INT(rem_));
    return(NULL);
  }

  if (rem_ < sizeof(f->u.type))
  {
    ZLOG_WARN("Error disassembling frame: " + ZLOG_INT(rem_));
    return(NULL);
  }

  uint16_t type = be16toh(f->u.type);
  if (type < PROTO_IPv4)
  {
    this->_type = TYPE_LLC;
  }
  else if (type < PROTO_LAST)
  {
    this->_type = TYPE_ETHER;
  }
  else
  {
    this->_type = TYPE_ERR;
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
  case PROTO_IPv4:
    // no break
  case PROTO_ARP:
    // no break
  case PROTO_VLAN:
    // no break
  case PROTO_IPv6:
    // no break
  case PROTO_VLAN2:
    this->_proto = proto_;
    status = true;
    break;
  default:
    break;
  }
  return (status);
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
Frame::Display() const
{
  std::cout << "------------------------------------------" << std::endl;
  std::cout << "----- IEEE802.3 Header ------------------" << std::endl;
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
