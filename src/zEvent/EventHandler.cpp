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

EventHandler::EventHandler ()
{
  // Start critical section
  this->_lock.lock ();

  // End critical section
  this->_lock.unlock ();
}

EventHandler::~EventHandler ()
{
}

void
EventHandler::RegisterEvent (Event *event_)
{

  if (event_)
  {
    // Start critical section
    this->_lock.lock ();

    // Register handler
    this->_event_list.push_back (event_);
    event_->registerHandler (this);

    // End critical section
    this->_lock.unlock ();
  }
}

void
EventHandler::UnregisterEvent (Event *event_)
{
  if (event_)
  {
    // Start critical section
    this->_lock.lock ();

    // Unregister handler
    this->_event_list.remove (event_);
    event_->unregisterHandler (this);

    // End critical section
    this->_lock.unlock ();
  }
}

bool
EventHandler::RegisterObserver (EventObserver *obs_)
{
  bool status = false;
  if (obs_)
  {
    // Start critical section
    this->_lock.lock ();

    // Register observer
    this->_obs_list.push_back (obs_);
    status = true;

    // End critical section
    this->_lock.unlock ();
  }
  return(status);
}

bool
EventHandler::UnregisterObserver (EventObserver *obs_)
{
  bool status = false;
  if (obs_)
  {
    // Start critical section
    this->_lock.lock ();

    // Unregister observer
    this->_obs_list.remove (obs_);
    status = true;

    // End critical section
    this->_lock.unlock ();
  }
  return(status);
}

void
EventHandler::notify (Event *event_, void *arg_)
{

  // Note: never call this routine directly; Only should be called by the event class

  // Start critical section
  this->_lock.lock ();

  // Create copy of the observer list
  std::list<EventObserver *> obs_list (this->_obs_list);

  // End critical section
  this->_lock.unlock ();

  // Notify all registered observers
  while (!obs_list.empty ())
  {
    EventObserver *obs = obs_list.front ();
    obs_list.pop_front ();
    obs->EventHandler (event_, arg_);
  }
}

}
}
