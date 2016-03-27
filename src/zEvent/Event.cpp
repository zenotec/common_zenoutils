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

Event::Event (Event::TYPE type_) :
    _type (type_)
{
}

Event::~Event ()
{
}

Event::TYPE
Event::Type () const
{
  Event::TYPE type = Event::TYPE_ERR;

  // Start critical section
  this->_lock.lock ();

  type = this->_type;

  // End critical section
  this->_lock.unlock ();

  return (type);
}

void
Event::Notify (EventNotification* notification_)
{
  // Start critical section
  this->_lock.lock ();

  // Make a copy of the handler list
  std::list<EventHandler *> handler_list (this->_handler_list);

  // End critical section
  this->_lock.unlock ();

  // Set event type
  notification_->type(this->Type());

  // Notify all registered event handlers
  while (!handler_list.empty ())
  {
    EventHandler *handler = handler_list.front ();
    handler_list.pop_front ();
    handler->notify (notification_);
  }
}

void
Event::registerHandler (EventHandler *handler_)
{
  // Start critical section
  this->_lock.lock ();

  // Add handler to list
  this->_handler_list.push_front (handler_);

  // End critical section
  this->_lock.unlock ();
}

void
Event::unregisterHandler (EventHandler *handler_)
{
  // Start critical section
  this->_lock.lock ();

  // Remove handler from list
  this->_handler_list.remove (handler_);

  // End critical section
  this->_lock.unlock ();
}

}
}
