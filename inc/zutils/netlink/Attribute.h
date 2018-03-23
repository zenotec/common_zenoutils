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

#ifndef __NETLINK_ATTRIBUTE_H__
#define __NETLINK_ATTRIBUTE_H__

#include <stdint.h>
#include <stddef.h>

#include <netlink/attr.h>

#include <string>
#include <vector>
#include <map>

#include <zutils/zUtils.h>

namespace netlink
{

//*****************************************************************************
// Class: Attribute
//*****************************************************************************

class Attribute
{

public:

  Attribute(const uint32_t id_ = 0, const bool nested_ = false);

  Attribute(const Attribute& other_);

  virtual
  ~Attribute();

  Attribute&
  operator=(const Attribute& other_);

  Attribute&
  operator+=(const Attribute& other_);

  virtual bool
  Assemble(struct nl_msg* msg_);

  virtual bool
  Disassemble(struct nlattr* attr_, size_t len_ = 0);

  uint32_t
  GetId() const;

  bool
  SetId(const uint32_t id_);

  virtual size_t
  GetLength() const;

  template<typename T>
    T
    Get() const
    {
      T value;
      this->Get(value);
      return (value);
    }

  template<typename T>
    bool
    Get(T& value_) const
    {
      bool status = false;
      size_t len = sizeof(T);
      if (len == this->GetLength())
      {
        status = this->Get((uint8_t*) &value_, len);
      }
      return (status);
    }

  bool
  Get(std::string& str_) const;

  bool
  Get(uint8_t* p_, size_t& len_) const;

  template<typename T>
    bool
    Set(const T value_)
    {
      return (this->Set((uint8_t*) &value_, sizeof(T)));
    }

  bool
  Set(const std::string& str_);

  bool
  Set(const uint8_t* p_, const size_t len_);

  bool
  Get(Attribute& attr_);

  bool
  Put(Attribute& attr_);

  virtual void
  Display(const std::string& prefix_ = "") const;

protected:

  bool nested;
  std::map<int, std::vector<uint8_t> > attrs;

private:

  uint32_t _id;

};
//
////*****************************************************************************
//// Class: AttributeTable
////*****************************************************************************
//
//class AttributeTable
//{
//
//public:
//
//  AttributeTable();
//
//  virtual
//  ~AttributeTable();
//
//  virtual bool
//  Assemble(struct nl_msg* msg_);
//
//  virtual bool
//  Disassemble(struct nlattr* attr_, size_t len_);
//
//  bool
//  Get(Attribute& attr_);
//
//  bool
//  Put(Attribute& attr_);
//
//  virtual size_t
//  GetLength() const;
//
//  virtual void
//  Display(const std::string& prefix_ = "") const;
//
//protected:
//
//private:
//
//  std::map<int, Attribute> _attrs;
//
//};
//
////*****************************************************************************
//// Class: AttributeNested
////*****************************************************************************
//
//class AttributeNested : public Attribute
//{
//
//public:
//
//  AttributeNested(const uint32_t id_);
//
//  virtual
//  ~AttributeNested();
//
//  AttributeNested&
//  operator=(const Attribute& other_);
//
//  virtual bool
//  Assemble(struct nl_msg* msg_);
//
//  virtual bool
//  Disassemble(struct nlattr* attr_, size_t len_ = 0);
//
//  bool
//  Get(Attribute& attr_);
//
//  bool
//  Put(Attribute& attr_);
//
//  size_t
//  GetLength() const;
//
//  virtual void
//  Display(const std::string& prefix_ = "") const;
//
//protected:
//
//private:
//
//  AttributeTable _attrs;
//
//};

}

#endif /* __NETLINK_ATTRIBUTE_H__ */
