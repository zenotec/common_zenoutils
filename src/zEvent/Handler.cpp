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
  this->_lock.Unlock();
}

EventHandler::~EventHandler()
{
}

void
EventHandler::RegisterEvent(Event *event_)
{
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
  if (obs_ && this->_lock.Lock())
  {
    this->_obs_list.push_back(obs_);
    this->_lock.Unlock();
  }
}

void
EventHandler::UnregisterObserver(EventObserver *obs_)
{
  if (obs_ && this->_lock.Lock())
  {
    this->_obs_list.remove(obs_);
    this->_lock.Unlock();
  }
}

Event *
EventHandler::GetEvent()
{
  Event *event = NULL;
  if (!this->empty() && this->_lock.Lock())
  {
    event = this->front();
    this->pop();
    this->_lock.Unlock();
  }
  return (event);
}

bool
EventHandler::Empty()
{
  bool empty = true;
  if (this->_lock.Lock())
  {
    empty = this->_event_list.empty();
    this->_lock.Unlock();
  }
  return (empty);
}

size_t
EventHandler::Size()
{
  size_t size = 0;
  if (this->_lock.Lock())
  {
    size = this->_event_list.size();
    this->_lock.Unlock();
  }
  return (size);
}

void
EventHandler::notify(Event *event_, void *arg_)
{

  // Never call this routine directly; Only should be called by the event class
  if (this->_lock.Lock())
  {
    std::list<EventObserver *> obs_list(this->_obs_list);
    this->push(event_);
    this->Post();
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
