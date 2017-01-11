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


#include <zutils/zProgram.h>

namespace zUtils
{
namespace zProgram
{

//*****************************************************************************
//*****************************************************************************
ProgramOption::ProgramOption() :
    _flags(ProgramOption::FLAGS_NONE), _short_name(0)
{
  this->_default.push_back(std::string(""));
}

ProgramOption::~ProgramOption()
{
}

ssize_t
zProgram::ProgramOption::Count() const
{
  return(this->_args.size());
}

uint32_t
ProgramOption::GetFlags() const
{
  return (this->_flags);
}

bool
ProgramOption::SetFlags(uint32_t flags_)
{
  this->_flags = flags_;
  return (true);
}

const char
ProgramOption::GetShortName() const
{
  return (this->_short_name);
}

bool
ProgramOption::SetShortName (const char name_)
{
  this->_short_name = name_;
  return(true);
}

const std::string&
ProgramOption::GetLongName() const
{
  return (this->_long_name);
}

bool
ProgramOption::SetLongName (const std::string& name_)
{
  this->_long_name = name_;
  return(true);
}

const std::string&
ProgramOption::GetDescription() const
{
  return (this->_desc);
}

void
ProgramOption::SetDescription(const std::string &desc_)
{
  this->_desc = desc_;
}

std::string
ProgramOption::GetDefault() const
{
  return(this->_default[0]);
}

bool
ProgramOption::SetDefault(const std::string& default_)
{
  this->_default[0] = default_;
  return (true);
}

const std::vector<std::string>&
ProgramOption::GetArguments() const
{
  if (this->_args.empty())
  {
    return(this->_default);
  }
  else
  {
    return(this->_args);
  }
}

bool
ProgramOption::AddArgument(const std::string& arg_)
{
  this->_args.push_back(arg_);
  return(true);
}


}
}
