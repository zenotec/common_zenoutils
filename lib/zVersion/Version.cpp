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

#ifdef HAVE_CONFIG_H
#include "config.h"
#else
#define VERSION " ????? "
#endif

#include <zutils/zVersion.h>

namespace zUtils
{

//*****************************************************************************
// Class: Version
//*****************************************************************************

Version::Version()
{
}

Version::~Version()
{
}

std::string
Version::GetVersion() const
{
  return (std::string(VERSION));
}

std::string
Version::GetDate() const
{
  return (std::string(__DATE__));
}

std::string
Version::GetTime() const
{
  return (std::string(__TIME__));
}

}
