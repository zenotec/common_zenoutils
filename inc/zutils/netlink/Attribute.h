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

#ifndef __ATTRIBUTE_H__
#define __ATTRIBUTE_H__

#include <stddef.h>

#include <netlink/attr.h>

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

  enum TYPE
  {
    TYPE_ERR = -1,
    TYPE_NONE = NLA_UNSPEC,
    TYPE_U8 = NLA_U8,     /**< 8 bit integer */
    TYPE_U16 = NLA_U16,    /**< 16 bit integer */
    TYPE_U32 = NLA_U32,    /**< 32 bit integer */
    TYPE_U64 = NLA_U64,    /**< 64 bit integer */
    TYPE_STRING = NLA_STRING, /**< NUL terminated character string */
    TYPE_FLAG = NLA_FLAG,   /**< Flag */
    TYPE_MSECS = NLA_MSECS,  /**< Micro seconds (64bit) */
    TYPE_NESTED = NLA_NESTED, /**< Nested attributes */
    TYPE_BINARY = NLA_BINARY,
    TYPE_S8 = NLA_S8,
    TYPE_S16 = NLA_S16,
    TYPE_S32 = NLA_S32,
    TYPE_S64 = NLA_S64,
    TYPE_LAST = __NLA_TYPE_MAX
  };

  Attribute(const Attribute::TYPE type_, const uint32_t id_, const size_t len_ = 0);

  virtual
  ~Attribute();

  Attribute&
  operator=(const Attribute& other_);

  bool
  IsValid() const;

  void
  SetValid();

  void
  ClrValid();

  Attribute::TYPE
  GetType() const;

  uint32_t
  GetId() const;

  size_t
  GetLength() const;

  size_t
  GetValue(uint8_t* p_, size_t len_) const;

  bool
  GetValue(std::string& str_) const;

  template<typename T>
    bool
    GetValue(T& value_) const
    {
      bool status = false;
      if (sizeof(T) == this->GetLength())
      {
        status = (this->GetValue((uint8_t*) &value_, sizeof(T)) == sizeof(T));
      }
      return (status);
    }

  size_t
  SetValue(const uint8_t* p_, size_t len_);

  bool
  SetValue(const std::string& str_);

  template<typename T>
    bool
    SetValue(T& value_)
    {
      return (this->SetValue((uint8_t*) &value_, sizeof(T)) == sizeof(T));
    }

protected:

  std::vector<uint8_t> _value;

private:

  bool _valid;
  TYPE _type;
  uint32_t _id;

};

//*****************************************************************************
// Class: AttributeMap
//*****************************************************************************

class AttributeMap
{

public:

  AttributeMap();

  virtual
  ~AttributeMap();

  bool
  Assemble(struct nlattr* hdr_, size_t len_);

  bool
  Disassemble(struct nlattr* hdr_, size_t len_);

  bool
  GetAttribute(Attribute& attr_);

  bool
  PutAttribute(Attribute& attr_);

protected:

private:

  std::map<int, struct nlattr*> _attrs;

};

//*****************************************************************************
// Class: AttributeNested
//*****************************************************************************

class AttributeNested
{

public:

  AttributeNested(const uint32_t id_);

  virtual
  ~AttributeNested();

  bool
  Assemble(struct nlattr* hdr_);

  bool
  Disassemble(struct nlattr* hdr_);

protected:

private:

  uint32_t _id;
  std::vector<AttributeMap> _attrs;

};

}

#endif /* __ATTRIBUTE_H__ */
