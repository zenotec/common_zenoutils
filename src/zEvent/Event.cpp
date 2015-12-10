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
    std::list<EventHandler *>::iterator itr = this->_handler_list.begin();
    std::list<EventHandler *>::iterator end = this->_handler_list.end();
    for (; itr != end; itr++)
    {
      (*itr)->notify(this);
    } // end for
    this->_lock.Unlock();
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
