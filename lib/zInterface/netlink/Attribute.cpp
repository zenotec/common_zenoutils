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

Attribute::Attribute(const uint32_t id_, const bool nested_) :
    nested(nested_), _id(id_)
{
}

Attribute::Attribute(const Attribute& other_) :
    nested(other_.nested), _id(other_._id), attrs(other_.attrs)
{
}

Attribute::~Attribute()
{
}

Attribute&
Attribute::operator=(const Attribute& other_)
{
  this->_id = other_._id;
  if (!this->nested && other_.nested)
  {
    // TODO: Convert to un-nested
  }
  else if (this->nested && !other_.nested)
  {
    // TODO: Convert to nested
  }
  else
  {
    this->attrs = other_.attrs;
  }
  return (*this);
}

Attribute&
Attribute::operator +=(const Attribute& other_)
{
  return (*this);
}

bool
Attribute::Assemble(struct nl_msg* msg_)
{
  bool status = true;

  if (msg_)
  {
    return (false);
  }

  if (this->nested)
  {
    struct nlattr* start = nla_nest_start(msg_, this->GetId());
    if (start)
    {
      FOREACH(auto& attr, this->attrs)
      {
        status &= (nla_put(msg_, attr.first, attr.second.size(), attr.second.data()) == 0);
      }
      nla_nest_end(msg_, start);
    }
    else
    {
      nla_nest_cancel(msg_, start);
    }
  }
  else
  {
    status = (nla_put(msg_, this->GetId(), this->attrs[0].size(), this->attrs[0].data()) == 0);
  }
  return (status);
}

bool
Attribute::Disassemble(struct nlattr* attr_, size_t len)
{
  bool status = false;
  fprintf(stderr, "AttributeValue::Disassemble(): %p[%d]: %d\n", attr_, nla_type(attr_), nla_len(attr_));
  if (attr_ && this->SetId(nla_type(attr_)))
  {
    status = this->Set((const uint8_t*)nla_data(attr_), nla_len(attr_));
  }
  return (status);
}

uint32_t
Attribute::GetId() const
{
  return (this->_id);
}

bool
Attribute::SetId(const uint32_t id_)
{
  this->_id = id_;
  return (true);
}

size_t
Attribute::GetLength() const
{
  size_t len = 0;
  if (this->nested)
  {
    FOREACH(auto& attr, this->attrs)
    {
      len += (sizeof(struct nlattr) + attr.second.size());
    }
  }
  else if (this->attrs.count(0))
  {
    len = this->attrs.at(0).size();
  }
  return (len);
}

bool
Attribute::Get(std::string& str_) const
{
  bool status = false;
  return (status);
}

bool
Attribute::Set(const std::string& str_)
{
  bool status = false;
  size_t len = strlen(str_.c_str());
  this->attrs[0].resize(len);
  status = (strcpy((char*)this->attrs[0].data(), str_.c_str()));
  return (status);
}

bool
Attribute::Get(uint8_t* p_, size_t& len_) const
{
  bool status = false;
  return (status);
}

bool
Attribute::Set(const uint8_t* p_, const size_t len_)
{
  bool status = false;
  this->attrs[0].resize(len_);
  if (memcpy(this->attrs[0].data(), p_, len_) == this->attrs[0].data())
  {
    status = true;
  }
  return (status);
}

bool
Attribute::Get(Attribute& attr_)
{
  bool status = false;
  if (this->nested)
  {

  }
  return (status);
}

bool
Attribute::Put(Attribute& attr_)
{
  bool status = false;
  if (this->nested)
  {
    this->attrs[attr_.GetId()] = attr_.attrs[0];
  }
  return (status);
}

void
Attribute::Display(const std::string& prefix_) const
{
  std::cout << prefix_ << "Attr[" << this->GetId() << "]: " << this->GetLength() << std::endl;
}
//
////*****************************************************************************
//// Class: AttributeTable
////*****************************************************************************
//
//AttributeTable::AttributeTable()
//{
//}
//
//AttributeTable::~AttributeTable()
//{
//}
//
//bool
//AttributeTable::Assemble(struct nl_msg* msg_)
//{
//  bool status = true;
//  FOREACH(auto& attr, this->_attrs)
//  {
//    status &= attr.second.Assemble(msg_);
//  }
//  return (status);
//}
//
//bool
//AttributeTable::Disassemble(struct nlattr* attr_, size_t len_)
//{
//
//  bool status = true;
//  struct nlattr* pos = NULL;
//  int rem = 0;
//
//  // Start fresh
//  this->_attrs.clear();
//
//  for (pos = attr_, rem = len_; nla_ok(pos, rem); pos = nla_next(pos, &rem))
//  {
//    fprintf(stderr, "AttributeTable::Disassemble(): %p[%d]: %d\n", pos, nla_type(pos), nla_len(pos));
//    status &= this->_attrs[nla_type(pos)].Disassemble(pos);
//  }
//
//  return (status);
//}
//
//size_t
//AttributeTable::GetLength() const
//{
//  return (this->_attrs.size());
//}
//
//bool
//AttributeTable::Get(Attribute& attr_)
//{
//  bool status = false;
//  if (this->_attrs.count(attr_.GetId()))
//  {
//    attr_ = this->_attrs.at(attr_.GetId());
//    status = true;
//  }
//  return (status);
//}
//
//bool
//AttributeTable::Put(Attribute& attr_)
//{
//  this->_attrs[attr_.GetId()] = attr_;
//  return (true);
//}
//
//void
//AttributeTable::Display(const std::string& prefix_) const
//{
//  std::cout << prefix_ << "Table" << std::endl;
//  FOREACH (auto& attr, this->_attrs)
//  {
//    attr.second.Display(prefix_ + std::string("\t"));
//  }
//  return;
//}
//
////*****************************************************************************
//// Class: AttributeNested
////*****************************************************************************
//
//AttributeNested::AttributeNested(const uint32_t id_) :
//    Attribute(id_)
//{
//}
//
//AttributeNested::~AttributeNested()
//{
//}
//
//bool
//AttributeNested::Assemble(struct nl_msg* msg_)
//{
//  bool status = false;
//  struct nlattr* start = nla_nest_start(msg_, (this->GetId() | NLA_F_NESTED));
//  if (msg_ && start)
//  {
//    status = this->_attrs.Assemble(msg_);
//    nla_nest_end(msg_, start);
//  }
//  else
//  {
//    nla_nest_cancel(msg_, start);
//  }
//  return (status);
//}
//
//bool
//AttributeNested::Disassemble(struct nlattr* attr_, size_t len_)
//{
//  bool status = Attribute::Disassemble(attr_, len_);
//  status &= this->_attrs.Disassemble((struct nlattr*)nla_data(attr_), nla_len(attr_));
//  return (status);
//}
//
//size_t
//AttributeNested::GetLength() const
//{
//  return (Attribute::GetLength());
//}
//
//bool
//AttributeNested::Get(Attribute& attr_)
//{
//  return (this->_attrs.Get(attr_));
//}
//
//bool
//AttributeNested::Put(Attribute& attr_)
//{
//  return (this->_attrs.Put(attr_));
//}
//
//void
//AttributeNested::Display(const std::string& prefix_) const
//{
//  std::cout << prefix_ << "Nested" << std::endl;
//  this->_attrs.Display(prefix_ + std::string("\t"));
//}

}
