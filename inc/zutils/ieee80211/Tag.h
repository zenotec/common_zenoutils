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
    ID_SSID = 0,
    ID_RATES = 1,
    ID_DSSS = 3,
    ID_TIM = 5,
    ID_COUNTRY = 7,
    ID_POWER_CAPS = 33, // 0x21
    ID_CHANNELS = 36, // 0x24
    ID_CHANNEL_SWITCH = 37, // 0x25
    ID_ERP_INFO = 42, // 0x2A
    ID_HT_CAPS = 45, // 0x2D
    ID_EXT_RATES = 50, // 0x32
    ID_SUPP_OP_CLASSES = 59, // 0x3B
    ID_HT_INFORMATION = 61, // 0x3D
    ID_EXT_CAPS = 127, // 0x7f
    ID_VENDOR_SPECIFIC = 221, // 0xDD
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
      case ID_DSSS: { return "ID_DSSS"; break; }
      case ID_TIM: { return "ID_TIM"; break; }
      case ID_COUNTRY: { return "ID_COUNTRY"; break; }
      case ID_POWER_CAPS: { return "ID_POWER_CAPS"; break; }
      case ID_CHANNELS: { return "ID_CHANNELS"; break; }
      case ID_CHANNEL_SWITCH: { return "ID_CHANNEL_SWITCH"; break; }
      case ID_ERP_INFO: { return "ID_ERP_INFO"; break; }
      case ID_HT_CAPS: { return "ID_HT_CAPS"; break; }
      case ID_EXT_RATES: { return "ID_EXT_RATES"; break; }
      case ID_SUPP_OP_CLASSES: { return "ID_SUPP_OP_CLASSES"; break; }
      case ID_HT_INFORMATION: { return "ID_HT_INFORMATION"; break; }
      case ID_EXT_CAPS: { return "ID_EXT_CAPS"; break; }
      case ID_VENDOR_SPECIFIC: { return "ID_VENDOR_SPECIFIC"; break; }
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
