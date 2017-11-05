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

#include <zutils/zUtils.h>

typedef std::pair<void*, size_t> AttributeBuffer;

//*****************************************************************************
// Class: Attribute
//*****************************************************************************

template<class T>
  class Attribute
  {
  public:

    Attribute(const uint32_t id_) :
        _id(id_)
    {
    }

    virtual
    ~Attribute()
    {
    }

    virtual T&
    operator()()
    {
      return (this->_value);
    }

    virtual void
    operator()(const T val_)
    {
      this->_value = val_;
    }

    uint32_t
    Id() const
    {
      return (this->_id);
    }

    T
    GetValue() const
    {
      return (this->_value);
    }

    void
    SetValue(const T val_)
    {
      this->_value = val_;
    }

  protected:

  private:

    uint32_t _id;
    T _value;

  };

#endif /* __ATTRIBUTE_H__ */
