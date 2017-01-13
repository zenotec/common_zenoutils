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
ProgramArgument::ProgramArgument() :
    _flags(ProgramArgument::FLAGS_NONE), _pos(0)
{
}

ProgramArgument::~ProgramArgument()
{
}

ssize_t
ProgramArgument::Count() const
{
  return((this->_value.empty()) ? 0 : 1);
}

bool
ProgramArgument::Empty() const
{
  return(this->_value.empty());
}

uint32_t
ProgramArgument::GetFlags() const
{
  return (this->_flags);
}

bool
ProgramArgument::SetFlags(uint32_t flags_)
{
  this->_flags = flags_;
  return (true);
}

std::string
ProgramArgument::GetName() const
{
  return (this->_name);
}

bool
ProgramArgument::SetName(const std::string& name_)
{
  this->_name = name_;
  return (true);
}

unsigned int
ProgramArgument::GetPosition() const
{
  return(this->_pos);
}

bool
ProgramArgument::SetPosition(const unsigned int order_)
{
  this->_pos = order_;
  return(true);
}

std::string
ProgramArgument::GetDescription() const
{
  return (this->_desc);
}

bool
ProgramArgument::SetDescription(const std::string& desc_)
{
  this->_desc = desc_;
  return (true);
}

std::string
ProgramArgument::GetDefault() const
{
  return(this->_default);
}

bool
ProgramArgument::SetDefault(const std::string& default_)
{
  this->_default = default_;
  return (true);
}

std::string
ProgramArgument::GetValue() const
{
  std::string value = this->_default;
  if (!this->_value.empty())
  {
    value = this->_value;
  }
  return(value);
}

bool
ProgramArgument::SetValue(const std::string& value_)
{
  this->_value = value_;
  return (true);
}

}
}
