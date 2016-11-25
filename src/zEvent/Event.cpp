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
}

Event::~Event()
{
}

Event::TYPE
Event::Type() const
{
  Event::TYPE type = Event::TYPE_ERR;

  // Start critical section
  Event::_lock.lock();

  type = Event::_type;

  // End critical section
  Event::_lock.unlock();

  return (type);
}

void
Event::Notify(zEvent::EventNotification* notification_)
{

  ZLOG_DEBUG("Notifying event handlers");

  // Start critical section
  Event::_lock.lock();

  // Make a copy of the handler list
  std::list<EventHandler *> handler_list(Event::_handler_list);

  // End critical section
  Event::_lock.unlock();

  int cnt = handler_list.size();

  // Notify all registered event handlers
  while (!handler_list.empty())
  {
    ZLOG_DEBUG("Notifying event handler: " + zLog::IntStr(--cnt));
    EventHandler *handler = handler_list.front();
    handler_list.pop_front();
    handler->notify(notification_);
  }

  return;
}

void
Event::Notify(zEvent::EventNotification& notification_)
{

  ZLOG_DEBUG("Notifying event handlers");

  // Start critical section
  Event::_lock.lock();

  // Make a copy of the handler list
  std::list<EventHandler *> handler_list(Event::_handler_list);

  // End critical section
  Event::_lock.unlock();

  int cnt = handler_list.size();

  // Notify all registered event handlers
  while (!handler_list.empty())
  {
    ZLOG_DEBUG("Notifying event handler: " + zLog::IntStr(--cnt));
    EventHandler *handler = handler_list.front();
    handler_list.pop_front();
    handler->notify(notification_);
  }

  return;
}

void
Event::registerHandler(EventHandler *handler_)
{
  // Start critical section
  Event::_lock.lock();

  // Add handler to list
  Event::_handler_list.push_front(handler_);

  // End critical section
  Event::_lock.unlock();
}

void
Event::unregisterHandler(EventHandler *handler_)
{
  // Start critical section
  Event::_lock.lock();

  // Remove handler from list
  Event::_handler_list.remove(handler_);

  // End critical section
  Event::_lock.unlock();
}

}
}
