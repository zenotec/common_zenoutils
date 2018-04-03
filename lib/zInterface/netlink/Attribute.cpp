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

void
__dump_hex(const char* prefix_, const uint8_t* addr_, size_t len_, bool verbose_)
{
  const char* nl = "";
  unsigned long pad = ((unsigned long)addr_ & 0x07);

  if (!len_)
    return;

  if (!verbose_)
    len_ = std::min(int(len_), 16);

  for (int cnt = 0, i = -pad; i < len_; cnt++, i++)
  {
    if (!(cnt % 8))
      printf("%s%s%p: ", nl, prefix_, &addr_[i]);
    if (i < 0)
      printf("-- ");
    else
      printf("%02x ", addr_[i]);
    nl = "\n";
  }
  printf("\n");
}

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
      this->Set(val->GetData(), val->GetLength());
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
  if (attr_ && this->SetId(nla_type(attr_)))
  {
    status = this->Set((const uint8_t*)nla_data(attr_), nla_len(attr_));
  }
  return (status);
}

const uint8_t*
AttributeValue::GetData() const
{
  return (this->_data.data());
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
  if (this->IsValid() && this->GetData() && this->GetLength())
  {
    str_ = std::string((char*)this->GetData());
  }
  return (status);
}

bool
AttributeValue::Set(const std::string& str_)
{
  bool status = false;
  size_t len = strlen(str_.c_str());
  this->_data.resize(len + 1); // add null terminator
  if (strncpy((char*)this->GetData(), str_.c_str(), this->GetLength()) == (char*)this->GetData())
  {
    this->SetValid();
    status = true;
  }
  return (status);
}

bool
AttributeValue::Get(uint8_t* p_, size_t& len_) const
{
  bool status = false;
  if (this->IsValid())
  {
    len_ = std::min(this->GetLength(), len_);
    status = (memcpy(p_, this->_data.data(), len_) == p_);
  }
  return (status);
}

bool
AttributeValue::Set(const uint8_t* p_, const size_t len_)
{
  bool status = false;
  this->_data.resize(len_);
  if (memcpy(this->_data.data(), p_, len_) == this->_data.data())
  {
    this->SetValid();
    status = true;
  }
  return (status);
}

void
AttributeValue::Display(const std::string& prefix_) const
{
  std::cout << prefix_ << "Value[" << this->GetId() << "]: " << this->GetLength() << std::endl;
  __dump_hex((prefix_ + "\t").c_str(), this->GetData(), this->GetLength(), false);
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
//    fprintf(stderr, "AttributeTable::Disassemble(): %p[%d]: %d %s\n",
//        pos, nla_type(pos), nla_len(pos), (nla_is_nested(pos) ? "+" : ""));
    SHARED_PTR(Attribute) a;
    if (nla_is_nested(pos))
    {
      a = STATIC_CAST(Attribute)(SHARED_PTR(AttributeNested)(new AttributeNested(nla_type(pos))));
    }
    else
    {
      a = STATIC_CAST(Attribute)(SHARED_PTR(AttributeValue)(new AttributeValue(nla_type(pos))));
    }
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
  if (attr_ && attr_->IsValid())
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
  std::cout << prefix_ << "Table: " << this->_attrs.size() << std::endl;
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
      AttributeValue* val = (AttributeValue*) other_;
      if (!this->Disassemble((struct nlattr*)val->GetData(), val->GetLength()))
      {
        std::cerr << "AttributeNested::operator=(): Error disassembling value" << std::endl;
      }
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
