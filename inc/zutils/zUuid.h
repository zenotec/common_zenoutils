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
  static std::string
  Create(const unsigned int len = 64)
  {
    uuid_t uuid;
    uuid_generate(uuid);
    char c_str[256] = { 0 };
    uuid_unparse(uuid, c_str);
    return (std::string(c_str));
  }

protected:

private:

};

}
}

#endif /* __ZUUID_H__ */
