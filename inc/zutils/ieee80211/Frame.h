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

#ifndef __IEEE80211_FRAME_H__
#define __IEEE80211_FRAME_H__

// libc includes
#include <stdio.h>

// libc++ includes
#include <list>

// libzutils includes
#include <zutils/ieee80211/Tag.h>

// local includes

namespace zUtils
{
namespace zWireless
{
namespace ieee80211
{

#define FRAME_PAYLOAD_MAXLEN        2048

//*****************************************************************************
// Class: Frame
//*****************************************************************************

class Frame
{

public:

  enum TYPE
  {
    TYPE_ERR = -1,
    TYPE_NONE = 0,
    TYPE_MGMT = 0,
    TYPE_CNTL = 1,
    TYPE_DATA = 2,
    TYPE_LAST
  };

  enum SUBTYPE
  {
    SUBTYPE_ERR = -1,
    SUBTYPE_NONE = 0,
    SUBTYPE_ASSREQ = 0,
    SUBTYPE_DATA = 0,
    SUBTYPE_ASSRESP = 1,
    SUBTYPE_RASSREQ = 2,
    SUBTYPE_RASSRESP = 3,
    SUBTYPE_PROBEREQ = 4,
    SUBTYPE_DATANULL = 4,
    SUBTYPE_PROBERESP = 5,
    SUBTYPE_TIMEADV = 6,
    SUBTYPE_CNTLWRAPPER = 7,
    SUBTYPE_BEACON = 8,
    SUBTYPE_BACKREQ = 8,
    SUBTYPE_DATAQOS = 8,
    SUBTYPE_ATIM = 9,
    SUBTYPE_BACK = 9,
    SUBTYPE_DISASS = 10,
    SUBTYPE_AUTHENTICATE = 11,
    SUBTYPE_RTS = 11,
    SUBTYPE_DEAUTH = 12,
    SUBTYPE_CTS = 12,
    SUBTYPE_DATAQOSNULL = 12,
    SUBTYPE_ACTION = 13,
    SUBTYPE_ACK = 13,
    SUBTYPE_ACTION_NOACK = 14,
    SUBTYPE_LAST
  };

  enum ADDRESS_ID
  {
    ADDRESS_ERR = -1,
    ADDRESS_1 = 0,
    ADDRESS_2 = 1,
    ADDRESS_3 = 2,
    ADDRESS_4 = 3,
    ADDRESS_LAST
  };

  enum TAGTYPE
  {
    TAGTYPE_NONE = 0x00,
    TAGTYPE_HEAD = 0x01,
    TAGTYPE_TAIL = 0x02,
    TAGTYPE_ALL = 0x03
  };

  Frame(const TYPE type_ = TYPE_ERR);

  virtual
  ~Frame();

  virtual uint8_t*
  Assemble(uint8_t* p_, size_t& rem_, bool fcs_ = false);

  virtual uint8_t*
  Disassemble(uint8_t* p_, size_t& rem_, bool fcs_ = false);

  virtual uint8_t*
  Peek(uint8_t* p_, size_t len_, bool fcs_ = false);

  uint8_t*
  AssembleTags(uint8_t* tag_, size_t& rem_, uint8_t tagtype_ = TAGTYPE_ALL);

  uint8_t*
  DisassembleTags(uint8_t* tag_, size_t& rem_, uint8_t tagtype_ = TAGTYPE_ALL);

  uint8_t
  Version() const;

  bool
  Version(const uint8_t version_);

  Frame::TYPE
  Type() const;

  bool
  Type(const Frame::TYPE type_);

  Frame::SUBTYPE
  Subtype() const;

  bool
  Subtype(const Frame::SUBTYPE stype_);

  bool
  ToDS() const;

  bool
  ToDS(const bool flag_);

  bool
  FromDS() const;

  bool
  FromDS(const bool flag_);

  bool
  MoreFragments() const;

  bool
  MoreFragments(const bool flag_);

  bool
  Retry() const;

  bool
  Retry(const bool flag_);

  bool
  PowerManagement() const;

  bool
  PowerManagement(const bool flag_);

  bool
  MoreData() const;

  bool
  MoreData(const bool flag_);

  bool
  Protected() const;

  bool
  Protected(const bool flag_);

  bool
  Order() const;

  bool
  Order(const bool flag_);

  uint16_t
  DurationId() const;

  bool
  DurationId(const uint16_t durid_);

  std::string
  Address(const ADDRESS_ID id_) const;

  bool
  Address(const ADDRESS_ID id_, const std::string& address_);

  bool
  Address(const ADDRESS_ID id_, const uint8_t* address_);

  uint8_t
  FragmentNum() const;

  bool
  FragmentNum(const uint8_t num_);

  uint16_t
  SequenceNum() const;

  bool
  SequenceNum(const uint16_t num_);

  uint16_t
  SequenceControl() const;

  bool
  SequenceControl(const uint16_t cntl_);

  uint16_t
  QosControl() const;

  bool
  QosControl(const uint16_t cntl_);

  uint32_t
  HtControl() const;

  bool
  HtControl(const uint32_t cntl_);

  uint32_t
  FrameCheck() const;

  bool
  FrameCheck(const uint32_t fcs_);

  bool
  GetTag(Tag& tag_, const int index_ = 0);

  bool
  PutTag(const Tag& tag_, const int index_ = 0);

  size_t
  GetPayload(uint8_t* buf_, const size_t len_) const;

  size_t
  GetPayloadLength() const;

  bool
  PutPayload(const uint8_t* buf_, const size_t len_);

  virtual void
  Display() const;

protected:

  static bool
  str2mac(const std::string& addr_, uint8_t* hwaddr_);

  static bool
  mac2str(const uint8_t* hwaddr_, std::string& addr_);

  static uint8_t*
  chklen(uint8_t* f_, size_t len_, size_t& rem_);

  static uint32_t
  chkfcs(uint8_t* f_, size_t& rem_);

private:

  uint16_t _fccntl;
  uint16_t _durationid;
  std::string _address[4];
  uint16_t _seqcntl;
  uint16_t _qoscntl;
  uint32_t _htcntl;
  std::list<Tag> _tags[Tag::TYPE_LAST];
  uint8_t _payload[FRAME_PAYLOAD_MAXLEN];
  size_t _psize;
  uint32_t _fcs;
};

}
}
}

#endif /* __IEEE80211_FRAME_H__ */
