//*****************************************************************************
//    Copyright (C) 2015 ZenoTec LLC (http://www.zenotec.net)
//
//    File: Handler.cpp
//    Description:
//
//*****************************************************************************

#include "zutils/zLog.h"
#include "zutils/zEvent.h"

namespace zUtils
{
namespace zEvent
{

//**********************************************************************
// EventHandler Class
//**********************************************************************

EventHandler::EventHandler()
{
  ZLOG_DEBUG("Creating handler: '" + zLog::PointerStr(this) + "'");
  this->_lock.Unlock();
}

EventHandler::~EventHandler()
{
  ZLOG_DEBUG("Destroying handler: '" + zLog::PointerStr(this) + "'");
}

void
EventHandler::RegisterEvent(Event *event_)
{
  ZLOG_DEBUG("Registering event: '" + zLog::PointerStr(this) + ", " + zLog::PointerStr(event_) + "'");
  if (event_ && this->_lock.Lock())
  {
    this->_event_list.push_back(event_);
    event_->registerHandler(this);
    this->_lock.Unlock();
  }
}

void
EventHandler::UnregisterEvent(Event *event_)
{
  ZLOG_DEBUG("Unregistering event: '" + zLog::PointerStr(this) + ", " + zLog::PointerStr(event_) + "'");
  if (event_ && this->_lock.Lock())
  {
    this->_event_list.remove(event_);
    event_->unregisterHandler(this);
    this->_lock.Unlock();
  }
}

void
EventHandler::RegisterObserver(EventObserver *obs_)
{
  ZLOG_DEBUG("Registering observer: '" + zLog::PointerStr(this) + ", " + zLog::PointerStr(obs_) + "'");
  if (obs_ && this->_lock.Lock())
  {
    this->_obs_list.push_back(obs_);
    this->_lock.Unlock();
  }
}

void
EventHandler::UnregisterObserver(EventObserver *obs_)
{
  ZLOG_DEBUG("Unregistering observer: '" + zLog::PointerStr(this) + ", " + zLog::PointerStr(obs_) + "'");
  if (obs_ && this->_lock.Lock())
  {
    this->_obs_list.remove(obs_);
    this->_lock.Unlock();
  }
}

void
EventHandler::notify(Event *event_, void *arg_)
{

  // Never call this routine directly; Only should be called by the event class
  if (this->_lock.Lock())
  {
    std::list<EventObserver *> obs_list(this->_obs_list);
    this->_lock.Unlock();

    // Notify all registered observers
    while (!obs_list.empty())
    {
      EventObserver *obs = obs_list.front();
      obs_list.pop_front();
      obs->EventHandler(event_, arg_);
    }
  }
}

}
}
