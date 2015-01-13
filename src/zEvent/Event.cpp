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
Event::_notify()
{
  if (this->_lock.Lock())
  {
    std::list<EventList *>::iterator itr = this->_eventlists.begin();
    std::list<EventList *>::iterator end = this->_eventlists.end();
    for (; itr != end; itr++)
    {
      (*itr)->_notify();
    } // end for
    this->_lock.Unlock();
  } // end if
}

void
Event::_addList(EventList *list_)
{
  if (this->_lock.Lock())
  {
    this->_eventlists.push_front(list_);
    this->_lock.Unlock();
  } // end if
}

void
Event::_remList(EventList *list_)
{
  if (this->_lock.Lock())
  {
    this->_eventlists.remove(list_);
    this->_lock.Unlock();
  } // end if
}

//**********************************************************************
// zEventList Class
//**********************************************************************

EventList::EventList()
{
}

EventList::~EventList()
{
}

void
EventList::Register(Event *event_)
{
  event_->_addList(this);
}

void
EventList::Unregister(Event *event_)
{
  event_->_remList(this);
}

bool
EventList::Wait(uint32_t usecs_)
{
  return (this->_sem.TimedWait(usecs_));
}

void
EventList::_notify()
{
  this->_sem.Post();
}

}
}
