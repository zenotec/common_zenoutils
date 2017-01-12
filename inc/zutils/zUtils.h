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

#ifndef __ZUTILS_H__
#define __ZUTILS_H__

#include <stdint.h>

#include <string>
#include <sstream>

namespace zUtils
{

//**********************************************************************
// String conversion utilities
//**********************************************************************

inline std::string
_tostr(char n_)
{
  char str[256];
  snprintf(str, 256, "%hhd", n_);
  return (std::string(str));
}

inline std::string
_tostr(unsigned char n_)
{
  char str[256];
  snprintf(str, 256, "%hhu", n_);
  return (std::string(str));
}

inline std::string
_tostr(int n_)
{
  char str[256];
  snprintf(str, 256, "%d", n_);
  return (std::string(str));
}

inline std::string
_tostr(unsigned int n_)
{
  char str[256];
  snprintf(str, 256, "%u", n_);
  return (std::string(str));
}

inline std::string
_tostr(long n_)
{
  char str[256];
  snprintf(str, 256, "%ld", n_);
  return (std::string(str));
}

inline std::string
_tostr(unsigned long n_)
{
  char str[256];
  snprintf(str, 256, "%lu", n_);
  return (std::string(str));
}

inline std::string
_tostr(void *p_)
{
  char str[256];
  snprintf(str, 256, "%p", p_);
  return (std::string(str));
}

template<typename T>
  inline std::string
  zToStr(const T value_)
  {
    return (_tostr(value_));
  }

inline int
_toint(const std::string& value_)
{
  return(atoi(value_.c_str()));
}

template<typename T>
  inline int
  zToInt(const T value_)
  {
    return(_toint(value_));
  }

}

#endif /* __ZUTILS_H__ */
