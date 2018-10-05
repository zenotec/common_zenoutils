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

#ifndef __IEEE80211_TAG_H__
#define __IEEE80211_TAG_H__

#include <vector>
#include <iostream>

namespace zUtils
{
namespace zWireless
{
namespace ieee80211
{

//*****************************************************************************
// Class: Tag
//*****************************************************************************

class Tag
{

public:

  enum ID
  {
    ID_ERR = -1,
    ID_SSID       = 0,
    ID_RATES      = 1,
    ID_FHPARMS    = 2,
    ID_DSPARMS    = 3,
    ID_CFPARMS    = 4,
    ID_TIM        = 5,
    ID_IBSSPARMS  = 6,
    ID_COUNTRY    = 7,
    ID_REQINFO    = 10,
    ID_CHALLENGE  = 16,
    /* 17-31 reserved for challenge text extension */
    ID_PWRCNSTR   = 32,
    ID_PWRCAP     = 33,
    ID_TPCREQ     = 34,
    ID_TPCREP     = 35,
    ID_SUPPCHAN   = 36,
    ID_CHANSWITCHANN  = 37,
    ID_MEASREQ    = 38,
    ID_MEASREP    = 39,
    ID_QUIET      = 40,
    ID_IBSSDFS    = 41,
    ID_ERP        = 42,
    ID_HTCAP      = 45,
    ID_RSN        = 48,
    ID_XRATES     = 50,
    ID_NEIGHBOR_REP   = 52,
    ID_FTIE       = 55,
    ID_TIMEOUT_INT    = 56,
    ID_REG_CLASSES    = 59,
    ID_HTINFO     = 61,
    ID_EXTCAP     = 127,
    /* 128-129 proprietary elements used by Agere chipsets */
    ID_AGERE1     = 128,
    ID_AGERE2     = 129,
    ID_TPC        = 150,
    ID_CCKM       = 156,
    /* 191-199 Table 8-54-Element IDs in Draft P802.11ac_D4.0.pdf */
    ID_VHTCAP     = 191,
    ID_VHTOP      = 192,
    ID_EXTBSSLOAD = 193,
    ID_WBWCHANSWITCH  = 194,
    ID_VHTXMTPWRENVLP = 195,
    ID_CHANSWITCHWRP  = 196,
    ID_AID        = 197,
    ID_QUIETCHAN  = 198,
    ID_OPMODNOTICE    = 199,
    /* Vendor Specific */
    ID_VENDOR     = 221,  /* vendor private */
    ID_LAST
  };

  enum TYPE
  {
    TYPE_HEAD = 0, // used to index into tag array, do not change
    TYPE_TAIL = 1,
    TYPE_LAST
  };

  std::string
  ToString(const Tag::ID id_) const
  {
    switch (id_)
    {
      case ID_ERR: { return "ID_ERR"; break; }
      case ID_SSID: { return "ID_SSID"; break; }
      case ID_RATES: { return "ID_RATES"; break; }
      case ID_DSPARMS: { return "ID_DSPARMS"; break; }
      case ID_TIM: { return "ID_TIM"; break; }
      case ID_COUNTRY: { return "ID_COUNTRY"; break; }
      case ID_PWRCAP: { return "ID_PWRCAP"; break; }
      case ID_SUPPCHAN: { return "ID_SUPPCHAN"; break; }
      case ID_CHANSWITCHANN: { return "ID_CHANSWITCHANN"; break; }
      case ID_ERP: { return "ID_ERP"; break; }
      case ID_HTCAP: { return "ID_HTCAP"; break; }
      case ID_XRATES: { return "ID_XRATES"; break; }
      case ID_REG_CLASSES: { return "ID_REG_CLASSES"; break; }
      case ID_HTINFO: { return "ID_HTINFO"; break; }
      case ID_EXTCAP: { return "ID_EXTCAP"; break; }
      case ID_VHTCAP: { return "ID_VHTCAP"; break; }
      case ID_VENDOR: { return "ID_VENDOR"; break; }
      default: { return "UNKNOWN"; break; }
    }
  }

  Tag(const Tag::ID id_ = ID_ERR, const size_t len_ = 0) :
      _valid(false), _id(id_)
  {
    this->_value.resize(len_, 0);
  }

  Tag(const Tag& other_)
  {
    this->_valid = other_._valid;
    this->_id = other_._id;
    this->_value = other_._value;
  }

  virtual
  ~Tag()
  {
    this->_valid = false;
  }

  Tag&
  operator=(const Tag& other_)
  {
    this->_valid = other_._valid;
    this->_id = other_._id;
    this->_value = other_._value;
    return(*this);
  }

  bool
  Valid() const
  {
    return (this->_valid);
  }

  Tag::ID
  Id() const
  {
    return (this->_id);
  }

  uint8_t
  Type() const
  {
    return ((this->_id <= ID_TIM) ? TYPE_HEAD : TYPE_TAIL); // HEAD TAG = 0, TAIL TAG = 1
  }

  size_t
  Length() const
  {
    return (this->_value.size());
  }

  size_t
  GetValue(uint8_t* p_, size_t len_) const
  {
    size_t cnt = std::min(this->Length(), len_);
    cnt = std::min(cnt, (size_t)256);
    for (int i = 0; i < cnt; i++)
    {
      p_[i] = this->_value[i];
    }
    return(cnt);
  }

  bool
  GetValue(std::string& str_) const
  {
    str_.resize(this->Length());
    return(this->GetValue((uint8_t*)str_.data(), str_.size()) == this->Length());
  }

  template<typename T>
    bool
    GetValue(T& value_) const
    {
      bool status = false;
      if (sizeof(T) == this->Length())
      {
        status = (this->GetValue((uint8_t*) &value_, sizeof(T)) == sizeof(T));
      }
      return(status);
    }

  size_t
  PutValue(const uint8_t* p_, size_t len_)
  {
    this->_valid = true;
    len_ = std::min(len_, (size_t)256);
    this->_value.resize(len_, 0);
    for (int i = 0; i < len_; i++)
    {
      this->_value[i] = p_[i];
    }
    return(len_);
  }

  bool
  PutValue(const std::string& str_)
  {
    return(this->PutValue((uint8_t*)str_.c_str(), str_.size()) == str_.size());
  }

  template<typename T>
    bool
    PutValue(T& value_)
    {
      return (this->PutValue((uint8_t*) &value_, sizeof(T)) == sizeof(T));
    }

  size_t
  AddValue(const uint8_t* p_, size_t len_)
  {
    this->_valid = true;
    uint8_t origLen = Length();
    uint8_t newLen = std::min(origLen + len_, (size_t)256);
    this->_value.resize(newLen, 0);
    for (int i = 0; i < newLen - origLen; i++)
    {
      this->_value[i + origLen] = p_[i];
    }
    return(newLen - origLen);
  }

  bool
  AddValue(const std::string& str_)
  {
    return(this->AddValue((uint8_t*)str_.c_str(), str_.size()) == str_.size());
  }

  template<typename T>
    bool
    AddValue(T& value_)
    {
      return (this->AddValue((uint8_t*) &value_, sizeof(T)) == sizeof(T));
    }

  virtual void
  Display() const
  {
    std::cout << std::endl << "Tag[" << this->_id << "] (" << Tag::ToString(this->_id) << ")" << std::endl;
  }

protected:

  std::vector<uint8_t> _value;

private:

  bool _valid;
  Tag::ID _id;

};

}
}
}

#endif /* __IEEE80211_TAG_H__ */
