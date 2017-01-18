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
// Class: zDisplay::Display
//*****************************************************************************

Display::Display(const size_t cols_, const size_t rows_) :
    Buffer(cols_, rows_), _update_cmd(*this), _clear_cmd(*this)
{
  this->_timer.RegisterObserver(this);
}

Display::~Display()
{
  this->_timer.UnregisterObserver(this);
  this->_timer.Stop();
}

//bool
//Display::RegisterCommands(zCommand::CommandHandler *handler_)
//{
//  bool status = true;
//  status &= handler_->RegisterCommand(&this->_update_cmd);
//  status &= handler_->RegisterCommand(&this->_clear_cmd);
//  return (status);
//}
//
//bool
//Display::UnregisterCommands(zCommand::CommandHandler *handler_)
//{
//  bool status = true;
//  status &= handler_->UnregisterCommand(&this->_update_cmd);
//  status &= handler_->UnregisterCommand(&this->_clear_cmd);
//  return (status);
//}

DisplayVar*
Display::CreateVar(const std::string &name_, const size_t len_)
{
  DisplayVar* var = new DisplayVar(name_, len_);
  if (var)
  {
    this->_vars.push_back(var);
  }
  return (var);
}

bool
Display::DeleteVar(zDisplay::DisplayVar* var_)
{
  this->_vars.remove(var_);
  delete (var_);
  return (true);
}

bool
Display::SetRefresh(const size_t rate_)
{
  // Convert from rate in Hz to microseconds
  uint32_t usec;
  usec = 1000000 / rate_;
  ZLOG_DEBUG("Setting refresh rate to: " + ZLOG_INT(rate_) + "(" + ZLOG_INT(usec) + ")");
  this->_timer.Start(usec);
  return (true);
}

void
Display::Flush()
{
  this->update(this->Buffer);
}

bool
Display::EventHandler(zEvent::EventNotification *notification_)
{

  bool status = false;
  const zTimer::TimerNotification *n = NULL;

  ZLOG_DEBUG("Handling Display timer event");

  if (notification_ && (notification_->Type() == zEvent::Event::TYPE_TIMER))
  {

    n = static_cast<const zTimer::TimerNotification *>(notification_);

    ZLOG_DEBUG("Updating display: " + ZLOG_UINT(this->_vars.size()));

    // Copy the display buffer
    DisplayBuffer buf(this->Buffer);

    // Iterate over all variables and update buffer
    FOREACH (auto& var, this->_vars)
    {
      ZLOG_DEBUG(var->GetName() + "[" + ZLOG_UINT(var->GetRow()) + "][" +
          ZLOG_UINT(var->GetColumn()) + "]: " + var->GetString());
      if (!buf.Update(var->GetString(), var->GetColumn(), var->GetRow()))
      {
        ZLOG_WARN("Display variable failed to update: " + var->GetName());
      }
    }

    // Update display
    status = this->update(buf);

  }

  return (status);

}

}
}
