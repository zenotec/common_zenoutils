/*
 * Copyright (c) 2017 - 2018 Cable Television Laboratories, Inc. ("CableLabs")
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

#ifndef __IEEE8023_FRAME_H__
#define __IEEE8023_FRAME_H__

// libc includes

#include <stdio.h>

// libc++ includes

#include <map>

// libzutils includes

#include <zutils/zSocket.h>

// local includes

namespace zUtils
{
namespace zSocket
{
namespace ieee8023
{

//*****************************************************************************
// Class: Frame
//*****************************************************************************

class Frame :
    public zSocket::Frame
{

public:

  enum SUBTYPE
  {
    SUBTYPE_ERR = -1,
    SUBTYPE_NONE = 0,
    SUBTYPE_ETHER,
    SUBTYPE_ETHER2,
    SUBTYPE_VLAN,
    SUBTYPE_LLC,
    SUBTYPE_LAST
  };

  enum PROTO
  {
    PROTO_ERR = -1,
    PROTO_NONE = 0,
    PROTO_IPv4 = 0x0800,
    PROTO_ARP = 0x0806,
    PROTO_VLAN = 0x8100,
    PROTO_IPv6 = 0x86DD,
    PROTO_VLAN2 = 0x9100,
    PROTO_LAST
  };

  Frame(const Frame::SUBTYPE subtype_ = Frame::SUBTYPE_NONE,
      const Frame::PROTO proto_ = Frame::PROTO_NONE);

  virtual
  ~Frame();

  virtual bool
  Assemble(zSocket::Buffer& sb_);

  virtual bool
  Disassemble(zSocket::Buffer& sb_);

  virtual bool
  Peek(const zSocket::Buffer& sb_);

  virtual uint8_t*
  Assemble(uint8_t* p_, size_t& rem_, bool fcs_ = false);

  virtual uint8_t*
  Disassemble(uint8_t* p_, size_t& rem_, bool fcs_ = false);

  virtual uint8_t*
  Peek(uint8_t* p_, size_t len_, bool fcs_ = false);

  Frame::SUBTYPE
  GetSubtype() const;

  bool
  SetSubtype(const Frame::SUBTYPE subtype_);

  Frame::PROTO
  GetProto() const;

  bool
  SetProto(const Frame::PROTO proto_);

  std::string
  GetDestination() const;

  bool
  SetDestination(const std::string& dst_);

  std::string
  GetSource() const;

  bool
  SetSource(const std::string& src_);

  size_t
  GetPayload(uint8_t*& buf_, const size_t len_) const;

  size_t
  GetPayloadLength() const;

  bool
  PutPayload(const uint8_t* buf_, const size_t len_);

  virtual void
  Display(const std::string& prefix_ = "") const;

protected:

  static bool
  str2mac(const std::string& addr_, uint8_t* hwaddr_);

  static bool
  mac2str(const uint8_t* hwaddr_, std::string& addr_);

  uint8_t*
  chklen(uint8_t* f_, size_t len_, size_t& rem_);

  uint32_t
  chkfcs(uint8_t* f_, size_t& rem_);

private:

  std::string _dst;
  std::string _src;
  Frame::SUBTYPE _subtype;
  Frame::PROTO _proto;
  std::vector<uint8_t> _payload;
  uint32_t _fcs;

};

}
}
}

#endif /* __IEEE8023_FRAME_H__ */
