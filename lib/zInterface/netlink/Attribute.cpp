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
#include <string.h>

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

AttributeValue::AttributeValue(const uint32_t id_) :
    Attribute(id_)
{
}

AttributeValue::AttributeValue(const Attribute* other_) :
    Attribute(0)
{
  this->operator =(other_);
}

AttributeValue::~AttributeValue()
{
}

AttributeValue&
AttributeValue::operator =(const Attribute* other_)
{
  if (other_)
  {
    this->SetId(other_->GetId());
    if (other_->IsNested())
    {
      AttributeNested* val = (AttributeNested*) other_;
      // TODO:
      std::cerr << "AttributeValue::operator=(): Not implemented" << std::endl;
    }
    else
    {
      AttributeValue* val = (AttributeValue*) other_;
      this->_data = val->_data;
    }
  }
  return (*this);
}

bool
AttributeValue::Assemble(struct nl_msg* msg_)
{
  bool status = true;
  if (msg_)
  {
    status = (nla_put(msg_, this->GetId(), this->_data.size(), this->_data.data()) == 0);
  }
  return (status);
}

bool
AttributeValue::Disassemble(struct nlattr* attr_, size_t len)
{
  bool status = false;
  fprintf(stderr, "AttributeValue::Disassemble(): %p[%d]: %d\n", attr_, nla_type(attr_), nla_len(attr_));
  if (attr_ && this->SetId(nla_type(attr_)))
  {
    status = this->Set((const uint8_t*)nla_data(attr_), nla_len(attr_));
  }
  return (status);
}

size_t
AttributeValue::GetLength() const
{
  return (this->_data.size());
}

bool
AttributeValue::Get(std::string& str_) const
{
  bool status = false;
  return (status);
}

bool
AttributeValue::Set(const std::string& str_)
{
  bool status = false;
  size_t len = strlen(str_.c_str());
  this->_data.resize(len);
  status = (strcpy((char*)this->_data.data(), str_.c_str()));
  return (status);
}

bool
AttributeValue::Get(uint8_t* p_, size_t& len_) const
{
  bool status = false;
  return (status);
}

bool
AttributeValue::Set(const uint8_t* p_, const size_t len_)
{
  bool status = false;
  this->_data.resize(len_);
  status = (memcpy(this->_data.data(), p_, len_) == this->_data.data());
  return (status);
}

void
AttributeValue::Display(const std::string& prefix_) const
{
  std::cout << prefix_ << "Value[" << this->GetId() << "]: " << this->GetLength() << std::endl;
}

//*****************************************************************************
// Class: AttributeTable
//*****************************************************************************

AttributeTable::AttributeTable()
{
}

AttributeTable::~AttributeTable()
{
}

bool
AttributeTable::Assemble(struct nl_msg* msg_)
{
  bool status = true;
  FOREACH(auto& attr, this->_attrs)
  {
    status &= attr.second->Assemble(msg_);
  }
  return (status);
}

bool
AttributeTable::Disassemble(struct nlattr* attr_, size_t len_)
{

  bool status = true;
  struct nlattr* pos = NULL;
  int rem = 0;

  // Start fresh
  this->_attrs.clear();

  for (pos = attr_, rem = len_; nla_ok(pos, rem); pos = nla_next(pos, &rem))
  {
    fprintf(stderr, "AttributeTable::Disassemble(): %p[%d]: %d\n", pos, nla_type(pos), nla_len(pos));
    SHARED_PTR(Attribute) a(STATIC_CAST(Attribute)(SHARED_PTR(AttributeValue)(new AttributeValue(nla_type(pos)))));
    status &= a->Disassemble(pos);
    this->_attrs[nla_type(pos)] = a;
  }

  return (status);
}

size_t
AttributeTable::GetLength() const
{
  return (this->_attrs.size());
}

bool
AttributeTable::Get(Attribute* attr_)
{
  bool status = false;
  if (attr_ && this->_attrs.count(attr_->GetId()))
  {
    if (attr_->IsNested())
    {
      *(AttributeNested*)attr_ = this->_attrs.at(attr_->GetId()).get();
    }
    else
    {
      *(AttributeValue*)attr_ = this->_attrs.at(attr_->GetId()).get();
    }
    status = true;
  }
  return (status);
}

bool
AttributeTable::Put(Attribute* attr_)
{
  bool status = false;
  if (attr_)
  {
    SHARED_PTR(Attribute) a;
    if (attr_->IsNested())
    {
      a = STATIC_CAST(Attribute)(SHARED_PTR(AttributeNested)(new AttributeNested(attr_)));
    }
    else
    {
      a = STATIC_CAST(Attribute)(SHARED_PTR(AttributeValue)(new AttributeValue(attr_)));
    }
    this->_attrs[attr_->GetId()] = a;
  }
  return (true);
}

void
AttributeTable::Display(const std::string& prefix_) const
{
  std::cout << prefix_ << "Table" << std::endl;
  FOREACH (auto& attr, this->_attrs)
  {
    attr.second->Display(prefix_ + std::string("\t"));
  }
  return;
}

//*****************************************************************************
// Class: AttributeNested
//*****************************************************************************

AttributeNested::AttributeNested(const uint32_t id_) :
    Attribute(id_, true)
{
}

AttributeNested::AttributeNested(const Attribute* other_) :
    Attribute(0)
{
  this->operator =(other_);
}

AttributeNested::~AttributeNested()
{
}

AttributeNested&
AttributeNested::operator =(const Attribute* other_)
{
  if (other_)
  {
    this->SetId(other_->GetId());
    if (other_->IsNested())
    {
      AttributeNested* val = (AttributeNested*) other_;
      this->_attrs = val->_attrs;
    }
    else
    {
      // TODO:
      std::cerr << "AttributeNested::operator=(): Not implemented" << std::endl;
    }
  }
  return (*this);
}

bool
AttributeNested::Assemble(struct nl_msg* msg_)
{
  bool status = false;
  struct nlattr* start = nla_nest_start(msg_, (this->GetId() | NLA_F_NESTED));
  if (msg_ && start)
  {
    status = this->_attrs.Assemble(msg_);
    nla_nest_end(msg_, start);
  }
  else
  {
    nla_nest_cancel(msg_, start);
  }
  return (status);
}

bool
AttributeNested::Disassemble(struct nlattr* attr_, size_t len_)
{
  bool status = true;
  struct nlattr* pos = NULL;
  int rem = 0;
  for (pos = (struct nlattr*)nla_data(attr_), rem = nla_len(attr_);
      nla_ok(pos, rem); pos = nla_next(pos, &rem))
  {
    status &= this->_attrs.Disassemble((struct nlattr*)nla_data(pos), nla_len(pos));
  }
  return (status);
}

size_t
AttributeNested::GetLength() const
{
  return (this->_attrs.GetLength());
}

bool
AttributeNested::Get(Attribute* attr_)
{
  return (this->_attrs.Get(attr_));
}

bool
AttributeNested::Put(Attribute* attr_)
{
  return (this->_attrs.Put(attr_));
}

void
AttributeNested::Display(const std::string& prefix_) const
{
  std::cout << prefix_ << "Nested[" << this->GetId() << "]: " << this->GetLength() << std::endl;
  this->_attrs.Display(prefix_ + std::string("\t"));
}

}
