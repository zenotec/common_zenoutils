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

//**********************************************************************
// Event Class
//**********************************************************************

zEvent::zEvent()
{
  this->_lock.Unlock();
}

zEvent::~zEvent()
{
}

void
zEvent::_notify()
{
  if (this->_lock.Lock())
  {
    std::list<zEventList *>::iterator itr = this->_eventlists.begin();
    std::list<zEventList *>::iterator end = this->_eventlists.end();
    for (; itr != end; itr++)
    {
      (*itr)->_notify();
    } // end for
    this->_lock.Unlock();
  } // end if
}

void
zEvent::_addList(zEventList *list_)
{
  if (this->_lock.Lock())
  {
    this->_eventlists.push_front(list_);
    this->_lock.Unlock();
  } // end if
}

void
zEvent::_remList(zEventList *list_)
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

zEventList::zEventList()
{
}

zEventList::~zEventList()
{
}

void
zEventList::Register(zEvent &event_)
{
  event_._addList(this);
}

void
zEventList::Unregister(zEvent &event_)
{
  event_._remList(this);
}

bool
zEventList::Wait(uint32_t us_)
{
  return (this->_sem.TimedWait(us_));
}

void
zEventList::_notify()
{
  this->_sem.Post();
}

}
