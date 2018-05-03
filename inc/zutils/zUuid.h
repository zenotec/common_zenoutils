/*
 * Copyright (c) 2014-2016 ZenoTec LLC (http://www.zenotec.net)
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

#ifndef __ZUUID_H__
#define __ZUUID_H__

#include <stdint.h>
#include <string.h>

#include <uuid/uuid.h>

#include <string>
#include <sstream>
#include <array>

namespace zUtils
{
namespace zUuid
{

class Uuid
{

public:

  struct uuid
  {
    union
    {
      uuid_t uuid;
      uint8_t u8[16];
      uint32_t u32[4];
      uint64_t u64[2];
    } u;
  };


  Uuid() :
    _uuid { 0 }
  {
    this->Regenerate();
  }

  Uuid(const uint8_t uuid_[16]) :
    _uuid { 0 }
  {
    this->operator =(uuid_);
  }

  Uuid(const uint32_t uuid_[4]) :
    _uuid { 0 }
  {
    this->operator =(uuid_);
  }

  Uuid(const uint64_t uuid_[2]) :
    _uuid { 0 }
  {
    this->operator =(uuid_);
  }

  virtual
  ~Uuid()
  {
  }

  Uuid&
  operator=(const uint8_t uuid_[16])
  {
    memcpy(&this->_uuid, uuid_, 16);
    return (*this);
  }

  Uuid&
  operator=(const uint32_t uuid_[4])
  {
    memcpy(&this->_uuid, uuid_, 16);
    return (*this);
  }

  Uuid&
  operator=(const uint64_t uuid_[2])
  {
    memcpy(&this->_uuid, uuid_, 16);
    return (*this);
  }

  bool
  operator<(const Uuid& other) const
  {
    return (uuid_compare(this->_uuid.u.uuid, other._uuid.u.uuid) < 0);
  }

  bool
  operator>(const Uuid& other) const
  {
    return (uuid_compare(this->_uuid.u.uuid, other._uuid.u.uuid) > 0);
  }

  bool
  operator<=(const Uuid& other) const
  {
    return (!(*this > other));
  }

  bool
  operator>=(const Uuid& other) const
  {
    return (!(*this < other));
  }

  bool
  operator==(const Uuid& other) const
  {
    return (uuid_compare(this->_uuid.u.uuid, other._uuid.u.uuid) == 0);
  }

  bool
  operator!=(const Uuid& other) const
  {
    return (uuid_compare(this->_uuid.u.uuid, other._uuid.u.uuid) != 0);
  }

  const uuid&
  operator()() const
  {
    return (this->_uuid);
  }

  bool
  operator()(const Uuid::uuid& uuid_)
  {
    this->_uuid = uuid_;
    return (true);
  }

  std::string
  Str() const
  {
    char c_str[256] = { 0 };
    uuid_unparse(this->_uuid.u.uuid, c_str);
    return (std::string(c_str));
  }

  void
  Regenerate()
  {
    uuid_generate(this->_uuid.u.uuid);
  }

protected:

private:

  struct uuid _uuid;

};

}
}


// This is required to be used as a key for std::unordered_map
namespace std
{
  template <>
  struct hash<zUtils::zUuid::Uuid>
  {
    size_t operator()(const zUtils::zUuid::Uuid& k) const
    {
      return (size_t(k().u.u64[0]));
    }
  };
}


#endif /* __ZUUID_H__ */
