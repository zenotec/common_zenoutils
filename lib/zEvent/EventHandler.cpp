//*****************************************************************************
//    Copyright (C) 2016 ZenoTec LLC (http://www.zenotec.net)
//
//    File:
//    Description:
//
//*****************************************************************************

#include <stdint.h>

#include <mutex>
#include <list>
#include <queue>
#include <vector>

#include <zutils/zLog.h>
#include <zutils/zEvent.h>

namespace zUtils
{
namespace zEvent
{

//**********************************************************************
// Class: EventHandler
//**********************************************************************

EventHandler::EventHandler()
{
  ZLOG_DEBUG("(" + ZLOG_P(this) + ")");
  // End critical section
  EventHandler::_event_lock.Unlock();
}

EventHandler::~EventHandler()
{
//  ZLOG_DEBUG("(" + ZLOG_P(this) + ")"); // TODO: Causes segfault
}

bool
EventHandler::RegisterEvent(Event *event_)
{
  bool status = false;
  if (event_)
  {
    // Start critical section
    if (EventHandler::_event_lock.Lock())
    {
      ZLOG_DEBUG("(" + ZLOG_P(this) + "): " + ZLOG_P(event_));
      // Register event
      EventHandler::_event_list.push_back(event_);
      status = event_->registerHandler(this);
      // End critical section
      EventHandler::_event_lock.Unlock();
    }
  }
  return (status);
}

bool
EventHandler::UnregisterEvent(Event *event_)
{
  bool status = false;
  if (event_)
  {
    // Start critical section
    if (EventHandler::_event_lock.Lock())
    {
      ZLOG_DEBUG("(" + ZLOG_P(this) + "): " + ZLOG_P(event_));
      // Unregister event
      EventHandler::_event_list.remove(event_);
      status = event_->unregisterHandler(this);
      // End critical section
      EventHandler::_event_lock.Unlock();
    }
  }
  return (status);
}

bool
EventHandler::RegisterObserver(EventObserver *obs_)
{
  bool status = false;
  if (obs_)
  {
    // Start critical section
    if (EventHandler::_event_lock.Lock())
    {
      ZLOG_DEBUG("(" + ZLOG_P(this) + "): " + ZLOG_P(obs_));
      // Register observer
      EventHandler::_obs_list.push_back(obs_);
      status = true;
      // End critical section
      EventHandler::_event_lock.Unlock();
    }
  }
  return (status);
}

bool
EventHandler::UnregisterObserver(EventObserver *obs_)
{
  bool status = false;
  if (obs_)
  {
    // Start critical section
    if (EventHandler::_event_lock.Lock())
    {
      ZLOG_DEBUG("(" + ZLOG_P(this) + "): " + ZLOG_P(obs_));
      // Unregister observer
      EventHandler::_obs_list.remove(obs_);
      status = true;
      // End critical section
      EventHandler::_event_lock.Unlock();
    }
  }
  return (status);
}

void
EventHandler::notify(const EventNotification* notification_)
{

  // Note: never call this routine directly; Only should be called by the event class

  // Start critical section
  if (!EventHandler::_event_lock.Lock())
  {
    return;
  }

  // Create copy of the observer list
  std::list<EventObserver *> obs_list(EventHandler::_obs_list);

  // End critical section
  EventHandler::_event_lock.Unlock();

  int cnt = obs_list.size();

  // Notify all registered observers
  while (!obs_list.empty())
  {
    ZLOG_DEBUG("(" + ZLOG_P(this) + "): " + ZLOG_P(obs_list.front()));
    obs_list.front()->EventHandler(notification_);
    obs_list.pop_front();
  }

  return;
}


}
}
