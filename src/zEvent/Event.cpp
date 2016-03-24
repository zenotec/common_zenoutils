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
// Event Class
//**********************************************************************

Event::Event (Event::TYPE type_, uint32_t id_) :
    _type (type_), _id (id_)
{
}

Event::Event (Event &other_) :
    _type (other_._type), _id (other_._id)
{
  other_._lock.lock();
  // Copy of the handler list
  this->_handler_list = other_._handler_list;
  other_._lock.unlock();
}

Event::~Event ()
{
}

Event::TYPE
Event::GetType ()
{
  Event::TYPE type = Event::TYPE_ERR;

  // Start critical section
  this->_lock.lock ();

  type = this->_type;

  // End critical section
  this->_lock.unlock ();

  return (type);
}

bool
Event::SetType (const Event::TYPE &type_)
{
  bool status = false;

  // Start critical section
  this->_lock.lock ();

  this->_type = type_;
  status = (this->_type == type_);

  // End critical section
  this->_lock.unlock ();

  return (status);
}

uint32_t
Event::GetId ()
{
  uint32_t id = 0;

  // Start critical section
  this->_lock.lock ();

  // Get copy of identifier to return to caller
  id = this->_id;

  // End critical section
  this->_lock.unlock ();

  return (id);

}

bool
Event::SetId (const uint32_t &id_)
{
  bool status = false;

  // Start critical section
  this->_lock.lock ();

  // Update identifier
  this->_id = id_;
  status = (this->_id == id_);

  // End critical section
  this->_lock.unlock ();

  // Return status
  return (status);

}

void
Event::Notify (void *arg_)
{
  // Start critical section
  this->_lock.lock ();

  // Make a copy of the handler list
  std::list<EventHandler *> handler_list (this->_handler_list);

  // End critical section
  this->_lock.unlock ();

  // Notify all registered event handlers
  while (!handler_list.empty ())
  {
    EventHandler *handler = handler_list.front ();
    handler_list.pop_front ();
    handler->notify (this, arg_);
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
