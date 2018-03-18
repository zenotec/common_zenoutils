/*
 * Copyright (c) 2018 Cable Television Laboratories, Inc. ("CableLabs")
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

#include <netlink/netlink.h>
#include <netlink/attr.h>

// libc++ includes

// libzutils includes
#include <zutils/zLog.h>
#include <zutils/netlink/Attribute.h>
using namespace zUtils;

// local includes

ZLOG_MODULE_INIT(zUtils::zLog::Log::MODULE_INTERFACE);

namespace netlink
{

//*****************************************************************************
// Class: Attribute
//*****************************************************************************

Attribute::Attribute(const Attribute::TYPE type_, const uint32_t id_, const size_t len_) :
    _valid(false), _type(type_), _id(id_)
{
}

Attribute::~Attribute()
{
}

Attribute&
Attribute::operator=(const Attribute& other_)
{
  this->_valid = other_._valid;
  this->_id = other_._id;
  this->_value = other_._value;
  return(*this);
}

bool
Attribute::IsValid() const
{
  return (this->_valid);
}

void
Attribute::SetValid()
{
  this->_valid = true;
}

void
Attribute::ClrValid()
{
  this->_valid = false;
}

Attribute::TYPE
Attribute::GetType() const
{
  return (this->_type);
}

uint32_t
Attribute::GetId() const
{
  return (this->_id);
}

size_t
Attribute::GetLength() const
{
  return (this->_value.size());
}

size_t
Attribute::GetValue(uint8_t* p_, size_t len_) const
{
  size_t cnt = std::min(this->GetLength(), len_);
  cnt = std::min(cnt, (size_t)256);
  for (int i = 0; i < cnt; i++)
  {
    p_[i] = this->_value[i];
  }
  return(cnt);
}

bool
Attribute::GetValue(std::string& str_) const
{
  str_.resize(this->GetLength());
  return(this->GetValue((uint8_t*)str_.data(), str_.size()) == this->GetLength());
}

size_t
Attribute::SetValue(const uint8_t* p_, size_t len_)
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
Attribute::SetValue(const std::string& str_)
{
  return(this->SetValue((uint8_t*)str_.c_str(), str_.size()) == str_.size());
}

//*****************************************************************************
// Class: AttributeMap
//*****************************************************************************

AttributeMap::AttributeMap()
{
}

AttributeMap::~AttributeMap()
{
}

bool
AttributeMap::Assemble(struct nlattr* hdr_, size_t len_)
{
  return (false);
}

bool
AttributeMap::Disassemble(struct nlattr* hdr_, size_t len_)
{
  struct nlattr* attr = NULL;
  int rem = 0;

  nla_for_each_attr(attr, hdr_, len_, rem)
  {
    this->_attrs[attr->nla_type] = attr;
  }

  return (true);
}

bool
AttributeMap::GetAttribute(Attribute& attr_)
{
  bool status = false;
  if (this->_attrs.count(attr_.GetId()))
  {
    struct nlattr* attr = this->_attrs[attr_.GetId()];
    status = (attr_.SetValue((const uint8_t*)nla_data(attr), nla_len(attr)) == nla_len(attr));
  }
  return (status);
}

bool
AttributeMap::PutAttribute(Attribute& attr_)
{
  return (false);
}

//*****************************************************************************
// Class: AttributeNested
//*****************************************************************************

AttributeNested::AttributeNested(const uint32_t id_) :
  _id(id_)
{
}

AttributeNested::~AttributeNested()
{
}

bool
AttributeNested::Assemble(struct nlattr* hdr_)
{
  return (false);
}

bool
AttributeNested::Disassemble(struct nlattr* hdr_)
{
  struct nlattr* attr = NULL;
  int rem = 0;

  for (attr = (struct nlattr*)nla_data(hdr_), rem = nla_len(hdr_);
      nla_ok(attr, rem); attr = nla_next(attr, &rem))
  {
    this->_attrs.emplace_back();
    this->_attrs[this->_attrs.size() - 1].Disassemble(attr, attr->nla_len);
  }

  return (true);
}

}
