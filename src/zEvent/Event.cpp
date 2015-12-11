//*****************************************************************************
//    Copyright (C) 2014 ZenoTec LLC (http://www.zenotec.net)
//
//    File: zEvent.cpp
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
// Event Class
//**********************************************************************

Event::Event()
{
  this->_lock.Unlock();
}

Event::~Event()
{
}

void
Event::Notify()
{
  if (this->_lock.Lock())
  {
    // Make a copy of the handler list
    std::list<EventHandler *> handler_list(this->_handler_list);
    this->_lock.Unlock();

    // Notify all registered event handlers
    while (!handler_list.empty())
    {
      EventHandler *handler = handler_list.front();
      handler_list.pop_front();
      handler->notify(this);
    }
  } // end if
}

void
Event::registerHandler(EventHandler *list_)
{
  if (this->_lock.Lock())
  {
    this->_handler_list.push_front(list_);
    this->_lock.Unlock();
  } // end if
}

void
Event::unregisterHandler(EventHandler *list_)
{
  if (this->_lock.Lock())
  {
    this->_handler_list.remove(list_);
    this->_lock.Unlock();
  } // end if
}

}
}
