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
// Class: zDisplay::DisplayPage
//*****************************************************************************

DisplayPage::DisplayPage(const std::string &name_, const size_t cols_, const size_t rows_) :
    _name(name_), DisplayBuffer(cols_, rows_)
{
}

DisplayPage::~DisplayPage()
{
}

bool
DisplayPage::operator==(const DisplayPage &other_) const
{
  return (DisplayBuffer::operator ==(other_));
}

bool
DisplayPage::operator!=(const DisplayPage &other_) const
{
  return (DisplayBuffer::operator !=(other_));
}

std::string
DisplayPage::GetName() const
{
  return (this->_name);
}

DisplayVar*
DisplayPage::CreateVar(const std::string &name_, const size_t len_)
{
  DisplayVar* var = new DisplayVar(name_, len_);
  if (var)
  {
    this->_vars.push_back(var);
  }
  return (var);
}

bool
DisplayPage::DeleteVar(zDisplay::DisplayVar* var_)
{
  this->_vars.remove(var_);
  delete (var_);
  return (true);
}

bool
DisplayPage::Refresh()
{
  FOREACH (auto& var, this->_vars)
  {
    if (!this->Update(var->GetString(), var->GetColumn(), var->GetRow()))
    {
      ZLOG_WARN("Display variable failed to update: " + var->GetName());
    }
  }
  return(true);
}

const DisplayBuffer&
DisplayPage::GetBuffer() const
{
  return(*this);
}

}
}
