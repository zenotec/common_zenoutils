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
    _cols(cols_), _rows(rows_), _update_cmd(*this), _clear_cmd(*this)
{
  this->_refresh_timer.RegisterObserver(this);
  this->_page_timer.RegisterObserver(this);
}

Display::~Display()
{
  this->_refresh_timer.UnregisterObserver(this);
  this->_page_timer.UnregisterObserver(this);
  this->_refresh_timer.Stop();
  this->_page_timer.Stop();
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

DisplayPage*
Display::CreatePage(const std::string& name_)
{
  DisplayPage* page = new DisplayPage(name_, this->_cols, this->_rows);
  if (page)
  {
    this->_pages.push_back(page);
  }
  return (page);
}

bool
Display::DeletePage(DisplayPage* page_)
{
  this->_pages.remove(page_);
  delete (page_);
  return (true);
}

bool
Display::SetRefreshRate(const size_t rate_)
{
  if (rate_)
  {
    // Convert from rate in Hz to microseconds
    uint32_t usec;
    usec = 1000000 / rate_;
    ZLOG_DEBUG("Setting refresh rate to: " + ZLOG_INT(rate_) + "(" + ZLOG_INT(usec) + ")");
    this->_refresh_timer.Start(usec);
  }
  else
  {
    this->_refresh_timer.Stop();
  }
  return (true);
}

bool
Display::SetPageTimeout(const size_t sec_)
{
  if (sec_)
  {
    uint32_t usec;
    usec = 1000000 * sec_;
    this->_page_timer.Start(usec);
  }
  else
  {
    this->_page_timer.Stop();
  }
  return (true);
}

ssize_t
Display::GetRows() const
{
  return (this->_rows);
}

ssize_t
Display::GetColumns() const
{
  return (this->_cols);
}

ssize_t
Display::GetSize() const
{
  return (this->_rows * this->_cols);
}

void
Display::Flush()
{
  if (!this->_pages.empty())
  {
    this->update(this->_pages.front()->GetBuffer());
  }
}

bool
Display::EventHandler(zEvent::EventNotification *notification_)
{

  bool status = false;

  ZLOG_DEBUG("Handling Display timer event");

  if (this->_pages.empty())
  {
    return(true);
  }

  if (notification_ && (notification_->Type() == zEvent::Event::TYPE_TIMER))
  {

    const zTimer::TimerNotification *n = NULL;
    n = static_cast<const zTimer::TimerNotification *>(notification_);

    // Get first page
    DisplayPage* page = this->_pages.front();

    if (n->GetEvent() == &this->_refresh_timer)
    {
      page->Refresh();
    }
    else if (n->GetEvent() == &this->_page_timer)
    {
      // Swap out first page to back
      this->_pages.push_back(this->_pages.front());
      this->_pages.pop_front();
    }

    // Update display
    status = this->update(page->GetBuffer());

  }

  return (status);

}

}
}
