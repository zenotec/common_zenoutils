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
  // End critical section
  EventHandler::_lock.Unlock();
}

EventHandler::~EventHandler()
{
}

void
EventHandler::RegisterEvent(Event *event_)
{

  if (event_)
  {
    // Start critical section
    EventHandler::_lock.Lock();

    // Register handler
    EventHandler::_event_list.push_back(event_);
    event_->registerHandler(this);

    // End critical section
    EventHandler::_lock.Unlock();
  }
}

void
EventHandler::UnregisterEvent(Event *event_)
{
  if (event_)
  {
    // Start critical section
    EventHandler::_lock.Lock();

    // Unregister handler
    EventHandler::_event_list.remove(event_);
    event_->unregisterHandler(this);

    // End critical section
    EventHandler::_lock.Unlock();
  }
}

bool
EventHandler::RegisterObserver(EventObserver *obs_)
{
  bool status = false;
  if (obs_)
  {

    ZLOG_DEBUG("Registering observer");

    // Start critical section
    EventHandler::_lock.Lock();

    // Register observer
    EventHandler::_obs_list.push_back(obs_);
    status = true;

    // End critical section
    EventHandler::_lock.Unlock();
  }
  return (status);
}

bool
EventHandler::UnregisterObserver(EventObserver *obs_)
{
  bool status = false;
  if (obs_)
  {

    ZLOG_DEBUG("Unregistering observer");

    // Start critical section
    EventHandler::_lock.Lock();

    // Unregister observer
    EventHandler::_obs_list.remove(obs_);
    status = true;

    // End critical section
    EventHandler::_lock.Unlock();
  }
  return (status);
}

void
EventHandler::notify(const EventNotification* notification_)
{

  // Note: never call this routine directly; Only should be called by the event class

  // Start critical section
  EventHandler::_lock.Lock();

  // Create copy of the observer list
  std::list<EventObserver *> obs_list(EventHandler::_obs_list);

  // End critical section
  EventHandler::_lock.Unlock();

  int cnt = obs_list.size();

  // Notify all registered observers
  while (!obs_list.empty())
  {
    EventObserver *obs = obs_list.front();
    obs_list.pop_front();
    obs->EventHandler(notification_);
  }

  return;
}

void
EventHandler::notify(const EventNotification& notification_)
{

  // Note: never call this routine directly; Only should be called by the event class

  // Start critical section
  EventHandler::_lock.Lock();

  // Create copy of the observer list
  std::list<EventObserver *> obs_list(EventHandler::_obs_list);

  // End critical section
  EventHandler::_lock.Unlock();

  int cnt = obs_list.size();

  // Notify all registered observers
  while (!obs_list.empty())
  {
    EventObserver *obs = obs_list.front();
    obs_list.pop_front();
    obs->EventHandler(&notification_);
  }

  return;
}

}
}
