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

  Attribute(const uint32_t id_ = 0, const bool nested_ = false) :
    _valid(false), _id(id_), _nested(nested_)
  {
  }

  virtual
  ~Attribute()
  {
  }

  virtual bool
  Assemble(struct nl_msg* msg_) = 0;

  virtual bool
  Disassemble(struct nlattr* attr_, size_t len_ = 0) = 0;

  bool
  IsValid() const
  {
    return (this->_valid);
  }

  bool
  SetValid()
  {
    this->_valid = true;
    return (this->_valid);
  }

  bool
  ClrValid()
  {
    this->_valid = false;
    return (!this->_valid);
  }

  bool
  IsNested() const
  {
    return (this->_nested);
  }

  uint32_t
  GetId() const
  {
    return (this->_id);
  }

  bool
  SetId(const uint32_t id_)
  {
    this->_id = id_;
    return (true);
  }

  virtual size_t
  GetLength() const = 0;

  virtual void
  Display(const std::string& prefix_ = "") const = 0;

protected:

private:

  bool _valid;
  bool _nested;
  uint32_t _id;

};

//*****************************************************************************
// Class: AttributeValue
//*****************************************************************************

class AttributeValue :
    public Attribute
{

public:

  AttributeValue(const uint32_t id_ = 0);

  AttributeValue(const Attribute* other_);

  virtual
  ~AttributeValue();

  AttributeValue&
  operator=(const Attribute* other_);

  virtual bool
  Assemble(struct nl_msg* msg_);

  virtual bool
  Disassemble(struct nlattr* attr_, size_t len_ = 0);

  const uint8_t*
  GetData() const;

  virtual size_t
  GetLength() const;

  template<typename T>
    T
    Get(T default_) const
    {
      bool status = false;
      T value;
      size_t len = sizeof(T);
      if ((len != this->GetLength()) || !this->Get((uint8_t*) &value, len))
      {
        value = default_;
      }

      return (value);
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

  virtual void
  Display(const std::string& prefix_ = "") const;

protected:

private:

  std::vector<uint8_t> _data;

};

//*****************************************************************************
// Class: AttributeTable
//*****************************************************************************

class AttributeTable
{

public:

  AttributeTable();

  virtual
  ~AttributeTable();

  virtual bool
  Assemble(struct nl_msg* msg_);

  virtual bool
  Disassemble(struct nlattr* attr_, size_t len_);

  bool
  Get(Attribute* attr_) const;

  bool
  Put(Attribute* attr_);

  virtual size_t
  GetLength() const;

  virtual void
  Display(const std::string& prefix_ = "") const;

protected:

private:

  std::map<int, SHPTR(Attribute)> _attrs;

};

//*****************************************************************************
// Class: AttributeNested
//*****************************************************************************

class AttributeNested :
    public Attribute
{

public:

  AttributeNested(const uint32_t id_);

  AttributeNested(const Attribute* other_);

  virtual
  ~AttributeNested();

  virtual AttributeNested&
  operator=(const Attribute* other_);

  virtual bool
  Assemble(struct nl_msg* msg_);

  virtual bool
  Disassemble(struct nlattr* attr_, size_t len_ = 0);

  bool
  Get(Attribute* attr_) const;

  bool
  Put(Attribute* attr_);

  size_t
  GetLength() const;

  virtual void
  Display(const std::string& prefix_ = "") const;

protected:

private:

  AttributeTable _attrs;

};

}

#endif /* __NETLINK_ATTRIBUTE_H__ */
