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


#include <zutils/zProgOpt.h>

namespace zUtils
{
namespace zProgOpt
{

//*****************************************************************************
//*****************************************************************************
Option::Option(uint32_t flags_) :
    _cnt(0), _flags(Option::FLAGS_NONE), _short_name(0)
{
  this->_setFlags(flags_);
}

Option::~Option()
{
}

uint32_t
Option::Flags() const
{
  return (this->_flags);
}

const char
Option::ShortName() const
{
  return (this->_short_name);
}

bool
Option::ShortName (const char name_)
{
  this->_short_name = name_;
  return(true);
}

std::string
Option::LongName() const
{
  return (this->_long_name);
}

bool
Option::LongName (const std::string& name_)
{
  this->_long_name = name_;
  return(true);
}

std::string
Option::HelpMsg() const
{
  return (this->_help);
}

void
Option::HelpMsg(const std::string &msg_)
{
  this->_help = msg_;
}

void
Option::_getArg(std::string &arg_)
{
  arg_ = this->_arg;
}

template int32_t
Option::Arg();

void
Option::_getArg(int &arg_)
{
  arg_ = 0;
  sscanf(this->_arg.c_str(), "%d", &arg_);
}

template uint32_t
Option::Arg();

void
Option::_getArg(unsigned int &arg_)
{
  arg_ = 0;
  sscanf(this->_arg.c_str(), "%u", &arg_);
}

template double
Option::Arg();

void
Option::_getArg(double &arg_)
{
  arg_ = 0.0;
  sscanf(this->_arg.c_str(), "%lf", &arg_);
}

bool
Option::_setArg(const std::string &arg_)
{
  this->_arg = arg_;
  return (true);
}

bool
Option::_setFlags(uint32_t flags_)
{
  this->_flags = flags_;
  return (true);
}

}
}
