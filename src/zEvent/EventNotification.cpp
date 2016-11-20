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
// Class: EventNotification
//**********************************************************************


EventNotification::EventNotification(zEvent::Event::TYPE type_) :
    _type(type_), _event(NULL)
{
}

EventNotification::EventNotification(zEvent::Event* event_) :
    _event(event_)
{
  if (event_)
  {
    this->type(event_->Type());
  }
}

EventNotification::~EventNotification()
{
}

zEvent::Event::TYPE
EventNotification::Type() const
{
  return(this->_type);
}

void
EventNotification::type(zEvent::Event::TYPE type_)
{
  this->_type = type_;
  return;
}

zEvent::Event*
EventNotification::GetEvent() const
{
  return(this->_event);
}


}
}
