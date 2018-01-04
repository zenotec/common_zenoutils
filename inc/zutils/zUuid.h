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

#include <uuid/uuid.h>

#include <string>
#include <sstream>

namespace zUtils
{
namespace zUuid
{

class Uuid
{

public:

  Uuid() :
    _uuid { 0 }
  {
    uuid_generate(this->_uuid);
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
    uuid_copy(this->_uuid, uuid_);
    return (*this);
  }

  Uuid&
  operator=(const uint32_t uuid_[4])
  {
    memcpy(&this->_uuid[0], &uuid_[0], sizeof(this->_uuid));
    return (*this);
  }

  Uuid&
  operator=(const uint64_t uuid_[2])
  {
    memcpy(&this->_uuid[0], &uuid_[0], sizeof(this->_uuid));
    return (*this);
  }

  bool
  operator<(const Uuid& other)
  {
    return (uuid_compare(this->_uuid, other._uuid) < 0);
  }

  bool
  operator>(const Uuid& other)
  {
    return (uuid_compare(this->_uuid, other._uuid) > 0);
  }

  bool
  operator<=(const Uuid& other)
  {
    return (!(*this > other));
  }

  bool
  operator>=(const Uuid& other)
  {
    return (!(*this < other));
  }

  bool
  operator==(const Uuid& other)
  {
    return (uuid_compare(this->_uuid, other._uuid) == 0);
  }

  bool
  operator!=(const Uuid& other)
  {
    return (uuid_compare(this->_uuid, other._uuid) != 0);
  }

  std::string
  Str() const
  {
    char c_str[256] = { 0 };
    uuid_unparse(this->_uuid, c_str);
    return (std::string(c_str));
  }

  void
  Regenerate()
  {
    uuid_generate(this->_uuid);
  }

protected:

private:

  uuid_t _uuid;

};

}
}

#endif /* __ZUUID_H__ */
