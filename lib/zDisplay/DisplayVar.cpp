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

#include <signal.h>

#include <string>
#include <mutex>
#include <list>
#include <vector>
#include <map>

#include <zutils/zSem.h>
#include <zutils/zQueue.h>
#include <zutils/zEvent.h>
#include <zutils/zTimer.h>
#include <zutils/zLog.h>
#include <zutils/zData.h>
#include <zutils/zSocket.h>
#include <zutils/zMessage.h>
#include <zutils/zCommand.h>

#include <zutils/zDisplay.h>

namespace zUtils
{
namespace zDisplay
{

//*****************************************************************************
// Class: zDisplay::DisplayVar
//*****************************************************************************

DisplayVar::DisplayVar(const std::string &name_, const size_t size_) :
    _name(name_), DisplayBuffer(size_), _row(0), _col(0)
{
}

DisplayVar::~DisplayVar()
{
}

bool
DisplayVar::operator==(const DisplayVar &other_) const
    {
  bool status = true;
  status |= (this->GetName() == other_.GetName());
  status |= (this->GetRow() == other_.GetRow());
  status |= (this->GetColumn() == other_.GetColumn());
  status |= (this->GetSize() == other_.GetSize());
  return (status);
}

bool
DisplayVar::operator!=(const DisplayVar &other_) const
    {
  bool status = true;
  status |= (this->GetName() == other_.GetName());
  status |= (this->GetRow() == other_.GetRow());
  status |= (this->GetColumn() == other_.GetColumn());
  status |= (this->GetSize() == other_.GetSize());
  return (!status);
}

std::string
DisplayVar::GetName() const
{
  return (this->_name);
}

ssize_t
DisplayVar::GetColumn() const
{
  return (this->_col);
}

bool
DisplayVar::SetColumn(size_t col_)
{
  this->_col = col_;
  return (true);
}

ssize_t
DisplayVar::GetRow() const
{
  return (this->_row);
}

bool
DisplayVar::SetRow(size_t row_)
{
  this->_row = row_;
  return (true);
}

}
}
