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

#include <zutils/zLog.h>
#include <zutils/zEvent.h>

namespace zUtils
{
namespace zEvent
{

//**********************************************************************
// Class: Event
//**********************************************************************

Event::Event(Event::TYPE type_) :
    _type(type_)
{
  // End critical section
  Event::_event_lock.Unlock();
}

Event::~Event()
{
}

Event::TYPE
Event::Type() const
{
  Event::TYPE type = Event::TYPE_ERR;

  // Start critical section
  Event::_event_lock.Lock();

  type = Event::_type;

  // End critical section
  Event::_event_lock.Unlock();

  return (type);
}

void
Event::Notify(zEvent::EventNotification* notification_)
{

  ZLOG_DEBUG("Notifying event handlers");

  // Start critical section
  Event::_event_lock.Lock();

  // Make a copy of the handler list
  std::list<EventHandler *> handler_list(Event::_handler_list);

  // End critical section
  Event::_event_lock.Unlock();

  int cnt = handler_list.size();

  // Notify all registered event handlers
  while (!handler_list.empty())
  {
    ZLOG_DEBUG("Notifying event handler: " + ZLOG_INT(--cnt));
    EventHandler *handler = handler_list.front();
    handler_list.pop_front();
    handler->notify(notification_);
  }

  return;
}

bool
Event::registerHandler(EventHandler *handler_)
{
  bool status = false;
  if (handler_)
  {
    // Start critical section
    if (Event::_event_lock.Lock())
    {
      // Add handler to list
      Event::_handler_list.push_front(handler_);
      // End critical section
      Event::_event_lock.Unlock();
      status = true;
    }
  }
  return (status);
}

bool
Event::unregisterHandler(EventHandler *handler_)
{
  bool status = false;
  if (handler_)
  {
    // Start critical section
    if (Event::_event_lock.Lock())
    {
      // Remove handler from list
      Event::_handler_list.remove(handler_);
      // End critical section
      Event::_event_lock.Unlock();
      status = true;
    }
  }
  return (status);
}

}
}
