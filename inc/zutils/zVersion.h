/*
 * Copyright (c) 2014-2018 ZenoTec LLC (http://www.zenotec.net)
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

#ifndef __ZVERSION_H__
#define __ZVERSION_H__

#include <stdint.h>

#include <string>

namespace zUtils
{

//*****************************************************************************
// Class::Version
//*****************************************************************************

class Version
{

public:

  virtual
  ~Version();

  static Version&
  Instance()
  {
    static Version instance;
    return (instance);
  }

  std::string
  GetVersion() const;

  std::string
  GetDate() const;

  std::string
  GetTime() const;

private:

  Version();

};

}

#endif /* __ZVERSION_H__ */
