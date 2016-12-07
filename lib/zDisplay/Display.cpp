/*
 * Lcd.cpp
 *
 *  Created on: Jan 16, 2016
 *      Author: kmahoney
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
    _disp_lock(zSem::Mutex::LOCKED), DisplayBuffer(cols_, rows_),
    _update_cmd(*this), _clear_cmd(*this)
{
  this->_timer_handler.RegisterObserver(this);
  this->_timer_handler.RegisterEvent(&this->_timer);
  this->_disp_lock.Unlock();
}

Display::~Display()
{
  this->_timer_handler.UnregisterEvent(&this->_timer);
  this->_timer_handler.UnregisterObserver(this);
  this->_timer.Stop();
}

Display&
Display::operator=(DisplayBuffer &other_)
{
  this->operator =(other_);
  return (*this);
}

Display&
Display::operator=(const DisplayBuffer &other_)
{
  this->operator =(other_);
  return (*this);
}

bool
Display::RegisterCommands(zCommand::CommandHandler *handler_)
{
  handler_->RegisterCommand(&this->_update_cmd);
  handler_->RegisterCommand(&this->_clear_cmd);
}

bool
Display::UnregisterCommands(zCommand::CommandHandler *handler_)
{
  handler_->UnregisterCommand(&this->_update_cmd);
  handler_->UnregisterCommand(&this->_clear_cmd);
}

bool
Display::SetRefresh(const size_t rate_)
{
  // Convert from rate in Hz to microseconds
  uint32_t usec;
  usec = 1000000 / rate_;
  ZLOG_DEBUG("Setting refresh rate to: " + ZLOG_INT(rate_) + "(" + ZLOG_INT(usec) + ")");
  this->_timer.Start(usec);
}

DisplayVar*
Display::CreateVar(const std::string &name_, const size_t len_)
{
  DisplayVar* var = new DisplayVar(name_, len_);
  if (var && this->_disp_lock.Lock())
  {
    this->_vars.push_back(var);
    this->_disp_lock.Unlock();
  }
  return (var);
}

bool
Display::DeleteVar(zDisplay::DisplayVar* var_)
{
  bool status = false;
  if (this->_disp_lock.Lock())
  {
    this->_vars.remove(var_);
    delete (var_);
    this->_disp_lock.Unlock();
    status = true;
  }
  return (status);
}

bool
Display::EventHandler(const zEvent::EventNotification *notification_)
{

  bool status = false;
  const zTimer::TimerNotification *n = NULL;

  ZLOG_DEBUG("Handling Display timer event");

  if (notification_ && (notification_->Type() == zEvent::Event::TYPE_TIMER))
  {

    n = static_cast<const zTimer::TimerNotification *>(notification_);

    if (this->_disp_lock.Lock())
    {

      ZLOG_DEBUG("Updating display");

      // Copy the display buffer
      DisplayBuffer buf = *this;

      // Iterate over all variables and update buffer
      std::list<zDisplay::DisplayVar*>::iterator it = this->_vars.begin();
      std::list<zDisplay::DisplayVar*>::iterator end = this->_vars.end();
      for (; it != end; ++it)
      {
        if (!buf.Update((*it)->GetString(), (*it)->GetColumn(), (*it)->GetRow()))
        {
          ZLOG_WARN("Display variable failed to update: " + (*it)->GetName());
        }
      }

      if (this->update(buf))
      {
        status = true;
        *this = buf;
      }

      this->_disp_lock.Unlock();

    }
  }

  return (status);

}

}
}
